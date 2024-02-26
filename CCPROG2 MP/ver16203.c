#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

//typedefs to provide conveniency
typedef char array50[50];
typedef char String10[11];
typedef char String120[121];

// Struct that contains data regarding the player's names and scores
struct Data
	{
		int scores;
		char Name[50];
	};

void instructions()
{
	printf("Instructions:\n\n");
	printf("Players are given 2000 points. Players must guess 15 words where each word's description are shown.\n");
	printf("Correct answers merits the player 200 points, while for every mistake, -300. Players will be given 2 \n");
	printf("clues, but each clue has a penalty. When a clue is chosen, scores will be deducted by 10 and the\n");
	printf("scoring will now be based on the number of letters minus the number of hints.\n\n");
	
	printf("If the guess is...\n");
	printf("Correct: Score +200\n");
	printf("Incorrect: Score -300\n\n");
	printf("If _ clue(s) is/are chosen...\n");
	printf("One: -10\n");
	printf("Two: -20\n");
	printf("New scoring system: 10 * (Word length - Amount of clues chosen)\n");
}

void inputName(char A[51])
{
	printf("Enter your name: ");
	fgets(A, 50, stdin); //A is the array A[51], 50 represents total characters allowed, stdin is standard input
}

void Dictionary(String120 dictparam[])
{
    FILE *fptr; 
	int i;
	
    fptr = fopen("ver16203.txt", "r");
	
	for(i = 0; i < 100; i++)
	{
		fgets(dictparam[i], 121, fptr);
	}
	
	fclose(fptr);
}

void Word_database(String10 WordList[])
{
    FILE *fptr; 
	int i;
	
    fptr = fopen("ver16203.txt", "r");
	
	for(i = 0; i < 50; i++)
	{
		fscanf(fptr, "%s", WordList[i]);
	}
	
	fclose(fptr);
}

void random_word_database(String10 RandList[], String10 WordList[], String120 HintsList[], int size)
{
    
    int i;
	char *temp;
	int random;
	
	for(i = 0; i < size; i++)
	{
		random = rand() % size; //Produces random number from 0 - 49
		//For of random sorting
		
		/*Note: Three arrays here so that the random number produced for each array will be the same,
		whereas if only the parameters are passed from the main, the indexes of each array will differ 
		so in order for them to correspond all must be in the same function when the random number is generated*/
		strcpy(temp, RandList[i]);
		strcpy(RandList[i], RandList[random]);
		strcpy(RandList[random], temp);
		
		strcpy(temp, WordList[i]);
		strcpy(WordList[i], WordList[random]);
		strcpy(WordList[random], temp);
		
		strcpy(temp, HintsList[i]);
		strcpy(HintsList[i], HintsList[random]);
		strcpy(HintsList[random], temp);
	}
}

void Hints_database(String120 HintList[], String120 dictionary[])
{
	int i;
	
	for(i = 0; i < 50; i++)
	{
		strcpy(HintList[i], dictionary[i+50]); //i + 50 as the descriptions in the txt file start at index 50, or at line 51
	}
}

void jumbler(String10 Word, int size)
{
	int i, j;
	char temp;
	
	for(i = 0; i < size - 1; i++)
	{
		for(j = i + 1; j < size; j++)
		{
		    if(Word[i] > Word[j]) //Is comparing characters therefore no need to use strcmp
			{
		        temp = Word[i];
		        Word[i] = Word[j];
		        Word[j] = temp;
		    }
		}
	}
}

void sjumbler(String10 Word, int size)
{
	int i, j;
	char temp;
	int random;
	
	for(i = 0; i < size-1; i++)
	{
		random = rand() % size; //Produces random number from 0 to (size - 1)
		temp = Word[i];
		Word[i] = Word[random];
		Word[random] = temp;
	}
	
}

void top_scorers(struct Data players[], int count)
{
	int i, j;
	int temp;
	char tempN[100];
	
	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (players[i].scores < players[j].scores)
			{
				temp = players[i].scores;
				players[i].scores = players[j].scores;
				players[j].scores = temp;
				
				strcpy(tempN, players[i].Name);
		        strcpy(players[i].Name, players[j].Name);
		        strcpy(players[j].Name, tempN);
		        
		        //Code not optimized, done the expanded way to provide visualization for the game creator
		        /* Can be done this way if want optimized (cleaner) version
		        	struct Data temp;
		        	
		        	temp = players[i];
					players[i] = players[j];
					players[j] = temp;
		        */
	    	}
		}
	}
}

void Player_database(struct Data temp[], int size)
{
    
    FILE *fp; 
	int i;
	
    fp = fopen("Player_database.txt", "w");
	
	for(i = 0; i < size; i++)
	{
		fprintf(fp, "%s ", temp[i].Name);
		fprintf(fp, "%d\n", temp[i].scores);
	}
	
	fclose(fp);
}

void duplicate_checker(struct Data temp[], int size, int removed_index)
{
	int i;
	
	for(i = removed_index; i < size; i++)
	{
		temp[i].scores = temp[i+1].scores;
		strcpy(temp[i].Name, temp[i+1].Name);
	}
}

void top5_database(struct Data temp[], int size)
{
    
    FILE *fp; 
	int i;
	
    fp = fopen("Top_5.txt", "w");
	
	for(i = 0; i < 5; i++)
	{
		if(i < size)
		{
			fprintf(fp, "Place %d: %s\t", i+1, temp[i].Name);
			fprintf(fp, "Score: %d\n", temp[i].scores);
		}
		else
			fprintf(fp, "Not Applicable\n");
	}			
	
	fclose(fp);
}

int main()
{
	
    char Words[50][11];
    char PlaceHolder[50][11]; //To store jumbled versions of the words, acts as a temporary storage
    char Hint[50][121];
    char dictionary[100][121];
    char Answer[11];
    int i = 0, ctr = 0, loop, word_length, k, size, choice, lettercount = 0, surrend = 0;
    int flag, word_flag = 0, ready = 0;
    int j, removed_index = 0, removed_ctr = 0, remaining = 0;
    
    
    srand(time(0)); //Without srand(), program will produce same set of random numbers on every iteration
    				//Seed here is time(0) as time() returns a time_t value that will always vary
	struct Data Players[20]; // Number of times players can play
    instructions();
    
    do
    {
    	printf("\nAre you ready to play the game? Yes(1), No(0) ");
    	scanf("%d", &ready);
    	if(ready == 1)
    	{
    		system("cls"); //Clears screen for visual purposes
		}
	}
    while(ready == 0);
    
    getchar(); //Gets the character when the player presses enter so that the next string input will not be affected, in this case fgets is skipped as it reads the enter that the player pressed.
	
    do
	{
		Dictionary(dictionary);
		Word_database(Words);
	    Word_database(PlaceHolder);
	    Hints_database(Hint, dictionary);
		random_word_database(PlaceHolder, Words, Hint, 50);
		
		removed_ctr = 0; //Amount of removed indexes, basically amount of player duplicates. Set to 0 every new game.
		removed_index = 0;
		
		printf("Enter your name: ");
		fgets(Players[i].Name, 50, stdin);
		
		if(i > 0) //Indicates that it's a duplicate
		{
			for(j = 0; j < i; j++)
			{
				if(strcmp(Players[i].Name, Players[j].Name) == 0)
				{
					printf("This player has played before, will only record this attempt.\n");
				}
			}
		}
		
	    Players[i].scores = 2000;
	    
		surrend = 0; //Basically the flag for surrender, 0 if player has not surrendered yet. Set to 0 at every start of the game.
		
	    for(loop = 0; loop < 15; loop++)
	    {
	    	printf("Current Score: %d\n", Players[i].scores);
	    	printf("Words left to guess: %d\n\n", 15 - loop);
	    	
	    	lettercount = 0; //lettercount is the number of times the player has opted for a hint
	    	choice = 0; //choice is the option that the player has chosen. It is set to 0 for every word
	    	word_flag = 0; //Flag to determine if word was correct. Set to 0 for every word at the start.

		    word_length = strlen(Words[loop]);
		    
		    sjumbler(PlaceHolder[loop], word_length);
		    
		    printf("Word to guess: %s\n", PlaceHolder[loop]); //TO check if words in file is taken properly
		    printf("Description: %s\n", Hint[loop]);
		    
		    printf("Enter: (1)-First letter (2)-Last letter (3)-Guess answer (4)-Give up\n");
		    printf("Giving up will merit you the points you have accumulated prior to giving up.\n");
		    
		    do
		    {
		    	printf("Option: ");
		    	scanf("%d", &choice);
		    	
		    	if(choice == 3)
		    	{
		    		printf("Enter Guess: ");
		    		scanf("%s", Answer);
		    		
		    		if(lettercount == 0 && strcmp(Words[loop], Answer) == 0)
		    		{
		    			Players[i].scores += 200;
		    			word_flag = 1;
		    			printf("Correct!");
		    			Sleep(1000);
					}
					
					if(lettercount > 0 && strcmp(Words[loop], Answer) == 0)
					{
						Players[i].scores += 10 * (word_length - lettercount);
						word_flag = 1;
						printf("Correct!");
		    			Sleep(1000);
					}
					
					if(strcmp(Words[loop], Answer) != 0)
					{
						Players[i].scores -= 300;
						printf("Wrong Answer! Think Again!\n");
					}
				}
				
				else if(choice == 1)
				{
					Players[i].scores -= 10;
					lettercount++;
					printf("First letter of the word: %c\n", Words[loop][0]);
				}
				
				else if(choice == 2)
				{
					Players[i].scores -= 10;
					lettercount++;
					printf("Last letter of the word: %c\n", Words[loop][word_length - 1]);
				}
				
				else if(choice == 4)
				{
					surrend = 1;
					word_flag = 1;
				}
				else
					printf("Wrong Input! Choose an appropriate option!\n");
				
				if(Players[i].scores <= 0)
				{
					word_flag = 1;
				}
		    	
			}while(word_flag != 1);
			
			if(surrend == 1)
			{
				loop = 15;
			}
			
			if(Players[i].scores <= 0)
			{
				loop = 15;
				printf("You lost! :D");
				Sleep(2000);
			}
			
			system("cls");
		}
		if(i > 0) //Checker for duplicates
		{
			for(j = 0; j < i; j++)
			{
				if(strcmp(Players[i].Name, Players[j].Name) == 0)
				{
					removed_index = j;
					duplicate_checker(Players, i, removed_index);
					removed_ctr++;
				}
			}
		}
		
	    printf("New Game? Yes(1) or No(0)? ");
	    scanf("%d", &flag);
	    printf("\n");
	    getchar();
	    
	    i++;
	    
	}while(flag == 1);
	
	printf("Game has ended, loading appropriate data to files...");
	Sleep(2000);
	
	remaining = i - removed_ctr; // Number of players left
	
	Player_database(Players, remaining);
	
    top_scorers(Players, remaining);
    
    top5_database(Players, remaining);
    
    printf("Done Loading!\n");
    Sleep(1000);
	printf("Will now display top 5 players\n");
    Sleep(1000);
    
    printf("Here are the top 5 players: \n\n");
    
    for(k = 0; k < 5; k++)
    {
    	if(k < remaining)
    	{
    		printf("%s %d\n", Players[k].Name, Players[k].scores);
		}
		else
			printf("Not Applicable\n");
	}
	
	printf("\nThe rest of the players: \n");
	
    for(k = 5; k < remaining; k++)
    {
    	printf("%s %d\n", Players[k].Name, Players[k].scores);
	}
	
    return 0;
}

/*
Function instructions
Description: To show instructions of the word game
*/

/*
Function inputName (NOT USED)
Description: Function to get name input not sure how to implement to main hence left here
@param A[50] - Limits the amount of characters inputted to 50
*/

/*
Function Dictionary
Description: Function to store all strings in Dictionary to a 2d array, to allow storing of strings to Hints_database function
@param String10 dictparam[] - 2d array that has an n number of elements each limited to 120 characters
*/

/*
Function Word_database
Description: Function to store words from WORDS.txt to a 2d array
@param String10 WordList[] - 2d array that has an n number of words each limited to 10 characters
*/

/*
Function random_word_database
Description: Function to randomize getting of three 2d arrays, specifically the words, hints, and placeholder for the words.
Three of the arrays are included in the parameters so that their corresponding indexes will be the same (to provide conveniency). 
@param String10 RandList[] - 2d array that has an n number of words each limited to 10 characters
@param String10 WordList[] - 2d array that has an n number of words each limited to 10 characters
@param String100 HintsList[] - 2d array that has an n number of words each limited to 10 characters
@param size - All of the 2d arrays have the same total strings, hence size here represents their total number of rows.
*/

/*
Function Hints_database
Description: Function to store hints of the words from Hints.txt to a 2d array
@param String100 HintList[] - 2d array that has an n number of words each limited to 100 characters
*/

/*
Function jumbler (NOT USED)
Description: Function to randomize a string based on the selection sort
@param String10 Word - An array that takes in a string of upto 10 characters
@param size - The total number of characters in the string taken
*/

/*
Function sjumbler
Description: Function to randomize a string based on the rand function
@param String10 Word - An array that takes in a string of upto 10 characters
@param size - The total number of characters in the string taken
*/

/*
Function top_scorers
Description: Function to get the top scorers based on the parameter count(which is 5 since the program is supposed to take top 5)
@param Data players[] - An array of structure for the tag Data, in order to be able to access and manipulate the data inside it
@param count - The total number of valid players that played the game
*/

/*
Function Player_database
Description: Function to "write" and print all the players and their corresponding scores in a txt file called "Player_database.txt"
@param Data temp[] - An array of structure for the tag Data, in order to be able to access the data inside it
@param size - The total number of valid players that played the game
*/

/*
Function duplicate_checker
Description: Function to remove the indicated index and move all the data in the array accordingly to fill in the removed index
@param Data temp[] - An array of structure for the tag Data, in order to be able to access the data inside it
@param size - The total number of valid players that played the game
@param removed_index - Number index that was removed from the array struct Data
*/

/*
Function top5_database
Description: Function to "write" and print all the top 5 players and their corresponding scores in a txt file called "Top_5.txt"
@param Data temp[] - An array of structure for the tag Data, in order to be able to access the data inside it
@param size - The total number of valid players that played the game
*/
