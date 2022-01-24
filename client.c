#include "socket_stuff.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  while (1) {
    if (strcmp(buffer,"The game is now starting...") != 0){
    printf("enter command: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("%s", buffer);
  }
    while(strcmp(buffer,"You folded.") != 0){
      read(server_socket, buffer, sizeof(buffer));
      printf("%s", buffer);
    }
  }
}
