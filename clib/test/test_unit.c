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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_unit.h"
#include "../src/kdtree.h"
#include "../src/rwfile.h"
#include "../src/point.h"
#include "../src/dist.h"
#include "../src/greedy.h"
#include "../src/avl.h"
#include "../src/draw.h"
#include "../src/annealing.h"
#include "../src/neighbourhood.h"
#include "../src/dfs.h"

int setup(void)  { return 0; }
int teardown(void) { return 0; }

int init_test(void){

	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	CU_pSuite pSuite = CU_add_suite("echantillon_test",setup,teardown);

	if(( NULL == CU_add_test(pSuite, "Test wf pf", well_formed_pf)) ||
		( NULL == CU_add_test(pSuite, "Test wf kdtree of pf", well_formed_pf_kd_tree)) ||
		( NULL == CU_add_test(pSuite, "Test wf points of pf", well_formed_pf_points)) ||
    NULL == CU_add_test(pSuite, "Test add node list", test_add_node_list)||
    NULL == CU_add_test(pSuite, "Test remove node list", test_remove_node_list))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_run_tests();
	CU_basic_show_failures(CU_get_failure_list());
	CU_cleanup_registry();

	return 0;
}

/**********************/
/*****Test tuile.c*****/
/**********************/

void well_formed_pf(void)
{
	double communication_radius = 1.00001;
 double capture_radius = 1.00001;
 int size = 10;

 TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

 CU_ASSERT_EQUAL((int)pf->communication_radius,(int)communication_radius);
 CU_ASSERT_EQUAL((int)pf->capture_radius,(int)capture_radius);
 CU_ASSERT_EQUAL(pf->cover,size*size-1);
 CU_ASSERT_EQUAL(avl_tree_num_entries(pf->solution),0);
 CU_ASSERT_EQUAL(pf->nbpoints,100);

 clean_pf(pf);
}

void well_formed_pf_kd_tree(void)
{
	double communication_radius = 1.00001;
 double capture_radius = 1.00001;
 int size = 10;

 struct kdres *covered_node_in_radius_list;
 double pos[2];
 pos[0] = 2;
 pos[1] = 2;
 int count_neighboor = 0;

 TPointFile* pf = create_point_file(size,communication_radius,capture_radius);
 struct kdtree*  kd_tree = pf->kdTree;
 covered_node_in_radius_list = kd_nearest_range(kd_tree,pos,capture_radius);

 while(!kd_res_end(covered_node_in_radius_list)){
  count_neighboor++;
  kd_res_next(covered_node_in_radius_list);
}
kd_res_free(covered_node_in_radius_list);

  	//We count the node itself
CU_ASSERT_EQUAL(5,count_neighboor);

clean_pf(pf);
}

void well_formed_pf_points(void)
{
  double communication_radius = 1.00001;
  double capture_radius = 1.00001;
  int size = 10;

  TPointFile* pf = create_point_file(size,communication_radius,capture_radius);

  CU_ASSERT_EQUAL(strcmp(pf->points[17]->name,"17"),0);
  CU_ASSERT_TRUE(abs(pf->points[17]->x - 1.0)< 0.01);
  CU_ASSERT_TRUE(abs(pf->points[17]->y - 7.0)< 0.01);
  CU_ASSERT_EQUAL(pf->points[17]->kind, K_Target);
  CU_ASSERT_EQUAL(pf->points[0]->kind, K_Well);
  for (int i = 1; i < pf->nbpoints; ++i)
  {
    CU_ASSERT_EQUAL(pf->points[i]->kind, K_Target);
  }
  clean_pf(pf);
}

void test_add_node_list(void)
{
  double communication_radius = 1.00001;
  double capture_radius = 1.00001;
  int size = 10;
  AVLTree* add_list;

  TPointFile* pf = create_point_file(size,communication_radius,capture_radius);
  add_node(pf,pf->points[1]);
  add_list = add_node_list(pf);
  CU_ASSERT_EQUAL(avl_tree_num_entries(add_list),2);
  avl_tree_free(add_list);
  add_node(pf,pf->points[11]);
  add_list = add_node_list(pf);
  CU_ASSERT_EQUAL(avl_tree_num_entries(add_list),4);
  avl_tree_free(add_list);
  add_node(pf,pf->points[12]);
  add_list = add_node_list(pf);
  CU_ASSERT_EQUAL(avl_tree_num_entries(add_list),5);
  avl_tree_free(add_list);
  add_node(pf,pf->points[22]);
  add_list = add_node_list(pf);
  CU_ASSERT_EQUAL(avl_tree_num_entries(add_list),6);
  avl_tree_free(add_list);
  int length = run_dfs(pf,pf->points[22]);
  CU_ASSERT_EQUAL(length,4)

  clean_pf(pf);
}

void test_remove_node_list(void)
{
  double communication_radius_1 = 2.00001;
  double communication_radius_2 = 3.00001;
  double capture_radius = 2.00001;
  int size = 5;
  AVLTree* remove_list;

  TPointFile* pf = create_point_file(size,communication_radius_1,capture_radius);
  TPointFile* pf_new = create_point_file(size,communication_radius_2,capture_radius);

  avl_tree_insert(pf->solution,&(pf->points[0]->name),pf->points[0]);
  avl_tree_insert(pf_new->solution,&(pf_new->points[0]->name),pf_new->points[0]);

  add_node(pf,pf->points[2]);
  add_node(pf,pf->points[10]);
  add_node(pf,pf->points[12]);
  add_node(pf,pf->points[14]);
  add_node(pf,pf->points[22]);
  remove_list = remove_node_list(pf);

  CU_ASSERT_EQUAL(avl_tree_num_entries(remove_list),0);
  clean_pf(pf);
  avl_tree_free(remove_list);

  add_node(pf_new,pf_new->points[2]);
  add_node(pf_new,pf_new->points[10]);
  add_node(pf_new,pf_new->points[12]);
  add_node(pf_new,pf_new->points[14]);
  add_node(pf_new,pf_new->points[22]);

  remove_list = remove_node_list(pf_new);

  CU_ASSERT_PTR_NOT_NULL(avl_tree_lookup(remove_list,&(pf_new->points[12]->name)));
  CU_ASSERT_EQUAL(avl_tree_num_entries(remove_list),1);
  clean_pf(pf_new);
  avl_tree_free(remove_list);
}