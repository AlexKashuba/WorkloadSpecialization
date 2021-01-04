#ifndef _SYNTH_QUERY_H_
#define _SYNTH_QUERY_H_

#include "global.h"
#include "helper.h"
#include "query.h"

class workload;

class synth_query : public base_query {
public:
  void init(uint64_t thd_id, workload *h_wl) {}
};

#endif
