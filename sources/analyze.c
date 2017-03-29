#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/load.h"
#include "../headers/operations.h"


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
   10 = element between parentheses

	The number of elements is stored in types at subscript 0 ( (*types)[0] = number of elements )
*/
int get_line_elements(const char* line, char*** elements, int** types, int* position) //Returns 0 if syntax error
{
	int i = 0, j = 1, k = 1, oParentheses = 0;

	//Allocating memory for new arrays
	*types = (int*) malloc(sizeof(int));
	check_alloc(*types);

	*elements = (char**) malloc(sizeof(char*));
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
				return 2;
			
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

		//If a "word" beginning by a number (1, 2, 3...) or by - was found
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


			(*elements)[j] = (char*) malloc(2 * sizeof(char));
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this letter type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			//Incrementing the number of elements
			(*types)[0]++;

			k = 2;

			while (line[i] != ' ' && ( (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122) || ( (line[i] >= 48) && (line[i] <= 57) ) || line[i] == '_' ) && line[i] != '\0' && line[i] != ':')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0'; 
				(*elements)[j][k - 3] = line[i++];
			}

			(*types)[j] = 8;

			j++;
		}

		//If a "word" beginning by " was found (if a string delimited by "" was found)
		if (line[i] == '"' && line[i+1] != '\0')
		{

			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
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

		//If elements in parentheses were found
		if (line[i] == 40 /* ASCII 40 = "(" */ && line[i+1] != '\0')
		{
			oParentheses++;

			//Reallocating memory for the first dimension
			*elements = (char**) realloc(*elements, (j+1) * sizeof(char*));
			check_alloc(*elements);

			(*elements)[j] = (char*) malloc(3 * sizeof(char)); //Allocating memory to store this element appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this element's type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			k = 2;
			(*elements)[j][0] = line[++i];
			(*elements)[j][1] = '\0';


			while(oParentheses > 0 && line[i] != '\0')
			{
				if (line[i] == 40) // New oppening parenthese
					oParentheses++;

				if (line[i] == 41) // New closing parenthese
					if (--oParentheses == 0)
						break;

				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				//Storing the message
				(*elements)[j][k - 2] = '\0';
				(*elements)[j][k - 3] = line[i++];
			}

			if (line[i] == '\0' && oParentheses > 0) //Syntax error, no closing ", returning 0
			{
				*position = i;
				return 0;
			}

			(*types)[0] += 1; //Incrementing the number of elements

			(*types)[j++] = 10; //Setting the element's type to string
		}

		i++;
	}

	*position = i;

	if((*types)[j-1] == 8 && j > 2 && strcmp((*elements)[j-2], ":") == 0)
		check_variable_type(*elements, types, j);

	//Checking if the structure of the line is correct
	if (j >= 2) //If at least two elements
	{
		for (i = 1; i < j - 1; i++)
			if ((*types)[i] == (*types)[i+1])
				return 2;
		if ((*types)[j-1] == 4)
			return 0;
	}

	return 1;
}


//Function that displays the elements (essential for testing)
void display_elements(char** elements, int* types)
{
	int i = 1;

	for (i = 1; i <= types[0]; i++)
	{	
		printf("Element %2d\tType id: %d\t", i, types[i]);
		printf("read: %s\n", elements[i]);
	}

	printf("\n");
}


//Function that checks the entry of the user and stores a new variable is asked (returns 0 if no new variable, 1 if a new variable is stored)
int check_variable_declaration(char** elements, int* types, Variable** var_table)
{
	int i = 0;

	if (types[0] >= 2)
	{
		if (strcmp(elements[1], "integer") == 0) //New integer variable was asked
		{
			if (check_if_usable(elements[2], *var_table) == 2) //If the name given is available
			{
				if (types[0] == 3) //If the user tried to initialize a variable with a value without giving this value
				{
					printf("ERROR: missing initialization value\n");
					return 0;
				}

				if (types[0] == 2)
				{
					store_variable(var_table, elements[2], "uninitialized", 1); //Storing a new uninitialized integer variable

				} else if (types[0] == 4 && strcmp(elements[3], "<-") == 0) {

					store_variable(var_table, elements[2], elements[4], 1); //Storing a new initialized integer variable

				}
			} else {
				return 0;
			}
		}


		if (strcmp(elements[1], "float") == 0) //New float variable was asked
		{
			if (check_if_usable(elements[2], *var_table) == 2) //If the name given is available
			{
				if (types[0] == 3) //If the user tried to initialize a variable with a value without giving this value
				{
					printf("ERROR: missing initialization value\n");
					return 0;
				}

				if (types[0] == 2)
				{
					store_variable(var_table, elements[2], "uninitialized", 2); //Storing a new uninitialized float variable

				} else if (types[0] == 4 && strcmp(elements[3], "<-") == 0) {

					store_variable(var_table, elements[2], elements[4], 2); //Storing a new initialized float variable

				}
			} else {
				return 0;
			}
		}


		if (strcmp(elements[1], "string") == 0) //New string variable was asked
		{
			if (check_if_usable(elements[2], *var_table) == 2) //If the name given is available
			{
				if (types[0] == 3) //If the user tried to initialize a variable with a value without giving this value
				{
					printf("ERROR: missing initialization value\n");
					return 0;
				}

				if (types[0] == 2)
				{
					store_variable(var_table, elements[2], "uninitialized", 3); //Storing a new uninitialized string variable

				} else if (types[0] == 4 && strcmp(elements[3], "<-") == 0) {

					store_variable(var_table, elements[2], elements[4], 3); //Storing a new initialized string variable

				}
			} else {
				return 0;
			}
		}
	}

	if (types[0] == 1)
	{
		for (i = 0; strcmp((*var_table)[i].name, NAME__END_VARTABLE) != 0; i++)
			if (strcmp((*var_table)[i].name, elements[2]) == 0)
			{
				printf("%s\n", (*var_table)[i].value); //Displaying the value of the variable
				return 0;
			}

		printf("Variable %s not found, maybe it has not been created yet\n", elements[2]);
	}

	return 0;
}


//Function that returns the type of line read in a file
/* RETURN VALUES
   -1 = line not allocated
	0 = Commented line
	1 = Empty line
	2 = File structure indicator line (@ VARS, @ ENDVARS, @ BEGIN, @ END)
	3 = Special statements line ($ if, $ else, $ for, $ while, $ endif, $ endfor, $ endwhile)
	4 = Classic line (assignation of value to a variable)
*/
int check_file_line_comment(char* line)
{
	int i = 0;

	if (line != NULL)
	{
		//Skipping the '\t' at the beginning of each line
		for (i = 0; line[i] != '\0' && line[i] == '\t'; i++);

		if (line[i] == '#') //If the program encounters a commented line
			return 0;

		if (line[i] == '\0') //If the program encounters an empty line
			return 1;

		if (line[i] == '@') //If the program encounters a file structure indicator (@ VARS, @ ENDVARS, @ BEGIN, @ END)
			return 2;

		if (line[i] == '$')
			return 3;

		return 4;
	}

	return -1;
}
