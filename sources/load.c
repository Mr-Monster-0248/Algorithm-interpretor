#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"

//Function that is run at the start of the program, returns different values depending on the mode chosen by the user
int check_parameters(int argc, char** argv)
{
	FILE* testFile = NULL;
	char filePath[30] = "IO/", convert;

	//Checking if a command line argument was given
	if (argc >= 2)
	{
		strcat(filePath, argv[1]); //Building file path

		testFile = fopen(filePath, "r"); //Attempting to open the file

		if (testFile == NULL) //If file has not been oppened
		{
			printf("The program cannot manage to open the file %s in the folder IO\nQuit the program? (y/n)\t> ", argv[1]);

			//Asking the user whether to quit the program or not
			do
			{
				fflush(stdin);
				filePath[0] = '\n';
				filePath[0] = getchar();
			} while (filePath[0] != 'y' && filePath[0] != 'Y' && filePath[0] != 'n' && filePath[0] != 'N');

			if (filePath[0] == 'y' || filePath[0] == 'Y')
			{
				//Exiting the program
				system(CLEAR);
				exit(EXIT_SUCCESS);	
			}

			return 1; //Launching the direct line interpreter
		}

		fclose(testFile);

		do
		{
			convert = 'a';
			system(CLEAR);
			printf("Type 1 to convert %s to C language (.c file)\nType 2 to execute %s directly\nYour choice: ", argv[1], argv[1]);
			
			convert = getchar();
			fflush(stdin);
		} while (convert != '1' && convert != '2');


		if (convert == '1')
			return 2; //Launching the C converter

		return 3; //Executing the program directly
	}

	return 1; //If no file was given as parameter, returning 1: launching the direct line interpreter
}


//Function that checks memory allocation of a pointer
void check_alloc(void* pointer)
{
	if (pointer == NULL)
	{
		printf("A memory error occured, please launch the program again\n");
		exit(EXIT_FAILURE);
	}
}


//Function that removes the '\n' character at the end of a string
void del_newLine_char(char* string)
{
	char *ptr = strchr(string, '\n');

	if (ptr)
		*ptr = '\0';
}


//Function that displays a file's content
void read_file(const char* pathToFile)
{
	FILE* file = fopen(pathToFile, "r");
	check_alloc(file);

	do
		printf("%c", fgetc(file));
	while(!feof(file));

	fclose(file);

	printf("\n");
}


//Function that checks whether the user entered a special command or not
/* RETURN VALUES

	0 = no special command
	1 = continue
	2 = exit
*/
int check_special_command(const char* line)
{
	//Easter egg
	if (strcmp(line, "noam") == 0 || strcmp(line, "noam ") == 0 || strcmp(line, "Noam") == 0 || strcmp(line, "Noam ") == 0)
	{
		printf("is a faggot\n\n");
		return 1;
	}

	if (strcmp(line, "exit") == 0 || strcmp(line, "exit ") == 0)
	{
		printf("\nGoodbye!\n\n");
		return 2;
	}

	if (strcmp(line, "help") == 0 || strcmp(line, "help ") == 0)
	{
		system(CLEAR);

		read_file(HELPFILE);
		return 1;
	}


	if (strcmp(line, "clear") == 0 || strcmp(line, "clear ") == 0 || strcmp(line, "cls") == 0 || strcmp(line, "cls ") == 0)
	{
		system(CLEAR);
		printf("\n");
		return 1;
	}

	if (strcmp(line, "bite") == 0 || strcmp(line, "bite ") == 0 || strcmp(line, "Bite") == 0 || strcmp(line, "Bite ") == 0)
	{
		printf("SIZE ERROR: too small to be handled\n\n");
		return 1;
	}

	return 0;
}


//Function to free 2D char array
void free_2D_char_array(char*** array, int subs)
{
	for (subs = subs; subs >= 0; subs--)
		free((*array)[subs]);

	free(*array);

	*array = NULL;
}


//Function that shifts the values to the left in elements after an operation
void shift_elements(char*** elements, int** types, int subscript)
{
	int i = 0;

	//Checking the validity of the given subscript
	if (subscript >= 0 && subscript <= (*types)[0] - 1)
	{
		//Shifting to the left twice in elements and types from the subscript right after the given one
			for (i = subscript; i <= (*types)[0] - 1; i++)
			{
				free( (*elements)[i] );
				(*elements)[i] = NULL;

				(*elements)[i] = (char*) malloc( (strlen((*elements)[i + 1]) + 1) * sizeof(char));
				check_alloc((*elements)[i]);

				strcpy((*elements)[i], (*elements)[i + 1]);
				(*types)[i] = (*types)[i + 1];
			}

			free((*elements)[i]);

			(*types)[0]--;
	}
}


//Function that return the line read in the given file
char* read_file_line(FILE* file)
{
	char c[2], *line = (char*) malloc(sizeof(char));
	int i = 1;

	check_alloc(line);

	c[1] = '\0';

	//While we do not reach the end of the line or of the file, reading a single character at the time
	do
	{
		//Reallocating enough space to store the whole line
		line = (char*) realloc(line, i++ * sizeof(char));
		check_alloc(line);

		//Reading a single character at the time from the file
		c[0] = fgetc(file);

		//If the reached the end of the line or the end of the file
		if (feof(file) || c[0] == '\n')
		{
			c[0] = '\0';

			strcat(line, c);

			return line;
		}

		//Addind the read character to the line to return
		strcat(line, c);

	} while (1);

	//The function is not supposed to reach this point, but we need to avoid compilation warnings
	return line;
}
