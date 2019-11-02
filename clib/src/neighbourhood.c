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

#include <stdlib.h>
#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"
#include "draw.h"
#include "neighbourhood.h"
#include "greedy.h"
#include "dfs.h"

AVLTree* add_node_list(TPointFile* pf){
	int i;
	AVLTree* current_solution_avl = pf->solution;
	AVLTreeValue* current_solution_list = avl_tree_to_array(current_solution_avl);
	int current_solution_list_length = avl_tree_num_entries(current_solution_avl);

	AVLTree* target_avl = avl_tree_new((AVLTreeCompareFunc) point_compare);

	for(i = 0; i < current_solution_list_length; i++){
		TPoint* current_sensor = (TPoint*)(current_solution_list[i]);

		double point_x = current_sensor->x;
		double point_y = current_sensor->y;

		Queue *target_in_radius_queue = current_sensor->communication_queue;
		QueueEntry *queue_iterator = target_in_radius_queue->head;

		while (queue_iterator != NULL) {

		    TPoint* target_in_radius = (TPoint*)(queue_iterator->data);
		    double target_in_radius_x = target_in_radius->x;
		    double target_in_radius_y = target_in_radius->y;
		    PKind target_in_radius_kind = target_in_radius->kind;

			if( (point_x != target_in_radius_x || point_y != target_in_radius_y) && (target_in_radius_kind == K_Target)){
				TPoint* target_in_avl = avl_tree_lookup(target_avl,&(target_in_radius->name));

				if(target_in_avl == NULL){
					avl_tree_insert(target_avl,&(target_in_radius->name), target_in_radius);
				}
			}
			queue_iterator = queue_iterator->next;
		}
	}
	free(current_solution_list);
	return target_avl;
}

void add_node(TPointFile* pf, TPoint* new_target){

	Queue *target_in_communication_radius_queue = new_target->communication_queue;
	Queue *target_in_capture_radius_queue = new_target->capture_queue;
	maj_pf(pf,new_target,target_in_communication_radius_queue,target_in_capture_radius_queue,0);
	avl_tree_insert(pf->solution,&(new_target->name),new_target);
}

AVLTree* remove_node_list(TPointFile* pf){
	int i;
	AVLTree* current_solution_avl = pf->solution;
	AVLTreeValue* current_solution_list = avl_tree_to_array(current_solution_avl);
	int current_solution_list_length = avl_tree_num_entries(current_solution_avl);

	AVLTree* target_avl = avl_tree_new((AVLTreeCompareFunc) point_compare);

	for(i = 0; i < current_solution_list_length; i++){
		TPoint* current_sensor = (TPoint*)(current_solution_list[i]);
		int current_sensor_is_ok = 0;
		double point_x = current_sensor->x;
		double point_y = current_sensor->y;
		PKind target_in_radius_kind = current_sensor->kind;

		if(target_in_radius_kind == K_Sensor){

			Queue *target_in_radius_queue = current_sensor->capture_queue;
			QueueEntry *queue_iterator = target_in_radius_queue->head;
			while (queue_iterator != NULL && current_sensor_is_ok==0) {

			    TPoint* target_in_radius = (TPoint*)(queue_iterator->data);
			    double target_in_radius_x = target_in_radius->x;
			    double target_in_radius_y = target_in_radius->y;
			    PKind target_in_radius_kind = target_in_radius->kind;

				if( (point_x != target_in_radius_x || point_y != target_in_radius_y) && (target_in_radius_kind == K_Target)){

					int target_in_radius_aux_size = avl_tree_num_entries(target_in_radius->aux);
					if(target_in_radius_aux_size <= 1){
						current_sensor_is_ok = 1;
					}
				}
				queue_iterator = queue_iterator->next;
			}
			if(current_sensor_is_ok==0){
				int dfs_result = run_dfs(pf,current_sensor);
				if(dfs_result!=current_solution_list_length-1){
					current_sensor_is_ok=1;
				}
			}
			if(current_sensor_is_ok==0){
				avl_tree_insert(target_avl,&(current_sensor->name), current_sensor);
			}
		}
	}
	free(current_solution_list);
	return target_avl;
}

void remove_node_communication_to_sensor(TPoint* old_sensor)
{
	int i;
	AVLTree* sensor_aux = old_sensor->aux;
	AVLTreeValue* sensor_aux_list = avl_tree_to_array(sensor_aux);
	int sensor_aux_list_length = avl_tree_num_entries(sensor_aux);

	for(i = 0; i < sensor_aux_list_length; i++){
		TPoint* current_sensor = (TPoint*)(sensor_aux_list[i]);
		avl_tree_remove(current_sensor->aux,&(old_sensor->name));
	}
	free(sensor_aux_list);
}

void remove_node_capture_to_sensor(TPoint* old_sensor)
{
	Queue *old_sensor_capture_queue = old_sensor->capture_queue;
	QueueEntry *queue_iterator = old_sensor_capture_queue->head;

	double point_x = old_sensor->x;
	double point_y = old_sensor->y;

	double target_x;
	double target_y;
	PKind target_kind;

	while (queue_iterator != NULL) {

		TPoint* target = (TPoint*)(queue_iterator->data);
		target_x = target->x;
	    target_y = target->y;
	    target_kind = target->kind;

	    if( (point_x != target_x || point_y != target_y) && (target_kind == K_Target)){
	    	avl_tree_remove(target->aux,&(old_sensor->name));
		}
		queue_iterator = queue_iterator->next;
	}
}

void set_new_capture_list(TPoint* old_sensor)
{
	Queue *old_sensor_capture_queue = old_sensor->capture_queue;
	QueueEntry *queue_iterator = old_sensor_capture_queue->head;

	double point_x = old_sensor->x;
	double point_y = old_sensor->y;

	double sensor_x;
	double sensor_y;
	PKind sensor_kind;
	AVLTree *avl_tree = avl_tree_new((AVLTreeCompareFunc) point_compare);

	while (queue_iterator != NULL) {
		TPoint* sensor = (TPoint*)(queue_iterator->data);
		sensor_x = sensor->x;
	    sensor_y = sensor->y;
	    sensor_kind = sensor->kind;

	    if( (point_x != sensor_x || point_y != sensor_y) && (sensor_kind == K_Sensor)){
	    	avl_tree_insert(avl_tree,&(sensor->name),sensor);
		}
		queue_iterator = queue_iterator->next;
	}
	old_sensor->kind = K_Target;
	avl_tree_free(old_sensor->aux);
	old_sensor->aux = avl_tree;
}

void remove_node(TPointFile* pf, TPoint* old_sensor){
	if (old_sensor->kind == K_Sensor){
		avl_tree_remove(pf->solution,&(old_sensor->name));
		remove_node_communication_to_sensor(old_sensor);
		remove_node_capture_to_sensor(old_sensor);
		set_new_capture_list(old_sensor);
	}
}