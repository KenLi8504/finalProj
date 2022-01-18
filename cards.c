#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

struct card{
  char rank [10];
  char suit [10];
  struct card *next;
};


struct card * makeCard(char * rank, char * suit){
  struct card *newCard = malloc (sizeof(struct card));
  strncpy(newCard -> rank,rank,10);
  strncpy(newCard -> suit,suit,10);
  return newCard;
}

void printCard(struct card * cardPointer){
  printf("You have a %s of %s\n",cardPointer -> rank,cardPointer -> suit);
}


struct card ** makeDeck(){
  struct card ** deck = calloc(52,sizeof(struct card));
  char * suits [4] = {"Diamonds","Clubs","Hearts","Spades"};
  char * ranks [13] = {"Two", "Three", "Four", "Five", "Six", "Seven",
                      "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 13; j++){
      struct card * cards = makeCard(ranks[j],suits[i]);
      deck[13*i + j] = cards;
      printCard(cards);
    }
  }
  return deck;
}

void printHand(struct card ** hand){
  for (int i = 0; i < sizeof(hand)/sizeof(struct card); i++){
    printCard(hand[i]);
  }
}

int main(){
  struct card ** deckInitial = makeDeck();
  printHand(deckInitial);
  return 0;
}
