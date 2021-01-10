#include "row.h"
#include "synthetic.h"
#include "synth_query.h"


void SynthTxnMan::init(thread_t *h_thd, workload *h_wl, uint64_t thd_id) {
  txn_man::init(h_thd, h_wl, thd_id);
  _wl = (SynthWorkload *)h_wl;
}

RC SynthTxnMan::run_txn(base_query *query) {
  auto *q = (synth_query*) query;
  switch (q->type) {
  case RDWR:
    return run_synth_rdwr(q->key);
  case RDONLY:
    return run_synth_rdonly(q->key);
  }
}


//From https://stackoverflow.com/questions/13001578/i-need-a-slow-c-sharp-function
static int64_t FindPrimeNumber(int n)
{
  int count=0;
  int64_t a = 2;
  while(count<n)
  {
    int64_t b = 2;
    int prime = 1;// to check if found a prime
    while(b * b <= a)
    {
      if(a % b == 0)
      {
        prime = 0;
        break;
      }
      b++;
    }
    if(prime > 0)
    {
      count++;
    }
    a++;
  }
  return (--a);
}

static int64_t slow(int x)
{
#if SYNTH_SLOW
  int i = x % 2;
  return FindPrimeNumber(SYNTH_PRIME + i); //set higher value for more time
#else
  return x + 5;
#endif
}

RC SynthTxnMan::run_synth_rdwr(int key) {
  RC rc = RCOK;
  itemid_t *m_item;
  m_item = index_read(_wl->the_index, key, 0);
  assert(m_item != NULL);
  row_t *row = ((row_t *)m_item->location);
  row_t *row_local = get_row(row, WR);
  if (row_local == NULL) {
    return finish(Abort);
  }
  int f2;
  row_local->get_value(2, f2);
  f2 = slow(f2);
  row_local->set_value(2, f2);
  rc = finish(rc);
  return rc;
}

static void fake_func(int i){
    return;
}

RC SynthTxnMan::run_synth_rdonly(int key) {
  RC rc = RCOK;
  itemid_t *m_item;
  m_item = index_read(_wl->the_index, key, 0);
  assert(m_item != NULL);
  row_t *row = ((row_t *)m_item->location);
  row_t *row_local = get_row(row, RD);
  if (row_local == NULL) {
    return finish(Abort);
  }
  int f2;
  row_local->get_value(2, f2);
  fake_func(f2);
  rc = finish(rc);
  return rc;
}
