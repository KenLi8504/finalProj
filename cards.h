#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct card{
  char rank [10];
  char suit [10];
  struct card *next;
};

struct hand{
  char * connection;
  int size;
  int money;
  int status;
  struct card *card1;
  struct card *card2;
  struct card *card3;
  struct card *card4;
  struct card *card5;
  struct card *card6;
  struct card *card7;
};

struct card * makeCard(char * rank, char * suit);
void printCard(struct card * cardPointer);
void swap (int *a, int *b);

void randomize ( int arr[], int n );


struct card ** makeDeck();

struct hand *makePlayer();

struct card * drawCard(struct card * prevCard, struct card ** hand);

void printHand(struct card ** hand);
