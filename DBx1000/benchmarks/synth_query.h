#ifndef _SYNTH_QUERY_H_
#define _SYNTH_QUERY_H_

#include "global.h"
#include "helper.h"
#include "query.h"

class workload;

enum SythQueryType {RDWR, RDONLY};

class synth_query : public base_query {
  static uint64_t the_n;
  static double denom;
  static double zeta_2_theta;

  // The following algorithm comes from the paper:
  // Quickly generating billion-record synthetic databases
  // However, it seems there is a small bug.
  // The original paper says zeta(theta, 2.0). But I guess it should be
  // zeta(2.0, theta).
  double zeta(uint64_t n, double theta) {
    double sum = 0;
    for (uint64_t i = 1; i <= n; i++)
      sum += pow(1.0 / i, theta);
    return sum;
  }

  uint64_t zipf(uint64_t n, double theta) {
//    assert(this->the_n == n);
    assert(theta == g_zipf_theta);
    double alpha = 1 / (1 - theta);
    double zetan = denom;
    double eta = (1 - pow(2.0 / n, 1 - theta)) /
                 (1 - zeta_2_theta / zetan);
    double u = (double)(rand() % 10000000) / 10000000;
    double uz = u * zetan;
    if (uz < 1) return 1;
    if (uz < 1 + pow(0.5, theta)) return 2;
    return 1 + (uint64_t)(n * pow(eta*u -eta + 1, alpha));
  }

public:
  SythQueryType type;
  int key;
  void init(uint64_t thd_id, workload *h_wl) override {
    if (the_n == 0) {
      zeta_2_theta = zeta(2, g_zipf_theta);
      //uint64_t table_size = g_synth_table_size / g_part_cnt;
      uint64_t table_size = SYNTH_ROWS; //g_synth_table_size;
      the_n = table_size - 1;
      denom = zeta(the_n, g_zipf_theta);
    }

    double x = (double)(rand() % 100) / 100.0;
    if (x < g_perc_payment) //perc read-write
//      gen_payment(thd_id);
      type = RDWR;
    else
      type = RDONLY;

#if SYNTH_UNIFORM
        key = (int)(rand() % SYNTH_ROWS);
#else
        key = zipf(the_n, g_zipf_theta);
#endif
  }
};

#endif
