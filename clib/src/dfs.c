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

#include <stdlib.h>
#include <stdio.h>
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "neighbourhood.h"


void dfs(TPointFile* pf, AVLTree* visited_avl, TPoint* node)
{
	TPoint* node_in_avl = avl_tree_lookup(visited_avl,&(node->name));
	if(node_in_avl == NULL) {
		avl_tree_insert(visited_avl,&(node->name), node);
		int i;
		AVLTreeValue* node_aux_list = avl_tree_to_array(node->aux);
		int node_aux_avl_length = avl_tree_num_entries(node->aux);

		for(i = 0; i < node_aux_avl_length; i++) {
			TPoint* neighbour = (TPoint*)(node_aux_list[i]);
			dfs(pf,visited_avl,neighbour);
		}
		free(node_aux_list);
	}
}

int run_dfs(TPointFile* pf, TPoint* removed_target)
{
	int remove_one = 0;
	AVLTree* visited_avl = avl_tree_new((AVLTreeCompareFunc) point_compare);
	if(removed_target != NULL){
		avl_tree_insert(visited_avl,&(removed_target->name), removed_target);
		remove_one = 1;
	}

	TPoint* first_node = pf->points[0];

	dfs(pf,visited_avl,first_node);
	int visited_avl_size = avl_tree_num_entries(visited_avl)-remove_one;
	avl_tree_free(visited_avl);

  return visited_avl_size;
}
