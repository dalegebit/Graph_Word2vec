#ifndef INCLUDE_RDSEXTR_RANDOM_WALK_H_
#define INCLUDE_RDSEXTR_RANDOM_WALK_H_

#include "gnode.h"


typedef bool (*rds_parr_event)(RGrapgh * ,size_t,size_t);

bool accum_weight_setter_event(RGrapgh * _G , size_t lower_bound, size_t upper_bound );
bool random_path_generator_event(RGrapgh * _G , size_t lower_bound, size_t upper_bound );
bool pathid_collector_event(RGrapgh * _G , size_t lower_bound, size_t upper_bound );


bool rds_parr_event_manager(RGrapgh * _G , size_t lower_bound, size_t upper_bound,rds_parr_event event,size_t thread_size);



#endif  // INCLUDE_RDSEXTR_RANDOM_WALK_H_
