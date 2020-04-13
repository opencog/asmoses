/*
 * moses/moses/representation/Build_Atomese_Knobs.cc
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

#include <opencog/combo/combo/convert_ann_combo.h>
#include <opencog/reduct/rules/meta_rules.h>
#include <opencog/reduct/rules/general_rules.h>
#include <opencog/moses/moses/complexity.h>

#include <opencog/atoms/core/Variables.h>
#include <opencog/atoms/core/TypeNode.h>
#include <opencog/atoms/core/NumberNode.h>
#include <opencog/utils/valueUtils.h>

#include "Build_Atomese_Knobs.h"

namespace opencog
{
namespace moses
{
Build_Atomese_Knobs::Build_Atomese_Knobs(Handle &exemplar,
                                         const Handle &t,
                                         Atomese_Representation &rep,
                                         const Handle &DSN,
                                         const HandleSet &ignore_ops,
                                         contin_t step_size,
                                         contin_t expansion,
                                         field_set::width_t depth,
                                         float perm_ratio)
		: _exemplar(exemplar), _rep(rep), _skip_disc_probe(true),
		  _arity(t->getOutgoingAtom(0)->get_arity()), _signature(t),
		  _step_size(step_size), _expansion(expansion), _depth(depth),
		  _perm_ratio(perm_ratio), _ignore_ops(ignore_ops)
{
	Handle tn = _signature->getOutgoingAtom(1);
	Type output_type = TypeNodeCast(tn)->get_kind();

	HandleSeq knob_vars;
	if (output_type == BOOLEAN_NODE) {
		logical_canonize(_exemplar);
		HandleSeq path={};
		knob_vars = build_logical(path, _exemplar);
		// TODO: logical_cleanup
	}
	else {
		OC_ASSERT(true, "NonBoolean output type is not supported")
		return;
	}
}

void Build_Atomese_Knobs::logical_canonize(Handle &prog)
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

HandleSeq Build_Atomese_Knobs::build_logical(HandleSeq& path, Handle &prog)
{
	Type flip;
	auto prev=prog;

	if (prog->get_type() == NOT_LINK) // TODO check if greater_than_link
	OC_ASSERT(true,
	          "ERROR: the tree is supposed to be in normal form; "
			          "and thus must not contain logical_not nodes.")

	if (prog->get_type() == AND_LINK)
		flip = OR_LINK;

	else if (prog->get_type() == OR_LINK)
		flip = AND_LINK;

	else OC_ASSERT(true,
	               "ERROR: the tree is supposed to be in normal form; "
			               "and thus must not contain logical_not nodes.")

	HandleSeq knob_vars = add_logical_knobs(path, prog);

	HandleSeq seq = prog->getOutgoingSet();
	for (Handle child : prog->getOutgoingSet()) {
		HandleSeq sub_knob_vars;
		// TODO support greater than link once build_contin is implemented.
		if (child->get_type() == KNOB_LINK) {
			if (not bool_value_to_bool(child->getOutgoingAtom(3)))
				continue;
			seq.erase(std::remove(seq.begin(), seq.end(), child));

			Handle pattern = child->getOutgoingAtom(1);
			auto is_node = pattern->is_node();
			if (is_node)
				pattern = createLink(flip, pattern);

			auto p = path;
			auto tmp_seq = seq;
			tmp_seq.push_back(pattern);
			auto parent = createLink(tmp_seq, prog->get_type());

			find_insert(prog, p, parent, true);
			p.push_back(parent);

			sub_knob_vars = is_node ?
			                add_logical_knobs(p, pattern, false) :
			                build_logical(p, pattern);

			HandleSeq knob_seq =
					{child->getOutgoingAtom(0),
					 pattern,
					 child->getOutgoingAtom(2),
					 child->getOutgoingAtom(3)};
			child = createLink(knob_seq, KNOB_LINK);
		}
		else
		{
			seq.erase(std::remove(seq.begin(), seq.end(), child));
			// TODO p is not necessary.
			auto p = path;
			p.push_back(prog);
			sub_knob_vars = build_logical(p, child);
		}
		knob_vars.insert(knob_vars.end(), sub_knob_vars.begin(), sub_knob_vars.end());
		seq.push_back(child);
	}

	auto flp = createLink(flip);
	auto tmp_seq=seq;
	tmp_seq.push_back(flp);

	path.push_back(createLink(tmp_seq, prog->get_type()));
	HandleSeq sub_knob_vars = add_logical_knobs(path, flp);
	knob_vars.insert(knob_vars.end(), sub_knob_vars.begin(), sub_knob_vars.end());
	seq.push_back(flp);
	prog = createLink(seq, prog->get_type());
	return knob_vars;
}

HandleSeq Build_Atomese_Knobs::add_logical_knobs(HandleSeq &path, Handle &prog,
                                                 bool add_if_in_exemplar)
{
	HandleSeq seq;
	sample_logical_perms(seq, prog->get_type());
	return logical_probe_rec(path, prog, seq, add_if_in_exemplar);
}

void Build_Atomese_Knobs::sample_logical_perms(HandleSeq &seq, Type tp)
{
	// TODO: this is not the correct implementation of the logical_perm,
	//  Just a placeholder.
	tp = tp == AND_LINK ? OR_LINK : AND_LINK;
	for (int i = 1; i < _arity + 1; i++) {
		seq.push_back(createNode(PREDICATE_NODE, "$" + std::to_string(i)));
	}

	std::string bitmask(2, 1);
	bitmask.resize(_arity, 0);

	do {
		HandleSeq tmp_seq;
		for (int i = 0; i < _arity; ++i)
		{
			if (bitmask[i]) tmp_seq.push_back(seq[i]);
		}
		seq.push_back(createLink(tmp_seq, tp));
		*tmp_seq.begin() = createLink(NOT_LINK, *tmp_seq.begin());
		seq.push_back(createLink(tmp_seq, tp));
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()) and
	         seq.size() < _arity*2);
}

HandleSeq
Build_Atomese_Knobs::logical_probe_rec(HandleSeq &path, Handle &prog,
                                       const HandleSeq &seq, bool add_if_in_exemplar)
{
	HandleSeq vars;
	for (const Handle child : seq) {
		bool is_comp = logical_subtree_knob(prog, child, add_if_in_exemplar);
		if (is_comp and not add_if_in_exemplar) continue;
		auto h = disc_probe(path, prog, child, 3, is_comp);
		if (h) vars.push_back(h);
	}
	return vars;
}

bool Build_Atomese_Knobs::logical_subtree_knob(Handle &prog, const Handle &child,
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

Handle Build_Atomese_Knobs::disc_probe(HandleSeq &path, Handle &prog,
                                       const Handle &child, int, bool is_comp)
{
	OC_ASSERT(true, "Not Implemented");
}
}
}
