#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/load.h"


//Function that displays where the SYNTAX ERROR was made
void disp_error(const int position)
{
	int i = 0;
	for (i = 0; i < position + 4; i++)
		printf(" ");

	printf("^\nSYNTAX ERROR\n\n");
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



//Function that check if the line is an operation 
int is_operation(int* types) //Return 1 if it's an operation
{
	int i, test = 0;

	for(i = 1; i <= types[0]; i++)
		if((i % 2 == 1 && types[i] == 1) || (i % 2 == 0 && types[i] == 4))
			test++;

	if(test >= types[0] && types[types[0]] == 1 && test > 2)
		return 1;
	else
		return 0;
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
			if (strcmp(lString, rString == 0))
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


static int eval_bool(const int lBool, const int rBool, const int comparator)
{
	switch(comparator)
	{
		case 6: // AND

	}
}



//Function to evaluate a boolean expression
/*
	RETURN VALUES:
	   BOOLEAN_ERROR_CODE = ERROR
		0 = FALSE
		1 = TRUE
*/
int eval_bool_expr(char* lValue, const int lValueType, char* rValue, const int rValueType, const int comparatorID)
{

	//Checking if the two elements are comparable
	if (lValueType != rValueType)
		return BOOLEAN_ERROR_CODE; //ERROR: CANNOT PERFORM COMPARISON BETWEEN TWO ELEMENTS OF DIFFERENT TYPES




}

//Function witch give a value of priority for each operator
int chech_operator_priority(char* operator)
{
	if(strcmp(operator, "*") == 0)
		return 2;
	else if (strcmp(operator, "/") == 0)
		return 2;
	else if (strcmp(operator, "+") == 0)
		return 1;
	else if (strcmp(operator, "-") == 0)
		return 1;
	else
		return 0;
}