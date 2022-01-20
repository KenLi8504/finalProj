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
  int size;
  struct card *card1;
  struct card *card2;
  struct card *card3;
  struct card *card4;
  struct card *card5;
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

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// A utility function to print an array
// void printArray (int arr[], int n)
// {
//     for (int i = 0; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");
// }

// A function to generate a random permutation of arr[]
void randomize ( int arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);

        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}


struct card ** makeDeck(){
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8,
           9, 10, 11, 12, 13, 14, 15,
           16, 17, 18, 19, 20, 21, 22,
           23, 24, 25, 26, 27, 28, 29,
           30, 31, 32, 33, 34, 35, 36,
           37, 38, 39, 40, 41, 42, 43,
           44, 45, 46, 47, 48, 49, 50,
           51};

  int n = sizeof(arr)/ sizeof(arr[0]);
  randomize (arr, n);
  //printArray(arr, n);

  struct card ** deck = calloc(52,sizeof(struct card));
  char * suits [4] = {"Diamonds","Clubs","Hearts","Spades"};
  char * ranks [13] = {"Two", "Three", "Four", "Five", "Six", "Seven",
                      "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

  for (int i = 0; i < 52; i++){
    int position = arr[i];
    //printf("The position is %d",position);
    struct card * cards = makeCard(ranks[position/4],suits[position%4]);
    //printCard(cards);
    deck[i] = cards;
  }

  for (int i = 0; i < 51; i++){
    deck[i] -> next = deck[i+1];
  }

  struct card *firstCard = deck[0];
  while( (firstCard -> next) != NULL){
    printCard(firstCard);
    firstCard = firstCard -> next;
  }
  printCard(firstCard);

  return deck;
}

struct hand *makePlayer(){
  struct hand *newHand = calloc(1,sizeof(struct hand));
  newHand -> size = 0;
  return newHand;
}

//current card is the card on the top of the deck
struct card * drawCard(struct card * prevCard, struct hand *playerhand){
  int count = playerhand->size;
   if (count == 0){
    playerhand->card1 = prevCard;
    //printf("Went from 0 cards to 1\n");
  }
  else if (count == 1){
    playerhand->card2 = prevCard;
    playerhand->card1-> next = playerhand -> card2;
    //printf("Went from 1 cards to 2\n");
  }
  else if (count == 2){
    playerhand->card3 = prevCard;
    playerhand->card2-> next = playerhand -> card3;
    //printf("Went from 2 cards to 3\n");
  }
  else if (count == 3){
    playerhand->card4 = prevCard;
    playerhand->card3-> next = playerhand -> card4;
    //printf("Went from 3 cards to 4\n");
  }
  else if (count == 4){
    playerhand->card5 = prevCard;
    playerhand->card4-> next = playerhand -> card5;
    //printf("Went from 4 cards to 5\n");
  }
  playerhand->size = playerhand -> size + 1;
  return prevCard->next;
}

void printHand(struct hand *playerhand){
  printf("This hand has %d cards\n",playerhand->size);
  struct card *firstCard = playerhand->card1;
  int i = 0;
  while (i < playerhand -> size){
    printCard(firstCard);
    firstCard = firstCard -> next;
    i++;
  }
}

void printDeck(struct card** deck){
  struct card *firstCard = deck[0];
  while( (firstCard -> next) != NULL){
    printCard(firstCard);
    firstCard = firstCard -> next;
  }
  printCard(firstCard);
}

int main(){
  struct card ** deckInitial = makeDeck();
  printDeck(deckInitial);
  struct card  * prevCard = deckInitial[0];

  printf("Creating players...\n");
  //
  struct hand *player1 = makePlayer();
  struct hand *player2 = makePlayer();
  //
  printf("Drawing cards...\n");
  //

  printf("The first card is..\n");
  //
  // printf("The next card is..\n");
  // printCard(prevCard->next);

  prevCard = drawCard(prevCard,player1);

  //printHand(player1);
  //printCard(prevCard);

  prevCard = drawCard(prevCard,player2);
  //printCard(prevCard);

  prevCard = drawCard(prevCard,player2);
  //printCard(prevCard);

  prevCard = drawCard(prevCard,player2);
  //printCard(prevCard);

  prevCard = drawCard(prevCard,player1);
  //printCard(prevCard);

  prevCard = drawCard(prevCard,player1);
  //printCard(prevCard);
  //
  printf("Printing cards...\n");
  printf("Player 1 has %d cards\n",player1->size);
  printHand(player1);
  printf("\n");
  printf("Printing player 2...\n");
  printf("Player 2 has %d cards\n",player1->size);
  printHand(player2);
  return 0;
}
