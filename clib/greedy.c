#include "rwfile.h"
#include "point.h"
#include "dist.h"
#include "avl.h"
#include "queue.h"


void set_new_sensor(TPoint* selected_target,Queue* sensor_queue){
	QueueEntry *queue_iterator = sensor_queue->head;
	AVLTree *avl_tree = avl_tree_new((AVLTreeCompareFunc) point_compare);
	printf("=>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>E\n");
	while (queue_iterator != NULL) {
		printf("toto\n");
		TPoint* sensor = (TPoint*)(queue_iterator->data);

		avl_tree_insert(avl_tree,&(sensor->name), sensor);
		queue_iterator = queue_iterator->next;
	}
	printf("=>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>S\n");
	selected_target->kind = K_Sensor;
	selected_target->aux = avl_tree;
}

void set_cover(TPointFile* pf, int new_covered_target_max){
	pf->cover -= new_covered_target_max;
}

void set_sensor_new_communication(TPoint* selected_target, Queue* sensor_queue){
	QueueEntry *queue_iterator = sensor_queue->head;

	while (queue_iterator != NULL) {
		TPoint* sensor = (TPoint*)(queue_iterator->data);
		AVLTree *avl_tree_sensor = sensor->aux;
		avl_tree_insert(avl_tree_sensor,&(selected_target->name), selected_target);
		queue_iterator = queue_iterator->next;
	}
}

void set_target_new_capture_sensor(TPoint* selected_target, Queue* visited_target_queue){
	QueueEntry *queue_iterator = visited_target_queue->head;

	double point_x = selected_target->x;
	double point_y = selected_target->y;

	double target_x;
	double target_y;
	char * target_name;
	PKind target_kind;

	while (queue_iterator != NULL) {
		TPoint* target = (TPoint*)(queue_iterator->data);
		target_x = target->x;
	    target_y = target->y;
	    target_name = target->name;
	    target_kind = target->kind;
	    if( (point_x != target_x || point_y != target_y) && (target_kind == K_Target)){
			avl_tree_insert(target->aux,&(selected_target->name), selected_target);
	    }
	    queue_iterator = queue_iterator->next;
	}
}


void maj_pf(TPointFile* pf,TPoint* selected_target, Queue* sensor_queue, Queue* visited_target_queue, int new_covered_target_max){

	set_new_sensor(selected_target,sensor_queue);
	set_cover(pf,new_covered_target_max);
	set_sensor_new_communication(selected_target,sensor_queue);
	set_target_new_capture_sensor(selected_target,visited_target_queue);
}


void find_best_target(TPointFile* pf, TPoint* point, TPoint** selected_target, Queue** visited_target_queue, int* new_covered_target_max, AVLTree** visited_target_avl){

	// printf("### ON TRAITE ###\n");
	// print_node(point);
	// printf("comm_queue : \n");
	// print_queue(point->communication_queue,print_node);
	// printf("########################\n");

	double communication_radius = pf->communication_radius;
	double capture_radius = pf->capture_radius;
	double point_x = point->x;
	double point_y = point->y;
	Queue *target_in_radius_queue = point->communication_queue;
	QueueEntry *queue_iterator = target_in_radius_queue->head;

	Queue *covered_target_in_radius_queue;
	QueueEntry *queue_iterator_2;

	int visit_node = 0;
	int new_covered_target;

	TPoint* covered_target_in_radius;
	double covered_target_in_radius_x;
	double covered_target_in_radius_y;
	char * covered_target_in_radius_name;
	PKind covered_target_in_radius_kind;
	AVLTree* covered_target_in_radius_aux;

	/* We iterate on all targets */
	while (queue_iterator != NULL) {
	    TPoint* target_in_radius = (TPoint*)(queue_iterator->data);
	    double target_in_radius_x = target_in_radius->x;
	    double target_in_radius_y = target_in_radius->y;
	    AVLTree* target_in_radius_aux = target_in_radius->aux;
	    char * target_in_radius_name = target_in_radius->name;
	    PKind target_in_radius_kind = target_in_radius->kind;
	    if( (point_x != target_in_radius_x || point_y != target_in_radius_y) && (target_in_radius_kind == K_Target)){

	  //   	printf("### ON VISITE %d###\n",visited_target_avl == NULL ? 0 : avl_tree_num_entries(*visited_target_avl));
			// print_node(target_in_radius);
			// printf("########################\n");

	    	visit_node = 0;
	    	new_covered_target = 0;
	    	/* If we use an AVL*/
	    	if(visited_target_avl != NULL){
	    		/* We look if we have already visited the current target */
	    		Queue *point_queue = avl_tree_lookup(*visited_target_avl,&(target_in_radius->name));

	    		/* We never have visited the current target */
	    		if (point_queue == NULL){
	    			/* We add the target in the avl (his name is the key), the
	    			 content his a stack in which we put the point*/
	    			Queue *queue = queue_new();
	    			queue_push_head(queue, point);

	    			avl_tree_insert(*visited_target_avl,&(target_in_radius->name), queue);
				}
				else{
					// printf("On poursuit PAS...\n");
					queue_push_head(point_queue, point);
					visit_node = 1;
					// printf("QUEUE\n");
					// print_queue(point_queue,print_node);
					// printf("QUEUE\n");
				}
	    	}
	    	if(visit_node==0){
	    		// printf("On poursuit ...\n");
	    		Queue *covered_target_in_radius_queue = target_in_radius->capture_queue;
	    		QueueEntry *queue_iterator_2 = covered_target_in_radius_queue->head;
	    		// printf("~~~~~~~~~~~~~~~~\n");
	    		while (queue_iterator_2 != NULL) {

	    			covered_target_in_radius = (TPoint*)(queue_iterator_2->data);

	    			covered_target_in_radius_x = covered_target_in_radius->x;
					covered_target_in_radius_y = covered_target_in_radius->y;
					covered_target_in_radius_name = covered_target_in_radius->name;
					covered_target_in_radius_kind = covered_target_in_radius->kind;
					covered_target_in_radius_aux = covered_target_in_radius->aux;
					if( (target_in_radius_x != covered_target_in_radius_x || target_in_radius_y != covered_target_in_radius_y) && (covered_target_in_radius_kind == K_Target)){

						// print_node(covered_target_in_radius);
						if (avl_tree_num_entries(covered_target_in_radius_aux) == 0){
							// printf("=> new\n");
	    					new_covered_target++;
	    				}
					}

	    			queue_iterator_2 = queue_iterator_2->next;
	    		}
	    		// printf("~~~~~~~~~~~~~~~~\n");
	    		if(avl_tree_num_entries(target_in_radius_aux) == 0){

	    			new_covered_target++;
	    		}
	    		// printf("=> %d VOISINS / MAX COURANT : %d\n",new_covered_target,*new_covered_target_max);
		    	if(new_covered_target >= *new_covered_target_max){

		    		*selected_target = target_in_radius;
		    		*visited_target_queue = covered_target_in_radius_queue;
		    		*new_covered_target_max = new_covered_target;

		    	}
	    	}
	    }
	    queue_iterator = queue_iterator->next;
  	}
}

void greedy_construction(TPointFile* pf){
	printf("\n################################ GREED ################################\n");
	// print_node(pf->points[5]);
	// print_queue(pf->points[5]->communication_queue,print_node);
	// printf("zzz\n");
	// print_queue(pf->points[5]->capture_queue,print_node);
	// printf("########################\n\n");

	TPoint* well_point = pf->points[0];

	TPoint* selected_target;
	Queue* visited_target_queue;
	int new_covered_target_max = 0;

	TPoint* global_selected_target;
	Queue* global_visited_target_queue;
	int global_new_covered_target_max = 0;

	int i;
	AVLTree* visited_target_avl;

	find_best_target(pf, well_point, &selected_target, &visited_target_queue,&new_covered_target_max, NULL);

	avl_tree_insert(pf->solution,&(selected_target->name),selected_target);

	Queue *well_queue = queue_new();
	queue_push_head(well_queue, well_point);
	maj_pf(pf,selected_target,well_queue,visited_target_queue,new_covered_target_max);

	// printf("######################## ON CHOISIT ########################\n");
	// print_node(selected_target);
	// printf("SOLUTION GLOBALE : \n");
	// print_avl_tree(pf->solution,print_node);
	// printf("########################\n\n");

	Queue * qued = selected_target->capture_queue;
	QueueEntry *queue_iterator = qued->head;

	while(pf->cover > 0){
		visited_target_avl = avl_tree_new((AVLTreeCompareFunc) point_compare);
		new_covered_target_max = 0;
		printf("cover : %d\n",pf->cover);
		AVLTree* current_solution_avl = pf->solution;
		AVLTreeValue* current_solution_list = avl_tree_to_array(current_solution_avl);
		int current_solution_list_length = avl_tree_num_entries(current_solution_avl);

		for(i = 0; i < current_solution_list_length; i++){
			TPoint* current_sensor = (TPoint*)(current_solution_list[i]);

			find_best_target(pf,current_sensor,&selected_target,&visited_target_queue,&new_covered_target_max,&visited_target_avl);
		}
		avl_tree_insert(pf->solution,&(selected_target->name),selected_target);
		// printf("######################## ON CHOISIT ########################\n");
		// print_node(selected_target);
		// printf("SOLUTION GLOBALE : \n");
		// print_avl_tree(pf->solution,print_node);
		// printf("########################\n\n");

		Queue *sensor_queue = avl_tree_lookup(visited_target_avl,&(selected_target->name));
		maj_pf(pf,selected_target,sensor_queue,visited_target_queue,new_covered_target_max);
	}
	printf("RESULT : %d\n",avl_tree_num_entries(pf->solution));
	printf("###### FIN GREED ########\n");
	printf("AUX\n");
	int e;
	for (e = 0; e < pf->nbpoints; ++e)
	{
		printf("########## NODE ########\n");
		print_node(pf->points[e]);
		printf("### AUX ###\n");
		print_avl_tree(pf->points[e]->aux,print_node);
		printf("####################\n");
	}
	printf("AUX\n");
}