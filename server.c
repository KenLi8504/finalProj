#include "socket_stuff.h"
#include "cards.h"

void process(char *s,int players);
void startgame(fd_set clients,int count,int fd, int listen_socket, int maxfd, int servers[]);

int main() {
  int servers [10];
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
    //all we know is a file descriptor is ready, we don't know which one
    //loop through read_fds for any available socket
    for (fd=0; fd<=maxfd; fd++) {
      //will return true if the provided file descriptor is in read_fds
      if (FD_ISSET(fd, &read_fds)) {
        //listen socket ready means new client
        if (fd == listen_socket) {
          client = server_connect(listen_socket);
          servers[countPlayers] = client;
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
              strcpy(buffer,"The game is now starting...\n");
              write(fd, buffer, sizeof(buffer));
              startgame(clients,countPlayers,fd,listen_socket,maxfd,servers);
            }
            else{
              strcpy(buffer,"Type \"start\" to start this game.\n");
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

void startgame(fd_set clients,int count,int fd, int listen_socket, int maxfd, int servers []){
  fd_set read_fds;
  char buffer[BUFFER_SIZE];

  for (fd=0; fd<=maxfd; fd++){
    if FD_ISSET(fd, &read_fds){
    strcpy(buffer,"Welcome to the casino! A game is beginning now...\n");
    write(fd, buffer, sizeof(buffer));
  }
}

  struct hand *allPlayers[count];
  for (int i = 0; i < count; i++){
    struct hand *newPlayer = makePlayer();
    newPlayer -> money = 10000;
    allPlayers[i] = newPlayer;
    allPlayers[i] -> status = 0;
  }

  for (int i = 0; i < count; i++){
    allPlayers[i] -> connection = servers[i];
  }
  strcpy(buffer,"Round start! Here are your cards.\n");
  for (int i = 0; i < count; i++){
    write(allPlayers[i] -> connection, buffer, sizeof(buffer));
  }

  while (1) {
    struct card ** currentDeck = makeDeck();
    struct card * currentCard = currentDeck[0];

    char * suit = malloc(10);
    char * rank = malloc (10);
    //printf("The suit of the card at the top is %s\n",currentCard->suit);
    //printf("The rank of the card at the top is %s\n",currentCard->rank);

    for (int i = 0; i < count; i++){
      char * first = malloc(30);
      char * second = malloc(10);
      strcpy(first,"You have a ");
      strcpy(second," of ");
      currentCard = drawCard(currentCard,allPlayers[i]);
      //printf("No problems2!\n");
      strcpy(suit,currentCard->suit);
      //printf("No problems3!\n");
      strcpy(rank,currentCard->rank);
      //printf("No problems4!\n");
      strcat(first,rank);
      //printf("No problems5!\n");
      strcat(first,second);
      //printf("No problems6!\n");
      strcat(first,suit);
      strcat(first,"\n");
      write(allPlayers[i] -> connection,first,sizeof(buffer));
      printf("%s",first);
    }
    for (int i = 0; i < count; i++){
      char * first = malloc(30);
      char * second = malloc(10);
      strcpy(first,"You have a ");
      strcpy(second," of ");
      currentCard = drawCard(currentCard,allPlayers[i]);
      //printf("No problems2!\n");
      strcpy(suit,currentCard->suit);
      //printf("No problems3!\n");
      strcpy(rank,currentCard->rank);
      //printf("No problems4!\n");
      strcat(first,rank);
      //printf("No problems5!\n");
      strcat(first,second);
      //printf("No problems6!\n");
      strcat(first,suit);
      strcat(first,"\n");
      write(allPlayers[i] -> connection,first,sizeof(buffer));
      printf("%s",first);
    }

    read(allPlayers[0]->connection,buffer,sizeof(buffer));
  }
}
  //
  //     // currentCard = drawCard(currentCard,allPlayers[i]);
  //     // //printf("No problems2!\n");
  //     // strcpy(suit,currentCard->suit);
  //     // //printf("No problems3!\n");
  //     // strcpy(rank,currentCard->rank);
  //     // //printf("No problems4!\n");
  //     // strcat(first,rank);
  //     // //printf("No problems5!\n");
  //     // strcat(second,first);
  //     // //printf("No problems6!\n");
  //     // strcat(second,suit);
  //     // printf("Done with second run!\n");
  //     // write(allPlayers[i] -> connection,second,30);
  //     // printf("Epic\n");
  //     // free(first);
  //     // free(second);
  //   }
  //
  //   for (int i = 0; i < count; i++){
  //     char * first = malloc(30);
  //     char * second = malloc(10);
  //
  //     strcpy(first,"You have a ");
  //     strcpy(second," of ");
  //     currentCard = drawCard(currentCard,allPlayers[i]);
  //
  //     //printf("No problems2!\n");
  //     strcpy(suit,currentCard->suit);
  //     //printf("No problems3!\n");
  //     strcpy(rank,currentCard->rank);
  //     //printf("No problems4!\n");
  //
  //     strcat(first,rank);
  //     //printf("No problems5!\n");
  //     strcat(first,second);
  //     //printf("No problems6!\n");
  //     strcat(first,suit);
  //     strcat(first,"\n");
  //     printf("%s",first);
  //     write(allPlayers[i] -> connection,first,30);
  //   }
  // }
  // }
  // }
  // }

//     printf("Still no problems\n");
//
//     read_fds = clients;
//     int activePlayers = count;
//     int client;
//     printf("Still no problems2\n");
//     while(activePlayers > 2){
//       printf("Still no problems3\n");
//       int i = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
//       error_check_sock(i, "select", listen_socket);
//       //printf("select ready: %d\n", i);
//       //all we know is a file descriptor is ready, we don't know which one
//       //loop through read_fds for any available socket
//       for (fd=0; fd<=maxfd; fd++) {
//         //will return true if the provided file descriptor is in read_fds
//             //if the socket got closed, select will trigger
//             //if tead returns 0 bytes, then we should close the
//             //connection, otherwise process it.
//             if (read(fd, buffer, sizeof(buffer)) ) {
//               if (strcmp(buffer,"fold") == 0){
//                 strcpy(buffer,"You folded.");
//                 write(fd, buffer, sizeof(buffer));
//                 activePlayers = activePlayers - 1;
//               }
//               else if (strcmp(buffer,"raise")){
//                 strcpy(buffer,"How much would you like to raise?");
//                 write(fd,buffer,sizeof(buffer));
//               }
//               else{
//                 strcpy(buffer,"Type \"start\" to start this game.\n");
//                 write(fd, buffer, sizeof(buffer));
//               }
//             }//data to be read
//             else {
//               //remove this descriptor from the full set
//               FD_CLR(fd, &clients);
//               close(fd);
//             }//close the connection
//           }//existing socket
//         }//descriptor is readhy
//       }//loop through sockets
//   }
// }
//   for (int i = 0; i < count; i++){
//     //free(allPlayers[i]);
//   }
