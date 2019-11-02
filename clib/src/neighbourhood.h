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

/** @file neighbourhood.h
 *
 * @brief Some neighbourhood for the simulated annealing.
 *
 * We used two neighbourhoods. In the first one, we try to remove a sensor,
 * in the second one, we try to add a sensor.
 */

#ifndef __NEIGHBOURHOOD__
#define __NEIGHBOURHOOD__


/**
 * Function to find all the targets that can be transformed as a sensor. For
 * this, the target need to be on the communication radius of a sensor in the
 * current solution.
 *
 * @param  pf All the data of our problem.
 *
 * @return    The targets we can add as a sensor to our solution.
 */
AVLTree* add_node_list(TPointFile* pf);

/**
 * Add a sensor to the current solution.
 *
 * @param pf         All the data of our problem.
 * @param new_target The target we want to add as a sensor.
 */
void add_node(TPointFile* pf, TPoint* new_target);

/**
 * Function to find all the sensors that can be transformed as a target. For
 * this, we have to keep the connexity by removing the sensor and all the
 * target have to be covered.
 *
 * @param  pf All the data of our problem.
 *
 * @return    The sensors we can remove to our solution.
 */
AVLTree* remove_node_list(TPointFile* pf);

/**
 * Remove a sensor to the current solution.
 *
 * @param pf         All the data of our problem.
 * @param old_sensor The sensor we want to remove.
 */
void remove_node(TPointFile* pf, TPoint* old_sensor);

#endif