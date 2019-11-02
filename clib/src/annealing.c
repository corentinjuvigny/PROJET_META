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

#include <time.h>
#include <stdlib.h>
#include "rwfile.h"
#include "tools.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "neighbourhood.h"
#include "dfs.h"
#include "annealing.h"

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

double energie_proba_distrib(double delta_E,double T){
	return exp(-delta_E/T);
}

TPoint* random_choice_on_avl(AVLTree* avl_tree){

	AVLTreeValue* data_list = avl_tree_to_array(avl_tree);
	int data_list_length = avl_tree_num_entries(avl_tree);
	int random_index = rand_int(data_list_length-1);
	TPoint* data = (TPoint*)(data_list[random_index]);
	free(data_list);
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

void modify_solution_with_neighbourhood(TPointFile* pf,  TPoint** neighboor_node, int* choice){

	AVLTree* neighbourhood_add = add_node_list(pf);
	AVLTree* neighbourhood_remove = remove_node_list(pf);
	*neighboor_node = random_data_in_neighbourhood(neighbourhood_add,neighbourhood_remove,choice);
	avl_tree_free(neighbourhood_add);
	avl_tree_free(neighbourhood_remove);
	// printf("ADD LIST\n");
	// print_avl_tree(neighbourhood_add,print_node);
	// printf("REMOVE LIST\n");
	// print_avl_tree(neighbourhood_remove,print_node);
	// printf("END LIST\n");
	// draw_data(pf,0.001,10);
	if(*choice == 0){
#if DEBUG
		printf("ADD\n");
		print_node(*neighboor_node);
#endif
		add_node(pf,*neighboor_node);
	}
	else if(*choice == 1){
#if DEBUG
		printf("REMOVE\n");
		print_node(*neighboor_node);
#endif
		remove_node(pf,*neighboor_node);
	}
}

void revert_choice(TPointFile* pf, TPoint** neighboor_node, int* choice){

	if(*choice == 0){
#if DEBUG
		printf("REMOVE\n");
		print_node(*neighboor_node);
#endif
		remove_node(pf,*neighboor_node);
	}
	else if(*choice == 1){
#if DEBUG
		printf("ADD\n");
		print_node(*neighboor_node);
#endif
		add_node(pf,*neighboor_node);
	}
}

void free_best_queue(Queue* best_queue){
	if(best_queue != NULL){
		queue_free(best_queue);
	}
}

void reconstruct_solution(TPointFile* pf, BestSolution* bs){
	Queue *best_solution_queue = bs->best_solution;
	QueueEntry *queue_iterator = best_solution_queue->head;

	while (queue_iterator != NULL) {
		char* point_name = (char *)(queue_iterator->data);
		long int node_name_int = strtol(point_name,NULL,0);

		if(node_name_int != 0){
			add_node(pf,pf->points[node_name_int]);
		}
		queue_iterator = queue_iterator->next;
	}
}

BestSolution* new_best_solution(){
	BestSolution * bs = xmalloc(sizeof(*bs));
	bs->best_solution = NULL;
	bs->size = 0;
	return bs;
}

BestSolution* compress_bs(TPointFile* pf, BestSolution* bs){

	free_best_queue(bs->best_solution);

	Queue* current_best_queue = queue_new();

	int i;
	AVLTree* current_best = pf->solution;
	AVLTreeValue* current_best_list = avl_tree_to_array(current_best);
	int current_best_list_length = avl_tree_num_entries(current_best);

	for(i = 0; i < current_best_list_length; i++){
		TPoint* current_sensor = (TPoint*)(current_best_list[i]);
		queue_push_head(current_best_queue, current_sensor->name);
	}
	free(current_best_list);
	bs->best_solution = current_best_queue;
	bs->size = current_best_list_length;

	return bs;
}

BestSolution* simulated_annealing(TPointFile* pf){

	srand(time(NULL));
	TPoint* neighboor_node;
	int choice = 0;
	int i;
	double phi = 0.999995;
	int step = 2;
	double T_initial = 50.0;
	int stop_criterion = 10;

	BestSolution *bs = new_best_solution();
	int f_x_min = avl_tree_num_entries(pf->solution);
	compress_bs(pf, bs);

	double T = T_initial;

	while(stop_criterion > 0){

#if DEBUG
		printf("\n########## NOUVEAU PALIER %d(CURRENT BEST : %d )##########\n",stop_criterion,f_x_min);
#endif

		for (i = 0; i < step; ++i){

#if DEBUG
			printf("########## ITERATION n°%d ##########\n",i);
#endif

			int f_x = avl_tree_num_entries(pf->solution);
			modify_solution_with_neighbourhood(pf, &neighboor_node, &choice);
			int f_x_p = avl_tree_num_entries(pf->solution);

#if DEBUG
			printf("f_x = %d et f_x_p %d\n",f_x,f_x_p);
#endif

			int delta_E = 10*(f_x_p - f_x);

			if(delta_E <= 0){
				f_x = f_x_p;
				if(f_x < f_x_min){
					compress_bs(pf, bs);
					f_x_min = f_x;

#if DEBUG
					printf("NOUVEAU MINIMUM : %d\n",f_x);
					print_avl_tree(pf->solution,print_node);
#endif

				}
			}
			else{
				double random_d = random_double();
				double energy_p = energie_proba_distrib(delta_E,T);
#if DEBUG
				printf("ENERGY : %f et K : %f\n",energy_p,random_d);
#endif

				if(random_d > energy_p){

#if DEBUG
					printf("ON REVERT\n");
#endif

					revert_choice(pf,&neighboor_node,&choice);
				}
			}
		}
		T = T * phi;

#if DEBUG
		printf("T courante : %f\n",T);
#endif

		stop_criterion--;
	}

#if DEBUG
	printf("FINI\n");
#endif

	return bs;
	// AVLTree* tree_test = add_node_list(pf);
	// printf("Adresse %d\n",&neighboor_node);
	// AVLTreeValue* current_solution_list = avl_tree_to_array(tree_test);
	// int current_solution_list_length = avl_tree_num_entries(tree_test);

	// modify_solution_with_neighbourhood(pf, &neighboor_node, &choice);
	// print_node(neighboor_node);
	// revert_choice(pf,&neighboor_node,&choice);
	// printf("Adresse %d\n",&neighboor_node);
	// print_node(neighboor_node);
	// modify_solution_with_neighbourhood(pf);
	// modify_solution_with_neighbourhood(pf);
	// modify_solution_with_neighbourhood(pf);
}