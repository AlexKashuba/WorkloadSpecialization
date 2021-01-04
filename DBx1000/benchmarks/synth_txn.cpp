#include "row.h"
#include "synthetic.h"

void SynthTxnMan::init(thread_t *h_thd, workload *h_wl, uint64_t thd_id) {
  txn_man::init(h_thd, h_wl, thd_id);
  _wl = (SynthWorkload *)h_wl;
}

RC SynthTxnMan::run_txn(base_query *query) { return run_synth(); }

RC SynthTxnMan::run_synth() {
  RC rc = RCOK;
  itemid_t *m_item;
  int key = (int)(rand() % 10);
  m_item = index_read(_wl->the_index, key, 0);
  assert(m_item != NULL);
  row_t *row = ((row_t *)m_item->location);
  row_t *row_local = get_row(row, WR);
  if (row_local == NULL) {
    return finish(Abort);
  }
  int f2;
  row_local->get_value(2, f2);
  f2 += 5;
  row_local->set_value(2, f2);
  rc = finish(rc);
  return rc;
}
