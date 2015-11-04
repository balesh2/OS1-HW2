#include <unistd.h>
#include <stdio.h>

int main() {
  int i;
  int pid;
  char* dir;
  char* rooms[7];
  char* path;
  FILE* file;

  pid = getpid();
  sprintf(dir, "balesh.rooms.%d", pid);
  rooms[0] = "Kitchen";
  rooms[1] = "Ballroom";
  rooms[2] = "Conservatory";
  rooms[3] = "Library";
  rooms[4] = "Study";
  rooms[5] = "Lounge";
  rooms[6] = "Cellar";

  for(i=0; i<7; i++) {
    sprintf(path, "./%s/%s", dir, rooms[i]);
    file = fopen(path, "w+");
    fputs("test\n", file);
    fclose(file);
    file = NULL;
  }


  mkdir(dir, 777);
}
