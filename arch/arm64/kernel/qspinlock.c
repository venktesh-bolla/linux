#include <asm/qspinlock.h>
#include <asm/processor.h>

void queued_spin_unlock_wait(struct qspinlock *lock)
{
	u32 val;

	for (;;) {
		smp_mb();
		val = atomic_read(&lock->val);

		if (!val) /* not locked, we're done */
			goto done;

		if (val & _Q_LOCKED_MASK) /* locked, go wait for unlock */
			break;

		/* not locked, but pending, wait until we observe the lock */
		cpu_relax();
	}

	for (;;) {
		smp_mb();
		val = atomic_read(&lock->val);
		if (!(val & _Q_LOCKED_MASK)) /* any unlock is good */
			break;

		cpu_relax();
	}

done:
	smp_acquire__after_ctrl_dep();
}
EXPORT_SYMBOL(queued_spin_unlock_wait);
