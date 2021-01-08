#include "index_btree.h"
#include "index_hash.h"
#include "mem_alloc.h"
#include "row.h"
#include "synthetic.h"
#include "table.h"
#include "thread.h"

RC SynthWorkload::init() {
  workload::init();
  string path;
  path = "./benchmarks/SYNTH_schema.txt";
  init_schema(path.c_str());

  init_table();
  return RCOK;
}

RC SynthWorkload::init_schema(const char *schema_file) {
  workload::init_schema(schema_file);
  the_table = tables["MAIN_TABLE"];
  the_index = indexes["MAIN_INDEX"];
  return RCOK;
}

RC SynthWorkload::init_table() {
  RC rc = RCOK;
  for (int rid = 0; rid < SYNTH_ROWS; rid++) {
    row_t *new_row = NULL;
    uint64_t row_id;
    int part_id = 0;
    rc = the_table->get_new_row(new_row, part_id, row_id);
    assert(rc == RCOK);
    uint64_t primary_key = rid;
    new_row->set_primary_key(primary_key);
    new_row->set_value(0, rid);
    new_row->set_value(1, 1.0);
    new_row->set_value(2, 1);
    itemid_t *m_item =
        (itemid_t *)mem_allocator.alloc(sizeof(itemid_t), part_id);
    assert(m_item != NULL);
    m_item->type = DT_row;
    m_item->location = new_row;
    m_item->valid = true;
    uint64_t idx_key = primary_key;
    rc = the_index->index_insert(idx_key, m_item, 0);
    assert(rc == RCOK);
  }
  return rc;
}

RC SynthWorkload::get_txn_man(txn_man *&txn_manager, thread_t *h_thd) {
  txn_manager = (SynthTxnMan *)mem_allocator.alloc(sizeof(SynthTxnMan),
                                                   h_thd->get_thd_id());
  new (txn_manager) SynthTxnMan();
  txn_manager->init(h_thd, this, h_thd->get_thd_id());
  return RCOK;
}
