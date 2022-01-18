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
void printArray (int arr[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

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

  // for (int i = 0; i < 4; i++){
  //   for (int j = 0; j < 13; j++){
  //     struct card * cards = makeCard(ranks[j],suits[i]);
  //     deck[13*i + j] = cards;
  //     printCard(cards);
  //   }
  // }

  for (int i = 0; i < 52; i++){
    int position = arr[i];
    //printf("The position is %d",position);
    struct card * cards = makeCard(ranks[position/4],suits[position%4]);
    printCard(cards);
    deck[i] = cards;
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
