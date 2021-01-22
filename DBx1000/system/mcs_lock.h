#ifndef DBX1000_MCS_LOCK_H
#define DBX1000_MCS_LOCK_H

// https://libfbp.blogspot.com/2018/01/c-mellor-crummey-scott-mcs-lock.html

#include "config.h"
#include <cstdint>
#include <assert.h>
#include <atomic>
#include <thread>

#include <iostream>

class mcs_lock {

  struct mcs_node {

    bool locked{true};
    uint8_t pad1[CL_SIZE - sizeof(bool)];

    mcs_node* next{nullptr};
    uint8_t pad2[CL_SIZE - sizeof(mcs_node*)];

  };

  static_assert(sizeof(mcs_node) == 2 * CL_SIZE, "");

public:

  void lock();

  void unlock();

private:

  std::atomic<mcs_node*> tail{nullptr};

  static thread_local mcs_node local_node;

};

#endif // DBX1000_MCS_LOCK_H
