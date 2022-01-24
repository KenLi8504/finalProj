#include "socket_stuff.h"
#include "cards.h"

void process(char *s,int players);
void startgame(fd_set client, int count);

int main() {
  int countPlayers = 0;

  //set to 1 when the game starts
  int gameStatus = 0;
  int fd, listen_socket, maxfd, client;
  //this is the fd_set that will be modified by select
  fd_set read_fds;
  //this is the full fd_set to keep a stable record
  fd_set clients;
  char buffer[BUFFER_SIZE];
  //make sure clients is totally empty
  FD_ZERO( &clients );
  listen_socket = server_setup();
  printf("listen socket: %d\n", listen_socket);
  //add the listening socket to the master set
  FD_SET(listen_socket, &clients);
  //we have to keep track of the largest file descriptor for select
  //currently, listen_socket is the only, and therefore largest
  maxfd = listen_socket;
  while (1) {
    //must reset read_fds after each time you process select
    read_fds = clients;
    /*
      magic time!
      the first argument is 1 more than the largest file descriptor
      select  will be looking at
      This will block until at least one of the descriptors in read_fds
      is ready.
      Returns the number of fds that area ready.
      read_fds will contain only rthe descriptors that are ready.
    */
    int i = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    error_check_sock(i, "select", listen_socket);
    printf("select ready: %d\n", i);
    for (fd=0; fd<=maxfd; fd++) {

    }
    //all we know is a file descriptor is ready, we don't know which one
    //loop through read_fds for any available socket
    for (fd=0; fd<=maxfd; fd++) {
      //will return true if the provided file descriptor is in read_fds
      if (FD_ISSET(fd, &read_fds)) {
        //listen socket ready means new client
        if (fd == listen_socket) {
          client = server_connect(listen_socket);
          countPlayers = countPlayers + 1;
          printf("There are %d players at the table\n",countPlayers);
          FD_SET(client, &clients);
          //make sure to update maxfd
          if (client > maxfd)
            maxfd = client;
        }//new connections
        //existing client
        else {
          //if the socket got closed, select will trigger
          //if tead returns 0 bytes, then we should close the
          //connection, otherwise process it.
          if (read(fd, buffer, sizeof(buffer)) ) {
            if (strcmp(buffer,"start") == 0){
              printf("starting game...\n");
              startgame(clients,countPlayers);
            }
            else{
              strcat(buffer,"Type \"start\" to start this game.\n");
              write(fd, buffer, sizeof(buffer));
            }
          }//data to be read
          else {
            //remove this descriptor from the full set
            FD_CLR(fd, &clients);
            countPlayers--;
            close(fd);
          }//close the connection
        }//existing socket
      }//descriptor is readhy
    }//loop through sockets
  }//forever
  close(listen_socket);
  return 0;
}

void startgame(fd_set clients,int count){
  int fd, listen_socket, maxfd, client;
  fd_set read_fds;
  char buffer[BUFFER_SIZE];
  printf("There are %d players playing\n",count);

  // struct hand *testPlayer = makePlayer();
  // printf("Good\n");
  // testPlayer -> money = 10000;
  // printf("Test player has %d money\n",testPlayer->money);

  // struct hand *player4 = makePlayer();
  // player4 -> money = 1000;
  //
  // printf("Player 4 has %d money\n",player4 -> money);


  struct hand *allPlayers[count];

  for (int i = 0; i < count; i++){
    struct hand *newPlayer = makePlayer();
    newPlayer -> money = 10000;
    allPlayers[i] = newPlayer;
  }

  // printf("NO PROBLEMS!\n");
  // printf("Player %d has %d money\n",1,allPlayers[0]->money);
  // printf("NO PROBLEMS!\n");
  //
  // for (int i = 0; i < count; i++){
  //   printf("Player %d has %d money\n",i+1,allPlayers[i]->money);
  // }
  // printf("NO PROBLEMS!\n");

  while (1) {
    // printf("There are %d players playing\n",count);
    //must reset read_fds after each time you process select
    read_fds = clients;
    /*
      magic time!
      the first argument is 1 more than the largest file descriptor
      select  will be looking at
      This will block until at least one of the descriptors in read_fds
      is ready.
      Returns the number of fds that area ready.
      read_fds will contain only rthe descriptors that are ready.
    */
    int i = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    error_check_sock(i, "select", listen_socket);
    //printf("select ready: %d\n", i);
    for (fd=0; fd<=maxfd; fd++) {

    }
    //all we know is a file descriptor is ready, we don't know which one
    //loop through read_fds for any available socket
    for (fd=0; fd<=maxfd; fd++) {
      //will return true if the provided file descriptor is in read_fds
      if (FD_ISSET(fd, &read_fds)) {
        //listen socket ready means new client
        if (fd == listen_socket) {
          client = server_connect(listen_socket);
          count = count + 1;
          printf("There are %d players at the table\n",count);
          FD_SET(client, &clients);
          //make sure to update maxfd
          if (client > maxfd)
            maxfd = client;
        }//new connections
        //existing client
        else {
          //if the socket got closed, select will trigger
          //if tead returns 0 bytes, then we should close the
          //connection, otherwise process it.
          if (read(fd, buffer, sizeof(buffer)) ) {
            write(fd, buffer, sizeof(buffer));
          }//data to be read
        }//existing socket
      }//descriptor is readhy
    }//loop through sockets
  }//forever
}

void process(char *s,int players) {
  int * countPointer = &players;
  while (*s) {
  if (*s >= 'a' && *s <= 'z')
    *s = ((*s - 'a') + 13) % 26 + 'a';
  else if (*s >= 'A' && *s <= 'Z')
    *s = ((*s - 'a') + 13) % 26 + 'a';
  s++;
  }
}
