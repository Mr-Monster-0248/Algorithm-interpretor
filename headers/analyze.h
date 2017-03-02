#ifndef DEF_ANALYZE
	#define DEF_ANALYZE

	
	//Check if the word is the name of a type
	int is_typeword(const char* instruction); //Return 1 if allowed

	//Check if the element is allowed
	int is_forbidden(const char* instruction); //Return 1 if allowed

	//Function that checks whether a name is already taken by an existing variable or not, if yes, returning the subscript of var_table on which this variable is stored, else returning -1
	int check_existance(const char* varName, Variable *var_table);

	//Function that checks the type of a variable (used when declaration of a new variable)
	void check_variable_type(char** element, int** types, int i);

	//Function that divides the string into substrings (one for each element of the line)
	int get_line_elements(const char* line, char*** elements, int** types, int* errorPosition); //Returns 0 if syntax error

	//Function that displays the elements (essential for testing)
	void display_elements(char** elements, int* types);

	


#endif