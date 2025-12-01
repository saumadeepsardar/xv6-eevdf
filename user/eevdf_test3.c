#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void compute_small(int n) {
  volatile int acc = 0;
  for (int i = 0; i < n * 500000; i++)
    acc ^= (i * acc) + 1;
}

int
main(void)
{
  printf("==== EEVDF Sleep/Wakeup Fairness Test ====\n");
  setschedtrace(1);

  int weights[3] = {1, 2, 4};
  int nchild = 3;

  for (int i = 0; i < nchild; i++) {
    int pid = fork();
    if (pid == 0) {
      setweight(weights[i]);

      int start = uptime();
      for (int iter = 0; iter < 5; iter++) {
        printf("[PID=%d, W=%d] Iter=%d: sleeping...\n",
               getpid(), weights[i], iter);
        sleep(20);             // simulate I/O-bound behavior
        compute_small(8 + i);  // small compute burst
      }
      int end = uptime();

      printf("Child(pid=%d, weight=%d) completed sleep/compute pattern in %d ms\n",
             getpid(), weights[i], (end - start) * 10);

      exit(0);
    }
  }

  for (int i = 0; i < nchild; i++)
    wait(0);

  setschedtrace(0);

  struct pinfo info;
  if (getpinfo(&info) == 0)
    printf("Parent(pid=%d): weight=%d runtime=%ld vdeadline=%ld sched_count=%ld\n",
           info.pid, info.weight, info.runtime, info.vdeadline, info.sched_count);

  printf("==== Test complete ====\n");
  exit(0);
}

