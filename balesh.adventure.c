#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int conflict(int c[6], int i, int j, int room) {
  int it;

  if(room == i) {
    return(1);
  }
  else {
    for(it=0; it<(j+1); it++){
      if(c[it] == room){
        return(1);
      }
    }
  }
  return(0);
}

int main() {
  //======Declare all Variables======
  int i;
  int j;
  int pid;
  int con;
  int room;
  int connections[7][6];
  char* dir;
  char* rooms[7];
  char* path;
  FILE* file;
  FILE* temp;
  //=================================

  //======Malloc Everything==========
  dir = malloc(sizeof(char)*18);
  for(i=0; i<7; i++) {
    rooms[i] = malloc(sizeof(char)*8);
  }
  path = malloc(sizeof(char)*29);
  //=================================

  //seed rand
  srand(time(NULL));

  //init connections matrix
  for(i=0; i<7; i++){
    for(j=0; j<6; j++){
      connections[i][j] = 9;
    }
  }

  //get pid
  pid = getpid();
  //add pid to dir name
  sprintf(dir, "balesh.rooms.%d", pid);

  //make list of room names
  rooms[0] = "Darkroom";
  rooms[1] = "Ballroom";
  rooms[2] = "Mailroom";
  rooms[3] = "Coatroom";
  rooms[4] = "Newsroom";
  rooms[5] = "Restroom";
  rooms[6] = "Entryway";

  //make the dir
  mkdir(dir, 0777);

  for(i=0; i<7; i++) {
    sprintf(path, "./%s/%s", dir, rooms[i]);
    file = fopen(path, "w");
    fputs("ROOM NAME: ", file);
    fputs(rooms[i], file);
    fputs("\n", file);

    con = rand()%4 + 3;
    for(j=0; j<con; j++) {
      if(connections[i][j] == 9){
        do {
          room = rand()%7;
        } while(conflict(connections[i], i, j, room));
        connections[i][j] = room;
        connections[room][i] = room;
        fputs("CONNECTION ", file);
        //fputs(j, file);
        fputs(": ", file);
        fputs(rooms[room], file);
        fputs("\n", file);
      }
    }

    fclose(file);
    file = NULL;
  }

  //=========Free Everything=========
  free(dir);
  free(path);
  //=================================
  
  return(0);

}
