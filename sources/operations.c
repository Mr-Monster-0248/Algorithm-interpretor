#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/operations.h"
#include "../headers/load.h"
#include "../headers/analyze.h"


//Function that displays where the SYNTAX ERROR was made
void disp_error(const int position)
{
	int i = 0;
	for (i = 0; i < position + 4; i++)
		printf(" ");

	printf("^\nERROR: SYNTAX ERROR\n\n");
}


//Function that displays where the SYNTAX ERROR was made at which line of the file
void disp_error_line(const char* line, const int position, const int lineNumber)
{
	int i = 0;

	printf("SYNTAX ERROR: line %d of the given file:\n>>> %s\n", lineNumber, line); //Displaying the line

	for (i = 0; i < position + 4; i++)
		printf(" ");
	printf("^\n\n");

	exit(EXIT_SUCCESS);
}


//Function that converts a string to an int
int convertTo_int(const char* value)
{
	return (int) strtol(value, NULL, 10);
}


//Function that converts a string to a float
float convertTo_float(const char* value)
{
	float val;
	sscanf(value, "%f", &val);

	return val;
}
/*

//Function that converts a boolean string to its associated int value
int bool_to_int(const char* value)
{
	if (strcmp(value, "true") == 0 || strcmp(value, "TRUE") == 0)
		return TRUE;
	
	if (strcmp(value, "false") == 0 || strcmp(value, "FALSE") == 0)
		return FALSE;

	return BOOLEAN_ERROR_CODE;
}
*/

//Function that performs an multiplication of integers values (formatted to string)
int int_multiplication(const char* lValue, const char* rValue)
{
	return convertTo_int(lValue) * convertTo_int(rValue);
}


//Function that performs an multiplication of float values (formatted to string)
float float_multiplication(const char* lValue, const char* rValue)
{
	return convertTo_float(lValue) * convertTo_float(rValue);
}


//Function that performs an addition of integers values (formatted to string)
int int_addition(const char* lValue, const char* rValue)
{
	return convertTo_int(lValue) + convertTo_int(rValue);
}


//Function that performs an addition of float values (formatted to string)
float float_addition(const char* lValue, const char* rValue)
{
	return convertTo_float(lValue) + convertTo_float(rValue);
}

//Function that performs an subtraction of integers values (formatted to string)
int int_subtraction(const char* lValue, const char* rValue)
{
	return convertTo_int(lValue) - convertTo_int(rValue);
}

//Function that performs a division of integers values (formatted to string)
int int_division(const char* lValue, const char* rValue)
{
	if( convertTo_int(rValue) == 0)
		return convertTo_int(lValue) + 1;

	return convertTo_int(lValue) / convertTo_int(rValue);
}


//Function that performs a subtraction of floats
float float_subtraction(const char* lValue, const char* rValue)
{
	return convertTo_float(lValue) - convertTo_float(rValue);
}


//Function that performs a division of floats
float float_division(const char* lValue, const char* rValue)
{
	float f = convertTo_float(rValue);

	if (f != 0)
		return convertTo_float(lValue) / f;

	return convertTo_float(lValue) + 1;
}


//Function that performs the modulo of integers
int int_modulo(const char* lValue, const char* rValue)
{
	if (convertTo_int(rValue) == 0) //If cannot perform operation
		return convertTo_int(lValue) + 1;

	return convertTo_int(lValue) % convertTo_int(rValue);
}


//Function that check if the line is an operation 
int is_operation(int* types, char** elements) //Return 1 if it's an int operation 2 if it's a float operation 3 if it is a string operation, -1 for error
{
	int i, testInt = 0, testString = 0, testFloat = 0;

	for(i = 1; i <= types[0]; i++)
	{
		if(i % 2 == 1)
		{
			if(types[i] == 1)
				testInt++;
			if(types[i] == 2)
				testFloat++;
			if(types[i] == 3)
				testString++;
		}
		else
			if(types[i] != 4)
				return 0;
	}

	if(testString > 0)
		return 3;
	else if(testInt > 0 && testString == 0 && testFloat == 0)
		return 1;
	else if(testFloat > 0 && testString == 0)
		return 2;
	else
		return 0;
}


//Function that stores a new variable in the array of all variables
void store_variable(Variable** var_table, char* varName, const int varType)
{
	int size = 0;

	//Determining the current size of the array
	for (size = 0; strcmp((*var_table)[size].name, NAME__END_VARTABLE) != 0; size++);

	//Reallocating memory for the array of variables
	*var_table = realloc(*var_table, (size + 2) * sizeof(Variable));
	check_alloc(*var_table);

	//Deleting the data of the last variable stored
	free( (*var_table)[size].name );
	free( (*var_table)[size].value );

	//Allocating memory for the name and value of the new variable
	(*var_table)[size].name = (char*) malloc( (strlen(varName) + 1) * sizeof(char));
	(*var_table)[size].value = (char*) malloc( (strlen(UNINITIALIZED_VAR_VALUE) + 1) * sizeof(char));
	check_alloc((*var_table)[size].name);
	check_alloc((*var_table)[size].value);

	//Storing the new name, value and type
	(*var_table)[size].type = varType;
	sprintf((*var_table)[size].name, "%s", varName);
	sprintf((*var_table)[size++].value, "%s", UNINITIALIZED_VAR_VALUE);

	//Creating a "end variable" to mark the end of the array of variables
	(*var_table)[size].name	= (char*) malloc( (strlen(NAME__END_VARTABLE) + 1) * sizeof(char));
	(*var_table)[size].value = (char*) malloc( (strlen(VALUE__END_VARTABLE) + 1) * sizeof(char));
	(*var_table)[size].type = TYPE__END_VARTABLE;
	check_alloc((*var_table)[size].name);
	check_alloc((*var_table)[size].value);
	sprintf((*var_table)[size].name, "%s", NAME__END_VARTABLE);
	sprintf((*var_table)[size].value, "%s", VALUE__END_VARTABLE);
}


//Function that check if the line is a comparison
int is_comparison(int* types) //return 1 if it's a numerical comparison and 2 for a string comparison, -1 for an error
{
	int i, testNum = 0, testString = 0;

	for(i = 1; i <= types[0]; i++)
	{
		if(i % 2)
			if(types[i] == 1 || types[i] == 2)
				testNum++;
			else
				testString++;
		else
			if(types[i] != 9)
				return 0;
	}

	if(testString > 0 && testNum == 0)
		return 2;
	if(testNum > 0 && testString == 0)
		return 1;
	if(testNum > 0 && testString > 0)
		return -1;

	return 0;
}


//Function that performs the boolean comparison between two integers/float
int eval_bool_int(const char* lValue, const char* rValue, const char* comparator)
{
	float val1 = convertTo_float(lValue), val2 = convertTo_float(rValue);

	if(strcmp(comparator, "=") == 0)
	{
		if(val1 == val2)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "<") == 0)
	{
		if(val1 < val2)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "<=") == 0 || strcmp(comparator, "=<") == 0)
	{
		if(val1 <= val2)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, ">") == 0 )
	{
		if(val1 > val2)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, ">=") == 0 || strcmp(comparator, "=>") == 0)
	{
		if(val1 >= val2)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "!=") == 0)
	{
		if(val1 != val2)
			return TRUE;
		return FALSE;
	}
	else if (strcmp(comparator, "AND") == 0 || strcmp(comparator, "and") == 0)
	{
		if(val1 && val2)
			return TRUE;
		return FALSE;
	}
	else if (strcmp(comparator, "OR") == 0 || strcmp(comparator, "or") == 0)
	{
		if(val1 || val2)
			return TRUE;
		return FALSE;
	}
	else
		return BOOLEAN_ERROR_CODE;
}


//Function that performs the boolean comparison between two strings
/*  
	RETURN VALUES:
	BOOLEAN_ERROR_CODE = ERROR
	FALSE = 0
	TRUE = 1
*/
int eval_bool_string(const char* lString, const char* rString, const char* comparator)
{
	if(strcmp(comparator, "=") == 0)
	{
		if(strcmp(lString, rString) == 0)
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "<") == 0)
	{
		if(strlen(lString) < strlen(rString))
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "<=") == 0 || strcmp(comparator, "=<") == 0)
	{
		if(strlen(lString) <= strlen(rString))
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, ">") == 0 )
	{
		if(strlen(lString) > strlen(rString))
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, ">=") == 0 || strcmp(comparator, "=>") == 0)
	{
		if(strlen(lString) >= strlen(rString))
			return TRUE;
		return FALSE;
	}
	else if(strcmp(comparator, "!="))
	{
		if(strcmp(lString, rString) != 0)
			return TRUE;
		return FALSE;
	}
	else
		return BOOLEAN_ERROR_CODE;
}


//Function that compute boolean comparison
/*  
	RETURN VALUES:
	BOOLEAN_ERROR_CODE = ERROR
	FALSE = 0
	TRUE = 1
*/
void compute__int_float_comparison(char*** elements, int** types)
{
	while((*types)[0] >= 3)
	{
		if(eval_bool_int((*elements)[1], (*elements)[3], (*elements)[2]))
			strcpy((*elements)[1], "true");
		else
			strcpy((*elements)[1], "false");
		(*types)[1] = 11;

		shift_elements(elements, types, 2);
		shift_elements(elements, types, 2);
	}
}


//Function that compute boolean comparison
void compute__string_comparison(char*** elements, int** types)
{
	while((*types)[0] >= 3)
	{
		if(eval_bool_string((*elements)[1], (*elements)[3], (*elements)[2]))
			strcpy((*elements)[1], "true");
		else
			strcpy((*elements)[1], "false");
		(*types)[1] = 11;

		shift_elements(elements, types, 2);
		shift_elements(elements, types, 2);
	}
}


//Function witch give a value of priority for each operator
int check_operator_priority(char* operator)
{
	if(strcmp(operator, "*") == 0)
		return 3;
	else if (strcmp(operator, "/") == 0)
		return 3;
	else if(operator[0] == '%')
		return 3;
	else if (strcmp(operator, "+") == 0)
		return 2;
	else if (strcmp(operator, "-") == 0)
		return 2;
	else if (strcmp(operator, ":") == 0)
		return 1;
	else
		return 0;
}


//Function witch find the operator with the highest priority (if it exist)
int highest_priority_operator(char** elements, int* types) //return the index of the highest operator and return 0 if there is no highest
{
	int i;

	for (i = 2; i <= types[0] - 1; i++)
		if (types[i] == 4 && check_operator_priority(elements[i]) == 3)
			return i;

	for (i = 2; i <= types[0] - 1; i++)
		if (types[i] == 4 && check_operator_priority(elements[i]) == 2)
			return i;

	for (i = 2; i <= types[0] - 1; i++)
		if (types[i] == 4)
			return i;

	return 0;
}


//Function that computes operations on integers
void compute__int_operation(char*** elements, int** types, Variable** var_table)
{
	int i = 0;

	//While there still are some * or / or % to perform, performing this highest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  3)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "*") == 0)
			sprintf((*elements)[i - 1], "%d", int_multiplication((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "%") == 0)
			sprintf((*elements)[i - 1], "%d", int_modulo((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "/") == 0)
		{
			if ( convertTo_int((*elements)[i + 1]) != 0 ) //Checking if division is possible (no division by 0 is allowed)
				sprintf((*elements)[i - 1], "%d", int_division((*elements)[i - 1], (*elements)[i + 1]));
			else
			{
				free((*elements)[i - 1]);
				(*elements)[i - 1] = (char*) malloc(36 * sizeof(char));
				check_alloc((*elements)[i - 1]);

				sprintf((*elements)[i - 1], "ERROR: cannot perform division by 0");

				break;
			}
		}

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);
	}

	//After that, performing the lowest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  2)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "+") == 0)
			sprintf((*elements)[i - 1], "%d", int_addition((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "-") == 0)
			sprintf((*elements)[i - 1], "%d", int_subtraction((*elements)[i - 1], (*elements)[i + 1]));

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);
	}

	//Variable assignation
	while ( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) == 1)
	{
		assign_value_to_variable((*elements)[1], (*elements)[3], var_table);


		shift_elements(elements, types, 2);
		shift_elements(elements, types, 2);
	}
}


//Function that computes operations on floats
void compute__float_operation(char*** elements, int** types, Variable** var_table)
{
	int i = 0;

	//While there still are some * or / or % to perform, performing this highest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  3)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "*") == 0)
			sprintf((*elements)[i - 1], "%f", float_multiplication((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "/") == 0)
		{
			if ( convertTo_int((*elements)[i + 1]) != 0 )
				sprintf((*elements)[i - 1], "%f", float_division((*elements)[i - 1], (*elements)[i + 1]));
			else
			{
				free((*elements)[i - 1]);
				(*elements)[i - 1] = (char*) malloc(36 * sizeof(char));
				check_alloc((*elements)[i - 1]);

				sprintf((*elements)[i - 1], "ERROR: cannot perform division by 0");
				break;
			}
		}

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);
	}

	//After that, performing the lowest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  2)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "+") == 0)
			sprintf((*elements)[i - 1], "%f", float_addition((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "-") == 0)
			sprintf((*elements)[i - 1], "%f", float_subtraction((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "%") == 0)
		{
			//Freeing (*elements)[1] and allocating enough space to store the error message
			free((*elements)[1]);
			(*elements)[1] = (char*) malloc(50 * sizeof(char));
			check_alloc((*elements)[1]);

			//The result which will be displayed at the end of this line will be this error message, the calculus is suposed to stop there
			sprintf((*elements)[1], "ERROR: illegal operation (%%) on floats operands\n");
			break;
		}

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);
	}

	//Variable assignation
	while ( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) == 1)
	{
		assign_value_to_variable((*elements)[1], (*elements)[3], var_table);

		shift_elements(elements, types, 2);
		shift_elements(elements, types, 2);
	}
}


//Recursive function that computes the entered line (recursive because of potential parentheses)
/* RETURN VALUES:
	0 = ERROR
	1 = NO ERROR
*/
int compute_numeric_line(char*** elements, int** types, Variable** var_table)
{
	int i = 0, errorParentheses = -1, errorType = -1;
	char** parenthesesElements = NULL;
	int* parenthesesTypes = NULL;

	//Looking for elements between parentheses
	for (i = 1; i <= (*types)[0]; i++)
	{
		//If an element between parentheses was found, computing its value
		if ((*types)[i] == 10) //If the entered line contains at least one parentheses element
		{
			errorType = get_line_elements((*elements)[i], &parenthesesElements, &parenthesesTypes, &errorParentheses);

			if (errorType == 0) //If syntax error
			{

				free((*elements)[0]);
				(*elements)[0] = (char*) malloc(39 * sizeof(char));
				check_alloc((*elements)[0]);
				sprintf((*elements)[0], "ERROR: SYNTAX ERROR, unknown position\n");

				//Freeing temporary arrays			
				free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
				free(parenthesesTypes);
				break;

			} else if (errorType == 2) //If structural error
			{

				free((*elements)[0]);

				(*elements)[0] = (char*) malloc(42 * sizeof(char));
				check_alloc((*elements)[0]);

				sprintf((*elements)[0], "ERROR: STRUCTURAL ERROR, unknow position\n");

				//Freeing temporary arrays				
				free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
				free(parenthesesTypes);

				break;
			}

			compute_numeric_line(&parenthesesElements, &parenthesesTypes, var_table); //Recursive call of the function if not error

			//Saving the result at subscript of parenthese element
			free((*elements)[i]);
			(*elements)[i] = (char*) malloc(( strlen(parenthesesElements[0]) + 1) * sizeof(char));
			check_alloc((*elements)[i]);
			strcpy((*elements)[i], parenthesesElements[1]);
			(*types)[i] = parenthesesTypes[1];

			//Freeing temporary arrays
			free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
			free(parenthesesTypes);
		}
	}

	//Checking if the program already found an error in the line
	if ( strstr((*elements)[0], "ERROR: S") != NULL) //If the program has already found an error in the line, interrupting the function here
		return 0;


	//Determining and performing operations
	switch(is_operation(*types, *elements))
	{
		case 1: //int operation
			compute__int_operation(elements, types, var_table); //Computing the operation
			return 1;
			break;

		case 2: //float operation
			compute__float_operation(elements, types, var_table); //Computing the operation
			return 1;
			break;

		default: //Any other not handled operation
			printf("Type of operation not handled!\n\n");
			return 0;
			break;
	}
}


//Check if the operation of strings only contain "+"
int check_string_operation(char** elements, int* types) //return 0 if the operation only contain + else it return 1
{
	int i;

	//Exploring the types 2 by 2 (so that we skip the elements)
	for(i = 2; i <= types[0]; i += 2)
		if(strcmp(elements[1], "+") != 0)
			return 1;

	return 0;
}


//Function that computes operations on strings
void compute_strings_operations(char*** elements, int** types)
{
	while((*types)[0] >= 3)
	{
		strcat((*elements)[1], (*elements)[3]);

		shift_elements(elements, types, 2);
		shift_elements(elements, types, 2);
	}
}


//Function that assigns a value to a given variable knowing only its name, returns 1 if correctly done, 0 if not done
int assign_value_to_variable(const char* varName, const char* valueToAssign, Variable** var_table)
{
	int i = 0;

	//Looking for the subscript
	do
	{
		//If we have found the subscript
		if (strcmp((*var_table)[i].name, varName))
			break;

		if (strcmp((*var_table)[i].name, NAME__END_VARTABLE))
			return 0;

		i++;
	} while (TRUE);

	free((*var_table)[i].value);

	(*var_table)[i].value = (char*) malloc((strlen(valueToAssign) + 1) * sizeof(char));
	check_alloc((*var_table)[i].value);

	strcpy((*var_table)[i].value, valueToAssign);

	return 1;
}
