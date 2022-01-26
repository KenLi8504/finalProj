#include "socket_stuff.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  int gamestarted = 0;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );


  while (1) {
    //printf("The buffer recieved was %s\n",buffer);
    if (strcmp(buffer,"The game is now starting...\n") != 0 && gamestarted == 0){
      printf("Type \"start\" to start the game when everyone has joined: ");
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
      read(server_socket, buffer, sizeof(buffer));
      printf("%s",buffer);
    }
    else{
      gamestarted = 1;
      if (strcmp(buffer,"The game is now starting...\n") == 0){
        printf("Please enter in your username: ");
        fgets(buffer, sizeof(buffer), stdin);
        *strchr(buffer, '\n') = 0;
        write(server_socket, buffer, sizeof(buffer));
        //printf("Username sent!\n");
        read(server_socket, buffer, sizeof(buffer));
        printf("%s", buffer);
      }

      read(server_socket, buffer, sizeof(buffer));
      printf("%s", buffer);
      // read(server_socket, buffer, sizeof(buffer));
      // printf("%s", buffer);

      while(strcmp(buffer,"What would you like to do? \"stay\" or \"hit\": ") == 0){

        printf("Got up to here\n");
        //read(server_socket, buffer, sizeof(buffer));
        //printf("The reading was %s\n", buffer);
        fgets(buffer2, sizeof(buffer2), stdin);
        *strchr(buffer2, '\n') = 0;
        while(strcmp(buffer2,"hit") != 0 && strcmp(buffer2,"stay") != 0){
          //printf("The buffer you inputted was %s with a value of %d",buffer2,strcmp(buffer2,"stay"));
          printf("%s",buffer);
          fgets(buffer2, sizeof(buffer2), stdin);
          *strchr(buffer2, '\n') = 0;
        }
        //printf("Got up to the write\n");
        write(server_socket,buffer2,sizeof(buffer));
        read(server_socket,buffer,sizeof(buffer));
        printf("%s",buffer);
      }
    }
  }
}
