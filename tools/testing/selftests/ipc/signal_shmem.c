/*
 * gcc tools/testing/selftests/ipc/signal_shmem.c -lrt
 */

#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

#ifndef PR_SIGNAL_SHMEM
#define PR_SIGNAL_SHMEM	49
#endif

static void print_signal(volatile siginfo_t *sig)
{
	printf("Signal over shmem received:\n");
	printf("Signal: %d (%s)\n", sig->si_signo, strsignal(sig->si_signo));
	printf("Errno: %d (%s)\n", sig->si_errno, strerror(sig->si_errno));
	printf("Code: %d\n", sig->si_code);
	printf("Sender PID: %d\n", sig->si_pid);
	printf("Sender UID: %d\n", sig->si_uid);
}

int main()
{
	int signal_shm;
	volatile void *addr;
	char *name = "/my_signal_page";
	volatile siginfo_t *sig;

	printf("PID: %d\n", getpid());

	signal_shm = shm_open("/my_signal_page", O_CREAT | O_RDWR,  0666);
	if (signal_shm < 0) {
		fprintf(stderr, "Cannot create shared memory: %s\n",
						strerror(errno));
		return -1;
	}

	if (ftruncate(signal_shm, getpagesize())) {
		fprintf(stderr, "ftruncate() failed: %d (%s)\n",
						errno, strerror(errno));
		return -1;
	}

	if (prctl(PR_SIGNAL_SHMEM, name, strlen(name), 0, 0) < 0) {
		fprintf(stderr, "prctl() failed: %d (%s)\n",
					errno, strerror(errno));
		return -1;
	}

	printf("Signal shared memory: %d\n", signal_shm);

	addr = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
				MAP_SHARED | MAP_LOCKED, signal_shm, 0);
	close(signal_shm);
	if (addr == MAP_FAILED) {
		fprintf(stderr, "shmat() failed: %d (%s)\n",
					errno, strerror(errno));
		return -1;
	}

	sig = (siginfo_t *) addr;
	while (1) {
		if (!sig->si_signo)
			continue;

		/* Let SIGUSR1 be delivered as usual */
		if (sig->si_signo == SIGUSR1)
			continue;

		print_signal(sig);

		/* Acknowledge signal by writing 0. */
		sig->si_signo = 0;
	}

	return 0;
}
