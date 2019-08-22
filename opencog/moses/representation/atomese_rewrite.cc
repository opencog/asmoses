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


bool atomeseRewriting::search_handle(Handle &source, Handle target,
                                    bool is_first) {

    if (is_first) {
        _handle_seq.clear();
        _type_store.clear();
        is_found = false;
    }

    if (source->is_link()) {
        HandleSeq handleSeq = source->getOutgoingSet();
        _type_store.push_back(make_pair(source->get_type(), handleSeq.size()));
        for (auto sib: handleSeq) {
            if (content_eq(sib, target)){
                is_found = true;
            }
            _handle_seq.push_back(sib);
        }

        for (auto sib: handleSeq) {
            if (is_found) {
                break;
            } else {
                search_handle(sib, target, false);
            }
        }

    } else {
        if (content_eq(source, target)) {
            is_found = true;
            _handle_seq.push_back(source);
        }
    }

    return is_found;
}

Handle atomeseRewriting::atom_rebuild(opencog::Handle &handle) {

    std::vector<pair<Type, int>>::reverse_iterator rit;
    int handleSeq_size = _handle_seq.size();
    HandleSeq handle_seq;

    for (rit = _type_store.rbegin(); rit != _type_store.rend(); ++rit) {
        int limit = handleSeq_size - rit->second - 1;
        if (handleSeq_size > -1) {
            for (int j = handleSeq_size - 1; j > limit; --j) {
                handle_seq.push_back(_handle_seq[j]);
            }
            handleSeq_size = limit + 1;
            if (limit >= 0) {
                _handle_seq[limit] = createLink(handle_seq, rit->first);
            } else {
                handle = createLink(handle_seq, rit->first);
                return handle;
            }
            handle_seq.clear();
        }
    }
}


Handle atomeseRewriting::handle_replace(opencog::Handle &source, opencog::Handle target,
                                      Handle& replace) {
    Handle result;
    if (search_handle(source, target)) {
        for (int i = 0; i < _handle_seq.size(); i++) {
            if(content_eq(target, _handle_seq[i])) {
                HandleSeq handleSeq = _handle_seq[i]->getOutgoingSet();
                Type type = replace->get_type();
                _handle_seq[i] = createLink(handleSeq, type);
                result = _handle_seq[i];
                break;
            }
        }
        atom_rebuild(source);
    } else {
        OC_ASSERT(false, "Unable to find handle");
    }
    return result;
}

Handle atomeseRewriting::insert_atom_above(opencog::Handle &source, opencog::Handle target,
                                           opencog::Type insert_type) {
    Handle result;
    if (content_eq(source, target)) {
        source = createLink(insert_type, source);
        result = source;
        return result;
    }
    if (search_handle(source, target)) {
        for (int i=0; i < _handle_seq.size(); i++) {
            if (content_eq(_handle_seq[i], target)) {
                _handle_seq[i] = createLink(insert_type, _handle_seq[i]);
                result = _handle_seq[i];
                break;
            }
        }
        atom_rebuild(source);
    } else {
        OC_ASSERT(false, "Unable to find handle");
    }
    return result;
}

Handle atomeseRewriting::append_atom_below(opencog::Handle &handle, opencog::Handle target,
                                         opencog::Handle &atom) {
    Handle result;
    if (content_eq(handle, target)) {
        HandleSeq handleSeq = handle->getOutgoingSet();
        handleSeq.push_back(atom);
        handle = createLink(handleSeq, handle->get_type());
        result = atom;
    } else {
        if (search_handle(handle, target)) {
            for (int i = 0; i < _handle_seq.size(); ++i) {
                if (content_eq(_handle_seq[i], target)) {
                    HandleSeq handleSeq = _handle_seq[i]->getOutgoingSet();
                    handleSeq.push_back(atom);
                    _handle_seq[i] = createLink(handleSeq, _handle_seq[i]->get_type());
                    result = _handle_seq[i];
                    break;
                }
            }
            if (!_handle_seq.empty()) {
                handle = atom_rebuild(handle);
            } else {
                OC_ASSERT(false, "Unable to find handle");
            }

        } else {
            OC_ASSERT(false, "Unable to find handle");
        }
    }

    return result;
}



Handle atomeseRewriting::erase_atom(Handle &source, Handle target) {
    bool erased = false;
    bool removed = true;
    Handle result;
    if (search_handle(source, target)) {
        for (int i = 0; i < _handle_seq.size(); i++) {
            if(content_eq(target, _handle_seq[i])) {
                erased = true;
                std::vector<pair<Type, int>>::iterator it;
                int value = 0;
                for (it = _type_store.begin(); it != _type_store.end(); ++it) {
                    value += it->second;
                    if (value > i) {
                        it->second -= 1;
                        removed = true;
                    }
                }
            }
            if (i == _handle_seq.size()-1) {
                _handle_seq.erase(_handle_seq.begin() + i);
            }
            else {
                if (erased ) {
                    if (removed) {
                        if (_handle_seq[i]->is_link()){
                            HandleSeq handleSeq = _handle_seq[i]->getOutgoingSet();
                            if (!handleSeq.empty()) {
                                result = handleSeq[0]->get_handle();
                            } else if (i >= 1) {
                                result = _handle_seq[i-1];
                            } else {
                                result = _handle_seq[i+1];
                            }
                        } else {
                            result = _handle_seq[i];
                        }
                        removed = false;
                    }
                    _handle_seq[i] = _handle_seq[i+1];
                }
            }

        }
        atom_rebuild(source);
    } else {
        OC_ASSERT(false, "Unable to find handle");
    }
    return result;
}

Handle atomeseRewriting::flatten_atom(Handle &source, Handle target) {
    Handle result;

    HandleSeq handleSeq;

    if (search_handle(source, target)) {
        for (int i = 0; i < _handle_seq.size(); i++) {
            if(content_eq(target, _handle_seq[i])) {
                HandleSeq outgoing = _handle_seq[i]->getOutgoingSet();
                for (auto sib: outgoing) {
                    _handle_seq.push_back(sib);
                }
                _handle_seq[i] = createLink(handleSeq, _handle_seq[i]->get_type());
                result = _handle_seq[i];
                std::vector<pair<Type, int>>::iterator it;
                int value = 0;
                for (it = _type_store.begin(); it != _type_store.end(); ++it) {
                    value += it->second;
                    if (value > i) {
                        it->second += outgoing.size();
                        break;
                    }
                }
                break;
            }
        }
        source = atom_rebuild(source);
    } else {
        OC_ASSERT(false, "Unable to find handle");
        }
    return result;
}

} // ~namespace moses
} // ~namespace opencog
