/*
 * moses/moses/representation/atomese_rewrite.cc
 *
 * Copyright (C) 2019 OpenCog Foundation
 *
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
#include <future>

#include <boost/range/irange.hpp>
#include <opencog/util/random.h>
#include <opencog/atoms/base/Handle.h>
#include <opencog/util/Logger.h>
#include <opencog/atoms/atom_types/atom_types.h>
#include <opencog/atoms/base/Handle.h>
#include <opencog/atoms/base/Link.h>
#include <opencog/atoms/base/Node.h>
#include <opencog/atoms/core/NumberNode.h>

#include "atomese_rewrite.h"

using namespace std;

namespace opencog { namespace moses {


int atomeseRewriting::search_handle(Handle &source, Handle &target,
                                    bool is_first) {
    if (is_first) {
        _handle_seq.clear();
        _type_store.clear();
    }
    if (content_eq(source, target)) {
        return 0;
    } else {
        if (source->is_link()) {
            HandleSeq handleSeq = source->getOutgoingSet();
            _type_store.insert(make_pair(source->get_type(), handleSeq.size()));
            for (auto sib: handleSeq) {
                _handle_seq.push_back(sib);
            }
            for (auto sib: handleSeq) {
                search_handle(sib, target, false);
                break;
            }
        } else {
            _handle_seq.push_back(source);
        }
    }
    return _handle_seq.size() > 0 ? 1 : -1;
}

Handle atomeseRewriting::atom_rebuild(opencog::Handle &handle) {

    std::map<Type, int>::iterator it;
    int handleSeq_size = _handle_seq.size();
    HandleSeq handle_seq;

    for (it = _type_store.begin(); it != _type_store.end(); ++it) {
        int limit = handleSeq_size - it->second - 1;
        if (handleSeq_size > -1) {
            for (int j = handleSeq_size - 1; j > limit; --j) {
                handle_seq.push_back(_handle_seq[j]);
            }
            handleSeq_size = limit + 1;
            if (limit >= 0) {
                _handle_seq[limit] = createLink(handle_seq, it->first);
            } else {
                handle = createLink(handle_seq, it->first);
                return handle;
            }
            handle_seq.clear();
        }
    }
}


void atomeseRewriting::insert_atom_above(opencog::Handle &handle, opencog::Handle &find,
                                         opencog::Type type) {
    if (search_handle(handle, find) == 1) {
        for (int i = 0; i < _handle_seq.size(); ++i) {
            if (content_eq(_handle_seq[i], find)) {
                HandleSeq handleSeq;
                handleSeq.push_back(_handle_seq[i]);
                _handle_seq[i] = createLink(handleSeq, type);
                break;
            }
        }
        handle = atom_rebuild(handle);
    } else {
        HandleSeq handleSeq;
        handleSeq.push_back(handle);
        handle = createLink(handleSeq, type);
    }
}

void atomeseRewriting::append_atom_below(opencog::Handle &handle, opencog::Handle &find,
                                         opencog::Handle &atom) {
    if (search_handle(handle, find) == 1) {
        for (int i = 0; i < _handle_seq.size(); ++i) {
            logger().debug() << "handle_seq: " << oc_to_string(_handle_seq[i]->get_handle(), empty_string);
            logger().debug() << "find: " << oc_to_string(find, empty_string);
            if (content_eq(_handle_seq[i], find)) {
                HandleSeq handleSeq = _handle_seq[i]->getOutgoingSet();
                handleSeq.push_back(atom);
                _handle_seq[i] = createLink(handleSeq, _handle_seq[i]->get_type());
                find = _handle_seq[i];
                break;
            }
        }
        handle = atom_rebuild(handle);
    } else {
        HandleSeq handleSeq = handle->getOutgoingSet();
        handleSeq.push_back(atom);
        handle = createLink(handleSeq, handle->get_type());
    }
}

int atomeseRewriting::search_erase_atom(opencog::Handle &source, opencog::Handle &target,
                                        bool is_first) {
    int target_value;
    if (is_first) {
        _handle_seq.clear();
        _type_store.clear();
    }
    bool is_found = false;
    if (content_eq(source, target)) {
        return 0;
    } else {
        if (source->is_link()) {
            HandleSeq handleSeq = source->getOutgoingSet();
            _type_store.insert(make_pair(source->get_type(), handleSeq.size()));
            for (int j=0; j < handleSeq.size(); j++) {
                if (!content_eq(handleSeq[j], target)) {
                    _handle_seq.push_back(handleSeq[j]);
                } else{
                    target_value = j+1;
                    is_found = true;
                    (--_type_store.end())->second -= 1;
                }
            }
            if (!is_found) {
                for (auto sib: handleSeq) {
                    search_erase_atom(sib, target, false);
                    break;
                }
            }
        } else {
            _handle_seq.push_back(source);
        }
    }
    target = _handle_seq[target_value];
    return _handle_seq.size() > 0 ? 1 : -1;
}

void atomeseRewriting::erase_atom(Handle &source, Handle &target) {
    if (search_erase_atom(source, target) == 1) {
        atom_rebuild(source);
    }
}

void atomeseRewriting::flatten_atom(Handle &source, Handle &target) {
    HandleSeq handleSeq;
    if (search_handle(source, target) == 1) {
        for (int i=0; i < _handle_seq.size(); ++i) {
            if (content_eq(_handle_seq[i], target)) {
                HandleSeq sib = _handle_seq[i]->getOutgoingSet();
                for (int j=0; j < sib.size(); j++) {
                    _handle_seq.push_back(sib[j]);
                }
                _handle_seq[i] = createLink(handleSeq, _handle_seq[i]->get_type());
                target = _handle_seq[i];
                (--_type_store.end())->second += sib.size();
                break;
            }
        }
        source = atom_rebuild(source);
    }
}

} // ~namespace moses
} // ~namespace opencog
