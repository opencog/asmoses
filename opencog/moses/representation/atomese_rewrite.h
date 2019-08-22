/*
 * moses/moses/representation/atomese_rewrite.h
 *
 * Copyright (C) 2019 OpenCog Foundation
 * All Rights Reserved
 *
 * Author: Behailu Dereje <Behailu04@gmail.com>
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

#ifndef ASMOSES_ATOMESE_REWRITE_H
#define ASMOSES_ATOMESE_REWRITE_H

#include <boost/utility.hpp>
#include <opencog/atoms/base/Handle.h>

namespace opencog { namespace moses {
class atomeseRewriting {

public:
    HandleSeq _handle_seq;
    std::vector<std::pair<Type, int>> _type_store;
    bool is_found = false;

public:
    Handle insert_atom_above(Handle &source, Handle target,
                           Type insert_type);

    Handle append_atom_below(Handle &source, Handle target,
                           Handle &append_atom);

    Handle erase_atom(Handle &source, Handle target);

    Handle flatten_atom(Handle &source, Handle target);

    bool search_handle(Handle &source, Handle target,
                      bool is_first = true);

    Handle handle_replace(Handle& source, Handle target,
                        Handle &replace);

    Handle atom_rebuild(Handle& handle);

};

} // ~namespace moses
} // ~namespace opencog


#endif //ASMOSES_ATOMESE_REWRITE_H
