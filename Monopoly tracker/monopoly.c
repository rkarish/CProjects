#include <stdio.h>
#include <string.h>

//Structure for a monopoly property
struct property
{
	char name[30];
	int user, num_houses, num_hotels;
	struct property *next;
};

//Function prototypes
struct property *buyProperty(struct property *propertyList, int playerNumber);
struct property *improveProperty(struct property *propertyList, int playerNumber);
struct property *sellProperty(struct property *propertyList, int playerNumber);
struct property *delete (struct property *propertyList, char propertyName[30]);
struct property *insertToBack(struct property *front, char propertyName[30],
							  int playerNumber, int houses, int hotels);
void printList(struct property *temp);

int main()
{
	int players = 0, turn = 1, choice = 0;
	struct property *propertyList = NULL;

	//Get the number of players
	printf("Welcome to the Monopoly Property Tracker!\n"
		   "How many people are playing:");
	scanf("%d", &players);

	//Check if the number of players is between 1 and 6
	while (players < 1 || players > 6)
	{
		printf("The number of users players must be between 1 - 6 (inclusive)\n"
			   "How many people are playing:");
		scanf("%d", &players);
	}

	while (turn <= players)
	{
		//Get the menu choice
		printf("Player %d, what would you like to do?\n"
			   "\t1 - Buy Property\n"
			   "\t2 - Improve property\n"
			   "\t3 - Sell Property\n"
			   "\t4 - View Properties\n"
			   "\t5 - End Turn\n"
			   "\t6 - End Game\n",
			   turn);
		scanf("%d", &choice);

		//Call the appropriate function per menu choice
		switch (choice)
		{
		case 1:
			propertyList = buyProperty(propertyList, turn);
			break;
		case 2:
			propertyList = improveProperty(propertyList, turn);
			break;
		case 3:
			propertyList = sellProperty(propertyList, turn);
			break;
		case 4:
			printList(propertyList);
			break;
		case 5:
			if (turn == players)
			{
				turn = 1;
			}
			else
			{
				turn++;
			}
			break;
		case 6:
			printList(propertyList);
			return 0;
		}
	}
	printList(propertyList);
	return 0;
}

//This function prints the propertyList
void printList(struct property *temp)
{
	struct property *helper = temp;
	while (temp != NULL)
	{
		printf("%s, owned by player %d (Houses : %d / Hotels : %d)\n", temp->name, temp->user, temp->num_houses, temp->num_hotels);
		temp = temp->next;
	}
	if (helper == NULL)
	{
		printf("The list is empty.\n");
	}
}

//This function inserts a property node to the back of the list
struct property *insertToBack(struct property *propertyList, char propertyName[30],
							  int playerNumber, int houses, int hotels)
{
	struct property *helper = propertyList;
	struct property *property = (struct property *)malloc(sizeof(struct property));

	//Load the data structure values to the temp property
	strcpy(property->name, propertyName);
	property->num_houses = houses;
	property->num_hotels = hotels;
	property->user = playerNumber;
	property->next = NULL;

	if (propertyList == NULL)
	{
		return property;
	}

	//Find the last node in the linked list
	while (helper->next != NULL)
	{
		helper = helper->next;
	}

	//Set the structure pointer to the temp property
	helper->next = property;

	return propertyList;
}

//This function deletes a node in the propertyList
struct property *delete (struct property *propertyList, char propertyName[30])
{
	struct property *temp, *temp_propertyList = propertyList;

	if (temp_propertyList != NULL)
	{
		//Deletes the first node if it's a match selected
		if (strcmp(temp_propertyList->name, propertyName) == 0)
		{
			propertyList = temp_propertyList->next;
			free(temp_propertyList);
			return propertyList;
		}
		while (temp_propertyList->next != NULL)
		{
			//Deletes the selected node
			if (strcmp(temp_propertyList->next->name, propertyName) == 0)
			{
				temp = temp_propertyList->next;
				temp_propertyList->next = temp->next;
				free(temp);
				return propertyList;
			}
			temp_propertyList = temp_propertyList->next;
		}
	}
	return propertyList;
}

//This function allows a user to add a property to the propertyList
struct property *buyProperty(struct property *propertyList, int playerNumber)
{
	char propertyName[30];
	struct property *temp;

	//Get the property name
	printf("What is the name of the property you wish to buy?\n");
	scanf("%s", propertyName);

	temp = propertyList;

	//Determine if the property is already owned
	while (temp != NULL)
	{
		if (strcmp(temp->name, propertyName) == 0 && temp->user == playerNumber)
		{
			printf("You already own that property!\n");
			return propertyList;
		}
		else if (strcmp(temp->name, propertyName) == 0)
		{
			printf("Player %d owns that property.\n", temp->user);
			return propertyList;
		}
		temp = temp->next;
	}

	return insertToBack(propertyList, propertyName, playerNumber, 0, 0);
}

//This function allows a user to improve a selected property
struct property *improveProperty(struct property *propertyList, int playerNumber)
{
	char propertyName[30];
	struct property *temp;

	temp = propertyList;

	//Get the property name
	printf("Which property would you like to improve?\n");
	scanf("%s", propertyName);

	while (temp != NULL)
	{
		if (strcmp(temp->name, propertyName) == 0 && temp->user == playerNumber)
		{
			int choice = 0, amount = 0;

			//Get the choice of houses or hotels
			printf("Do you wish to build houses(1) or hotels(2)?\n");
			scanf("%d", &choice);

			//Get the number of houses or hotels to be added to the selected property
			if (choice == 1)
			{
				printf("How many houses do you want to build?\n");
				scanf("%d", &amount);
				temp->num_houses += amount;
				return propertyList;
			}
			else if (choice == 2)
			{
				printf("How many hotels do you want to build?\n");
				scanf("%d", &amount);
				temp->num_hotels += amount;
				return propertyList;
			}
			else
			{
				return propertyList;
			}
		}

		temp = temp->next;
	}

	//Notify the user if the property isn't on the list
	printf("You do not own that property.\n");
	return propertyList;
}

//This calculates the value of the seleted property and deletes it from the list
struct property *sellProperty(struct property *propertyList, int playerNumber)
{
	char propertyName[30];
	struct property *temp;

	temp = propertyList;

	//Get the property to be sold
	printf("Which property would you like to sell?\n");
	scanf("%s", propertyName);

	while (temp != NULL)
	{
		//Find the selected property, calculate its value, and delete the property
		if (strcmp(temp->name, propertyName) == 0 && temp->user == playerNumber)
		{
			printf("You sold %s for $%d.00!\n", temp->name, temp->num_hotels * 50 + temp->num_houses * 25 + 100);
			return delete (propertyList, propertyName);
		}
		else
		{
			printf("You do not own that property.\n");
		}

		temp = temp->next;
	}

	return propertyList;
}
