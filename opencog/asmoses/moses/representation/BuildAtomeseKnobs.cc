/*
 * moses/moses/representation/BuildAtomeseKnobs.cc
 *
 * Copyright (C) 2020 OpenCog Foundation
 *
 * Author: Kasim Ebrahim
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <future>

#include <boost/range/irange.hpp>

#include <opencog/util/iostreamContainer.h>
#include <opencog/util/lazy_random_selector.h>
#include <opencog/util/exceptions.h>
#include <opencog/util/dorepeat.h>
#include <opencog/util/oc_omp.h>
#include <opencog/util/random.h>

#include <opencog/asmoses/combo/combo/convert_ann_combo.h>
#include <opencog/asmoses/reduct/rules/meta_rules.h>
#include <opencog/asmoses/reduct/rules/general_rules.h>
#include <opencog/asmoses/moses/moses/complexity.h>

#include <opencog/atoms/core/Variables.h>
#include <opencog/atoms/core/TypeNode.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/asmoses/utils/valueUtils.h>

#include "BuildAtomeseKnobs.h"

namespace opencog
{
namespace moses
{
BuildAtomeseKnobs::BuildAtomeseKnobs(Handle &exemplar,
                                     const Handle &t,
                                     AtomeseRepresentation &rep,
                                     const Handle &DSN,
                                     bool linear_contin,
                                     const HandleSet &ignore_ops,
                                     contin_t step_size,
                                     contin_t expansion,
                                     field_set::width_t depth,
                                     float perm_ratio)
		: _exemplar(exemplar), _rep(rep),
		  _arity(t->getOutgoingAtom(0)->get_arity()), _signature(t),
		  _step_size(step_size), _expansion(expansion), _depth(depth),
		  _perm_ratio(perm_ratio), _skip_disc_probe(true),
		  _linear_contin(linear_contin), _ignore_ops(ignore_ops)
{
	Handle tn = _signature->getOutgoingAtom(1);
	Type output_type = TypeNodeCast(tn)->get_kind();

	if (output_type == BOOLEAN_NODE) {
		logical_canonize(_exemplar);
		HandleSeq path={};
		build_logical(path, _exemplar);
		// TODO: logical_cleanup
	}
	else if (output_type == NUMBER_NODE) {
		build_contin(_exemplar);
	}
	else {
		OC_ASSERT(false, "NonBoolean output type is not supported")
		return;
	}

	Handle vars = createLink(_variables, VARIABLE_LIST);
	Handle LMBDA = createLink(HandleSeq{vars, _exemplar}, LAMBDA_LINK);
	_rep.set_rep(createLink(HandleSeq{DSN, LMBDA}, DEFINE_LINK));
	_rep.set_variables(_variables);
}

void BuildAtomeseKnobs::logical_canonize(Handle &prog)
{
	if (prog->get_type() == AND_LINK)
		prog = createLink(HandleSeq{prog}, OR_LINK);
	else if (prog->get_type() == OR_LINK)
		prog = createLink(HandleSeq{prog}, AND_LINK);
	else if (prog->get_type() == TRUE_LINK or
	         prog->get_type() == FALSE_LINK)
		prog = createLink(HandleSeq{createLink(HandleSeq{}, OR_LINK)},
		                  AND_LINK);
	else if (prog->get_type() == PREDICATE_NODE)
		prog = createLink(HandleSeq{createLink(HandleSeq{prog}, AND_LINK)},
		                  OR_LINK);
	else if (is_predicate(prog)) {
		Handle lh = prog->getOutgoingAtom(0);
		lh = linear_combination(lh);
		prog = createLink(OR_LINK,
		                  createLink(AND_LINK,
		                             createLink(GREATER_THAN_LINK,
		                                        lh, prog->getOutgoingAtom(1))));
	}
	else OC_ASSERT(true, "Error: unknown program type in logical_canonize.")
}

inline Handle remove_copy(Handle prog, Handle ch, Handle sub)
{
	auto b = prog->getOutgoingSet().begin();
	auto e = prog->getOutgoingSet().end();

	HandleSeq seq;
	std::remove_copy_if(b, e, std::back_inserter(seq), [&ch](Handle h){return content_eq(ch, h);});
	seq.push_back(sub);
	return createLink(seq, prog->get_type());
}

inline Handle find_insert(Handle prog, HandleSeq &path, Handle sub,
                          bool update=false)
{
	// The path needs to keep up with the changing program.
	auto prev = prog;
	for(int i = path.size()-1; i>-1; i--)
	{
		Handle parent = path[i];
		sub = remove_copy(parent, prev, sub);
		prev = parent;
		if (update)
			path[i] = sub;
	}
	return sub;
}

inline HandleSeq new_path(HandleSeq path, Handle ch, HandleSeq seq, Handle prog)
{
	seq.push_back(ch);
	auto parent = createLink(seq, prog->get_type());
	find_insert(prog, path, parent, true);
	path.push_back(parent);
	return path;
}

void BuildAtomeseKnobs::build_logical(HandleSeq& path, Handle &prog)
{
	Type flip;
	auto prev=prog;

	if (prog->get_type() == NOT_LINK) // TODO check if greater_than_link
	OC_ASSERT(false,
	          "ERROR: the tree is supposed to be in normal form; "
			          "and thus must not contain logical_not nodes.")

	if (prog->get_type() == AND_LINK)
		flip = OR_LINK;

	else if (prog->get_type() == OR_LINK)
		flip = AND_LINK;

	else OC_ASSERT(false,
	               "ERROR: the tree is supposed to be in normal form; "
			               "and thus must not contain logical_not nodes.")

	add_logical_knobs(path, prog);

	HandleSeq seq = prog->getOutgoingSet();
	for (Handle child : prog->getOutgoingSet()) {
		// TODO support greater than link once build_contin is implemented.
		if (child->get_type() == KNOB_LINK) {
			if (not bool_value_to_bool(child->getOutgoingAtom(3)))
				continue;
			seq.erase(std::remove(seq.begin(), seq.end(), child));

			Handle pattern = child->getOutgoingAtom(1);

			auto is_node = pattern->is_node();
			auto is_pred = is_predicate(pattern);

			if (is_node || is_pred)
				pattern = createLink(flip, pattern);

			HandleSeq p = new_path(path, pattern, seq, prog);
			if(is_node || is_pred)
				add_logical_knobs(p, pattern, false);
			else
				build_logical(p, pattern);

			if(is_pred)
				build_predicate(pattern, child->getOutgoingAtom(1));

			HandleSeq knob_seq =
					{child->getOutgoingAtom(0),
					 pattern,
					 child->getOutgoingAtom(2),
					 child->getOutgoingAtom(3),
					 child->getOutgoingAtom(4)};
			child = createLink(knob_seq, KNOB_LINK);
		}
		else {
			seq.erase(std::remove(seq.begin(), seq.end(), child));
			auto p = path;
			bool is_pred = is_predicate(child);

			Handle orig=child;
			if (nameserver().isA(orig->get_type(), NODE) ||
			    orig->get_type() == NOT_LINK || is_pred) {
				child = createLink(flip, orig);
				p = new_path(p, child, seq, prog);
				add_logical_knobs(p, child, false);
			} else
				p.push_back(prog);

			if (is_pred) {
				build_predicate(child, orig);
			} else
				build_logical(p, child);
		}
		seq.push_back(child);
	}

	auto flp = createLink(flip);
	auto tmp_seq=seq;
	tmp_seq.push_back(flp);

	path.push_back(createLink(tmp_seq, prog->get_type()));
	add_logical_knobs(path, flp);
	seq.push_back(flp);
	prog = createLink(seq, prog->get_type());
}

void BuildAtomeseKnobs::add_logical_knobs(HandleSeq &path, Handle &prog,
                                                 bool add_if_in_exemplar)
{
	HandleSeq seq;
	sample_logical_perms(seq, prog->get_type());
	logical_probe_rec(path, prog, seq, add_if_in_exemplar);
}

void BuildAtomeseKnobs::sample_logical_perms(HandleSeq &seq, Type head_type)
{
	Type type = head_type == AND_LINK ? OR_LINK : AND_LINK;
	HandleSeq arg_types = _signature->getOutgoingAtom(0)->getOutgoingSet();

	for (int i = 0; i < _arity; i++) {
		Type arg_type = TypeNodeCast(arg_types[i])->get_kind();
		if (arg_type == BOOLEAN_NODE) {
			Handle arg = createNode(PREDICATE_NODE, "$" + std::to_string(i+1));
			if (_ignore_ops.find(arg) == _ignore_ops.end())
				seq.push_back(arg);
		}
		else if (arg_type == NUMBER_NODE) {
			Handle grt = createNode(TYPE_NODE, "GreaterThanLink");
			if (_ignore_ops.find(grt) == _ignore_ops.end()) {
				Handle arg =
						createLink(GREATER_THAN_LINK,
						           createNode(SCHEMA_NODE, "$" + std::to_string(i+1)),
						           createNode(NUMBER_NODE, "0"));
				seq.push_back(arg);
			}
		}
	}

	unsigned ps = seq.size(); // the actual number of arguments to consider

	if (_perm_ratio <= -1.0 or ps < 2)
		return;

	std::vector<HandleSeq> combs;

	for (arity_t a = 0; a < (arity_t)ps; a++) {
		for (arity_t b = a + 1; b < (arity_t)ps; b++) {
			combs.push_back({seq[a], seq[b]});
			combs.push_back({createLink(NOT_LINK, seq[a]), seq[b]});
		}
	}

	unsigned max_pairs = combs.size();

	// Actual number of pairs to create ...
	size_t n_pairs = 0;
	if (0.0 < _perm_ratio)
		n_pairs = static_cast<size_t>(floor(ps + _perm_ratio * (max_pairs - ps)));
	else {
		n_pairs = static_cast<size_t>(floor((1.0 + _perm_ratio) * ps));
		if (ps < n_pairs) n_pairs = 0;  // Avoid accidental rounding to MAX_UINT
	}

	if (logger().is_debug_enabled()) {
		logger().debug() << "perms.size: " << ps
		                 << " max_pairs: " << max_pairs
		                 << " logical knob pairs to create: "<< n_pairs;
	}

	lazy_random_selector randpair(max_pairs);
	dorepeat (n_pairs) {
		size_t i = randpair();
		seq.push_back(createLink(combs[i], type));
	}

	if (logger().is_fine_enabled())
		ostream_container(logger().fine() << "Perms:" << std::endl, seq, "\n");
}

void
BuildAtomeseKnobs::logical_probe_rec(HandleSeq &path, Handle &prog,
                                       const HandleSeq &seq, bool add_if_in_exemplar)
{
	for (const Handle& child : seq) {
		bool is_comp = logical_subtree_knob(prog, child, add_if_in_exemplar);
		if (is_comp and not add_if_in_exemplar) continue;
		auto h = disc_probe(path, prog, child, 3, is_comp);
		if (h) _variables.push_back(h);
	}
}

bool BuildAtomeseKnobs::logical_subtree_knob(Handle &prog, const Handle &child,
                                               bool add_if_in_exemplar)
{
	bool is_comp=false;
	auto reduced_neg_child = createLink(HandleSeq{child}, NOT_LINK);
	reduct::logical_reduction r;
	r(1)(reduced_neg_child);

	HandleSeq seq;
	for (Handle h : prog->getOutgoingSet())
	{
		// remove only one.
		if ((content_eq(h, child) or content_eq(h, reduced_neg_child)) and not is_comp) {
			is_comp = true;
			if (not add_if_in_exemplar) seq.push_back(h);
		} else {
			seq.push_back(h);
		}
	}
	prog = createLink(seq, prog->get_type());
	return is_comp;
}

inline Handle identity(Type t)
{
	if (t == AND_LINK)
		return createLink(TRUE_LINK);
	if (t == OR_LINK)
		return createLink(FALSE_LINK);
	OC_ASSERT(false, "Unknown Type");
	return Handle::UNDEFINED;
}

Handle BuildAtomeseKnobs::disc_probe(HandleSeq& path, Handle &prog,
                                       const Handle &child, int mult, bool is_comp)
{
	Type type=prog->get_type();

	std::vector<int> knob_settings={0}; // absent knob setting.
	auto idx_prog = prog;
	if (is_comp)
	{
		auto seq = prog->getOutgoingSet();
		seq.push_back(child);
		idx_prog = createLink(seq, type);
	}
	for (auto i : boost::irange(1, mult)) {
		auto cand_seq(prog->getOutgoingSet());
		Handle cand_handle;
		switch (i) {
			case 1:
				cand_seq.push_back(child);
				break;
			case 2:
				cand_seq.push_back(createLink(HandleSeq{child}, NOT_LINK));
				break;
			default: OC_ASSERT(false, "Error Unknown knob setting!")
		}
		auto sub = createLink(cand_seq, type);
		cand_handle = find_insert(idx_prog, path, sub);

		complexity_t initial_c = atomese_complexity(cand_handle);

		_rep.clean_atomese_prog(cand_handle, true, true);

		auto tmp_c = atomese_complexity(cand_handle);
		if (initial_c <= tmp_c)
			knob_settings.push_back(i);
	}

	if (knob_settings.size() < 2) // No need to create knob if there is no
		return Handle();          // other option.

	HandleSeq prog_seq(prog->getOutgoingSet());
	auto knob_var =
			createNode(VARIABLE_NODE, randstr(std::string("$knob") + "-"));

	std::stringstream knob_settings_str;
	std::copy(knob_settings.begin(), knob_settings.end(),
	          std::ostream_iterator<int>(knob_settings_str, " "));
	HandleSeq knob_seq =
			{knob_var,
			 child,
			 Handle(createNumberNode(std::move(knob_settings_str.str()))),
			 createLink(is_comp ? TRUE_LINK : FALSE_LINK),
			 identity(type)};
	prog_seq.push_back(createLink(knob_seq, KNOB_LINK));

	prog = createLink(prog_seq, type);
	Handle del = find_insert(idx_prog, path, prog, true);

	field_set::disc_spec ds(knob_settings.size());
	_rep.disc.insert(std::make_pair(ds, knob_var));
	return knob_var;
}

inline Handle pick_const(Handle &prog)
{
	auto b = prog->getOutgoingSet().begin();
	auto e = prog->getOutgoingSet().end();

	HandleSeq seq;
	Handle c;
	std::remove_copy_if(b, e,
	                    std::back_inserter(seq),
	                    [&c](Handle h) {
		                    if (h->get_type() == NUMBER_NODE)
		                    {
			                    c = h;
			                    return true;
		                    }
		                    return false;
	                    });
	prog = createLink(seq, prog->get_type());
	return c;
}

void BuildAtomeseKnobs::build_contin(Handle &prog)
{
	Type type = prog->get_type();

	if (type == PLUS_LINK or type == TIMES_LINK) {
		Handle cn = pick_const(prog);
		if (type == PLUS_LINK)
			prog = add_to(div_with_lc(multi_const(linear_combination(prog))), cn);
		else
			prog = add_to(div_with_lc(multi_const(linear_combination(prog), cn)));
		return;
	}

	if (type == DIVIDE_LINK) {
		OC_ASSERT(prog->get_arity()==2, "DivideLink expects 2 arguments!");
		Handle n = multi_const(linear_combination(prog->getOutgoingAtom(0)));
		prog = add_to(div_with_lc(n, prog->getOutgoingAtom(1)));
		return;
	}

	else {
		prog = createLink(PLUS_LINK, prog);
		prog = add_to(div_with_lc(multi_const(linear_combination(prog))));
	}
}

Handle BuildAtomeseKnobs::linear_combination(Handle prog, bool in_SLE)
{
	HandleSeq seq;
	Handle const_node = Handle(createNumberNode(0));

	if (prog->get_type() == NUMBER_NODE)
		const_node = prog;
	else if (prog->get_type() == SCHEMA_NODE) {// if prog is a contin arg.
		seq.push_back(multi_const(prog));
	}
	else if (nameserver().isA(prog->get_type(), LINK)) {
		for (Handle child : prog->getOutgoingSet())
		{
			if (child->get_type() == NUMBER_NODE) {
				if (!NumberNodeCast(const_node)->value().at(0))
					const_node = child;
				else
					seq.push_back(create_const_knob(child));
			}
			else if (nameserver().isA(child->get_type(), NODE)) {
				if (_linear_contin)
					seq.push_back(multi_const(child));
				else
					seq.push_back(
							createLink(TIMES_LINK, child,
							           linear_combination(Handle(createNumberNode(1)))));
			}
			else if (child->get_type() == TIMES_LINK) {
				if (_linear_contin)
					seq.push_back(make_knob_rec(child));
				else {
					HandleSeq ch_seq;
					Handle const_mul = Handle(createNumberNode(1));
					for(Handle h : child->getOutgoingSet())
					{
						if (h->get_type() == NUMBER_NODE and
								NumberNodeCast(const_mul)->value().at(0) == 1.0) {
							const_mul = h;
							continue;
						}
						ch_seq.push_back(h);
					}
					ch_seq.push_back(linear_combination(const_mul));
					seq.push_back(make_knob_rec(createLink(ch_seq, TIMES_LINK)));
				}
			}
			else {
				if (_linear_contin)
					seq.push_back(multi_const(make_knob_rec(child)));
				else
					seq.push_back(
							createLink(TIMES_LINK, linear_combination(child),
							           linear_combination(Handle(createNumberNode(1)))));
			}
		}
	}

	HandleSeq tmp(linear_combination(in_SLE));
	seq.insert(seq.end(), tmp.begin(), tmp.end());
	if (!in_SLE)
		seq.push_back(create_const_knob(const_node));

	return createLink(seq, PLUS_LINK);
}

HandleSeq BuildAtomeseKnobs::linear_combination(bool in_SLE)
{
	HandleSeq seq;
	for (int idx : boost::irange(1, _arity + 1))
	{
		HandleSeq arg_types = _signature->getOutgoingAtom(0)->getOutgoingSet();
		Type arg_type = TypeNodeCast(arg_types[idx-1])->get_kind();
		if (arg_type == NUMBER_NODE) {
			Handle arg = Handle(createNode(SCHEMA_NODE, "$" + std::to_string(idx)));
			seq.push_back(multi_const(arg, Handle(createNumberNode(0))));
		}
		else if (arg_type == BOOLEAN_NODE) {
			Handle imp_t = createNode(TYPE_NODE, "MpulseLink");
			if (_ignore_ops.find(imp_t) == _ignore_ops.end()) {
				Handle arg = Handle(createNode(PREDICATE_NODE, "$" + std::to_string(idx)));
				Handle imp = createLink(MPULSE_LINK, arg);
				seq.push_back(multi_const(imp, Handle(createNumberNode(0))));
			}
		}
	}

	if (!in_SLE) {
		string_seq func_strs = {"SinLink", "LogLink", "ExpLink"};
		for (unsigned i=0; i < func_strs.size(); i++)
		{
			Handle imp_t = createNode(TYPE_NODE, func_strs[i]);
			if (_ignore_ops.find(imp_t) != _ignore_ops.end())
				continue;
			Handle sub_child=createLink(PLUS_LINK);
			switch(i)
			{
				case 0:
					seq.push_back(multi_const(
							createLink(SIN_LINK, linear_combination(sub_child, true)),
							Handle(createNumberNode(0))));
					break;
				case 1:
					seq.push_back(multi_const(
							createLink(LOG_LINK, linear_combination(sub_child, true)),
							Handle(createNumberNode(0))));
					break;
				case 2:
					seq.push_back(multi_const(
							createLink(EXP_LINK, linear_combination(sub_child, true)),
							Handle(createNumberNode(0))));
					break;
				default:
				OC_ASSERT(false, "Unknown OP!");
			}
		}
	}
	return seq;
}

Handle BuildAtomeseKnobs::create_const_knob(Handle const_node, Type type)
{
	Handle const_knob;
	if (const_node)
		const_knob = create_contin_knob(NumberNodeCast(const_node)->value().at(0));
	else if (type != NOTYPE) {
		// if const_node is nullptr, create contin knob with identity.
		const_knob = type == PLUS_LINK ?
		             create_contin_knob(0) :
		             create_contin_knob(1);
	}
	return const_knob;
}

Handle BuildAtomeseKnobs::create_contin_knob(int i)
{
	Handle knob_var =
			createNode(VARIABLE_NODE, randstr(std::string("$knob") + "-"));
	_variables.push_back(knob_var);
	field_set::contin_spec cs(i, _step_size, _expansion, _depth);
	_rep.contin.insert(std::make_pair(cs, knob_var));
	return knob_var;
}

Handle BuildAtomeseKnobs::multi_const(Handle l, Handle r)
{
	Handle contin_knob = create_const_knob(r, TIMES_LINK);
	return createLink(TIMES_LINK, l, contin_knob);
}

Handle BuildAtomeseKnobs::div_with_lc(Handle n, Handle d)
{
	d = d ? linear_combination(d) :
	    linear_combination(Handle(createNumberNode(1)));
	return createLink(DIVIDE_LINK, n, d);
}

Handle BuildAtomeseKnobs::add_to(Handle l, Handle r)
{
	Handle contin_knob = create_const_knob(r, PLUS_LINK);
	return createLink(PLUS_LINK, l, contin_knob);
}

Handle BuildAtomeseKnobs::make_knob_rec(Handle prog)
{
	if (nameserver().isA(prog->get_type(), NODE))
		return prog->get_type() == NUMBER_NODE ?
		       create_const_knob(prog) : prog;

	HandleSeq seq;
	for (Handle ch : prog->getOutgoingSet())
		seq.push_back(make_knob_rec(ch));

	return createLink(seq, prog->get_type());
}

bool BuildAtomeseKnobs::is_predicate(const Handle &prog) const
{
	if (prog->get_type() == GREATER_THAN_LINK)
		return true;
	if (prog->get_type() == NOT_LINK)
		return prog->getOutgoingAtom(0)->get_type() == GREATER_THAN_LINK;
	return false;
}

void
BuildAtomeseKnobs::build_predicate(Handle &prog, Handle child)
{
	OC_ASSERT(is_predicate(child),
	          "ERROR child is supposed to be Predicate.")
	HandleSeq seq;
	std::remove_copy_if(prog->getOutgoingSet().begin(),
	                    prog->getOutgoingSet().end(),
	                    std::back_inserter(seq),
	                    [&child](Handle h){return content_eq(child, h);});
	bool is_neg = child->get_type() == NOT_LINK;
	if (is_neg) {
		child = child->getOutgoingAtom(0);
		auto contin_h = child->getOutgoingAtom(0);
		build_contin(contin_h);
		child = createLink(NOT_LINK,
		                   createLink(GREATER_THAN_LINK,
		                              contin_h,
		                              child->getOutgoingAtom(1)));
	} else {
		auto contin_h = child->getOutgoingAtom(0);
		build_contin(contin_h);
		child = createLink(GREATER_THAN_LINK,
		                   contin_h,
		                   child->getOutgoingAtom(1));
	}
	seq.push_back(child);
	prog = createLink(seq, prog->get_type());
}
}
}
