#include <stdio.h>
#include <time.h>

struct scrabbleTiles
{
	int points[26], distribution[26];
};

int dictionaryIndex = 0;

/*
 * - This function returns 7 pseudo random capital letters A-Z
 * - Returns a char pointer
 */
char *generateTiles(struct scrabbleTiles scrabbleTiles)
{
	char *tiles = malloc(7 * sizeof(char));
	int seed = (int)time(NULL), i = 0, j = 0, randomChar = 0;
	srand(seed);
	for (i = 0; i < 7; i++)
	{
		randomChar = (rand() % 26) + 65;
		for (j = 0; j < 26; j++)
		{
			if (randomChar == (j + 65) && scrabbleTiles.distribution[j] > 0)
			{
				tiles[i] = (char)randomChar;
				scrabbleTiles.distribution[j]--;
				break;
			}
			else if (randomChar == (j + 65) && scrabbleTiles.distribution[j] == 0)
			{
				i--;
				break;
			}
		}
	}
	return tiles;
}

/*
 * - This function determines a words point value
 * - Returns the point value of the word
 */
int calculatePointValue(struct scrabbleTiles scrabbleTiles, char *wordInput)
{
	int i = 0, j = 0, points = 0;

	for (i = 0; i < strlen(wordInput); i++)
	{
		for (j = 0; j < 26; j++)
		{
			if ((int)wordInput[i] == (j + 65))
			{
				points += scrabbleTiles.points[j];
			}
		}
	}
	return points;
}

/*
 * - This function determines if the input word matches the players tiles
 *   and if the word is in the dictionary
 * - Returns 1 for a validated word and 0 for an invalid word
 */
int validateWord(char *playerTiles, char *wordInput, char dictionary[][7],
				 int dictionaryIndex)
{
	int i = 0, j = 0, k = 0, match = 0;

	for (i = 0; i < strlen(playerTiles); i++)
	{
		for (j = 0; j < strlen(wordInput); j++)
		{
			if (playerTiles[i] == wordInput[j])
			{
				match++;
				if (match >= strlen(wordInput))
				{
					for (k = 0; k < dictionaryIndex; k++)
					{
						if (strcmp(dictionary[k], wordInput) == 0)
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

/*
 * - This function opens a text file and reads in the dictionary
 * - Returns a pointer to the dictionary array
 */
char *importDictionary()
{
	FILE *ifp;
	int i = 0;
	char(*dictionary)[19];

	ifp = fopen("dictionary.txt", "r");
	fscanf(ifp, "%d", &dictionaryIndex);

	dictionary = malloc(dictionaryIndex * sizeof(*dictionary));

	for (i = 0; i < dictionaryIndex; i++)
	{
		fscanf(ifp, "%s", dictionary[i]);
	}

	fclose(ifp);
	return *dictionary;
}

int main()
{
	/*
	 * - i 					= counter variable
	 * - j					= secondary counter variable
	 * - option 			= closes the program when set to 2
	 * - wordInput[7]		= array that holds the word inputed by the user
	 * - bestWord[7]		= array that holds the best scoring word
	 * - *playerTiles		= pointer to the randomly generated tiles
	 * - *dictionary		= pointer to the dictionary
	 * - tempPoints			= holds the current score
	 * - points				= holds the largest point value scored
	 * - scrabbleTiles		= struct that holds the point value and distribution of a scrabble set
	 */
	int i = 0, j = 0, option = 0, tempPoints = 0, points = 0;
	char wordInput[7], bestWord[7], *playerTiles, *dictionary;
	struct scrabbleTiles scrabbleTiles = {
		{1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1,
		 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10},
		{9, 2, 2, 4, 12, 2,
		 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1},
	};

	/*
	 * - Use the importDictionary function to import the dictionary
	 *   and set the pointer to dictionary pointer
	 * - Use the generateTiles function to generate a random set of tiles
	 * 	 that fits the distribution of a scrabble set
	 */
	dictionary = importDictionary();
	playerTiles = generateTiles(scrabbleTiles);

	//Sample set of tiles for testing
	//playerTiles[0] = 'S';
	//playerTiles[1] = 'H';
	//playerTiles[2] = 'A';
	//playerTiles[3] = 'B';
	//playerTiles[4] = 'C';
	//playerTiles[5] = 'T';
	//playerTiles[6] = 'C';

	/*
	 * - Convert the input word to uppercase
	 * - Validate the input word and calculate the point value
	 * - Save the largest score and print the value and word when the user quits
	 */
	printf("Welcome to the Scrabble Practice Program!\nHere are your letters: ");
	for (i = 0; i < 7; i++)
	{
		printf("%c", playerTiles[i]);
	}
	while (option != 2)
	{
		printf("\nWhat would you like to do?\n\t1 - Enter Word\n\t2- Quit\n");
		//fflush(NULL);
		scanf("%d", &option);
		if (option == 1)
		{
			printf("Enter the word:");
			//fflush(NULL);
			scanf("%s", &wordInput);
			for (i = 0; i < strlen(wordInput); i++)
			{
				wordInput[i] = toupper(wordInput[i]);
			}
			if (validateWord(playerTiles, wordInput, dictionary,
							 dictionaryIndex) == 1)
			{
				tempPoints = calculatePointValue(scrabbleTiles, wordInput);
				printf("That word is worth %d points.", tempPoints);
				if (tempPoints > points)
				{
					points = tempPoints;
					for (j = 0; j < 7; j++)
					{
						bestWord[j] = wordInput[j];
					}
				}
				continue;
			}
			else
			{
				printf("The word you entered isn't correct.");
				continue;
			}
		}
	}
	if (points != 0)
	{
		printf("Your best word was %s worth %d points.", bestWord, points);
	}

	free(playerTiles);
	return 0;
}
