#ifndef DEF_CONSTANTS
	#define DEF_CONSTANTS


	//Variable structure to store different data about each variable
	typedef struct Variable
	{
		char* name;
		int type;
		char* value;
	} Variable;
	

	#ifdef __WIN32__
		#define CLEAR "cls"
	#else
		#ifdef __unix__
			#define CLEAR "clear"
		#endif
	#endif

	#define LINE_SIZE 300 //The number of characters which will be read on a line

	#define HELPFILE "help.txt" //Path to help.txt

	//Booleans
	#define BOOLEAN_ERROR_CODE 42 //Basic error code for booleans (used for conversion to int) (no need to explain the value)
	#define TRUE 1
	#define FALSE 0

	#define EXISTS 1
	#define DOESNT_EXISTS 0

	#define UNINITIALIZED_VAR_VALUE "~undefined~"

	#define NAME__END_VARTABLE "end of var_table"
	#define VALUE__END_VARTABLE "end of var_table" 
	#define TYPE__END_VARTABLE 33663 //"END" on a alphanumeric keyboard/pad (Nokia 3310 type)
	
#endif
