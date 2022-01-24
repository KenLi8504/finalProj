#finalProj

Members: Ken Li
Class Period: 5

Project
Implementing a kind of casino; accounts and money will be saved on a file, and people can connect and play Texas Holdem.

UI and usage
Connecting
Have one computer serve as a server, with a data document that stores info. Have other computers connect to this server, and give them the option to sign in with a username and password or to create a new account when they first connect.
Gameplay
Cards will be presented to the user by printing in the card's rank and suit; for each round of betting, the player will be asked to type in raise/call/fold and then a corresponding amount of money. There can be counters for how much money an individual has and how much money is in the pot.

Technical Design
I think that I can mirror my cards in a struct kind of similar to the songs/library project, where I have 13 linked lists, each one representing a rank, with 4 suits in each list, and every time a card is drawn it is "removed"; this ensures that after a card has been drawn once, it will not be drawn again. After this, I can recombine the cards into another hand struct that can be compared with other hand structs to see which hand is stronger and who takes the pot. This part will use allocation of memory.
I also plan on storing usernames + total money in a text file, which would use working with files.
Finally, to connect to the server and play games at a "table", I can use sockets and semaphores; this will ensure that only a set number of people can play at a time.

Timeline
Cards, card functions, and hands - 3 days (Complete by Saturday 1/15)
Connecting to server and semaphores - 3-4 days (Complete by next Tuesday-Wednesday 1/18-1/19)
Storing the account information and money - 1-2 days - Complete by (Thursday-Friday 1/20-1/21)

Final Notes
I accomplished a lot less than I wanted. Right now, all I have is a shuffle and distribute card program. You set up the server with ./server, and then you connect as many people as you want with ./client. When one of them types "start", the game starts for all of them, though they need to type some random thing to move on, and two random cards are distrubuted to each of the players.
