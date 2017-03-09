#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/load.h"


//Check if the word is the name of a type
int is_typeword(const char* instruction) //Return 1 if allowed
{
    int i = 0;
    char allowedType[4][20] = {"integer", "float", "string", "char"}; //array of forbidden words
    
    for(i = 0; i < 4; i++)
        if(strcmp(allowedType[i], instruction) == 0) //check if the instruction is in the allowed type array
			return 0;

    return 1;
}


//Check if the element is allowed
int is_forbidden(const char* instruction) //Return 1 if allowed
{
    int i = 0;
    char forbiddenWords[8][20] = {"true", "false", "exit", "help", "cls", "clear", "noam", "Noam"}; //array of forbidden words
    
    for(i = 0; i < 4; i++)
        if(strcmp(forbiddenWords[i], instruction) == 0) //check if the instruction is in the forbbiden words array
			return 0;

    return 1;
}


//Function that checks if a given variable name is usable
/* RETURN VALUES
	0 = not usable (name is forbidden)
	1 = usable, a variable has already this name
	2 = usable, no variable has already this name
*/
int check_if_usable(const char* varName, Variable *var_table)
{
	int i = 0;

	//Checking if the given name is in the table of forbidden names
	if (!is_forbidden(varName))
		return 0;

	//Checking if the given name is already attributed to a variable
	for (i = 0; strcmp(var_table[i].name, NAME__END_VARTABLE) != 0; i++)
		if (strcmp(varName, var_table[i].name) == 0)
			return 1;

	return 2;
}


//Give a type to a variable when trying to declare a new variable
void check_variable_type(char** elements, int** types, int i)
{
	if(strcmp(elements[i - 2], ":") == 0 && is_typeword(elements[i - 1]) == 0)
	{
		if(strcmp(elements[i - 1], "integer") == 0)
			(*types)[i - 3] = 5;

		else if(strcmp(elements[i - 1], "float") == 0)
			(*types)[i - 3] = 6;
		
		else if(strcmp(elements[i - 1], "string") == 0)
			(*types)[i - 3] = 7;
	}
	else
	{
		(*types)[i - 3] = 0;
		(*types)[i - 1] = 0;
	}
}


//Function that divides the string into substrings (one for each element of the line)
/* Types:
	0 = declaration error
	1 = int
	2 = float
	3 = string
	4 = operator
	5 = new int variable
	6 = new float variable
	7 = new string variable
	8 = variable
	9 = comparator

	The number of elements is stored in types at subscript 0 ( (*types)[0] = number of elements )
*/
/* VALGRIND ERRORS

	#1: Solved
		Not enough space was allocated to store the string "<<<" and the '\0' character

	#2: Not solved


*/
int get_line_elements(const char* line, char*** elements, int** types, int* position) //Returns 0 if syntax error
{
	int i = 0, j = 1, k = 1;

	//Allocating memory for new arrays
	*types = (int*) malloc(sizeof(int));
	check_alloc(*types);

	*elements = (char**) malloc(sizeof(char*)); //ERRORS DETECTED BY VALGRIND (ERRORS all but #1) ==> ALLOC
	check_alloc(*elements);


	//Initialising the number of elements to 0
	(*types)[0] = 0;

	//Initializing the first element to "<<<" since the first subscript (0) of types contains the total number of elements
	(*elements)[0] = (char*) malloc(4 * sizeof(char));
	check_alloc((*elements)[0]);
	
	strcpy((*elements)[0], "<<<");


	while (line[i] != '\0')
	{

		//If an operator was found
		if ( ( (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%') && line[i+1] == ' ' && line[i+2] != '\0'))
		{

			if (i < 2)
			{
				*position = i;
				return 0;
			}

			(*types)[0]++; //Incrementing the number of elements

			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
			check_alloc(*elements);

			(*elements)[j] = (char*) malloc(2 * sizeof(char));
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int));
			check_alloc(*types);

			//Storing the character in a string
			(*elements)[j][0] = line[i];
			(*elements)[j][1] = '\0';

			//Setting type of element to 5 (operator)
			(*types)[j++] = 4;
		}

		//If a comparator was found
		if ( ( (line[i] == ':' || line[i] == '=' || line[i] == '<' || line[i] == '>') && line[i+1] == ' ' && line[i+2] != '\0') || ( ( (line[i] == '!' && line[i+1] == '=') || (line[i] == '<' && line[i+1] == '=') ) && line[i+2] == ' ' && line[i+3] != '\0'))
		{

			if (i < 2)
			{
				*position = i;
				return 0;
			}

			(*types)[0]++; //Incrementing the number of elements

			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
			check_alloc(*elements);

			(*elements)[j] = (char*) malloc(2 * sizeof(char));
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int));
			check_alloc(*types);

			//Storing the character in a string
			(*elements)[j][0] = line[i];
			(*elements)[j][1] = '\0';

			//Setting type of element to 5 (operator)
			(*types)[j++] = 9;
		}

		//If a "word" beginning by a number (1, 2, 3...) or by  was found
		if ( (line[i] >= 48 && line[i] <= 57) || (line[i] == '-' && line[i + 1] != '\0' && line[i + 1] >= 48 && line[i + 1] <= 57) )
		{
			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
			check_alloc(*elements);

			(*elements)[j] = (char*) malloc(2 * sizeof(char)); //Allocating memory to store this number appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this number type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			//Incrementing the number of elements
			(*types)[0]++;

			k = 2;

			//If the number may be negative
			if (line[i] == '-')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
			}


			//Reading the line until founding a character which is not a number
			while (line[i] != ' ' && line[i] >= 48 && line[i] <= 57 && line[i] != '\0')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
			}


			//Determining the type of the element
			if (line[i] == ' ' || line[i] == '\0') //If the element is of type int
				(*types)[j] = 1;
			else
				if (line[i] != '.' && line[i] != '\0') //Syntax error
				{
					*position = i;
					return 0;
				}

			if (line[i] == '.' && line[i + 1] >= 48 && line[i + 1] <= 57)
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
				
				while (line[i] != ' ' && line[i] >= 48 && line[i] <= 57 && line[i] != '\0')
				{

					(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
					check_alloc((*elements)[j]);

					(*elements)[j][k - 2] =  '\0';
					(*elements)[j][k - 3] = line[i++];
				}

				if (line[i] == ' ' || line[i] == '\0') //If the element is of type float
					(*types)[j] = 2;
				else //Syntax error
					if (line[i] != '\0')
					{
						*position = i;
						return 0;
					}
			}

			if (line[i] == '.' && (line[i+1] < 48 || line[i+1] > 57))
			{
				*position = i + 1;
				return 0;
			}


			j++;
		}

		//If a "word" beginning by a letter (a,b,c,...,A,B,C,...) was found
		if ( ( (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122) ) && line[i] != '\0')
		{
			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
			check_alloc(*elements);


			(*elements)[j] = (char*) malloc(2 * sizeof(char)); //Allocating memory to store this letter appart the original line //ERROR DETECTED BY VALGRIND HERE (ERROR #2) (WRITE ERROR)
			check_alloc((*elements)[j]); //ERROR DETECTED BY VALGRIND (ERROR #3) (READ ERROR)

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this letter type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			//Incrementing the number of elements
			(*types)[0]++;

			k = 2;

			while (line[i] != ' ' && ( (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122) || ( (line[i] >= 48) && (line[i] <= 57) ) || line[i] == '_' ) && line[i] != '\0' && line[i] != ':')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char)); //ERRORS DETECTED BY VALGRIND (ERRORS #4 #5) (READ ERROR + WRITE ERROR)
				check_alloc((*elements)[j]); //ERROR DETECTED BY VALGRIND (ERROR #6) (READ ERROR)

				(*elements)[j][k - 2] =  '\0'; //ERROR DETECTED BY VALGRIND (ERROR #7) (READ ERROR)
				(*elements)[j][k - 3] = line[i++]; //ERROR DETECTED BY VALGRIND (ERROR #8) (READ ERROR)
			}

			(*types)[j] = 8;

			j++;
		}

		//If a "word" beginning by " was found (if a string delimited by "" was found)
		if (line[i] == '"' && line[i+1] != '\0')
		{

			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, j * sizeof(char*));
			check_alloc(*elements);

			(*elements)[j] = (char*) malloc(3 * sizeof(char)); //Allocating memory to store this string appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this string type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			k = 2;
			(*elements)[j][0] = line[i++];
			(*elements)[j][1] = '\0';


			while(line[i] != '"' && line[i] != '\0')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				//Storing the message
				(*elements)[j][k - 2] = '\0';
				(*elements)[j][k - 3] = line[i++];
			}

			if (line[i] == '\0') //Syntax error, no closing ", returning 0
			{
				*position = i;
				return 0;
			}

			(*types)[0] += 1; //Incrementing the number of elements

			(*types)[j++] = 3; //Setting the element's type to string
		}


		i++;
	}

	if((*types)[j-1] == 8 && j > 2 && strcmp((*elements)[j-2], ":") == 0) //ERROR DETECTED BY VALGRIND (ERROR #9) (READ ERROR)
		check_variable_type(*elements, types, j);

	//Checking if the structure of the line is correct
	if (j >= 2) //If at least two elements
		for (i = 0; i < j - 1; i++)
			if ((*types)[i] == (*types)[i+1])
				return 2;
	if ((*types)[j] == 4)
		return 2;

	return 1;
}


//Function that displays the elements (essential for testing)
void display_elements(char** elements, int* types)
{
	int i = 1;

	for (i = 1; i <= types[0]; i++)
	{
		printf("Element %d = %s\t", i, elements[i]);
		printf("type id: %d\n", types[i]);
	}

	printf("\n");
}