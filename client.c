#include "socket_stuff.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
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
      if (strcmp(buffer,"The game is now starting...\n") == 0){
        printf("Please enter in your username: ");
        fgets(buffer, sizeof(buffer), stdin);
        *strchr(buffer, '\n') = 0;
        write(server_socket, buffer, sizeof(buffer));
        //printf("Username sent!\n");
        read(server_socket, buffer, sizeof(buffer));
        printf("%s", buffer);
      }

      while(strcmp(buffer,"What would you like to do? Stay or hit: ") != 0){
        read(server_socket, buffer, sizeof(buffer));
        printf("%s", buffer);
      }

      while(strcmp(buffer,"What would you like to do? Stay or hit:  ") == 0){
        printf("%s", buffer);
        //printf("Enter a command:");
        fgets(buffer, sizeof(buffer), stdin);
        *strchr(buffer, '\n') = 0;
        write(server_socket, buffer, sizeof(buffer));
        read(server_socket, buffer, sizeof(buffer));
      }
    }
  }
}
