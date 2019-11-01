#include <time.h>
#include <stdlib.h>
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "neighbourhood.h"
#include "dfs.h"

double random_double()
{
	return (double)rand() / (double)RAND_MAX ;
}

int rand_int(int limit) {
	int divisor = RAND_MAX/(limit+1);
	int retval;

	do {
		retval = rand() / divisor;
	} while (retval > limit);

	return retval;
}

float energie_proba_distrib(float delta_E,float T){

	return exp(-delta_E/T);
}

TPoint* random_choice_on_avl(AVLTree* avl_tree){

	AVLTreeValue* data_list = avl_tree_to_array(avl_tree);
	int data_list_length = avl_tree_num_entries(avl_tree);
	int random_index = rand_int(data_list_length-1);
	TPoint* data = (TPoint*)(data_list[random_index]);

	return data;
}

TPoint* random_data_in_neighbourhood(AVLTree* neighbourhood_add, AVLTree* neighbourhood_remove, int* choice){
	int rand_int_between_0_and_1 = rand_int(1);

	int size_add = avl_tree_num_entries(neighbourhood_add);
	int size_remove = avl_tree_num_entries(neighbourhood_remove);

	if((rand_int_between_0_and_1 || size_remove==0) && size_add>0){
		*choice = 0;
		return random_choice_on_avl(neighbourhood_add);
	}
	else if (size_remove > 0){
		*choice = 1;
		return random_choice_on_avl(neighbourhood_remove);
	}
	else{
		*choice = -1;
		return NULL;
	}
}

void modify_solution_with_neighbourhood(TPointFile* pf){
	int choice = 0;
	AVLTree* neighbourhood_add = add_node_list(pf);
	AVLTree* neighbourhood_remove = remove_node_list(pf);
	TPoint* neighboor_node = random_data_in_neighbourhood(neighbourhood_add,neighbourhood_remove,&choice);
	if(choice == 0){
		printf("ADD\n");
		add_node(pf,neighboor_node);
	}
	else if(choice == 1){
		printf("REMOVE\n");
		remove_node(pf,neighboor_node);
	}
}


void simulated_annealing(TPointFile* pf){
	int i;
	srand(time(NULL));

	AVLTree* tree_test = add_node_list(pf);

	AVLTreeValue* current_solution_list = avl_tree_to_array(tree_test);
	int current_solution_list_length = avl_tree_num_entries(tree_test);

	// modify_solution_with_neighbourhood(pf);
	// modify_solution_with_neighbourhood(pf);
	// modify_solution_with_neighbourhood(pf);
	// modify_solution_with_neighbourhood(pf);
}