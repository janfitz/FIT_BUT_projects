/*
** Soubor:		interpret.c - implementace interpretu jazyka IFJ15
** Datum:		12.11.2015 	
** Kodovani: 	UTF-8
*/

#include "interpret.h"

start_of_stack for_temp = NULL;
start_of_stack xxx = NULL;

/*
** Function for solving all instructions in instruction stack
*/
int ins_solve(start_of_stack Active)
{
	while((Active->TAK.ins_code != INS_PROGRAM_END) || (Active->TAK.ins_code != INS_STOPPER))
	{
		switch(Active->TAK.ins_code)
		{	
			/*Special instruction that indicate the end of each program or strooper in if of cycles*/
		    case INS_PROGRAM_END:
		    	return EXIT_SUCCESS;
		     	break;

		    case INS_STOPPER:
		    	return EXIT_SUCCESS;
		        break;

			/*Block of instructions for arithmetic operations*/
		    case INS_ADD:
		    	//Adding two int numbers
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->integer = (Active->TAK.arg1->integer) + (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Adding two real numbers
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) + (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}	
		    	//Adding two numbers of different data types
		    	else if ((Active->TAK.arg1->type == INTEGER) || (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->integer) + (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	else if((Active->TAK.arg1->type == DOUBLE) || (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) + (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Error of adding strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}	
	    	
		        break;

		    case INS_SUB:
		       	//Sub two int numbers
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->integer = (Active->TAK.arg1->integer) - (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Sub two real numbers
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) - (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}		    	
		    	//Sub two numbers of different data types
		    	else if ((Active->TAK.arg1->type == INTEGER) || (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->integer) - (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	else if((Active->TAK.arg1->type == DOUBLE) || (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) - (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Error of sub strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    case INS_MUL:
		    	//Multiply two int numbers
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->integer = (Active->TAK.arg1->integer) * (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Multiply two real numbers
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) * (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Multiply two numbers of different data types
		    	else if ((Active->TAK.arg1->type == INTEGER) || (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->integer) * (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	else if((Active->TAK.arg1->type == DOUBLE) || (Active->TAK.arg2->type == INTEGER))
		    	{
		    		Active->TAK.dest->real = (Active->TAK.arg1->real) * (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Error of multiplying strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}				   	
		        break;

		    case INS_DIV:
		    	//Divide two int numbers
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		//Error of zero dividing
			    	if((Active->TAK.arg2->integer == 0))
			    	{		    		
			    		print_err(ERR_zero_division, 0);
			    		return ERR_zero_division;
			    	}

		    		Active->TAK.dest->integer = (Active->TAK.arg1->integer) / (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Divide two real numbers
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		//Error of zero dividing
			    	if((Active->TAK.arg2->real == 0))
			    	{
			    		print_err(ERR_zero_division, 0);
			    		return ERR_zero_division;
			    	}

		    		Active->TAK.dest->real = (Active->TAK.arg1->real) / (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Divide two numbers of different data types
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		//Error of zero dividing
			    	if((Active->TAK.arg2->real == 0))
			    	{
			    		print_err(ERR_zero_division, 0);
			    		return ERR_zero_division;
			    	}

		    		Active->TAK.dest->real = (Active->TAK.arg1->integer) / (Active->TAK.arg2->real);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	else if((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		//Error of zero dividing
			    	if((Active->TAK.arg2->integer == 0))
			    	{
			    		print_err(ERR_zero_division, 0);
			    		return ERR_zero_division;
			    	}

		    		Active->TAK.dest->real = (Active->TAK.arg1->real) / (Active->TAK.arg2->integer);
		    		Active->TAK.dest->type = DOUBLE;
		    	}
		    	//Error of dividing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

			/*Block of instructions for compararing*/
		    case INS_EQUAL:
		    	//Comparing if two int numbers are equal
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) == (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if two real numbers are equal
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) == (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if two integer and real numbers are equal
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) == (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if two real and integer numbers are equal
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) == (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{	
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) == 0)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	else
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    case INS_GREATER:
		    	//Comparing if first argument is greater
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) > (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first argument is greater
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) > (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first integer number is greater then second real number
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) > (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first real number is greater then second integer number
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) > (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing two strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) > 1)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	else
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    case INS_LESS:
		    	//Comparing if first argument is leseer
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) < (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first argument is lesser
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) < (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first integer number is lesser then second real number
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) < (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first real number is lesser then second integer number
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) < (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) < 0)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	else
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    case INS_GREATER_EQUAL:
		    	//Comparing if first argument is greater or equal
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) >= (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first argument is greater or equal
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) >= (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first integer number is greater or equal then second real number
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) >= (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first real number is greater or equal then second integer number
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) >= (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) >= 0)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	else
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    case INS_LESS_EQUAL:
		    	//Comparing if first argument is lesser or equal
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) <= (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first argument is lesser or equal
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) <= (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first integer number is lesser or equal then second real number
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) <= (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first real number is lesser or equal then second integer number
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) <= (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) <= 0)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		        break;

		    case INS_NOT_EQUAL:
		     	//Comparing if arguments are not equal - integer
		    	if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->integer) != (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if arguments are not equal - real
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->real) != (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first integer number is not equal then second real number
		    	else if ((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg2->type == DOUBLE))
		    	{
		    		if((Active->TAK.arg1->integer) != (Active->TAK.arg2->real))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing if first real number is not equal then second integer number
		    	else if ((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg2->type == INTEGER))
		    	{
		    		if((Active->TAK.arg1->real) != (Active->TAK.arg2->integer))
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	//Comparing strings
		    	else if ((Active->TAK.arg1->type == STRING) || (Active->TAK.arg2->type == STRING))
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string) != 0)
		    		{
		    			Active->TAK.dest->integer = 1;
		    		}
		    		else
		    		{
		    			Active->TAK.dest->integer = 0;
		    		}
		    		Active->TAK.dest->type = INTEGER;
		    	}
		    	else
		    	{
		    		print_err(ERR_data_type, 0);
		    		return ERR_data_type;
		    	}
		        break;

		    /*Block of instructions for conditions*/
	    	case INS_IF:
		    	//Condition is true and data type_is integer
		    	if((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg1->integer != 0))
		    	{
		    		if(!ins_solve(Active->prev)) //Calling back INS_FOR			
			    	{
			    		return EXIT_SUCCESS;
			    	} 
		    		while(Active->TAK.ins_code != INS_END_IF)
		    		{
		    			Active = Active->prev;
		    		}
		    	}
		    	//Condition is true and data_type is real
		    	else if((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg1->real != 0))
		    	{
		    		if(!ins_solve(Active->prev)) //Calling back INS_FOR			
			    	{
			    		return EXIT_SUCCESS;
			    	} 
		    		while(Active->TAK.ins_code != INS_END_IF)
		    		{
		    			Active = Active->prev;
		    		}
		    	}
		    	//Condition is false
		    	else
		    	{
		    		while(Active->TAK.ins_code != INS_STOPPER)
		    		{
		    			Active = Active->prev;

		    		}
		    	}
		        break;

		    case INS_ELSE:
	    		if(!ins_solve(Active->prev)) //Calling back INS_FOR			
			    {
			    	return EXIT_SUCCESS;
			    } 
	    		while(Active->TAK.ins_code != INS_END_IF)
		   		{
		   			Active = Active->prev;
		   		}
		        break;

		    /*Block of instructions for input/output*/
		    case INS_CIN:
			    {	
		      		//Retype the varible from stdin to integer nubmer
			        if ((Active->TAK.dest->type == INTEGER))
			    	{		    		
				       	if (scanf("%ld", &Active->TAK.dest->integer) <= 0) {
				       		print_err(ERR_std_in_number, line);
				       		return EXIT_FAILURE;
				       	}
			    	}
			    	//Retype the varible from stdin to real nubmer
			        else if ((Active->TAK.dest->type == DOUBLE))
			    	{
			    		if (scanf("%lf", &Active->TAK.dest->real) <= 0) {
				       		print_err(ERR_std_in_number, line);
				       		return EXIT_FAILURE;
				       	}
			    	}
			    	//Store the variable form stdin to string structure		    	
			    	else if ((Active->TAK.dest->type == STRING))
			    	{
			    		//Declare temporary variables for working with stdin
				    	char temp_string[1025];
				    	if (scanf("%s", temp_string) <= 0) {
				       		print_err(ERR_std_in_number, line);
				       		return EXIT_FAILURE;
				       	}
			    		Active->TAK.dest->string->string = temp_string;
				    }
				    else
		    		{
		    			print_err(ERR_data_type, 0);
		    			return ERR_data_type;
		    		}
				} break;

		    case INS_COUT:
			    {	
			    	//Doing stdout output while INS_COUT is avaliable
			    	while(Active->TAK.ins_code != INS_COUT_END)
			    	{
			    		//Write down integer number
			    		if ((Active->TAK.arg1->type == INTEGER))
				    	{		    		
					       	printf("%ld", Active->TAK.arg1->integer);
				    	}
				    	//Write down real number			    	
				        else if ((Active->TAK.arg1->type == DOUBLE))
				    	{
				    		printf("%g", Active->TAK.arg1->real);
				    	}
				    	//Write down string		    	
				    	else if ((Active->TAK.arg1->type == STRING))
				    	{
				    		printf("%s", Active->TAK.arg1->string->string);
					    }
					    else
		    			{
		    				print_err(ERR_data_type, 0);
		    				return ERR_data_type;
		    			}
			    		Active = Active->prev; //Turn to another instruction	
			    	}
			    	printf("\n"); //Another output of program will be on newline		    	
			    } break;

		    case INS_COUT_END:
		    	//Empty instruction. Only marks the end of stdout
		        break;

		    /*Instruction for assignment*/
		    case INS_ASSIGN:
			    if(Active->TAK.dest->type == INTEGER) 
			    {	
			    	if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == STRING) 
			    { 
			    	if(Active->TAK.arg1->type == STRING)
			    	{			    		
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == DOUBLE)
			    { 
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}			    	
			    }
			    else if(Active->TAK.dest->type == AUTO)
			    { 
			    	Active->TAK.dest->type = Active->TAK.arg1->type;
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == STRING)
			    	{
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == AUTO)
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    	else
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else
			    {
			    	print_err(ERR_data_type, 0);
			    	return ERR_data_type;
			    }
		    	break;

		    case INS_FOR:
		    	active_stack_push(Stack_for, Active);		    	
		    	break;

		    /*Instruction for cycle for*/
		    case INS_FOR_CONDITION:
			    {			    
					 //Save current active instruction for getting back by INS_FOR_STOPPER
			    	//Condition is true and data type_is integer
			    	if((Active->TAK.arg1->type == INTEGER) && (Active->TAK.arg1->integer != 0))
			    	{	
			    		if(!ins_solve(Active->prev)) //Calling back INS_FOR			
				    	{
				    		return EXIT_SUCCESS;
				    	} 	
			    		while(Active->TAK.ins_code != INS_FOR_STOPPER)
			    		{
			    			Active = Active->prev; 			    			
			    		}
			    	}
			    	//Condition is true and data_type is real
			    	else if((Active->TAK.arg1->type == DOUBLE) && (Active->TAK.arg1->real != 0))
			    	{
			    		if(!ins_solve(Active->prev)) //Calling back INS_FOR			
				    	{
				    		return EXIT_SUCCESS;
				    	} 		
			    		while(Active->TAK.ins_code != INS_FOR_STOPPER)
			    		{
			    			Active = Active->prev; 
			    			
			    		}
			    	}
			    	else if((Active->TAK.arg1->type == STRING) /*|| (Active->TAK.arg2->type == STRING)*/)
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_data_type;
			    		
			    	}
			    	//Condition is false
			    	else
			    	{
			    		while(Active->TAK.ins_code != INS_END_FOR)
			    		{
			    			Active = Active->prev;
			    		}
			    	}
			    }break;

		    case INS_FOR_INC:
		    	//The same instruction as INS_ASSIGN but in for cycle
		    	if(Active->TAK.dest->type == INTEGER) 
			    {	
			    	if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == STRING) 
			    { 
			    	if(Active->TAK.arg1->type == STRING)
			    	{
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == DOUBLE)
			    { 
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}			    	
			    }
			    else if(Active->TAK.dest->type == AUTO)
			    { 
			    	Active->TAK.dest->type = Active->TAK.arg1->type;
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == STRING)
			    	{
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == AUTO)
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_data_type;
			    	}
			    	else
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_data_type;
			    	}
			    }
		        break;

		    case INS_FOR_STOPPER:
		    	active_stack_top(Stack_for, &Active);
		    	if(!ins_solve(Active)) //Calling back INS_FOR			
			    {
			    	return EXIT_SUCCESS;
			    } 
		    	break;

		    case INS_END_FOR:
		    	//An empty instruction that indicate the end of the for cycle
		        break;

		    /*Block of instructions for functions*/
		    case INS_FUNC_DECL:
		    	//Empty instruction only indicate the start of function declaration
		        break;

		    case INS_ARG_ASSIGN:
		    	//Empty instruction
		    	break;
		    
		    case INS_FUNC_CALL:	
		    	//Assign arguments of function
		    	if(Active->prev->TAK.ins_code == INS_ARG_ASSIGN)
		    	{
		    		start_of_stack arg_temp = Active->prev;
		    		while (arg_temp->TAK.ins_code == INS_ARG_ASSIGN) {
		    			if(arg_temp->TAK.dest->type == INTEGER) {	
				    	if(arg_temp->TAK.arg1->type == DOUBLE) {
				    		arg_temp->TAK.dest->integer = arg_temp->TAK.arg1->real;
				    	}
				    	else if(arg_temp->TAK.arg1->type == INTEGER) {
				    		arg_temp->TAK.dest->integer = arg_temp->TAK.arg1->integer;
				    	}
				    	else {
				    		print_err(ERR_other_running_ERRs, 0);
				    		return ERR_other_running_ERRs;
				    	}
					    }
					    else if(arg_temp->TAK.dest->type == STRING) { 
					    	if(arg_temp->TAK.arg1->type == STRING) {
					    		arg_temp->TAK.dest->string->string = arg_temp->TAK.arg1->string->string;
					    	}
					    	else {
					    		print_err(ERR_other_running_ERRs, 0);
					    		return ERR_other_running_ERRs;
					    	}
					    }
					    else if(arg_temp->TAK.dest->type == DOUBLE) { 
					    	if(arg_temp->TAK.arg1->type == INTEGER) {
					    		arg_temp->TAK.dest->real = arg_temp->TAK.arg1->integer;
					    	}
					    	else if(arg_temp->TAK.arg1->type == DOUBLE) {
					    		arg_temp->TAK.dest->real = arg_temp->TAK.arg1->real;
					    	}
					    	else {
					    		print_err(ERR_other_running_ERRs, 0);
					    		return ERR_other_running_ERRs;
					    	}			    	
					    }
					    else if(arg_temp->TAK.dest->type == AUTO) { 
					    	arg_temp->TAK.dest->type = arg_temp->TAK.arg1->type;
					    	if(arg_temp->TAK.arg1->type == INTEGER) {
					    		arg_temp->TAK.dest->integer = arg_temp->TAK.arg1->integer;
					    	}
					    	else if(arg_temp->TAK.arg1->type == STRING) {
					    		arg_temp->TAK.dest->string->string = arg_temp->TAK.arg1->string->string;
					    	}
					    	else if(arg_temp->TAK.arg1->type == DOUBLE) {
					    		arg_temp->TAK.dest->real = arg_temp->TAK.arg1->real;
					    	}
					    	else if(arg_temp->TAK.arg1->type == AUTO) {
					    		print_err(ERR_data_type, 0);
					    		return ERR_data_type;
					    	}
					    	else {
					    		print_err(ERR_data_type, 0);
					    		return ERR_data_type;
					    	}
					    }
					    arg_temp = arg_temp->prev;
		    		}
		    	}
		    	//Find an address of calling function for jump
		    	Elem_t_ptr ret_push = NULL;
		    	ret_push = malloc(sizeof(Elem_t));
		    	ret_push->Next = NULL;
		    	ret_push->Data = Active->TAK.dest;
		    	Prec_Stack_Insert(return_stack, ret_push);
		    	Element * temp_func = inc_stack->Top;
		      	while(temp_func != NULL)
		    	{
		    		if(strcmp(Active->TAK.arg1->string->string, temp_func->name->string->string) == 0)
		    		{
		    			//Jump to function stored in Active		    	
		    			if(ins_solve(temp_func->addr)) 		
		    			{
		    				return EXIT_FAILURE;
		    			}
		    			break;
		    		}
		    		temp_func = temp_func->Next;
		    	}		    	
		        break;
		   
		    case INS_RETURN:
		    	if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {
		    		return_stack->Top->Data->type = Active->TAK.dest->type;
		    		if (Active->TAK.dest->type == INTEGER) {
		    			return_stack->Top->Data->integer = Active->TAK.dest->integer;
		    			Prec_Stack_Delete(return_stack);
		    			return EXIT_SUCCESS;
		    		}
		    		if (Active->TAK.dest->type == DOUBLE) {
		    			return_stack->Top->Data->real = Active->TAK.dest->real;
		    			Prec_Stack_Delete(return_stack);
		    			return EXIT_SUCCESS;
		    		}
		    		if (Active->TAK.dest->type == STRING) {
		    			return_stack->Top->Data->string = Active->TAK.dest->string;
		    			Prec_Stack_Delete(return_stack);
		    			return EXIT_SUCCESS;
		    		}
		    	}
		       break;		    

		    case INS_FUNC_END_DECL:
		    	//Empty instruction
		        break;
		    
		    case INS_ARG_REC_ASSIGN:
		    	if(Active->TAK.dest->type == INTEGER) 
			    {	
			    	if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == STRING) 
			    { 
			    	if(Active->TAK.arg1->type == STRING)
			    	{
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}
			    }
			    else if(Active->TAK.dest->type == DOUBLE)
			    { 
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else
			    	{
			    		print_err(ERR_other_running_ERRs, 0);
			    		return ERR_other_running_ERRs;
			    	}			    	
			    }
			    else if(Active->TAK.dest->type == AUTO)
			    { 
			    	Active->TAK.dest->type = Active->TAK.arg1->type;
			    	if(Active->TAK.arg1->type == INTEGER)
			    	{
			    		Active->TAK.dest->integer = Active->TAK.arg1->integer;
			    	}
			    	else if(Active->TAK.arg1->type == STRING)
			    	{
			    		Active->TAK.dest->string->string = Active->TAK.arg1->string->string;
			    	}
			    	else if(Active->TAK.arg1->type == DOUBLE)
			    	{
			    		Active->TAK.dest->real = Active->TAK.arg1->real;
			    	}
			    	else if(Active->TAK.arg1->type == AUTO)
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_data_type;
			    	}
			    	else
			    	{
			    		print_err(ERR_data_type, 0);
			    		return ERR_data_type;
			    	}
			    }
		        break;

		    case INS_FIND:
		    	if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {		   
		    		return_stack->Top->Data->type = INTEGER; 				    			
		    		return_stack->Top->Data->integer = find(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string);
		    		return_stack->Top->Data->real = find(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string);		    			
		    		return EXIT_SUCCESS;
		    	}		
		    	break;

			case INS_SUBSTRING:
				if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {	    		
		    		char *c = substr(Active->TAK.arg1->string->string, Active->TAK.arg2->integer, Active->TAK.dest->integer);
		    		return_stack->Top->Data->type = STRING;
		    		return_stack->Top->Data->string = malloc(sizeof(String_t));
		    		string_init(return_stack->Top->Data->string);
		    		convert_to_string(&(return_stack->Top->Data->string), c);				    			
		    		return EXIT_SUCCESS;
		    	}		
				break;

			case INS_SORT:
				if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {	    		
		    		char *c = sort(Active->TAK.arg1->string->string);
		    		return_stack->Top->Data->type = STRING;
		    		return_stack->Top->Data->string = malloc(sizeof(String_t));
		    		string_init(return_stack->Top->Data->string);
		    		convert_to_string(&(return_stack->Top->Data->string), c);				    			
		    		return EXIT_SUCCESS;
		    	}	
				break;

			case INS_CONCAT:
				if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {	    		
		    		char *c = concat(Active->TAK.arg1->string->string, Active->TAK.arg2->string->string);
		    		printf("%s\n", c);
		    		return_stack->Top->Data->type = STRING;
		    		return_stack->Top->Data->string = malloc(sizeof(String_t));
		    		string_init(return_stack->Top->Data->string);
		    		convert_to_string(&(return_stack->Top->Data->string), c);				    			
		    		return EXIT_SUCCESS;
		    	}		
				break;

			case INS_LENGTH:
				if (return_stack->Top->Data == NULL) {
		    		Prec_Stack_Delete(return_stack);
		    		return EXIT_SUCCESS;
		    	}
		    	else {		   
		    		return_stack->Top->Data->type = INTEGER; 				    			
		    		return_stack->Top->Data->integer = length(Active->TAK.arg1->string->string);
		    		return_stack->Top->Data->real = length(Active->TAK.arg1->string->string);		    			
		    		return EXIT_SUCCESS;
		    	}				
				break;
	    
		    default: 
		    break;
		}			
		Active = Active->prev;
	}
	return EXIT_SUCCESS;
}
