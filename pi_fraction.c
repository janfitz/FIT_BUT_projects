/****************************************
* C PI project                          *
* Vypocet cisla PI pomoci ruznych metod *
* Jan Fitz, xfitzj00@stud.fit.vutbr.cz  *
* 24.11.2015                            *
*****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#define MIN_ARGUMENTU 2 // Minialni mozny pocet argumentu 

/******************
* Napoveda --help *
******************/
const char *napoveda =
	" \n"
	;

/**************************************
* Funkce pro osetreni chybovych stavu *
**************************************/
int errors(int code)
{
	// Chyba 3:
	else if((code == 3))
		fprintf(stderr, "Bylo zadano prilis malo argumentu pro zpracovani vystupu programu! \n");
	// Chyba 4:
	else if((code == 4))
		fprintf(stderr, "Argumenty byly zadany ve spatnem tvaru, poradi, nebo ve spatne kombinaci! Spravne pouziti argumentu zobrazite pomoci prepinace \'--help\' \n");

	return EXIT_SUCCESS;
}

/***************************************
* Implemetace funkci                   *
***************************************/
double getPI(unsigned int pocetIteraci)
{
	long double f = 0.0;
	long double b = 2.0;
	long double citatel = 0;
	long double n = pocetIteraci*2;

	for(long double i = 0; i != pocetIteraci; i++) 
	{
		citatel = (n - 1);
		f = (((citatel * citatel)) / ((b + f))); 
		n =  n - 2;
	}

	fprintf(stdout, "%.50Lf \n", (4 / (f + 1)));
	return EXIT_SUCCESS;
}

/***************************************
* Hlavni cast programu - funkce main() *
***************************************/
int main(int argc, char *argv[])
{
	// Testovani spravneho poctu zadanych argumentu funkce main.
	int pocet_argumentu = argc;
	if(pocet_argumentu < MIN_ARGUMENTU)
	{
		errors(3); // Volani funkce s chybovym kodem "3"
		return EXIT_FAILURE;
	}		
	// Soubor podminek, ktere vybiraji spravnou funkci, ktera se ma provest na zaklade predlozenych argumentu.
	if((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-help") == 0) || (strcmp(argv[1], "-h") == 0))
		fprintf(stdout, napoveda);
	else if((strcmp(argv[1], "--getPI") == 0) && (argc == 3))
		getPI(atof(argv[2]));	

	// Osetreni chyboveho stavu, kdy nevyhovuje ani jedna z vyse uvedenych kombinaci argumentu terminalu.
	else
	{
		errors(4); // Volani funkce s chybovym kodem "4"
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
