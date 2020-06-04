/*
 * moses/moses/representation/KnobLink.h
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

#ifndef ASMOSES_KNOBLINK_H
#define ASMOSES_KNOBLINK_H

#include <opencog/atoms/core/FunctionLink.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/value/Value.h>
#include <opencog/atoms/base/Link.h>

namespace opencog
{
/**
 * KnobLink is just a wrapper to CondLink used to simplify
 * representation construction.
 * */
class KnobLink : public FunctionLink
{
protected:
	Handle _equiv;
	void init();
public:
	KnobLink(const HandleSeq&&, Type=KNOB_LINK);

	virtual ValuePtr execute(AtomSpace*, bool silent=false) override;

	static Handle factory(const Handle&);

	bool is_component() {return _is_component;}

private:
	bool _is_component;
};

typedef std::shared_ptr<KnobLink> KnobLinkPtr;
static inline KnobLinkPtr KnobLinkCast(const Handle& h)
   { return std::dynamic_pointer_cast<KnobLink>(h); }
static inline KnobLinkPtr KnobLinkCast(AtomPtr a)
   { return std::dynamic_pointer_cast<KnobLink>(a); }

#define createKnobLink std::make_shared<KnobLink>

}
#endif //ASMOSES_KNOBLINK_H
