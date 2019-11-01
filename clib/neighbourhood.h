#ifndef __NEIGHBOURHOOD__
#define __NEIGHBOURHOOD__



AVLTree* add_node_list(TPointFile* pf);
void add_node(TPointFile* pf, TPoint* new_target);

AVLTree* remove_node_list(TPointFile* pf);
void remove_node(TPointFile* pf, TPoint* old_sensor);

#endif