/*
 * opencog/atoms/core/ImpulseLink.h
 *
 * Copyright (C) 2019 Yidnekachew W.
 *
 * All Rights Reserved
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

#ifndef ASMOSES_IMPULSELINK_H
#define ASMOSES_IMPULSELINK_H

#include <opencog/atoms/core/FunctionLink.h>
#include <opencog/atoms/core/ScopeLink.h>
#include <opencog/atoms/core/Quotation.h>
#include <opencog/asmoses/atomese/atom_types/atom_types.h>

namespace opencog
{
class ImpulseLink : public FunctionLink
{
public:
	ImpulseLink(const HandleSeq&, Type= IMPULSE_LINK);

	virtual ValuePtr execute(AtomSpace*, bool);

	static Handle factory(const Handle&);

	void init(void);
};

typedef std::shared_ptr<ImpulseLink> ImpulseLinkPtr;

static inline ImpulseLinkPtr ImpulseLinkCast(const Handle& h)
{
	AtomPtr a(h);
	return std::dynamic_pointer_cast<ImpulseLink>(a);
}

static inline ImpulseLinkPtr ImpulseLinkCast(AtomPtr a)
{ return std::dynamic_pointer_cast<ImpulseLink>(a); }

#define createImpulseLink std::make_shared<ImpulseLink>

/** @}*/
}

#endif //ASMOSES_IMPULSELINK_H
