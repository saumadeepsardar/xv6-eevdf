#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(2, "Usage: setweight <weight>\n");
    exit(1);
  }

  int w = atoi(argv[1]);
  setweight(w);
  printf("Process weight set to %d\n", w);
  exit(0);
}

