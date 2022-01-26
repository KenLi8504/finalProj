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
              // for (fd = 0 ; fd <= maxfd; fd++){
              //   if (FD_ISSET(fd, &read_fds) && fd != listen_socket){
              //     write(fd, buffer, sizeof(buffer));
              //   }
              // }
              startgame(clients,countPlayers,fd,listen_socket,maxfd,servers);
            }
            else{
              printf("Refusal\n");
              strcpy(buffer,"Type \"start\" to start this game.\n");
              write(fd, buffer, sizeof(buffer));
              printf("Refusal sent\n");
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

  for (int i = 0; i < count; i++){
    strcpy(buffer,"The game is now starting...\n");
    //strcpy(buffer,"Welcome to the casino! A game is beginning now...\n");
    write(allPlayers[i]->connection,buffer,sizeof(buffer));
  }

  for (int i = 0; i < count; i++){
    if (allPlayers[i] -> connection != fd){
    read(allPlayers[i]->connection,buffer,sizeof(buffer));
  }
  }

/////////////////////////////
int countusernames = 0;
while (countusernames < count){
  for (int i = 0; i < count; i++){
    read(allPlayers[i]->connection,buffer,sizeof(buffer));
    printf("The buffer is %s\n",buffer);
    char * temp = malloc(20);
    printf("Got up here\n");
    strcpy(temp,buffer);
    printf("Got up here\n");
    allPlayers[i] -> username = temp;
    printf("Got up here\n");
    strcpy(buffer,"Your username was submitted! Waiting on other players to submit their usernames...\n");
    write(allPlayers[i]->connection,buffer,sizeof(buffer));
    countusernames = countusernames + 1;
    printf("%d out of %d players here\n",countusernames,count);
  }
}
/////////////////////////////

  for (int i = 0; i < count; i++){
    printf("One player is %s\n",allPlayers[i]->username);
    allPlayers[i] -> username = buffer;
  }

  for (int i = 0; i < count; i++){
    printf("One player is %s\n",allPlayers[i]->username);
    allPlayers[i] -> status = 0;
  }

//setting in usernames
int usernames = 0;


  strcpy(buffer,"Round start! Here are your cards.\n");
  for (int i = 0; i < count; i++){
    write(allPlayers[i] -> connection, buffer, sizeof(buffer));
  }

  while (1) {
    struct card ** currentDeck = makeDeck();
    struct card * currentCard = currentDeck[0];

    char * suit = malloc(10);
    char * rank = malloc (10);

    for (int i = 0; i < count; i++){
      char * first = malloc(30);
      char * second = malloc(10);
      strcpy(first,"You have a ");
      strcpy(second," of ");
      currentCard = drawCard(currentCard,allPlayers[i]);
      strcpy(suit,currentCard->suit);
      strcpy(rank,currentCard->rank);
      strcat(first,rank);
      strcat(first,second);
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
      strcpy(suit,currentCard->suit);
      strcpy(rank,currentCard->rank);
      strcat(first,rank);
      strcat(first,second);
      strcat(first,suit);
      strcat(first,"\n");
      write(allPlayers[i] -> connection,first,sizeof(buffer));
      printf("%s",first);
    }

    int playersStillHitting = count;
    while (playersStillHitting > 0){
      int playersStillOnThisTurn = playersStillHitting;
      while(playersStillOnThisTurn > 0){
        for (int i = 0; i < count; i++){
          if (allPlayers[i] ->status == 0){
            strcpy(buffer,"What would you like to do? \"stay\" or \"hit\": ");
            write(allPlayers[i]->connection,buffer,sizeof(buffer));
            printf("I sent out the writing!\n");
            read(allPlayers[i]->connection,buffer,sizeof(buffer));
            printf("I got the reading back!\n");
            if (strcmp(buffer,"stay") == 0){
              printf("They decided to stay\n");
              strcpy(buffer,"You decided to stay. You will no longer be able to make any new moves");
              write(allPlayers[i]->connection,buffer,sizeof(buffer));
              allPlayers[i] -> status = 1;
              playersStillHitting = playersStillHitting - 1;
              playersStillOnThisTurn = playersStillOnThisTurn - 1;
            }
            else{
              printf("They decided to hit\n");
              strcpy(buffer,"You decided to hit Here is your next card. Wait for everyone else to finish this turn before continuing.");
              write(allPlayers[i]->connection,buffer,sizeof(buffer));
              currentCard = drawCard(currentCard,allPlayers[i]);
              char * first = malloc(30);
              char * second = malloc(10);
              strcpy(first,"You have a ");
              strcpy(second," of ");
              strcpy(suit,currentCard->suit);
              strcpy(rank,currentCard->rank);
              strcat(first,rank);
              strcat(first,second);
              strcat(first,suit);
              strcat(first,"\n");
              write(allPlayers[i] -> connection,first,sizeof(buffer));
              printf("%s",first);
              playersStillOnThisTurn = playersStillOnThisTurn - 1;
            }
          }
        }
      }
    }
    printf("Done\n");
  }
}
