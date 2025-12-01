#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void heavy_compute(int n) {
  volatile int x = 0;
  for (int i = 0; i < n * 1000000; i++)
    x += i ^ (x << 1);
}

int
main(void)
{
  printf("==== EEVDF CPU Load Test (no yield) ====\n");
  setschedtrace(1);

  int weights[3] = {1, 3, 5};
  int nchild = 3;

  for (int i = 0; i < nchild; i++) {
    int pid = fork();
    if (pid == 0) {
      setweight(weights[i]);
      int start = uptime();
      heavy_compute(50 - i * 10);  // different workloads
      int end = uptime();
      printf("Child(pid=%d, weight=%d) finished in %d ms\n",
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

