#ifndef DEF_LOAD
	#define DEF_LOAD

	//Function that is run at the start of the program, returns different values depending on the mode chosen by the user
	int check_parameters(int argc, char** argv);

	//Function that checks memory allocation of a pointer
	void check_alloc(void* pointer);

	//Function that removes the '\n' character at the end of a string
	void del_newLine_char(char* string);

	//Function that displays a file's content
	void read_file(const char* pathToFile);

	//Function that checks whether the user entered a special command or not
	/* RETURN VALUES

		0 = no special command
		1 = continue
		2 = exit
	*/
	int check_special_command(const char* line);

	//Function to free 2D array
	void free_2D_char_array(char*** array, int subs);

	//Function that shifts the values to the left in elements after an operation
	void shift_elements(char*** elements, int** types, int subscript);

	//Function that return the line read in the given file
	char* read_file_line(FILE* file);

	//Function to free all memory allocated to all elements of all variables
	void free__var_table(Variable** var_table);

#endif