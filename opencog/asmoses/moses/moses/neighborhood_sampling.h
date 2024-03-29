/** neighborhood_sampling.h ---
 *
 * Copyright (C) 2010 OpenCog Foundation
 *
 * Author: Moshe Looks, Nil Geisweiller, Xiaohui Liu
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
#ifndef _OPENCOG_NEIGHBORHOOD_SAMPLING_H
#define _OPENCOG_NEIGHBORHOOD_SAMPLING_H

#include <iostream>
#include <algorithm>
#include <limits>

#include <boost/math/special_functions/binomial.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <opencog/util/dorepeat.h>
#include <opencog/util/lazy_random_selector.h>

#include "opencog/asmoses/moses/eda/initialization.h"
#include "opencog/asmoses/moses/representation/instance_set.h"
#include "types.h"

namespace opencog { namespace moses {

/**
 * This procedure generates the initial deme randomly
 *
 * @param fs  the deme
 * @param n   the size of deme
 * @param out deme iterator (where to store the instance)
 * @param end deme iterator, containing the end iterator,
 *            necessary to check that 'out' does not go out
 *            of the deme if it does so then an assert is raised
 */
template<typename Out>
void generate_initial_sample(const field_set& fs, int n, Out out, Out end)
{
    dorepeat(n) {

        instance inst(fs.packed_width());

        randomize(fs, inst);

        // Bias towards the exemplar instance
        for (field_set::bit_iterator it = fs.begin_bit(inst);
             it != fs.end_bit(inst); ++it)
            if (randGen().randbool())
                *it = false;
        for (field_set::disc_iterator it = fs.begin_disc(inst);
             it != fs.end_disc(inst); ++it)
            if (randGen().randbool())
                *it = 0;

        //add it
        OC_ASSERT(out != end);  // to avoid invalid memory write
        *out++ = inst;
    }
}

// Flip Left and Right
void flip_LR(field_set::disc_iterator itr);

// Twiddle one contin bit only, according to the following rules
//
// 1. If it is Stop then select Left or Right
//
// 2. Otherwise if the next one is Stop (or invalid) then select Stop
// or flip
//
// 3. Otherwise just flip
void twiddle_contin_bit(field_set::disc_iterator itr,
                        field_set::disc_iterator next_itr,
                        opencog::RandGen& rng = randGen());

/**
 * This routine modifies the instance 'inst' so that the new instance
 * has the n'th-from-last significant bit changed.
 *
 * For example, if the contin[it.idx()] is encoded with depth = 4,
 * as [L R S S], then the neighbors worth considering are [L S S S],
 * (a decrease of significant bits), [L L S S] (same number of bits,
 * but direction changed), [L R L S], and [L R R S] (increase the
 * number of significant bits).  This routine will randomly chose one
 * of these four possibiliities.
 *
 * @todo: in order to better approximate the real-number metric, we
 * should also consider the neighbor [LRRRRR..] of [RLLLLL...]
 *
 * @param fs   deme
 * @param inst the instance to be modified
 * @param it   the contin iterator pointing into the instance
 * @param dist the distance to consider.
 */
void generate_contin_neighbor(const field_set& fs,
                              instance& inst,
                              field_set::contin_iterator it,
                              unsigned dist,
                              opencog::RandGen& rng = randGen());

/**
 * Sample (with replacement) 'sample_size' instances from the
 * neighborhood that surrounds the central instance, at a distance
 * 'dist' from the center. That is, sample 'sample_size' instances
 * which have 'dist' different knob settings than the given instance.
 *
 * @todo: term algebra fields are ignored for now
 *
 * @param fs            deme
 * @param dist          distance
 * @param sample_size   number of instances to be generated
 * @param out           deme where to store the instances
 * @param end           deme iterator, specifying the end of the deme.
 *                      Used to check that 'out' does not go out of
 *                      bounds.  If it does, an assert is raised.
 * @param center_inst   the center instance
 */
template<typename Out>
void sample_from_neighborhood(const field_set& fs, unsigned dist,
                              unsigned sample_size, Out out, Out end,
                              const instance & center_inst)
{
    OC_ASSERT(center_inst.size() == fs.packed_width(),
              "Please make sure that the center_inst"
              " have the same size with the field_set");

    unsigned dim = fs.dim_size();

    OC_ASSERT(dist <= dim,
              "the sampling distance %u"
              " cannot be greater than the field dimension %u", dist, dim);

    dorepeat(sample_size) {

        instance new_inst(center_inst);
        lazy_random_selector select(dim, randGen());

        for (unsigned i = 1; i <= dist; ) {
            size_t r = select();
            // modify bit
            if (r < fs.n_bits()) {
                field_set::bit_iterator itb = fs.begin_bit(new_inst);
                itb += r;
                *itb = !(*itb);
                i++;
            // modify disc
            } else if (r >= fs.n_bits() && (r < (fs.n_bits() + fs.n_disc_fields()))) {
                field_set::disc_iterator itd = fs.begin_disc(new_inst);
                itd += r - fs.n_bits();
                disc_t temp = 1 + randGen().randint(itd.multy() - 1);
                if ( *itd == temp)
                    *itd = 0;
                else
                    *itd = temp;
                i++;
            // modify contin
            } else if ( r >= (fs.n_bits() + fs.n_disc_fields())) {
                field_set::contin_iterator itc = fs.begin_contin(new_inst);
                //cout << "i = " << i << "  r = " << r << endl;
                itc += r - fs.n_bits() - fs.n_disc_fields();
                // @todo: now the distance is 1, choose the distance
                // of contin possibly different than 1
                generate_contin_neighbor(fs, new_inst, itc, 1);
                i++;
            }
        }
        OC_ASSERT(out != end); // to avoid invalid memory write
        *out++ = new_inst;
        // cout << "********** Added instance:" << fs.to_string(new_inst) << endl;
    }
}

/**
 * Generates instances at distance 'dist' from an instance considered
 * as center (often the exemplar but not necessarily).
 * It calls a recursive function vary_n_knobs which varies
 * instance fields one by one (in all possible ways)
 *
 * @param fs            deme
 * @param dist          distance
 * @param out           deme (where to store the instances)
 * @param end           deme iterator, containing the end iterator,
 *                      necessary to check that 'out' does not go out
 *                      of the deme if it does so then an assert is raised
 * @param center_inst   the center instance
 */
template<typename Out>
void generate_all_in_neighborhood(const field_set& fs, unsigned dist,
                                  Out out, Out end,
                                  const instance& center_inst)
{
    OC_ASSERT(center_inst.size() == fs.packed_width(),
              "the size of center_instance should be equal to the width of fs");
    vary_n_knobs(fs, center_inst, dist, 0, out, end);
}


/**
 * Generates instances at distance 'dist' from the exemplar
 * (i.e., with n elements changed from 0 from the exemplar)
 * It calls a recursive function vary_n_knobs which varies
 * instance fields one by one (in all possible ways)
 *
 * @param fs   deme
 * @param dist distance
 * @param out  deme (where to store the instances)
 * @param end  deme iterator, containing the end iterator,
 *             necessary to check that 'out' does not go out
 *             of the deme if it does so then an assert is raised
 */
template<typename Out>
void generate_all_in_neighborhood(const field_set& fs,
                                  unsigned dist, Out out, Out end)
{
    instance inst(fs.packed_width());
    generate_all_in_neighborhood(fs, dist, out, end, inst);
}

/**
 * vary_n_knobs -- Generate all possible instances, centered on 'inst',
 * up to a distance 'dist' from 'inst'.  This will vary all knobs that
 * have a raw index equal or greater than the 'starting_index', in
 * all possible ways.
 *
 * The current algorithm is recursive: it explores all possible settings
 * by recursing on 'dist' and on the field 'starting_index'.  Recursion
 * terminates when 'dist' drops to zero, and when 'starting_index' moves
 * past the maximum number of fields.
 *
 * The deme must be large enough to hold all generated instances.
 *
 * This function is the main work-horse for generate_all_in_neighborhood().
 *
 * XXX TODO: the current algo could be speeded up a fair bit, cutting
 * out some of the if tests, and the final recursive call.
 *
 * @todo: term algebra is ignored for the moment.
 *
 * @param fs              deme
 * @param inst            exemplar
 * @param dist            distance
 * @param starting_index  position of a field to be varied
 * @param out             deme iterator (where to store the instances)
 * @param end             deme iterator, containing the end iterator.
 *                        This is used to verify that the write to the deme
 *                        does not go out of bounds.
 * @return the out iterator pointing to the element after the last insertion
 */
template<typename Out>
Out vary_n_knobs(const field_set& fs,
                 const instance& inst,
                 unsigned dist,
                 unsigned starting_index,
                 Out out, Out end)
{
    // Record the current instance.
    if (dist == 0) {
        OC_ASSERT(out != end);  // to avoid invalid memory write
        *out++ = inst;
        return out;
    }

    instance tmp_inst = inst;

    // term knobs.
    if ((fs.begin_term_raw_idx() <= starting_index) &&
        (starting_index < fs.end_term_raw_idx()))
    {
        // @todo: handle term algebras XXX
        out = vary_n_knobs(fs, tmp_inst, dist,
                           starting_index + fs.end_term_raw_idx(),
                           out, end);
    }
    // contin knobs
    else
    if ((fs.begin_contin_raw_idx() <= starting_index) &&
        (starting_index < fs.end_contin_raw_idx()))
    {
        // Modify the contin knob pointed by itr, then recurse on
        // starting_index and dist.
        field_set::contin_iterator itc = fs.begin_contin(tmp_inst);
        size_t contin_idx = fs.raw_to_contin_idx(starting_index);
        itc += contin_idx;

        // The 'depth' is the max possible size of this contin field,
        // while length is the actual size, for this instance.
        size_t depth = fs.contin()[itc.idx()].depth;
        size_t length = fs.contin_length(tmp_inst, contin_idx);

        field_set::disc_iterator itr = fs.begin_raw(tmp_inst);
        itr += starting_index;
        size_t relative_raw_idx = starting_index - fs.contin_to_raw_idx(contin_idx);

        // case tmp_inst at itr is Stop
        if (*itr == field_set::contin_spec::Stop) {
            // Assume that this is the first stop encountered for this
            // contin field.  Skip straight to the next contin field.
            size_t next_contin = starting_index + depth - relative_raw_idx;
            out = vary_n_knobs(fs, tmp_inst, dist, next_contin, out, end);

            // Turn this stop pseudo-bit to Left, and Right, and recurse
            // to next.
            *itr = field_set::contin_spec::Left;
            out = vary_n_knobs(fs, tmp_inst, dist - 1, starting_index + 1, out, end);
            *itr = field_set::contin_spec::Right;
            out = vary_n_knobs(fs, tmp_inst, dist - 1, starting_index + 1, out, end);
        }
        // case tmp_inst at itr is Left or Right
        else
        {
            // Recursive call, moved for one position
            // XXX TODO, unroll the last tail call, just like the single-bit
            // knob case, below.
            out = vary_n_knobs(fs, tmp_inst, dist, starting_index + 1, out, end);
            // Left<->Right
            *itr = field_set::contin_spec::switchLR(*itr);
            out = vary_n_knobs(fs, tmp_inst, dist - 1, starting_index + 1, out, end);

            // Set all remaining 'pseudo-bits' in this contin field to Stop,
            // remRLs is the number of remaining non-Stop pseudo-bits,
            // including this one.
            unsigned remRLs = length - relative_raw_idx;
            if (remRLs <= dist) {
                for(; relative_raw_idx < length; --length, ++itr) {
                    // Stop
                    *itr = field_set::contin_spec::Stop;
                }
                // Skip straight to the next contin field.
                size_t next_contin = starting_index + depth - relative_raw_idx;
                out = vary_n_knobs(fs, tmp_inst, dist - remRLs, next_contin,
                                   out, end);
            }
        }
    }

    // Discrete knobs
    else
    if ((fs.begin_disc_raw_idx() <= starting_index) &&
        (starting_index < fs.end_disc_raw_idx()))
    {
        field_set::disc_iterator itd = fs.begin_disc(tmp_inst);
        itd += fs.raw_to_disc_idx(starting_index);

#define UNROLL_TAIL_CALL_DISC 1
#ifdef UNROLL_TAIL_CALL_DISC
        // For distance == 1, it will be much faster and easier if we
        // just take the tail call and explicitly turn it into a loop.
        // (This avoids insanely deep stacks, too.)
        // Note, however, the recursive call starts at end, and works
        // backwards, so this loop is not exactly the same...
        if (1 == dist) {
            unsigned end_idx = fs.end_disc_raw_idx();

            // Recursive call, do the bit knobs first (just like below).
            out = vary_n_knobs(fs, tmp_inst, dist, end_idx, out, end);

            for ( ; starting_index < end_idx; starting_index++) {
                OC_ASSERT(out != end, "Write past end of array!");

                disc_t tmp_val = *itd;
                for (unsigned i = 1; i <= itd.multy() - 1; ++i) {
                    // Vary to all legal values.  The neighborhood should
                    // not equal to itself, so if it is same, set it to 0.
                    if (tmp_val == i)
                        *itd = 0;
                    else
                        *itd = i;
                    *out++ = tmp_inst; // record the resulting inst.
                }
                *itd = tmp_val;        // put the bit back.
                itd ++;                // move to the next disc.
            }
            return out;
        }
#endif
        // Recursive call, moved for one position.
        // Note this steps past all the disc knobs, and goes to do the 
        // the single-bit knobs.  Only after returning from that,
        // does it do the disc knobs, below.
        out = vary_n_knobs(fs, tmp_inst, dist, starting_index + 1, out, end);

        // Modify the disc and recursive call, moved for one position
        disc_t tmp_val = *itd;
        for (unsigned i = 1; i <= itd.multy() - 1; ++i) {
            // Vary to all legal values.  The neighborhood should
            // not equal to itself, so if it is same, set it to 0.
            if (tmp_val == i)
                *itd = 0;
            else
                *itd = i;
            out = vary_n_knobs(fs, tmp_inst, dist - 1, starting_index + 1, out, end);
        }
    }
    // Single-bit knobs
    else
    if ((fs.begin_bit_raw_idx() <= starting_index) &&
        (starting_index < fs.end_bit_raw_idx()))
    {
        field_set::bit_iterator itb = fs.begin_bit(tmp_inst);
        itb += starting_index - fs.begin_bit_raw_idx();

#define UNROLL_TAIL_CALL 1
#ifdef UNROLL_TAIL_CALL
        // For distance == 1, it will be much faster and easier if we
        // just take the tail call and explicitly turn it into a loop.
        // (This avoids insanely deep stacks, too.)
        // Note, however, the recursive call starts at end, and works
        // backwards, so this loop is not exactly the same...
        if (1 == dist) {
            unsigned end_idx = fs.end_bit_raw_idx();
            for ( ; starting_index < end_idx; starting_index++) {
                OC_ASSERT(out != end, "Write past end of array!");
                *itb = !(*itb);       // change one bit.
                *out++ = tmp_inst;    // record the resulting inst.
                *itb = !(*itb);       // put the bit back.
                itb ++;               // move to the next bit.
            }
            return out;
        }
#endif
        // Recursive call, moved for one position.
        out = vary_n_knobs(fs, tmp_inst, dist, starting_index + 1, out, end);

        // Modify tmp_inst at itb, change to the opposite value.
        *itb = !(*itb);

        // Recursive call, moved for one position.
        out = vary_n_knobs(fs, tmp_inst, dist - 1, starting_index + 1, out, end);
    }
    else
    {
        // The current recursive algo used here will over-run the end
        // of the array by one. This is normal. Do nothing. If 'dist'
        // was zero, the very first statement at top takes care of
        // things for us.
    }
    return out;
}

/**
 * safe_binomial_coefficient -- compute the binomial coefficient
 *
 * This is algo is "safe" in that it attempts to deal with numeric
 * overflow in computing the binomial coefficient, so that overflows
 * are clamped to max, instead of wrapping over, or throwing.
 */
size_t safe_binomial_coefficient(unsigned k, unsigned n);

/**
 * Count the number of neighbors surrounding instance 'inst', at a
 * distance of 'dist' or less, starting with knob 'starting_index'.
 *
 * Given an instance 'inst', the 'neighborhood at distance one' is the
 * set of all instances with one changed knob setting.  The set with
 * two changed knob settings is the neighborhood at distance two, etc.
 *
 * This routine is implemented with a recursive algorithm, and so
 * counting starts with knobs numbered with 'starting_index' or higher.
 * To avoid wasting cpu cycles, counting is stopped when 'max_count'
 * is exceeded.
 *
 * XXX/TODO: the performance of this thing can be strongly improved
 * by looping on the tail-call, just as in the xxx routine...
 *
 * @param fs              deme
 * @param inst            centeral instance
 * @param dist            distance
 * @param starting_index  position of a field to be varied
 * @param max_count       stop counting once this value is reached.
 */
size_t count_neighborhood_size_from_index(const field_set& fs,
                                          const instance& inst,
                                          unsigned dist,
                                          unsigned starting_index,
                                          size_t max_count
                                          = std::numeric_limits<size_t>::max());

/**
 * Count the number of neighbors surrounding instance 'inst', at a
 * distance of 'dist' or less.
 *
 * Given an instance 'inst', the 'neighborhood at distance one' is the
 * set of all instances with one changed knob setting.  The set with
 * two changed knob settings is the neighborhood at distance two, etc.
 *
 * To avoid wasting cpu cycles, counting is stopped when 'max_count'
 * is exceeded.
 *
 * @param fs              deme
 * @param inst            central instance
 * @param dist            distance
 * @param max_count       stop counting when the count exceeds this value.
 * @return                the size of the nieghborhood.
 */
size_t count_neighborhood_size(const field_set& fs,
                               const instance& inst,
                               unsigned dist,
                               size_t max_count
                               = std::numeric_limits<size_t>::max());

// For backward compatibility, like above but with null instance
size_t count_neighborhood_size(const field_set& fs,
                               unsigned dist,
                               size_t max_count
                               = std::numeric_limits<size_t>::max());

/// Fill the deme with at most number_of_new_instances, at distance
/// dist.  Return the actual number of new instances created (this
/// number is bounded by the possible neighbors at distance dist).
size_t sample_new_instances(size_t total_number_of_neighbours,
                            size_t number_of_new_instances,
                            size_t current_number_of_instances,
                            const instance& center_inst,
                            instance_set<composite_score>& deme,
                            unsigned dist);

/// Just like the above, but computes total_number_of_neighbours
/// instead of taking it argument.
size_t sample_new_instances(size_t number_of_new_instances,
                            size_t current_number_of_instances,
                            const instance& center_inst,
                            instance_set<composite_score>& deme,
                            unsigned dist);

} // ~namespace moses
} // ~namespace opencog

#endif // _OPENCOG_NEIGHBORHOOD_SAMPLING_H
