#include <stdlib.h>
#include <stdio.h>
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "neighbourhood.h"


void dfs(TPointFile* pf, AVLTree* visited_avl, TPoint* node){
	TPoint* node_in_avl = avl_tree_lookup(visited_avl,&(node->name));
	if(node_in_avl == NULL){
		avl_tree_insert(visited_avl,&(node->name), node);
		int i;
		AVLTreeValue* node_aux_list = avl_tree_to_array(node->aux);
		int node_aux_avl_length = avl_tree_num_entries(node->aux);

		for(i = 0; i < node_aux_avl_length; i++){
			TPoint* neighbour = (TPoint*)(node_aux_list[i]);
			dfs(pf,visited_avl,neighbour);
		}
		free(node_aux_list);
	}
}

int run_dfs(TPointFile* pf, TPoint* removed_target){

	AVLTree* visited_avl = avl_tree_new((AVLTreeCompareFunc) point_compare);
	if(removed_target != NULL){
		avl_tree_insert(visited_avl,&(removed_target->name), removed_target);
	}

	TPoint* first_node = pf->points[0];

	dfs(pf,visited_avl,first_node);
	int visited_avl_size = avl_tree_num_entries(visited_avl)-1;
	avl_tree_free(visited_avl);
	return visited_avl_size;
}


