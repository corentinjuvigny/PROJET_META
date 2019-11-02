/*

Copyright (c) 2019-20208, Adrien BLASSIAU and Corentin JUVIGNY

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

/** @file dfs.h
 *
 * @brief Depth-first search library to check graph connexity.
 */

#ifndef __DFS__
#define __DFS__

/**
 * This function applies an iteration of a dfs algorithm, using an avl for
 * faster lookup.
 *
 * @param pf          All the data of our problem.
 * @param visited_avl All the already visited points.
 *
 * @param node        A point we want to expore.
 */
void dfs(TPointFile* pf, AVLTree* visited_avl, TPoint* node);

/**
 * This function runs the dfs agorithm on our data.
 *
 * @param  pf             All the data of our problem.
 * @param  removed_target A point we want to remove.
 * @return                The number of points explored.
 *
 */
int run_dfs(TPointFile* pf, TPoint* removed_target);

#endif