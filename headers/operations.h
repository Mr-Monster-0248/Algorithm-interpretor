#ifndef DEF_OPERATIONS
	#define DEF_OPERATIONS

	//Function that displays where the SYNTAX ERROR was made
	void disp_error(const int position);

	//Function that displays where the SYNTAX ERROR was made at which line of the file
	void disp_error_line(const char* line, const int position, const int lineNumber);

	//Function that converts a string to a long int
	int convertTo_int(const char* value);

	//Function that converts a string to a float
	float convertTo_float(const char* value);

	//Function that converts a boolean string to its associated int value
	int bool_to_int(const char* value);

	//Function that performs an addition of integers values (formatted to string)
	int int_addition(const char* lValue, const char* rValue);

	//Function that check if the line is an operation 
	int is_operation(int* types); //return 1 if it's an operation

	//Function that stores a new variable in the array of all variables
	void store_variable(Variable** var_table, char* varName, char* varValue, const int varType);
	
#endif