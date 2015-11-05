#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int conflict(int c[6], int j, int room) {
  //======Declare all Variables======
  int it;
  //=================================

  //checks if any of the already-assigned connections are equal to the
  //connection we want to make
  //iterates through all connections
  for(it=0; it<6; it++){
    //returns 1 if the connection has already been created
    if(c[it] == room){
      return(1);
    }
  }
  //If there was no conflict, return 0
  return(0);
}

int check(char* c[6], char* str) {
  //======Declare all Variables======
  int i;
  int j;
  //=================================

  //checks if str is a valid connection
  //iterates through list of connections
  for(i=0; i<6; i++){
    //compares str to each connection
    //returns 0 if the strings match
    if(!strcmp(c[i], str)){
      return(0);
    }
  }

  //returns 1 if no matching connection was found
  return(1);
}

int turn(char* dir, char* curr) {
  //======Declare all Variables======
  int i;
  int steps;
  int rep;
  FILE* file;
  char* path;
  char buff[255];
  char* co[6];
  char* str;
  char* p;
  //=================================

  //======Malloc Everything==========
  path = malloc(sizeof(char)*29);
  p = malloc(sizeof(char)*300);
  str = malloc(sizeof(char)*8);
  for(i=0; i<6; i++){
    co[i] = malloc(sizeof(char)*8);
  }
  //=================================

  //init steps
  steps = 0;
  //init p
  sprintf(p, "Entryway\n");
  //loops until end room is found
  while(strcmp(curr, "Restroom")){
    //cats dir and current room to make the path
    sprintf(path, "./%s/%s", dir, curr);
    //opens file
    file = fopen(path, "r");
    //skips the first two words, since they are ROOM NAME
    fscanf(file, "%s", buff);
    fscanf(file, "%s", buff);
    //gets the rest of the line to be printed as the location
    fgets(buff, 255, (FILE*)file);
    //prints the location
    printf("\nCURRENT LOCATION:%s", buff);
    //prints all connections
    printf("POSSIBLE CONNECTIONS:");
    //repeats for the max of 6 connections
    //if there are fewer connections we will break sooner
    for(i=0; i<6; i++){
      //skip the first two words again since they are CONNECTION #:
      fscanf(file, "%s", buff);
      fscanf(file, "%s", buff);
      //also skip the next char since we don't want leading whitespace in our
      //connection name
      fscanf(file, "%c", buff);
      //get the connection name
      fgets(buff, 9, (FILE*)file);
      //next, the kind of hacked part
      //it needs to be this way so that we can print out all the connections
      //comma-delimeted and ending in a period.
      //It will also allow us to check that we aren't using the last line
      //if the connection is the first connection, print it by itself
      if(i==0){
        //puts the buffer containing the connection name into an array of
        //connections
        sprintf(co[i], "%s", buff);
        //prints the connection name
        printf("%s", co[i]);
      }
      //if the connection is not the 6th connection and not the last line of the
      //file, print the connection with a comma before it and without a period
      //or newline
      else if(i<5 && 96 < buff[2] && buff[2] < 123) {
        sprintf(co[i], "%s", buff);
        printf(", %s", co[i]);
      }
      //if the connection is the 6th connection and not the last line of the
      //file, print the connection with a comma before it and a period and
      //newline after
      else if(i==5 && 96 < buff[2] && buff[2] < 123) {
        sprintf(co[i], "%s", buff);
        printf(", %s.\n", co[i]);
      }
      //if the connection is the last line in the file, just print a period and
      //a newline, and break
      else if(!(96 < buff[2] && buff[2] < 123)) {
        printf(".\n");
        break;
      }
      //get the last line
      fgets(buff, 255, (FILE*)file);
    }

    //loops until the user gives good input
    do{
      //prompts the user
      printf("WHERE TO?>");
      //gets user's reply
      scanf("%s", str);
      //checks if the reply is valid
      rep = check(co, str);
      //if the reply is not valid, error
      if(rep == 1){
        printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
      }
      //save the reply to the path that the user has taken if the response is
      //valid
      else{
        curr = str;
        sprintf(p, "%s%s\n", p, curr);
      }
    }while(rep == 1);
    //iterate the step counter
    steps = steps + 1;
  }

  //print the success message with number of steps and path
  printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\nYOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n%s", steps, p);

  free(p);
  free(path);

  return(0);
}

int play(char** rooms, char* dir) {
  //======Declare all Variables======
  int i;
  char* curr;
  //=================================

  //======Malloc Everything==========
  curr = malloc(sizeof(char)*8);
  curr = "Entryway";
  //=================================

  turn(dir, curr);

  return(0);
}

int main() {
  //======Declare all Variables======
  int i;
  int j;
  int pid;
  int con;
  int counter;
  int room;
  char* tmp;
  int connections[7][6];
  char* dir;
  char* rooms[7];
  char* path;
  FILE* file;
  //=================================

  //======Malloc Everything==========
  dir = malloc(sizeof(char)*18);
  for(i=0; i<7; i++) {
    rooms[i] = malloc(sizeof(char)*8);
  }
  path = malloc(sizeof(char)*29);
  tmp = malloc(sizeof(char));
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

  //create all the connections in an array
  for(i=0; i<7; i++){
    //get the number of connections for that room
    con = (rand()%4) + 3;
    for(j=0; j<con; j++) {
      //only add a new connection if that connection has not already been filled
      if(connections[i][j] == 9){
        //do this while the room was already a connection
        do {
          //do this while the room was the current room
          do{
            //pick a random room
            room = rand()%7;
          }while(room == i);
        }while(conflict(connections[i], j, room));

        //save the accepted connection
        connections[i][j] = room;
        //saves to connection in the randomly chosen room
        //into the first open position in the array
        for(counter=0; counter<6; counter++){
          if(connections[room][counter] == 9) {
            connections[room][counter] = i;
            break;
          }
        }
      }
    }

  }

  //for testing
  //prints the array of connections
  /*
  for(i=0; i<7; i++){
    printf("room %d\n", i);
    for(j=0; j<6; j++){
      if(connections[i][j] != 9) {
        printf("%s\n", rooms[connections[i][j]]);
      }
      else{
        printf("9\n");
      }
    }
  }
  */


  //puts the connections from the array to the files
  for(i=0; i<7; i++){
    //cats the dir and file name to the path
    sprintf(path, "./%s/%s", dir, rooms[i]);
    //opens the file with write priveledges
    file = fopen(path, "w");
    //puts the room name in the file
    fputs("ROOM NAME: ", file);
    fputs(rooms[i], file);
    fputs("\n", file);
    //puts the connections in the file
    for(j=0; j<6; j++){
      if(connections[i][j] != 9){
        //puts the connection number in a temp string var
        sprintf(tmp, "%d", (j+1));
        //prints the connections to the file
        fputs("CONNECTION ", file);
        fputs(tmp, file);
        fputs(": ", file);
        fputs(rooms[connections[i][j]], file);
        fputs("\n", file);
      }
    }
    //puts the room type at the end of the file
    fputs("ROOM TYPE: ", file);
    //if the file open is Restroom, put end type
    if(i == 5) {
      fputs("END_ROOM", file);
    }
    //if the file open is Entryway, put start type
    else if(i == 6) {
      fputs("START_ROOM", file);
    }
    //otherwise put mid type
    else {
      fputs("MID_ROOM", file);
    }
    fputs("\n", file);
    //close the file
    fclose(file);
    file = NULL;
  }

  //play the game
  play(rooms, dir);

  //=========Free Everything=========
  free(dir);
  free(path);
  //=================================

  return(0);

}
