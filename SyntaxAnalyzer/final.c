#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

bool error = false;
static char line[200]; //is used for explanatory purposes, where the error occured, what was expected
static char wrong[200]; //what was received instead
static char lookahead[200][200]; //the input file is read into this. Iterates through
static int i = 0; //used to jump to the next token
void FACTOR(); //used because error was thrown without declaration
void TERM();
void EXPR();
void BLOCK();

void file_to_array()
{
	/* File pointer to hold reference to our file */
	FILE *FilePtr;

	/*
	 * Open file in r (read) mode.
	 * "data/file1.txt" is complete file path to read
	 */
	FilePtr = fopen("input.txt", "r");

	/* fopen() return NULL if the file don't exist or the path is wrong */
	if (FilePtr == NULL)
	{
		/* Unable to open file hence exit */
		printf("Unable to open file.\n");
		printf("Please check whether file exists and you have read privilege.\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	/* File open success message */
	printf("File opened successfully. Reading file contents character by character. \n\n");

	char character;
	char file_array[200][200];
	int row = 0, col = 0;
	do
	{
		character = fgetc(FilePtr);

		if (character == '\n' || character == EOF)
		{
			file_array[row][col] = '\0';
			row++;
			col = 0;
		}
		else
		{
			file_array[row][col] = character;
			col++;
		}
	} while (character != EOF);
	
	fclose(FilePtr);

	int i = 0;

	while (i < 200)
	{
		strcpy(lookahead[i], file_array[i]);
		i++;
	}
	/*
	int loop_analyzor = 0;
	while (loop_analyzor < 50)
	{
		printf("%s\n", lookahead[loop_analyzor]);
		loop_analyzor++;
	}
	*/
}

void RETURN() 
{
		FACTOR();
		if (strcmp(lookahead[i], "semi") == 0)
		{
			i++;
		}
	
}

void FACTOR() //used as the example as all functions work the same
{
	if (strcmp(lookahead[i], "lparen") == 0) //if lookahead-token matches the given, proceed
	{
		i++; //iterate to next token
		FACTOR(); //call function according to CFG
		if (strcmp(lookahead[i], "rparen") == 0) i++;
		else { //if unexpected token, set error to true and move on. If multiple errors exist, only the last one will be displayed
			error = true;
			line[200] = "FACTOR - Expected rparent";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else if (strcmp(lookahead[i], "id") == 0 || strcmp(lookahead[i], "num" ) == 0) //some productions might have multiple if-else constructions
		i++;
	else
	{
		error = true;
		line[200] = "FACTOR - Expected ID or NUM";
		wrong[200] = lookahead[i]; i++;
	}

}

void COND()
{

	FACTOR();
	if (strcmp(lookahead[i], "comp") != 0) 
	{
		error = true;
		line[200] = "COND - Expected comp";
		wrong[200] = lookahead[i]; i++;
	}
	else
	{
		i++;
		FACTOR();
	}
}

void TERM2()
{
	if (strcmp(lookahead[i], "multdiv") == 0)
	{
		i++;
		TERM();
	}
	else if (strcmp(lookahead[i], "addsub") == 0); 
	else
	{
		error = true;
		line[200] = "TERM2 - Expected multdiv or addsub";
		wrong[200] = lookahead[i]; i++;
		
	}
}

void TERM()
{
	FACTOR();
	TERM2();
}

void EXPR2()
{

	if (strcmp(lookahead[i],  "addsub") == 0)
	{
		i++;
		EXPR();
	}
	else if (strcmp(lookahead[i],  "semi" ) == 0 || strcmp(lookahead[i], "rparen") == 0 );
	else
	{
		error = true;
		line[200] = "EXPR2 - Expected addsub, rparen or semi";
		wrong[200] = lookahead[i]; i++;
		
	}
}

void EXPR()
{
	TERM();
	EXPR2();
}

void RHS()
{

	if (strcmp(lookahead[i], "lparen") == 0 || strcmp(lookahead[i], "id") == 0 || strcmp(lookahead[i], "num") == 0)
		EXPR();
	else if (strcmp(lookahead[i],  "literal") == 0)
		i++;
	else
		error = true;
		line[200] = "RHS - Expected literal";
		wrong[200] = lookahead[i]; i++;
		
}

void RHSORFCALL()
{

	if (strcmp(lookahead[i],  "assign") == 0)
	{
		i++;
		RHS();
	}
	else
	{
		error = true;
		line[200] = "RHSORFCALL - Expected assign";
		wrong[200] = lookahead[i]; i++;
		
	}
}

void STMT()
{
	if (strcmp(lookahead[i],  "vtype") == 0)
	{
		i++;
		if (strcmp(lookahead[i],  "id") == 0)
		{
			i++;
			if (strcmp(lookahead[i],  "semi") == 0)
			{
				i++;
			}
			else
			{
				error = true;
				line[200] = "STMT - Expected semi";
				wrong[200] = lookahead[i]; i++;
				
			}
		}
		else
		{
			error = true;
			line[200] = "STMT - Expected id";
			wrong[200] = lookahead[i]; i++;
			
		}
	}
	else if (strcmp(lookahead[i],  "id") == 0)
	{
		i++;
		RHSORFCALL();
			if (strcmp(lookahead[i],  "semi") == 0)
			{
				i++;
			}
			else
			{	error = true;
				line[200] = "STMT - Expected semi";
				wrong[200] = lookahead[i]; i++;
			}
	}
	else if (strcmp(lookahead[i],  "if") == 0)
	{
		if (strcmp(lookahead[i],  "lparen") == 0)
		{
			i++;
			COND();
			if (strcmp(lookahead[i],  "rparen") == 0)
			{
				i++;
				if (strcmp(lookahead[i],  "lbrace") == 0)
				{
					i++;
					BLOCK();
					if (strcmp(lookahead[i],  "rbrace") == 0)
					{
						i++;
						if (strcmp(lookahead[i],  "else") == 0)
						{
							i++;
							if (strcmp(lookahead[i],  "lbrace") == 0)
							{
								i++;
								BLOCK();
								if (strcmp(lookahead[i],  "rbrace") == 0)
									i++;
								else
								{
									error = true;
									line[200] = "STMT - Expected rbrace";
									wrong[200] = lookahead[i]; i++;
								}
							}
							else
							{
								error = true;
								line[200] = "STMT - Expected lbrace";
								wrong[200] = lookahead[i]; i++;
							}
						}
						else
						{
							error = true;	
							line[200] = "STMT - Expected else";
							wrong[200] = lookahead[i]; i++;
						}
					}
					else
					{
						error = true;	
						line[200] = "STMT - Expected rbrace";
						wrong[200] = lookahead[i]; i++;
					}
				}
				else
				{
					error = true;	
					line[200] = "STMT - Expected lbrace";
					wrong[200] = lookahead[i]; i++;
				}
			}
			else
			{
				error = true;	
				line[200] = "STMT - Expected rparen";
				wrong[200] = lookahead[i]; i++;
			}
		}
		else
		{
			error = true;	
			line[200] = "STMT - Expected lparen";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else if (strcmp(lookahead[i],  "while") == 0)
	{
		if (strcmp(lookahead[i],  "lparen") == 0)
		{
			i++;
			COND();
			if (strcmp(lookahead[i],  "rparen") == 0)
			{
				i++;
				if (strcmp(lookahead[i],  "lbrace") == 0)
				{
					i++;
					BLOCK();
					if (strcmp(lookahead[i],  "rbrace") == 0)
					{
						i++;
					}
					else
					{
						error = true;	
						line[200] = "STMT - Expected rbrace";
						wrong[200] = lookahead[i]; i++;
					}
				}
				else
				{
					error = true;	
					line[200] = "STMT - Expected lbrace";
					wrong[200] = lookahead[i]; i++;
				}
			}
			else
			{
				error = true;	
				line[200] = "STMT - Expected rparen";
				wrong[200] = lookahead[i]; i++;
			}
		}
		else
		{
			error = true;	
			line[200] = "STMT - Expected lparen";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else
	{
		error = true;
		line[200] = "STMT - Expected while";
		wrong[200] = lookahead[i]; i++;
		
	}

}

void BLOCK()
{
	if (strcmp(lookahead[i], "if") == 0 || strcmp(lookahead[i], "vtype") == 0 || strcmp(lookahead[i], "id") == 0 || strcmp(lookahead[i], "while") == 0)
	{
		STMT();
		BLOCK();
	}
	else if (strcmp(lookahead[i], "rbrace") == 0 || strcmp(lookahead[i], "return") == 0);
	else {
		error = true;
		line[200] = "BLOCK - Expected if, vtype, id, while, rbrace or return";
		wrong[200] = lookahead[i]; i++;
	}
}

void MOREARGS()
{
	if (strcmp(lookahead[i], "comma") == 0)
	{
		i++;
		if (strcmp(lookahead[i], "vtype") == 0)
		{
			i++;
			if (strcmp(lookahead[i], "id") == 0)
			{
				i++;
				MOREARGS();
			}
			else
			{
				error = true;
				line[200] = "MOREARGS- Expected id";
				wrong[200] = lookahead[i]; i++;
			}
		}
		else
		{
			error = true;
			line[200] = "MOREARGS - Expected vtype";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else if (strcmp(lookahead[i], "rparen") == 0);
	else
	{
		error = true;
		line[200] = "MOREARGS - Expected vtype or rparen";
		wrong[200] = lookahead[i]; i++;
	}

}

void ARG()
{
	if (strcmp(lookahead[i], "vtype") == 0)
	{
		i++;
		if (strcmp(lookahead[i], "id") == 0)
		{
			i++;
			MOREARGS();
		}
		else if (strcmp(lookahead[i], "rparen") == 0)i++;
		else
		{
			error = true;
			line[200] = "ARGS - Expected id or rparn";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else if (strcmp(lookahead[i], "rparen") == 0);
	else
	{
		error = true;
		line[200] = "ARGS - Expected vtype or rparen";
		wrong[200] = lookahead[i]; i++;

	}
}

void DECL(int j)
{
	i = j;
	if (strcmp(lookahead[i], "lparen") == 0)
	{
		i++;
		ARG();
		if (strcmp(lookahead[i], "rparen") == 0)
		{
			i++;
			if (strcmp(lookahead[i], "lbrace") == 0)
			{
				i++;
				BLOCK();
				RETURN();
				if (strcmp(lookahead[i], "rbrace") != 0)
				{
					error = true;
					line[200] = "DECL - Expected rbrace";
					wrong[200] = lookahead[i]; i++;
				}
				else i++;
			}
			else
			{
				error = true;
				line[200] = "DECL - Expected lbrace";
				wrong[200] = lookahead[i]; i++;
			}
		}
		else
		{
			error = true;
			line[200] = "DECL - Expected rparen";
			wrong[200] = lookahead[i]; i++;
		}
	}
	else if (strcmp(lookahead[i], "semi") == 0);
	else
	{
		error = true;
		line[200] = "DECL - Expected semi or lparen";
		wrong[200] = lookahead[i]; i++;

	}
}

void CODE()
{
	int i = 0;
	if (strcmp(lookahead[i], "vtype") == 0)
	{
		i++;
		if (strcmp(lookahead[i], "id") == 0)
		{
			i++;
			DECL(i);
			CODE();
		}
		else
		{
			error = true;
			line[200] = "CODE - Expected id";
			wrong[200] = lookahead[i]; i++;
		}

	}
	else if (strcmp(lookahead[i], "$") == 0);
	else
	{
		error = true;
		strcpy(line, "CODE - Expected vtype or $\n");
		strcpy(wrong, lookahead[i]); i++;
	}
}

void main()
{
	file_to_array();
	CODE(); //CODE is called first

	if (error = true) {
		printf("Code does not pass the parser\n");
		printf(line);
		printf("Got instead:");
		printf(wrong);
		printf("\n");
	}
	else
	{
		printf("Success\n");
	}

	system("pause");
	return 0;
}
