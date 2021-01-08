#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "global.h"
#include "txn.h"
#include "wl.h"

class SynthWorkload : public workload
{
public:
	RC init();
	RC init_table();
	RC init_schema(const char * schema_file);
	RC get_txn_man(txn_man *& txn_manager, thread_t * h_thd);
//	void summarize();
//	void tick() { time = get_sys_clock(); };
	INDEX * the_index;
	table_t * the_table;
private:
//	uint64_t time;
};

class SynthTxnMan : public txn_man
{
public:
	void init(thread_t * h_thd, workload * h_wl, uint64_t part_id); 
	RC run_txn(base_query * m_query);
private:
	RC run_synth_rdwr(int key);
        RC run_synth_rdonly(int key);
//	RC testConflict(int access_num);
	
	SynthWorkload * _wl;
};

#endif
