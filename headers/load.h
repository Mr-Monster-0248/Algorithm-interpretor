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

	//Function that shifts all the value of an integer array to the left from a given subscript (which will be overwritten in the operation)
	void shift_left__int_array(int** array, int* arraySize, int shiftFrom);

	//Function that shifts all the value of an integer array to the left from a given subscript (which will be overwritten in the operation)
	void shift_left__string_array (char*** array, int* arraySize, int shiftFrom);

	//Function that shifts all the value of an integer array to the left from a given subscript (which will be overwritten in the operation)
	void shift_left__string_array__number_not_changed(char*** array, int* arraySize, int shiftFrom);

#endif