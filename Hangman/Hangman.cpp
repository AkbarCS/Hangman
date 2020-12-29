#include "stdafx.h"
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

struct wordsAndGuesses {
	int wordNumber;
	string currentWord;
	string playerGuess;
	string lettersAvailable;
	string guessedSoFar;
	wordsAndGuesses *next;
};

//add element to linked list
wordsAndGuesses* addElementToLinkedList(wordsAndGuesses* pointer, int wordNumber, string currentWord, 
	                                    string playerGuess, string lettersAvailable, string guessedSoFar)
	{
	//create new entry
	wordsAndGuesses* newEntry = new wordsAndGuesses;

	//set the data members
	newEntry->wordNumber = wordNumber;
	newEntry->currentWord = currentWord;
	newEntry->lettersAvailable = lettersAvailable;
	newEntry->guessedSoFar = guessedSoFar;
	newEntry->next = NULL;

	//link it into list - insert to end of linked list, the pointer is at the beginning of the linked list
	wordsAndGuesses* pointer2 = pointer;

	//if the linked list is empty and is being initialised for the first time
	if(pointer == NULL)
		{
		pointer = newEntry;
		pointer2 = pointer;
		}
	else 
		{	
		while(pointer->next != NULL)
			{
			pointer = pointer->next;
			}

		//insert at the end of the linked list
		pointer->next = newEntry;
		}

	//return beginning of linked list
	return pointer2;
	}

//print out contents of linked list
void printWordsAndGuesses(wordsAndGuesses* pointer, int wordNumber, string currentWord, string playerGuess,
	                      string lettersAvailable, string guessedSoFar)
	{
	if (pointer != NULL)
		{
		while (1)
			{
			printf("\n");
			printf("Word Number: %d\n", pointer->wordNumber);
			printf("Word: '%s'\n", pointer->currentWord.c_str());
			printf("Final Guess: '%s'\n", pointer->playerGuess.c_str());
			printf("Remaining: '%s'\n", pointer->lettersAvailable.c_str());
			printf("Guesses: '%s'\n", pointer->guessedSoFar.c_str());

			if (pointer->next == NULL)
				{
				break;
				}
			else
				{
				pointer = pointer->next;
				}
			}
		}

	printf("\n");
	printf("Word Number: %d\n", wordNumber);
	printf("Word: '%s'\n", currentWord.c_str());
	printf("Final Guess: '%s'\n", playerGuess.c_str());
	printf("Remaining: '%s'\n", lettersAvailable.c_str());
	printf("Guesses: '%s'\n", guessedSoFar.c_str());

	printf("\n");
	}

//save linked list to file
void saveWordsAndGuessesToFile(wordsAndGuesses* pointer, int wordNumber, string currentWord, string playerGuess, 
	                           string lettersAvailable, string guessedSoFar)
	{
	string filepath;
	int filepathAccepted = 0;
	char acceptFilepath;

	//get filename
	printf("Please specify the filename to save the list to:");
	getline(cin, filepath);
	
	while(filepathAccepted == 0)
		{
		printf("Save to file '%s'? (Y/N)", filepath.c_str());

		acceptFilepath = getchar();
		getchar();				//to consume "\n" character in order to prevent the newline problem

		if(acceptFilepath == 89 || acceptFilepath == 121)
			{
			filepathAccepted = 1;
			}
		else 
			{
			printf("Please specify the filename to save the list to:");
			getline(cin, filepath);
			}
		}
	
	//automatically add the extension ".txt" to filepath
	string filepath2 = filepath;
	string txt = ".txt";
	filepath2 = filepath + txt;

	//create file of that name
	ofstream fileOutput;
	fileOutput.open(filepath2.c_str());
	
	//put data into file	

	if (pointer != NULL)
		{
		while (1)
			{
			fileOutput << "Word " << pointer->wordNumber << ": '" << pointer->currentWord << "', '" << pointer->playerGuess << "', '" << pointer->lettersAvailable << "', '" << pointer->guessedSoFar << "'" << endl;

			if (pointer->next == NULL)
				{
				break;
				}
			else
				{
				pointer = pointer->next;
				}
			}
		}

	fileOutput << "Word " << wordNumber << ": '" << currentWord << "', '" << playerGuess << "', '" << lettersAvailable << "', '" << guessedSoFar << "'" << endl;
			
	fileOutput.close();
	
	printf("Saved words to file '%s'\n", filepath.c_str());
	}

int main(int argc, char *argv[])
	{
	int wrongGuessesNum = 10;
	int seed = (unsigned int)time(NULL);
	const char *filePath = "words.txt";
	int considerCapitals = 0;

	int i = 0;

	for(i=0; i<argc; i++)
		{
		if (argv[i+1] == NULL)
			{
			break;
			}
		else
			{ 
			string argument = argv[i+1];
			string argument2 = argv[i+1];

			argument.erase(argument.begin() + 2, argument.end());
			const char *argumentType = argument.c_str();
			
			argument2.erase(argument2.begin(), argument2.begin() + 2);
			const char *argumentParameter = argument2.c_str();
			
			if (strcmp(argumentType, "-g") == 0 || strcmp(argumentType, "-G") == 0)
				{
				wrongGuessesNum = atoi(argumentParameter);
				}
			else if (strcmp(argumentType, "-s") == 0 || strcmp(argumentType, "-S") == 0)
				{
				seed = atoi(argumentParameter);
				}
			else if (strcmp(argumentType, "-f") == 0 || strcmp(argumentType, "-F") == 0)
				{
				filePath = argumentParameter;
				}
			else if (strcmp(argumentType, "-n") == 0 || strcmp(argumentType, "-N") == 0)
				{
				considerCapitals = 1;
				}
			}	
		}

	string line;
	int wordCount = 0;
	int longestWordLength = 0;
	ifstream dictionaryFile;
	dictionaryFile.open(filePath);

	while (getline(dictionaryFile, line))
		{
		const char *tmp = line.c_str();
		
		if ((int)strlen(tmp) > longestWordLength)
			{
			longestWordLength = strlen(tmp);
			}

		wordCount++;
		}

	dictionaryFile.close();
	
	printf("Welcome to CPP Hangman (G52CPP Lab Exercise)\n");
	printf("Random seed was %d\n", seed);
	printf("Load dictionary from file: '%s'\n", filePath);
	printf("%d words with max length %d\n", wordCount, longestWordLength);

	int wordNumber = 0;
	wordsAndGuesses* pointerToWordsAndGuessesLinkedList = NULL;
	srand(seed);
	
	while (1)
		{	
		int randomWordIndex = rand() % wordCount;

		int wordFinder = 0;
		string currentWord;
		dictionaryFile.open(filePath);

		while (getline(dictionaryFile, line))
			{
			wordFinder++;

			//check if word is valid and consider capitals
			int wordIsValid = 1;
			int j = 0;
			int lineLength = line.size();

			if (considerCapitals == 0)
				{
				for (j = 0; j < lineLength; j++)
					{
					if (line.at(j) < 97 || line.at(j) > 122)
						{
						wordIsValid = 0;
						break;
						}
					}
				}
			else if (considerCapitals == 1)
				{
				for (j = 0; j < lineLength; j++)
					{
					if (j == 0)
						{
						if (line.at(j) < 65 || line.at(j) > 122)
							{
							wordIsValid = 0;
							break;
							}
						else if (line.at(j) > 90 && line.at(j) < 97)
							{
							wordIsValid = 0;
							break;
							}
						}
					else
						{
						if (line.at(j) < 97 || line.at(j) > 122)
							{
							wordIsValid = 0;
							break;
							}
						}
					}
				}

			if (wordFinder >= randomWordIndex && wordIsValid == 1)
				{
				currentWord = line;
				break;
				}
			}

		dictionaryFile.close();

		int lengthOfCurrentWord = strlen(currentWord.c_str());

		string lettersAvailable = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		string playerGuess = "";
		string guessedSoFar = "";
		int wrongGuesses = wrongGuessesNum;

		for (i = 0; i < lengthOfCurrentWord; i++)
			{
			playerGuess += "-";
			}

		while (wrongGuesses > 0)
			{
			printf("Guess the word : '%s'\n", playerGuess.c_str());
			printf("Available : %s\n", lettersAvailable.c_str());
			printf("Guessed so far : %s\n", guessedSoFar.c_str());
			printf("%d more wrong guesses permitted.\n", wrongGuesses);
			printf("Type the letter of your guess and ENTER/RETURN:");

			char letterGuess;
			letterGuess = getchar();
			getchar();		//to consume "\n" character in order to prevent the newline problem

			if (letterGuess == 33)
				{
				exit(1);
				}

			//check if letter is valid and only accept letters in the latin alphabet. only accept uppercase letters if considerCapitals = 1
			int inputIsValid = 1;
			if (letterGuess >= 97 && letterGuess <= 122)									//if letter is lowercase, do not convert it
				{
				inputIsValid = 1;
				}
			else if (letterGuess >= 65 && letterGuess <= 90 && considerCapitals == 0)		//if letter is uppercase
				{
				letterGuess += 32;
				inputIsValid = 1;
				}
			else if (letterGuess >= 65 && letterGuess <= 90 && considerCapitals == 1)
				{
				inputIsValid = 1;
				}
			else if (letterGuess == 33)
				{
				printf("Exiting game");
				std::exit(0);
				}
			else if (letterGuess == 63)
				{
				printf("The secret word is %s\n", currentWord.c_str());
				inputIsValid = 0;
				}
			else if (letterGuess == 42)
				{
				printWordsAndGuesses(pointerToWordsAndGuessesLinkedList, wordNumber, currentWord, playerGuess,
					                 lettersAvailable, guessedSoFar);
				inputIsValid = 0;
				}
			else if (letterGuess == 36)
				{
				saveWordsAndGuessesToFile(pointerToWordsAndGuessesLinkedList, wordNumber, currentWord, playerGuess, 
					                      lettersAvailable, guessedSoFar);
				inputIsValid = 0;
				}
			else
				{
				printf("Invalid input, only characters A to Z from the alphabet are accepted.\n");
				printf("Upper case letters are only accepted if the capitals command line argument is set.\n");
				inputIsValid = 0;
				}

			//check if letter is not in current word
			if (currentWord.find(letterGuess) == -1 && inputIsValid == 1)
				{
				guessedSoFar += letterGuess;
				wrongGuesses--;

				if (wrongGuesses == 0)
					{
					printf("*** Wrong! You have run out of guesses. You lose. The word was %s\n", currentWord.c_str());

					//add to linked list + restart game
					addElementToLinkedList(pointerToWordsAndGuessesLinkedList, wordNumber, currentWord, playerGuess, 
						                   lettersAvailable, guessedSoFar);
					wordNumber++;
					}
				else
					{
					printf("*** Wrong! You are permitted another %d wrong guesses before you lose.\n", wrongGuesses);
					}
				}
			else if (inputIsValid == 1)
				{
				guessedSoFar += letterGuess;

				for (i = 0; i < lengthOfCurrentWord; i++)
					{
					if (currentWord.at(i) == letterGuess)
						{
						playerGuess[i] = letterGuess;
						}
					}

				//check if word has been completed
				if (playerGuess.find("-") == -1)
					{
					printf("*** Well done, you have guessed all of the letters.\n");
					printf("Well done! The word was '%s'\n", currentWord.c_str());

					//add to linked list + restart game
					pointerToWordsAndGuessesLinkedList = 
						addElementToLinkedList(pointerToWordsAndGuessesLinkedList, wordNumber, currentWord, playerGuess, 
							                   lettersAvailable, guessedSoFar);
					wordNumber++;

					break;
					}
				}

			//remove letter chosen from letters available string
			if (inputIsValid == 1)
				{
				if(letterGuess >= 97 && letterGuess <= 122)			//if lower case, convert to upper case
					{
					letterGuess -= 32;
					}

				int letterToRemove = lettersAvailable.find(letterGuess);

				char tmp = 45;

				if (letterToRemove != -1)
					{
					lettersAvailable[letterToRemove] = tmp;
					}
				}
			}
		}

	return 0;
	}