#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
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


//Function that converts a boolean string to its associated int value
int bool_to_int(const char* value)
{
	if (strcmp(value, "true") == 0 || strcmp(value, "TRUE") == 0)
		return TRUE;
	
	if (strcmp(value, "false") == 0 || strcmp(value, "FALSE") == 0)
		return FALSE;

	return BOOLEAN_ERROR_CODE;
}


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
		return convertTo_int(rValue) + convertTo_int(lValue);

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

	return convertTo_float(lValue) + convertTo_float(rValue);
}



//Function that check if the line is an operation 
int is_operation(int* types) //Return 1 if it's an int operation 2 if it's a float operation 3 if it is a string operation
{
	int i, isAFloat = FALSE, isAInt = TRUE; 

	//Exploring the types 2 by 2 (so that we skip the operators)
	for(i = 1; i <= types[0]; i += 2)
	{
		if (types[i] == 2)
		{
			isAFloat = TRUE;
			continue;
		}
		if (types[i] == 3) //If string
			return 3;
	}
		
	if (isAFloat) //If float
		return 2;

	if (isAInt) //If integer
		return 1;

	return 0; //ERROR
}


//Function that stores a new variable in the array of all variables
void store_variable(Variable** var_table, char* varName, char* varValue, const int varType)
{
	int size = 0;

	//Determining the current size of the array
	for (size = 0; strcmp((*var_table)[size].name, NAME__END_VARTABLE) != 0; size++);

	//Reallocating memory for the array of variables
	*var_table = realloc(*var_table, (size + 1) * sizeof(Variable));
	check_alloc(*var_table);

	//Deleting the data of the last variable stored
	free( (*var_table)[size].name );
	free( (*var_table)[size].value );

	//Allocating memory for the name and value of the new variable
	(*var_table)[size].name = (char*) malloc( (strlen(varName) + 1) * sizeof(char));
	(*var_table)[size].value = (char*) malloc( (strlen(varValue) + 1) * sizeof(char));
	check_alloc((*var_table)[size].name);
	check_alloc((*var_table)[size].value);

	//Storing the new name, value and type
	(*var_table)[size].type = varType;
	sprintf((*var_table)[size].name, "%s", varName);
	sprintf((*var_table)[size++].value, "%s", varValue);

	//Creating a "end variable" to mark the end of the array of variables
	(*var_table)[size].name	= (char*) malloc( (strlen(NAME__END_VARTABLE) + 1) * sizeof(char));
	(*var_table)[size].value = (char*) malloc( (strlen(VALUE__END_VARTABLE) + 1) * sizeof(char));
	(*var_table)[size].type = TYPE__END_VARTABLE;
	check_alloc((*var_table)[size].name);
	check_alloc((*var_table)[size].value);
	sprintf((*var_table)[size].name, "%s", NAME__END_VARTABLE);
	sprintf((*var_table)[size].value, "%s", VALUE__END_VARTABLE);
}


//Function that performs the boolean comparison between two integers
static int eval_bool_int(const int lValue, const int rValue, const int comparator)
{
	switch(comparator)
	{
		case 0: // =
			if (lValue == rValue)
				return TRUE;
			return FALSE;
			break;

		case 1: // !=
			if (lValue != rValue)
				return TRUE;
			return FALSE;
			break;

		case 2: // <=
			if (lValue <= rValue)
				return TRUE;
			return FALSE;
			break;

		case 3: // >=
			if (lValue >= rValue)
				return TRUE;
			return FALSE;
			break;

		case 4: // <
			if (lValue < rValue)
				return TRUE;
			return FALSE;
			break;

		case 5: // >
			if (lValue > rValue)
				return TRUE;
			return FALSE;
			break;

		default:
			return BOOLEAN_ERROR_CODE;
			break;
	}
}


//Function that performs the boolean comparison between two floats
static int eval_bool_float(const float lValue, const float rValue, const int comparator)
{
	switch(comparator)
	{
		case 0: // =
			if (lValue == rValue)
				return TRUE;
			return FALSE;
			break;

		case 1: // !=
			if (lValue != rValue)
				return TRUE;
			return FALSE;
			break;

		case 2: // <=
			if (lValue <= rValue)
				return TRUE;
			return FALSE;
			break;

		case 3: // >=
			if (lValue >= rValue)
				return TRUE;
			return FALSE;
			break;

		case 4: // <
			if (lValue < rValue)
				return TRUE;
			return FALSE;
			break;

		case 5: // >
			if (lValue > rValue)
				return TRUE;
			return FALSE;
			break;

		default:
			return BOOLEAN_ERROR_CODE;
			break;
	}
}


//Function that performs the boolean comparison between two strings
static int eval_bool_string(char* lString, char* rString, const int comparator)
{
	switch(comparator)
	{
		case 0: // =
			if (strcmp(lString, rString) == 0)
				return TRUE;
			return FALSE;
			break;

		case 1: // !=
			if (strcmp(lString, rString) != 0)
				return TRUE;
			return FALSE;
			break;

		default:
			return BOOLEAN_ERROR_CODE;
			break;
	}
}


//Function that performs the boolean comparison between two booleans
static int eval_bool(const int lBool, const int rBool, const int comparator)
{
	switch(comparator)
	{
		case 6: // AND
			if (lBool && rBool)
				return TRUE;
			return FALSE;
			break;

		case 7: // OR
			if (lBool || rBool)
				return TRUE;
			return FALSE;
			break;

		default:
			return BOOLEAN_ERROR_CODE;
			break;

	}
}


//Function to evaluate a boolean expression
/* RETURN VALUES:
	BOOLEAN_ERROR_CODE = ERROR
	0 = FALSE
	1 = TRUE
*/
int eval_bool_expr(char* lValue, const int lValueType, char* rValue, const int rValueType, const int comparatorID)
{

	//Checking if the two elements are comparable
	if ((lValueType != rValueType) || lValueType > 3 || rValueType > 3)
		return BOOLEAN_ERROR_CODE; //ERROR: CANNOT PERFORM COMPARISON BETWEEN TWO ELEMENTS OF DIFFERENT TYPES

	switch(lValueType)
	{
		case 1:
			return eval_bool_int(convertTo_int(lValue), convertTo_int(rValue), comparatorID);
			break;

		case 2:
			return eval_bool_float(convertTo_float(lValue), convertTo_float(rValue), comparatorID);
			break;

		case 3:
			return eval_bool_string(lValue, rValue, comparatorID);
			break;

		default:
			return BOOLEAN_ERROR_CODE;
			break;
	}
}


//Function witch give a value of priority for each operator
int check_operator_priority(char* operator)
{
	if(strcmp(operator, "*") == 0)
		return 2;
	else if (strcmp(operator, "/") == 0)
		return 2;
	else if(operator[0] == '%')
		return 2;
	else if (strcmp(operator, "+") == 0)
		return 1;
	else if (strcmp(operator, "-") == 0)
		return 1;
	else
		return 0;
}


//Function witch find the operator with the highest priority (if it exist)
int highest_priority_operator(char** elements, int* types) //return the index of the highest operator and return 0 if there is no highest
{
	int i;

	for (i = 2; i <= types[0] - 1; i++)
		if (types[i] == 4 && check_operator_priority(elements[i]) == 2)
			return i;

	for (i = 2; i <= types[0] - 1; i++)
		if (types[i] == 4)
			return i;

	return 0;
}


//Function that computes operations on integers
void compute__int_operation(char*** elements, int** types)
{
	int i = 0;

	//While there still are some * or / or % to perform, performing this highest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  2)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "*") == 0)
			sprintf((*elements)[i - 1], "%d", int_multiplication((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "/") == 0)
		{
			if ( convertTo_int((*elements)[i + 1]) != 0 )
			{
				sprintf((*elements)[i - 1], "%d", int_division((*elements)[i - 1], (*elements)[i + 1]));

				display_elements(*elements, *types);
			}
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
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  1)
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
}


//Function that computes operations on floats
void compute__float_operation(char*** elements, int** types)
{
	int i = 0;

	//While there still are some * or / or % to perform, performing this highest priority operations
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  2)
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
	while( (i = highest_priority_operator(*elements, *types)) && check_operator_priority((*elements)[i]) ==  1)
	{
		//Analyzing the operator, computing, and storing the result in a substring of elements
		if(strcmp((*elements)[i], "+") == 0)
			sprintf((*elements)[i - 1], "%f", float_addition((*elements)[i - 1], (*elements)[i + 1]));

		if(strcmp((*elements)[i], "-") == 0)
			sprintf((*elements)[i - 1], "%f", float_subtraction((*elements)[i - 1], (*elements)[i + 1]));

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);

		//Shifting the array to the left from the position of the operation to remove the performed operation
		shift_elements(elements, types, i);
		shift_elements(elements, types, i);
	}
}


//Recursive function that computes the entered line (recursive because of potential parentheses)
/* RETURN VALUES:
	0 = ERROR
	1 = NO ERROR
*/
int compute_numeric_line(char*** elements, int** types)
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

				//Freeing temporary 				
				free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
				free(parenthesesTypes);

				break;

			} else if (errorType == 2) //If structural error
			{
				free((*elements)[0]);

				(*elements)[0] = (char*) malloc(42 * sizeof(char));
				check_alloc((*elements)[0]);

				sprintf((*elements)[0], "ERROR: STRUCTURAL ERROR, unknow position\n");

				//Freeing temporary 				
				free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
				free(parenthesesTypes);

				break;
			}


			compute_numeric_line(&parenthesesElements, &parenthesesTypes); //Recursive call

			free((*elements)[i]);

			(*elements)[i] = (char*) malloc(( strlen(parenthesesElements[0]) + 1) * sizeof(char));
			check_alloc((*elements)[i]);

			strcpy((*elements)[i], parenthesesElements[1]);

			(*types)[i] = parenthesesTypes[1];

			free(parenthesesTypes);

			free_2D_char_array(&parenthesesElements, parenthesesTypes[0]);
		}
	}

	//Checking if the program already found an error in the line
	if ( strstr((*elements)[0], "ERROR: S") != NULL) //If the program has already found an error in the line, interrupting the function here
		return 0;


	//Determining and performing operations
	switch(is_operation(*types))
	{
		case 1: //int operation
			compute__int_operation(elements, types); //Computing the operation
			return 1;
			break;

		case 2: //float operation
			compute__float_operation(elements, types); //Computing the operation
			return 1;
			break;

		default: //Any other not handled operation
			printf("Type of operation not handled!\n\n");
			return 0;
			break;
	}
}
