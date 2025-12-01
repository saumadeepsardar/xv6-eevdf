#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void cpu_burst(int n) {
  volatile int x = 1;
  for (int i = 0; i < n * 1000000; i++)
    x = x * 3 + 1;
}

int
main(void)
{
  printf("==== EEVDF Mixed CPU + Sleep Test ====\n");
  setschedtrace(1);

  int weights[3] = {2, 4, 8};
  int nchild = 3;

  for (int i = 0; i < nchild; i++) {
    int pid = fork();
    if (pid == 0) {
      setweight(weights[i]);

      int start = uptime();
      for (int k = 0; k < 10; k++) {
        cpu_burst(10);      // Short CPU burst
        sleep(1);           // Voluntary yield replacement
      }
      int end = uptime();

      printf("[Child %d] weight=%d finished in %d ms\n",
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

