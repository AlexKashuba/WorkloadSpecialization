#include "mcs_lock.h"

void mcs_lock::lock() {
  // to acquire the lock a thread atomically appends its own local node at the
  // tail of the list returning tail's previous contents
  auto prior_node = tail.exchange(&local_node, std::memory_order_acquire);
  if (prior_node != nullptr) {
    local_node.locked = true;
    // if the list was not previously empty, it sets the predecessor’s next
    // field to refer to its own local node
    prior_node->next = &local_node;
    // thread then spins on its local locked field, waiting until its
    // predecessor sets this field to false
    while (local_node.locked)
      asm volatile("pause\n" : : : "memory");
  }
  // now first in the queue, own the lock and enter the critical section...
}

void mcs_lock::unlock() {
  //...leave the critical section
  // check whether this thread's local node’s next field is null
  if (local_node.next == nullptr) {
    // if so, then either:
    // 1. no other thread is contending for the lock
    // 2. there is a race condition with another thread about to
    // to distinguish between these cases atomic compare exchange the tail field
    // if the call succeeds, then no other thread is trying to acquire the lock,
    // tail is set to nullptr, and unlock() returns
    mcs_node *p = &local_node;
    if (tail.compare_exchange_strong(p, nullptr, std::memory_order_release,
                                     std::memory_order_relaxed)) {
      return;
    }
    // otherwise, another thread is in the process of trying to acquire the
    // lock, so spins waiting for it to finish
    while (local_node.next == nullptr) {
    };
  }
  // in either case, once the successor has appeared, the unlock() method sets
  // its successor’s locked field to false, indicating that the lock is now free
  local_node.next->locked = false;
  // at this point no other thread can access this node and it can be reused
  local_node.next = nullptr;
}

thread_local mcs_lock::mcs_node mcs_lock::local_node = mcs_lock::mcs_node{};
