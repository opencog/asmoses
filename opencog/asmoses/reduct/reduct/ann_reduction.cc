/*
 * moses/comboreduct/reduct/logical_reduction.cc
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * All Rights Reserved
 *
 * Written by Moshe Looks
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
#include "reduct.h"
#include "opencog/asmoses/reduct/rules/meta_rules.h"
#include "opencog/asmoses/reduct/rules/ann_rules.h"

namespace opencog { namespace reduct {
const rule& ann_reduction() {
    using namespace opencog::combo;
    static ann_rule r;
    return r;
}

} // ~namespace reduct
} // ~namespace opencog
