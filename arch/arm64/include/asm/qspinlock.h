#ifndef _ASM_ARM64_QSPINLOCK_H
#define _ASM_ARM64_QSPINLOCK_H

#include <asm-generic/qspinlock_types.h>
#include <asm/atomic.h>

extern void queued_spin_unlock_wait(struct qspinlock *lock);
#define queued_spin_unlock_wait queued_spin_unlock_wait

#define	queued_spin_unlock queued_spin_unlock
/**
 * queued_spin_unlock - release a queued spinlock
 * @lock : Pointer to queued spinlock structure
 *
 * A smp_store_release() on the least-significant byte.
 */
static inline void queued_spin_unlock(struct qspinlock *lock)
{
	smp_store_release((u8 *)lock, 0);
}

#define queued_spin_is_locked queued_spin_is_locked
/**
 * queued_spin_is_locked - is the spinlock locked?
 * @lock: Pointer to queued spinlock structure
 * Return: 1 if it is locked, 0 otherwise
 */
static __always_inline int queued_spin_is_locked(struct qspinlock *lock)
{
	/*
	 * See queued_spin_unlock_wait().
	 *
	 * Any !0 state indicates it is locked, even if _Q_LOCKED_VAL
	 * isn't immediately observable.
	 */
	smp_mb();
	return atomic_read(&lock->val);
}

#include <asm-generic/qspinlock.h>

#endif /* _ASM_ARM64_QSPINLOCK_H */
