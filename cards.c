#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Cards have rank and suit, and next points to the next card in the "linked list"
struct card{
  char rank [10];
  char suit [10];
  struct card *next;
};

//size is how many cards they have
//money is how much money they have left
//connection is used to send specific cards to the specific people so only players can see their own hand
//status is whether they folded or are still in play; 0 is still in play, 1 is folded
//each of the card pointers point to the card that they have
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

//mkaes the cards
struct card * makeCard(char * rank, char * suit){
  struct card *newCard = malloc (sizeof(struct card));
  strncpy(newCard -> rank,rank,10);
  strncpy(newCard -> suit,suit,10);
  return newCard;
}

//prints the card that is passed
void printCard(struct card * cardPointer){
  printf("You have a %s of %s\n",cardPointer -> rank,cardPointer -> suit);
}

//prints the entire hand
void printHand(struct hand *playerhand){
  struct card *firstCard = playerhand->card1;
  int i = 0;
  while (i < playerhand -> size){
    printCard(firstCard);
    firstCard = firstCard -> next;
    i++;
  }
}

//prints the deck (just for testing/debugging)
void printDeck(struct card** deck){
  struct card *firstCard = deck[0];
  while( (firstCard -> next) != NULL){
    printCard(firstCard);
    firstCard = firstCard -> next;
  }
  printCard(firstCard);
}



//Swap and randomize was an algorithm that I searched up to shuffle a deck of cards
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
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

//uses randomize and swap to randomly shuffle an array of numbers, then maps these numbers to a specific card
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
    struct card * cards = makeCard(ranks[position/4],suits[position%4]);
    deck[i] = cards;
  }

  //creates the linked list
  for (int i = 0; i < 51; i++){
    deck[i] -> next = deck[i+1];
  }
  return deck;
}

//creates the player
struct hand *makePlayer(){
  struct hand *newHand = calloc(1,sizeof(struct hand));
  newHand -> size = 0;
  return newHand;
}

//current card is the card on the top of the deck
//passes in a player to "draw" the current card by changing the correct card pointer
//corresponding to the player so that their "nth" card pointer will be the current card,
//then returns the next card in the linked list
struct card * drawCard(struct card * prevCard, struct hand *playerhand){
  int count = playerhand->size;
   if (count == 0){
    playerhand->card1 = prevCard;
  }
  else if (count == 1){
    playerhand->card2 = prevCard;
    playerhand->card1-> next = playerhand -> card2;
  }
  playerhand->size = playerhand -> size + 1;
  return prevCard->next;
}

//combines the player's hand with the 5 shared cards
void combine(struct hand * playerhand, struct hand * shared){
    playerhand->card3 = shared->card1;
    playerhand->card2-> next = shared -> card1;

    playerhand->card4 = shared->card2;
    //playerhand->card3-> next = shared -> card2;

    playerhand->card5 = shared->card3;
    //playerhand->card4-> next = shared -> card3;

    playerhand->card6 = shared->card4;
    //playerhand->card5-> next = shared -> card4;

    playerhand->card7 = shared->card5;
    //playerhand->card6-> next = shared -> card5;
  playerhand->size = 7;
}

int identifyLargest(struct hand * playerhand){
  //260-270
  //royal flush

  //200-259
  //straight flush

  //180-199
  //four of a kind

  //160-179
  //full house

  //100=159
  //flush

  //80=99
  //straight


  //60=79
  //three of a kind

  //40=59
  //two pairs

  //20-39
  //pair

  //0=19
  //high card
}

int checkValue(struct hand *playerhand){
  int value = 0;

}

//Used for four of a kind, full house, pairs, three of a kind
int checkRanks(struct hand *playerhand){
  char * distinct[] = {"","","","","","",""};
  int freq[] = {0,0,0,0,0,0,0};
  int counter = 0;
  struct card *curr = playerhand -> card1;
  int i = 0;
  while (i < (playerhand -> size)) {
    printf("Looking at card %d\n",i);
    int new = 0;
    for (int j = 0; j < counter; j++){
      printf("The value is %d\n",strcmp(distinct[j],(curr->rank)));
      if (strcmp(distinct[j],(curr->rank)) == 0) {
        new = j;
      }
    }

    if (new == 0){
      //printf("New Card %d was found\n",*(curr -> rank));
      counter = counter + 1;
      strcpy((curr -> rank),distinct[counter]);
      freq[counter] = 1;
    }

    else{
      freq[counter] = freq[counter] + 1;
    }
    curr = curr -> next;
    i++;
    printf("\n");
  }
  printf("There are %d distinct cards\n",counter);
  for (int i = 0; i<counter; i++){
    printf("%s\t",distinct[i]);
    printf("%d\t",freq[i]);
    printf("\n");
  }
  printf("\n");
  return 0;
}

// int main(){
//   struct card ** deckInitial = makeDeck();
//
// printf("Creating players...\n");
// struct hand *player1 = makePlayer();
// struct hand *player2 = makePlayer();
// struct hand *player3 = makePlayer();
// struct hand *player4 = makePlayer();
// player4 -> money = 1000;
//
// printf("Player 4 has %d money\n",player4 -> money);
// return 0;
// }
