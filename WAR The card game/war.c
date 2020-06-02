#include <stdio.h>

struct card {
	int value;
	char suit;
};

struct queue {
	struct card playerCards[52];
	int front;
	int numElements;
	int size;
};

struct stack {
	struct card warCards[52];
	int top;
};

//Function prototypes
struct card pop(struct stack *stack), 
			dequeue(struct queue *queue);
void		loadHand(FILE *inputFile, struct queue *queue),
			initialize_queue(struct queue *queue, int size),
			initialize_stack(struct stack *stack),
			enqueue(struct queue *queue, struct card card),
			push(struct stack *stack, struct card card);
int			isEmpty_queue(struct queue *queue),
			compareCards(int player1, int player2),
			isEmpty_stack(struct stack *stack);



int main() {
	FILE *inputFile,							//Input file pointer
		 *outputFile;							//Output file pointer

	int numGames,								//Holds the number of games to be played
		i;										//For loop counter variable

	struct queue queue1,						//Queue struct for player 1
				 queue2;						//Queue struct for player 2
	struct queue *player1_queue = &queue1,		//Queue struct pointer for player 1
				 *player2_queue = &queue2;		//Queue struct pointer for player 2

	struct stack stack;							//Stack struct for the war stack
	struct stack *warStack = &stack;			//Stack struct pointer for the war stack

	struct card player1_card, player2_card;		//Card struct for player 1 and 2

	//Open the input and output file
	inputFile = fopen("games.txt", "r");
	outputFile = fopen("war.out", "w");

	//Get the number of games from the input file
	fscanf(inputFile, "%d ", &numGames);

	for (i = 0; i < numGames; i++) {
		fprintf(outputFile, "Game #%d\n", i + 1);		//Print the current game to the output file
		initialize_queue(player1_queue, 52);			//Initialize the queue1 struct for player 1's queue
		loadHand(inputFile, player1_queue);				//Load player 1's hand from the input file
		initialize_queue(player2_queue, 52);			//Initialize the queue1 struct for player 2's queue
		loadHand(inputFile, player2_queue);				//Load player 2's hand from the input file
		initialize_stack(warStack);						//Initialize the war stack

		while (isEmpty_queue(player1_queue) != 1 && isEmpty_queue(player2_queue) != 1) {
			player1_card = dequeue(player1_queue);		//Dequeue a card from player 1's queue
			player2_card = dequeue(player2_queue);		//Dequeue a card from player 2's queue
			push(warStack, player1_card);				//Push player 1's card to the war stack
			push(warStack, player2_card);				//Push player 2's card to the war stack

			//Print the player's cards to the output file
			fprintf(outputFile, "%d%c\t%d%c\n", player1_card.value, player1_card.suit, 
				player2_card.value, player2_card.suit);

			//Player 1 wins
			if (compareCards(player1_card.value, player2_card.value) == 1) {
				//Empty the war stack to player 1's queue
				while (isEmpty_stack(warStack) != 1) {
					enqueue(player1_queue, pop(warStack));
				}
			//Player 2 wins
			} else if (compareCards(player1_card.value, player2_card.value) == 2) {
				//Empty the war stack to player 2's queue
				while (isEmpty_stack(warStack) != 1) {
					enqueue(player2_queue, pop(warStack));
				}
			//Draw
			} else if (compareCards(player1_card.value, player2_card.value) == 0) {
				player1_card = dequeue(player1_queue);		//Dequeue a card from player 1's queue
				player2_card = dequeue(player2_queue);		//Dequeue a card from player 2's queue
				push(warStack, player1_card);				//Push player 1's card to the war stack
				push(warStack, player2_card);				//Push player 1's card to the war stack
				//Print the player's cards to the output file
				fprintf(outputFile, "WAR\n%d%c\t%d%c\n", player1_card.value, player1_card.suit, 
					player2_card.value, player2_card.suit);
			}
			//Check if player 1 is out of cards
			if (isEmpty_queue(player1_queue) == 1) {
				fprintf(outputFile, "Player 2 wins!\n");
				break;
			//Check if player 2 is out of cards
			} else if (isEmpty_queue(player2_queue) == 1) {
				fprintf(outputFile, "Player 1 wins!\n");
				break;
			}
		}
	}
	//Close the input and output files
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}

//Loads a hand of cards from an input file and loads the cards into the queue
void loadHand(FILE *inputFile, struct queue *queue) {
	int i;
	struct card card;
	for (i = 0; i < 26; i++) {
		fscanf(inputFile,"%d %c ", &card.value, &card.suit);	//Read the card values from the input file
		enqueue(queue,card);	//Add the card to the queue
	}
}

//Set the queue to the initial values
void initialize_queue(struct queue *queue, int size) {
	queue->front = 0;
	queue->numElements = 0;
	queue->size = size;
}

//Adds a card to the queue
void enqueue(struct queue *queue, struct card card) {
	queue->playerCards[(queue->front + queue->numElements) % queue->size].suit = card.suit;
	queue->playerCards[(queue->front + queue->numElements) % queue->size].value = card.value;
	(queue->numElements)++;
}

//Remove a card from the queue
struct card dequeue(struct queue *queue) {
	int temp;
	temp = queue->front;
	queue->front = (queue->front + 1) % queue->size;
	(queue->numElements)--;
	return queue->playerCards[temp];
}

//Returns 1 if the queue is empty
int isEmpty_queue(struct queue *queue) {
	if (queue->numElements == 0) {
		return 1;
	} else {
		return 0;
	}
}

//Determines who's card wins
int compareCards(int player1, int player2) {
	if (player1 == 1 && player2 != 1) {
		return 1;
	} else if (player2 == 1 && player1 != 1) {
		return 2;
	} else if (player1 > player2) {
		return 1;
	} else if (player1 < player2) {
		return 2;
	} else {
		return 0;
	}
}

//Set the stack to the initial values
void initialize_stack(struct stack *stack) {
	int i;
	stack->top = -1;
	for (i = 0; i < 52; i++) {
		stack->warCards[i].value = 0;
		stack->warCards[i].suit = ' ';
	}
}

//Returns 1 if the stack is empty
int isEmpty_stack(struct stack *stack) {
	if (stack->top == -1) {
		return 1;
	} else {
		return 0;
	}
}

//Push a card to the stack
void push(struct stack *stack, struct card card) {
	stack->warCards[stack->top+1].suit = card.suit;
	stack->warCards[stack->top+1].value = card.value;
	(stack->top)++;
}

//Pop a card from the stack
struct card pop(struct stack *stack) {
	struct card card;
	card.suit = stack->warCards[stack->top].suit;
	card.value = stack->warCards[stack->top].value;
	(stack->top)--;
	return card;
}

