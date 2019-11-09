/*

Copyright (c) 2019-2020, Adrien BLASSIAU and Corentin JUVIGNY

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

/** @file greedy.h
 *
 * @brief Greedy algorithm to construct a viable solution of our problem.
 *
 */

#ifndef __GREEDY__
#define __GREEDY__

/**
 * This function applies a greedy algorithm to construct a solution of the
 * problem. At each step, we add a target that communicate with a sensor of
 * the current solution. This target is the one that detect the more target.
 *
 * @param pf								All the data of our problem.
 */
void greedy_construction(TPointFile* pf);

/**
 * This function updates the current solution by transforming a target in
 *  sensor
 *
 * @param pf								All the data of our problem.
 * @param selected_target          			The new sensor.
 * @param sensor_queue 						All the sensors in the
 *                           				communicating radius of the new
 *                           				sensor.
 * @param visited_target_queue 				All the target in the capture
 *                                 			radius of the new sensor.
 * @param new_covered_target_max 			The number of new targets covered.
 */
void maj_pf(TPointFile* pf,TPoint* selected_target, Queue* sensor_queue, 
            Queue* visited_target_queue, int new_covered_target_max);

#endif
