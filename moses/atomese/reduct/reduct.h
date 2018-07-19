/** reduct.h ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Author: Yidnekachew Wondimu <searchyidne@gmail.com>
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

#ifndef _OPENCOG_ATOMESE_REDUCT_H
#define _OPENCOG_ATOMESE_REDUCT_H

namespace opencog {
namespace atomese {

/**
 * Apply full_reduction on an atomese program.
 *
 * @param tr a combo_tree to be reduced.
 * @todo it should accept a Handle as an argument after the combo->atomese converter is done.
 * @return
 */
Handle full_reduce(combo::combo_tree& tr);

}
}

#endif //_OPENCOG_ATOMESE_REDUCT_H
