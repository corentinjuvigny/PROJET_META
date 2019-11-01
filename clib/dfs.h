#ifndef __DFS__
#define __DFS__

void dfs(TPointFile* pf, AVLTree* visited_avl, TPoint* node);
int run_dfs(TPointFile* pf, TPoint* removed_target);

#endif