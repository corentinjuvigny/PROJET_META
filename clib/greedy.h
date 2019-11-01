#ifndef __GREEDY__
#define __GREEDY__

void greedy_construction(TPointFile* pf);
void maj_pf(TPointFile* pf,TPoint* selected_target, Queue* sensor_queue, Queue* visited_target_queue, int new_covered_target_max);

#endif
