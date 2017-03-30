#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/load.h"
#include "../headers/analyze.h"
#include "../headers/operations.h"


int main(int argc, char** argv)
{
	int mode = 0, *types = NULL, i = 0, position = 0, exitProgram = 0, getLineError = 0, lineNumber = 0;
	char *line = NULL, **elements = NULL;
	Variable* var_table = malloc(sizeof(Variable));
	FILE* algFile = NULL;

	check_alloc(var_table);

	system(CLEAR);

	mode = check_parameters(argc, argv);

	//Initializing the variables array
	var_table[0].name = (char*) malloc( (strlen(NAME__END_VARTABLE) + 1) * sizeof(char));
	var_table[0].value = (char*) malloc( (strlen(VALUE__END_VARTABLE) + 1) * sizeof(char));
	var_table[0].type = 0;
	check_alloc(var_table[0].name);
	check_alloc(var_table[0].value);
	sprintf(var_table[0].name, "%s", NAME__END_VARTABLE);
	sprintf(var_table[0].value, "%s", VALUE__END_VARTABLE);

	switch(mode)
	{
		case 1: //Direct line interpreter
				printf("Welcome to the algorithmic interpreter\nType \"exit\" to exit the interpreter or \"help\" to see the manual\n\n");
				
				line = (char*) malloc(LINE_SIZE * sizeof(char));
				check_alloc(line);

				do
				{
					getLineError = 0;
					exitProgram = 0;
					
					printf(">>> ");

					fflush(stdin);
					fgets(line, LINE_SIZE, stdin); //Getting the line entered by the user
					del_newLine_char(line);


					//Checking if the user entered a special command
					exitProgram = check_special_command(line);
					if (exitProgram == 1)
						continue;
					if (exitProgram == 2)
						break;
				

					//Getting the elements and their types in the line, continue processing only if no syntax error
					if ((getLineError = get_line_elements(line, &elements, &types, &position)) == 0)
					{
						disp_error(position);

						free_2D_char_array(&elements, types[0]);

						free(types);


						continue;
					} else if (getLineError == 2)
					{
						printf("ERROR: STRUCTURAL ERROR\n\n");

						free_2D_char_array(&elements, types[0]);

						free(types);

						continue;
					} else if (getLineError == 3)
					{
						printf("ERROR: CANNOT ASSIGN VALUE TO A NON VARIABLE OBJECT\n\n");

						free_2D_char_array(&elements, types[0]);

						free(types);

						continue;
					}

					for( i = 1; i < types[0]; i++)
						if(types[i] == 0)
							printf("Declaration error on element %d\n\n", i);

					
					if (types[0] >= 3)
					{
						check_variable_declaration(elements, types, &var_table);

						switch(is_operation(types, elements))
						{
							case 1:
								compute_numeric_line(&elements, &types, &var_table);
								printf("%s\n", elements[1]);
								break;
							case 2:
								compute_numeric_line(&elements, &types, &var_table);
								printf("%s\n", elements[1]);
								break;
							case 3:
								compute_strings_operations(&elements, &types);
								printf("%s\n", elements[1]);
								break;
						}
						
						switch(is_comparison(types))
						{
							case 1:
								compute__int_float_comparison(&elements, &types);
								printf("%s\n", elements[1]);
								break;
							case 2:
								compute__string_comparison(&elements, &types);
								printf("%s\n", elements[1]);
								break;
						}
					} else if (types[0] == 1){

						replace_names_by_values(&elements, &types, var_table);

						printf("%s\n", elements[1]);
					}
					
					


					//Freeing memory previously allocated before exiting or starting a new lap
					free_2D_char_array(&elements, types[0]);
					free(types);

					printf("\n");
				} while (TRUE);

				free(var_table);
				free(line);
			break;



		/*case 2: //C converter
			printf("The C converter is not available yet, it is a feature that will be soon implemented\n");
			break;*/



		case 3: //Program interpreter
			printf("Welcome to the algorithmic interpreter:\nHint: if your program runs into a problem, hit Ctrl + C to stop the interpreter\n\"\"\"\n");

			//Opening the file
			algFile = fopen(argv[1], "r");
			check_alloc(algFile);

			do
			{
				lineNumber++;
				getLineError = 0;

				//Reading the line in the file
				line = read_file_line(algFile);

				switch(check_file_line_comment(&line))
				{
					//Line not allocated
					case -1:
						fclose(algFile);
						return EXIT_FAILURE;
						break; //To avoid compilation warnings

					case 0:
						continue;
						break; //To avoid compilation warnings

					case 1:
						continue;
						break; //To avoid compilation warnings

					//Need to compute something
					case 4:
						//Getting the elements and their types in the line, continue processing only if no syntax error
						if ((getLineError = get_line_elements(line, &elements, &types, &position)) == 0)
						{
							disp_error_line(line, position, lineNumber);

							free_2D_char_array(&elements, types[0]);

							free(types);


							continue;
						} else if (getLineError == 2)
						{
							printf("ERROR: STRUCTURAL ERROR\n\n");

							free_2D_char_array(&elements, types[0]);

							free(types);

							continue;
						}

						for(i = 1; i < types[0]; i++)
							if(types[i] == 0)
								printf("Declaration error on element %d\n\n", i);

						if (types[0] >= 3 && is_operation(types, elements) != 0 && is_operation(types, elements) != 3)
							compute_numeric_line(&elements, &types, &var_table);

						if (types[0] >= 3 && is_operation(types, elements) == 3)
							compute_strings_operations(&elements, &types);

						if (types[0] >= 3 && is_comparison(types) == 1)
							printf("It's a comparison\n");

						break;


					default:
						printf("Type of line not handled yet!\n");
						break;
				}

			} while (TRUE);

			printf("\n\"\"\"\n");
			break;

		default: //Error
			system(CLEAR);
			printf("An error occured: impossible to recognize the mode choice\nExiting the program....\n\n");
			return EXIT_FAILURE;
			break; //To avoid compilation warnings
	}

	return EXIT_SUCCESS;
}
