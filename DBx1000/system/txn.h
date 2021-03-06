#ifndef _TXN_H_
#define _TXN_H_

#include "global.h"
#include "helper.h"
//#include "wl.h"

class workload;
class thread_t;
class row_t;
struct row_storage;
class table_t;
class base_query;
class INDEX;

// each thread has a txn_man.
// a txn_man corresponds to a single transaction.

// For VLL
enum TxnType { VLL_Blocked, VLL_Free };

class Access {
public:
  access_t type;
  row_t *orig_row;
  row_t *data;
  row_t *orig_data;
  void cleanup();
};

class txn_man {
public:
  virtual void init(thread_t *h_thd, workload *h_wl, uint64_t part_id);
  void release();
  thread_t *h_thd;
  workload *h_wl;
  myrand *mrand;
  uint64_t abort_cnt;

  virtual RC run_txn(base_query *m_query) = 0;
  uint64_t get_thd_id();
  workload *get_wl();
  void set_txn_id(txnid_t txn_id);
  txnid_t get_txn_id();

  void set_ts(ts_t timestamp);
  ts_t get_ts();

  pthread_mutex_t txn_lock;
  row_t *volatile cur_row;
  // [DL_DETECT, NO_WAIT, WAIT_DIE]
  bool volatile lock_ready;
  bool volatile lock_abort; // forces another waiting txn to abort.
  // [TIMESTAMP, MVCC]
  bool volatile ts_ready;
  // [HSTORE]
  int volatile ready_part;
  RC finish(RC rc);
  void cleanup(RC rc);

  ////////////////////////////////
  // LOGGING
  ////////////////////////////////
  //	void 			gen_log_entry(int &length, void * log);

protected:
  void insert_row(row_t *row, table_t *table);

public:
  // For OCC
  uint64_t start_ts;
  uint64_t end_ts;
  // following are public for OCC
  int row_cnt;
  int wr_cnt;
  //	int * 			row_cnts;
  Access **accesses;
  int num_accesses_alloc;

  // For VLL
  TxnType vll_txn_type;
  itemid_t *index_read(INDEX *index, idx_key_t key, int part_id);
  row_t *get_row(row_t *row, access_t type, row_storage *t_storage=NULL);

private:
  // insert rows
  uint64_t insert_cnt;
  row_t *insert_rows[MAX_ROW_PER_TXN];
  txnid_t txn_id;
  ts_t timestamp;
};

#endif
