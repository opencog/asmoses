/** reduct.cc ---
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

#include <moses/comboreduct/reduct/reduct.h>
#include <moses/comboreduct/converter/combo_atomese.h>
#include "reduct.h"

namespace opencog {
namespace atomese {

Handle full_reduce(combo::combo_tree& tr)
{
//	combo::combo_tree& tr = combo::atomese_combo(to_reduce);
	reduct::full_reduce(tr);
	return combo::atomese_combo(tr);
}

}
}
