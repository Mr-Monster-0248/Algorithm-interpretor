int get_line_elements(const char* line, char*** elements, int** types, int i) //Returns 0 if syntax error
{
	int j = 1, k = 1;
	i = 0;


	//Erasing the previous arrays (if there were previous arrays)
	if(*types != NULL)
		free(*types);
	if(*elements != NULL)
		free(*elements);

	//Allocating memory for new arrays
	*types = (int*) malloc(sizeof(int));
	check_alloc(*types);

	*elements = (char**) malloc(sizeof(char*));
	check_alloc(*elements);


	//Initialising the number of elements to 0
	(*types)[0] = 0;

	//Initializing the first element to "<<<" since the first subscript (0) of types contains the total number of elements
	(*elements)[0] = (char*) malloc(2 * sizeof(char));
	check_alloc((*elements)[0]);
	sprintf((*elements)[0], "<<<");

	while (line[i] != '\0')
	{
		//If an operator was found
		if ( (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%' || line[i] == ':') && line[i+1] == ' ' && line[i+2] != '\0')
		{
			if (i < 2)
				return 0;

			(*types)[0]++; //Incrementing the number of elements

			(*elements)[j] = (char*) malloc(2 * sizeof(char));
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int));
			check_alloc(*types);

			//Storing the character in a string
			(*elements)[j][0] = line[i];
			(*elements)[j][1] = '\0';

			//Setting type of element to 5 (operator)
			(*types)[j++] = 4;
		}



		//If a "word" beginning by a number (1, 2, 3...) or by  was found
		if ( (line[i] >= 48 && line[i] <= 57) || (line[i] == '-' && line[i + 1] != '\0' && line[i + 1] >= 48 && line[i + 1] <= 57) )
		{

			(*elements)[j] = (char*) malloc(2 * sizeof(char)); //Allocating memory to store this number appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this number type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			//Incrementing the number of elements
			(*types)[0]++;

			k = 2;

			if (line[i] == '-')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
			}


			while (line[i] != ' ' && line[i] >= 48 && line[i] <= 57 && line[i] != '\0')
			{

				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
			}




			//Determining the type of the element
			if (line[i] == ' ' || line[i] == '\0') //If the element is of type int
				(*types)[j] = 1;
			else
				if (line[i] != '.' && line[i] != '\0') //Syntax error
					return 0;

			if (line[i] == '.' && line[i + 1] >= 48 && line[i + 1] <= 57)
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
				
				while (line[i] != ' ' && line[i] >= 48 && line[i] <= 57 && line[i] != '\0')
				{

					(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
					check_alloc((*elements)[j]);

					(*elements)[j][k - 2] =  '\0';
					(*elements)[j][k - 3] = line[i++];
				}

				if (line[i] == ' ' || line[i] == '\0') //If the element is of type float
					(*types)[j] = 2;
				else //Syntax error
					if (line[i] != '\0')
						return 0;
			}


			j++;
		}

		//If a "word" beginning by a letter (a,b,c,...,A,B,C,...) was found
		if ( ( (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122) ) && line[i] != '\0')
		{
			(*elements)[j] = (char*) malloc(2 * sizeof(char)); //Allocating memory to store this letter appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this letter type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			//Incrementing the number of elements
			(*types)[0]++;

			k = 2;

			while (line[i] != ' ' && ( (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122) || ( (line[i] >= 48) && (line[i] <= 57) ) || line[i] == '_' ) && line[i] != '\0' && line[i] != ':')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				(*elements)[j][k - 2] =  '\0';
				(*elements)[j][k - 3] = line[i++];
			}

			(*types)[j] = 8;

			j++;
		}

		//If a "word" beginning by " was found (if a string delimited by "" was found)
		if (line[i] == '"' && line[i+1] != '\0')
		{
			(*elements)[j] = (char*) malloc(3 * sizeof(char)); //Allocating memory to store this string appart the original line
			check_alloc((*elements)[j]);

			*types = (int*) realloc(*types, (j+1) * sizeof(int)); //Allocating memory to store this string type
			check_alloc(*types);
			(*types)[j] = 0; //Setting the type code to error by default

			k = 2;
			(*elements)[j][0] = line[i++];
			(*elements)[j][1] = '\0';


			while(line[i] != '"' && line[i] != '\0')
			{
				(*elements)[j] = (char*) realloc((*elements)[j], k++ * sizeof(char));
				check_alloc((*elements)[j]);

				//Storing the message
				(*elements)[j][k - 2] = '\0';
				(*elements)[j][k - 3] = line[i++];
			}

			if (line[i] == '\0') //Syntax error, no closing ", returning 0
				return 0;

			(*types)[0]++; //Incrementing the number of elements

			(*types)[j++] = 3; //Setting the element's type to string
		}


		i++;

	}

	if((*types)[j-1] == 8)
		check_variable_type(*elements, types, j);

	return 1;
}
