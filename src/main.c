/* MAIN.c
 *   by Nikolaus Rieder, Stefan Bittgen, Eva Grabler
 * Description:
 *   Main execution of the project
**/

/*--- COMMON LIBRARIES ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // for bool-datatype
#include <unistd.h>	 // getopt(3)
/*--- CUSTOM LIBRARIES ---*/
#include "hospital_structures.h" // -> Project-Datastructures
#include "main.h"				 // -> Prototypes for this source
#include "persons.h"			 // -> Functions for persons & personLists
#include "seat_rows.h"			 // -> Functions for seats, rows & rowLists
#include "gtk3.h"				 // -> Functions for GUI
/*--- MACROS ---*/

int main(int argc, char *argv[])
{
	if (argc == 1) // Program is executed normally in GUI-Mode (no parameters given)
	{
		// NOTE GTK-Main-Function is executed here
		ListPersons_t *persons_active = createListPersons();
		ListPersons_t *persons_completed = createListPersons();
		ListRows_t *row_1 = createRow();
		ListRows_t *row_2 = createRow();
		ListRows_t *row_3 = createRow();
		ListRows_t *row_4 = createRow();
		ListRows_t *row_5 = createRow();
		ListRows_t *rows[MAX_ROWS] = {row_1, row_2, row_3, row_4, row_5};
		gui_main(argc, argv, persons_active, persons_completed, rows);
		exportListPersons(persons_completed);
		freeListPersons(persons_active);
		freeListPersons(persons_completed);
		for (int i = 0; i < MAX_ROWS; i++)
		{
			freeRow(rows[i]);
		}
	}
	else if (argc > 1) // Program is executed in CLI-Mode (parameters exist)
	{
		int opt;
		char *valid_options = "h"; // NOTE Please retain the order of the options alphabetically
		/* NOTE Valid options for CLI-Execution:
		b = binary | Read from provided binary/tmp-storage to export it into output-file
		h = help | Print help message for program execution
		*/
		// bool b_given = false;
		while ((opt = getopt(argc, argv, valid_options))) // Parse parameters
		{
			switch (opt) // NOTE Please retain the order of this switch-statement alphabetically
			{
			// case 'b':
			// 	// TODO Parse argument of b
			// 	b_given = true;
			// 	break;
			case 'h':
				print_help(argv[0]);
				break;
			default:
				fprintf(stderr, "Non-valid option given.\n");
				print_help(argv[0]);
				exit(EXIT_FAILURE);
				break;
			}
		}
		// if (b_given)
		// {
		// 	fprintf(stderr, "Not implemented.\n");
		// }
	}

	// Possible example of Person-List-creation: ListPersons_t activePersons = createListPersons();
	// ^ Will be executed 2 times to generate an active Persons-List and an inactive Persons-List
	// Possible example of Row-List-creation: ListRows_t row1 = createListRow();
	// ^ Will be executed 5 times to generate 5 rows.

	return EXIT_SUCCESS;
}

void print_help(char *const argzero)
{
	printf("%s [OPTIONS]... [ARGUMENTS]...\n", argzero);
	printf("Application to track patients & their seat-neighbours in a simulated hospital.\n\n");
	printf("When no options are given, program is executed normally with a Graphical-User-Interface.\n");
	printf("When options are given, program is executed in CLI-Mode, providing optional features.\n\n");
	printf("----OPTIONS:\n");
	printf("\t -h \t\t print this help text.\n");
}
