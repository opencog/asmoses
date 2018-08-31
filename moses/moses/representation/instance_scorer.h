/*
 * moses/moses/representation/instance_scorer.h
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * Copyright (C) 2012,2013 Poulin Holdings LLC
 * All Rights Reserved
 *
 * Written by Moshe Looks, Nil Geisweiller, Linas Vepstas
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
#ifndef _INSTANCE_SCORER_H
#define _INSTANCE_SCORER_H

#include "instance.h"
#include "field_set.h"
#include "representation.h"
#include "../scoring/behave_cscore.h"
#include "moses/comboreduct/converter/combo_atomese.h"

namespace opencog
{
namespace moses
{

struct iscorer_base : public std::unary_function<instance, composite_score>
{
	virtual composite_score operator()(const instance &) const = 0;

	virtual ~iscorer_base()
	{}
};

/**
 * Mostly for testing the optimization algos.  Returns minus the
 * hamming distance of the candidate to a given target instance and
 * constant null complexity.
 */
struct distance_based_scorer : public iscorer_base
{
	distance_based_scorer(const field_set &_fs,
	                      const instance &_target_inst)
			: fs(_fs), target_inst(_target_inst)
	{}

	composite_score operator()(const instance &inst) const;

protected:
	const field_set &fs;
	const instance &target_inst;
};

struct combo_based_scorer : public iscorer_base
{
	combo_based_scorer(behave_cscore &cs,
	                   representation &rep, bool reduce)
			: _cscorer(cs), _rep(rep), _reduce(reduce)
	{}

	composite_score operator()(const instance &inst) const;

protected:
	behave_cscore &_cscorer;
	representation &_rep;
	bool _reduce; // whether the exemplar should be reduced before being
	// evaluated.  This is advantagous when _cscorer is
	// also a cache; the reduced form will have more cache
	// hits.
};

struct atomese_based_scorer : public iscorer_base
{
	atomese_based_scorer(behave_cscore &cs,
	                     representation &rep, bool reduce)
			: _cscorer(cs), _rep(rep), _reduce(reduce)
	{}

	composite_score operator()(const instance &inst) const;

protected:
	behave_cscore &_cscorer;
	representation &_rep;
	bool _reduce; // whether the exemplar should be reduced before being
	// evaluated.  This is advantagous when _cscorer is
	// also a cache; the reduced form will have more cache
	// hits.
};

} //~namespace moses
} //~namespace opencog

#endif
