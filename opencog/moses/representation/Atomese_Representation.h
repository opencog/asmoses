/*
 * moses/moses/representation/Atomese_Representation.h
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
#ifndef _ATOMESE_REPRESENTATION_H
#define _ATOMESE_REPRESENTATION_H

#include <opencog/util/random.h>
#include <opencog/reduct/reduct/reduct.h>
#include "field_set.h"

namespace opencog
{
namespace moses
{

/**
 * The Atomese_Representation struct provides a mechanism to construct
 * a representation from an atomese program/exemplar. The representation
 * is constructed as an atomese lambda abstraction, this struct also
 * provides a mechanism to run the representation with knob settings to
 * get candidate programs.
 */
struct Atomese_Representation : public boost::noncopyable
{
	Atomese_Representation(const reduct::rule& simplify_candidate,
	                       const reduct::rule& simplify_knob_building,
	                       const Handle& exemplar_,
	                       const Handle& t,
	                       AtomSpace* as,
	                       const HandleSet& ignore_ops={},
	                       float perm_ratio=0.0);
protected:
	Handle _exemplar;
	field_set _fields;
	const reduct::rule* _simplify_candidate; // used to simplify candidates
	const reduct::rule* _simplify_knob_building;
	AtomSpace* _as;
	const Handle _DSN =
			createNode(DEFINED_SCHEMA_NODE, randstr(std::string("REP") + "-"));
	Handle _rep;

	Handle get_candidate(const Handle &h);

public:
	Handle get_candidate(const instance inst);
};
}
}
#endif
