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
	int check_if_usable(const char* varName, Variable *var_table, int* sub);

	//Function that checks the type of a variable (used when declaration of a new variable)
	void check_variable_type(char** element, int** types, int i);

	//Function that divides the string into substrings (one for each element of the line)
	/* RETURN VALUES
		0 = syntax error
		1 = no error, continue to perform
		2 = structural error
		3 = assignation error

		Types:
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
	   11 = boolean

		The number of elements is stored in types at subscript 0 ( (*types)[0] = number of elements )
	*/
	int get_line_elements(const char* line, char*** elements, int** types, int* errorPosition); //Returns 0 if syntax error

	//Function that displays the elements (essential for testing)
	void display_elements(char** elements, int* types);

	//Function that checks the entry of the user and stores a new variable is asked (returns 0 if no new variable, 1 if a new variable is stored)
	int check_variable_declaration(char** elements, int* types, Variable** var_table);

	//Function that returns the type of line read in a file
	/* RETURN VALUES
	   -1 = line not allocated
		0 = Commented line
		1 = Empty line
		2 = File structure indicator line (@ VARS, @ ENDVARS, @ BEGIN, @ END)
		3 = Special statements line ($ if, $ else, $ for, $ while, $ endif, $ endfor, $ endwhile)
		4 = Classic line (assignation of value to a variable)
	*/
	int check_file_line_comment(char** line);


	//Function that replaces variables names in elements by their values
	/* RETURN VALUES
		0 = ERROR: undeclared variable
		1 = ERROR: forbidden variable name
		2 = ERROR: uninitialized variable
		3 = no error, continue to perform
	*/
	int replace_name_by_value(char*** elements, int** types, const int sub, Variable* var_table);

	//Function that replaces all variables in elements by their value except the assigned one in case of an assignation
	int get_var_values(char*** elements, int** types, Variable* var_table);

#endif