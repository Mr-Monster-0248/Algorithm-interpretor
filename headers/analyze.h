#ifndef DEF_ANALYZE
	#define DEF_ANALYZE

	
	//Check if the word is the name of a type
	int is_typeword(const char* instruction); //Return 1 if allowed

	//Check if the element is allowed
	int is_forbidden(const char* instruction); //Return 1 if allowed

	//Function that checks if a given variable name is usable
	/* RETURN VALUES
		0 = not usable (name is forbidden)
		1 = usable, a variable has already this name
		2 = usable, no variable has already this name
	*/
	int check_if_usable(const char* varName, Variable *var_table);

	//Function that checks whether a name is already taken by an existing variable or not, if yes, returning the subscript of var_table on which this variable is stored, else returning -1
	int check_existance(const char* varName, Variable *var_table);

	//Function that checks the type of a variable (used when declaration of a new variable)
	void check_variable_type(char** element, int** types, int i);

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
	int get_line_elements(const char* line, char*** elements, int** types, int* errorPosition); //Returns 0 if syntax error

	//Function that displays the elements (essential for testing)
	void display_elements(char** elements, int* types);

#endif