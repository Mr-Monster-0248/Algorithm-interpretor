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

	//Function that performs an multiplication of float values (formatted to string)
	float float_multiplication(const char* lValue, const char* rValue);

	//Function that performs an multiplication of integers values (formatted to string)
	int int_multiplication(const char* lValue, const char* rValue);

	//Function that performs an addition of float values (formatted to string)
	float float_addition(const char* lValue, const char* rValue);

	//Function that performs an subtraction of integers values (formatted to string)
	int int_subtraction(const char* lValue, const char* rValue);

	//Function that performs a division of integers values (formatted to string)
	int int_division(const char* lValue, const char* rValue);

	//Function that performs a subtraction of floats
	float float_subtraction(const char* lValue, const char* rValue);

	//Function that performs a division of floats
	float float_division(const char* lValue, const char* rValue);

	//Function that performs the modulo of integers
	int int_modulo(const char* lValue, const char* rValue);

	//Function that check if the line is an operation 
	int is_operation(int* types, char** elements); //return 1 if it's an operation

	//Function that stores a new variable in the array of all variables
	void store_variable(Variable** var_table, char* varName, const int varType);

	//Function that performs the boolean comparison between two integers/float
	int eval_bool_int(const char* lValue, const char* rValue, const char* comparator);

	//Function that performs the boolean comparison between two strings
	int eval_bool_string(const char* lString, const char* rString, const char* comparator);

	//Function that check if the line is a comparison
	int is_comparison(int* types); //return 1 if yes, 0 for no

	//Function to evaluate a boolean expression
	/* RETURN VALUES:
	   	BOOLEAN_ERROR_CODE = ERROR
		0 = FALSE
		1 = TRUE
	*/

	//Function that compute boolean comparison
	void compute__int_float_comparison(char*** elements, int** types);

	//Function that compute boolean comparison
	void compute__string_comparison(char*** elements, int** types);

	//Function witch give a value of priority for each operator
	int check_operator_priority(char* operator);

	//Function that computes operations on integers
	void compute__int_operation(char*** elements, int** types, Variable** var_table);

	//Function witch find the operator with the highest priority (if it exist)
	int highest_priority_operator(char** elements, int* types); //return the index of the highest operator and return 0 if there is no highest

	//Function that computes operations on floats
	void compute__float_operation(char*** elements, int** types, Variable** var_table);

	//Recursive function that computes the entered line (recursive because of potential parentheses)
	/* RETURN VALUES:
		0 = ERROR
		1 = NO ERROR
	*/
	int compute_numeric_line(char*** elements, int** types, Variable** var_table);

	//Check if the operation of strings only contain "+"
	int check_string_operation(char** elements, int* types); //return 0 if the operation only contain + else it return 1

	//Function that computes operations on strings
	void compute_strings_operations(char*** elements, int** types);

	//Function that assigns a value to a given variable knowing only its name, returns 1 if correctly done, 0 if not done
	int assign_value_to_variable(const char* varName, const char* valueToAssign, Variable** var_table);

	
#endif