/** populate_atomspace.h ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Authors: Kasim Ebrahim <se.kasim.ebrahim@gmail.com>
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


#ifndef MOSES_POPULATE_ATOMSPACE_H
#define MOSES_POPULATE_ATOMSPACE_H

#include <moses/data/table/table.h>
#include <opencog/atomspace/AtomSpace.h>

namespace opencog { namespace moses {

/**
 * Populate the given atomspace with the input data.
 * @param Atomspace&     as
 * @param Itable&     itable
 *
 * @return                 LinkValue pointer containing the logical_and.
 */
void populate(AtomSpace* as, const ITable &itable)
{
	OC_ASSERT(false, "populate_atomspace Not implemented yet!");
}

}
}

#endif
