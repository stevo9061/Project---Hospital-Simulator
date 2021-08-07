/*--- COMMON LIBRARIES ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*--- CUSTOM LIBRARIES ---*/
#include "hospital_structures.h"
#include "persons.h"

/*--- MACROS ---*/
#define MAX_LENGTH_NAME 20
#define MAX_PERSONS_SEATED 25	  // Max.  number of seated persons
#define MAX_PERSONS_ACTIVE 50	  // Max. number of active persons
#define MAX_PERSONS_COMPLETED 100 // Max. number of completed persons

// TODO Error handling in this file

// Create empty struct of person
Person_t *createStructPerson()
{
	Person_t *newStructPerson = calloc(1, sizeof(Person_t));
	if (newStructPerson == NULL)
	{
		fprintf(stderr, "Error while creating struct person.\n");
		return NULL;
	}
	return newStructPerson;
}

// Create list of persons
ListPersons_t *createListPersons()
{
	ListPersons_t *newList = calloc(1, sizeof(ListPersons_t)); // initialized with 0
	if (newList == NULL)
	{
		fprintf(stderr, "Error while creating list of persons.\n");
		return NULL;
	}
	newList->count = 0;
	newList->start = NULL; // create first element and set it as start of list
	newList->last = NULL;  // set first element also as last of list

	printf("Success: Creating list of persons.\n");
	return newList;
}

// Fill empty struct of person with given information
Person_t *fillStructPerson(char arrival, char *first_name, char *last_name)
{

	// create empty struct
	Person_t *newPerson = createStructPerson();

	if (newPerson == NULL)
	{
		// NOTE Error message already printed in createStructPerson()
		return NULL;
	}

	// set arrival
	newPerson->arrival = arrival;

	// set first name
	newPerson->first_name = malloc(sizeof(char) * (strlen(first_name) + 1));
	if (newPerson->first_name == NULL)
	{
		free(newPerson);
		return NULL;
	}
	strcpy(newPerson->first_name, first_name);

	// set last name
	newPerson->last_name = malloc(sizeof(char) * (strlen(last_name) + 1));
	if (newPerson->last_name == NULL)
	{
		free(newPerson->first_name);
		free(newPerson);
		return NULL;
	}
	strcpy(newPerson->last_name, last_name);

	return newPerson;
}

// Insert person into list of persons according to mode of arrival
int addPerson(ListPersons_t *list, Person_t *person)
{
	if (list == NULL || person == NULL)
	{
		fprintf(stderr, "Invalid parameter (NULL) given to addPerson().\n");
		return -1;
	}
	// check if person can be added, considering limits
	if (list->count >= MAX_PERSONS_ACTIVE)
	{
		fprintf(stderr, "New person can not be added - Limit reached, no more new active patients allowed.\n");
		return -1;
	}

	if (person->arrival == 'Z')
	{
		if (list->countZivil >= MAX_PERSONS_SEATED)
		{
			fprintf(stderr, "New person with mode of arrival 'Zivil' can not be added - Limit reached, no seats left.\n");
			return -1;
		}
	}

	// if list is empty: insert new person as first element (can be either type R or Z)
	if (list->start == NULL)
	{
		list->start = person;
		list->last = person;

		// if list ist not empty: find place for insertion depending on mode of arrival R or Z
	}
	else
	{

		Person_t *tmp = list->start;

		if (person->arrival == 'R')
		{

			// case 1: only Zs in list! - insert R  before first Z
			if (tmp->arrival == 'Z')
			{ // first element of list is Z
				tmp->node.prev = person;
				person->node.next = tmp;
				list->start = person; // assign pointer to new start
			}
			else if (tmp->arrival == 'R')
			{ // first element of list is R

				// case 2: only Rs in list! - insert R last
				if (list->last->arrival == 'R')
				{
					list->last->node.next = person; // set person als new last element
					person->node.prev = list->last; // set tmp as previous element of person
					list->last = person;

					// case 3: Rs and Zs in list - insert R before first Z
				}
				else
				{
					while (tmp->arrival == 'R')
					{						  // find first person of Type Z
						tmp = tmp->node.next; //
					}
					person->node.prev = tmp->node.prev; // set last r as previous of person, link between person and previous element - established in person
					person->node.next = tmp;			// set tmp as next of person, link between person and following element - established in person
					tmp->node.prev->node.next = person; // set person as next of last r, link between person and previous element - established in previous element
					tmp->node.prev = person;			// set person als previous of tmp, link between person and following element - established in following element
				}
			}

			// case 4: type Z: always insert last
		}
		else
		{
			tmp = list->last;
			tmp->node.next = person; // set person als new last element
			person->node.prev = tmp; // set tmp as previous element of person
			list->last = person;
		}
	}

	// adjust count
	list->count = list->count + 1;
	if (person->arrival == 'Z')
	{
		list->countZivil = list->countZivil + 1;
	}

	return 0;
}

int appendPerson(Person_t *person, ListPersons_t *list)
{
	Person_t *tmp = NULL;

	if (list == NULL || person == NULL)
	{
		fprintf(stderr, "Invalid parameter (NULL) given to appendPerson().\n");
		return -1;
	}

	// delete first element if limit is reached
	if (list->count >= MAX_PERSONS_COMPLETED)
	{
		fprintf(stderr, "New person can not be added - Limit reached, no more completed patients allowed.\n");
		return -1;
	}

	// if list is empty: insert new person as first element
	if (list->start == NULL)
	{
		list->start = person;
		list->last = person;
		person->node.prev = NULL;
		person->node.next = NULL; // necessary, because initially filled with next of active list
	}
	else
	{
		// append person as last element
		tmp = list->last;
		tmp->node.next = person; // set person als new last element
		person->node.prev = tmp; // set tmp as previous element of person
		person->node.next = NULL;
		list->last = person;
	}

	// adjust count
	list->count = list->count + 1;
	if (person->arrival == 'Z')
	{
		list->countZivil = list->countZivil + 1;
	}

	return 0;
}

// Move person from list - always first person
int movePerson(ListPersons_t *listActive, ListPersons_t *listCompleted)
{

	Person_t *tmp = listActive->start;

	if (tmp == NULL)
	{
		fprintf(stderr, "List is empty.\n");
		return -1;
	}

	// reassign pointer to start of list
	listActive->start = tmp->node.next;

	// append to list of completed persons
	appendPerson(tmp, listCompleted);

	// adjust count
	listActive->count = listActive->count - 1;
	if (tmp->arrival == 'Z')
	{
		listActive->countZivil = listActive->countZivil - 1;
		if (tmp->seat != NULL && tmp->seat->node.prev != NULL)
		{
			tmp->neighbour[0] = tmp->seat->node.prev->occupied;
		}
		if (tmp->seat != NULL && tmp->seat->node.next != NULL)
		{
			tmp->neighbour[1] = tmp->seat->node.next->occupied;
		}
	}
	printf("Patient moved to list of completed patients:\n");
	printPerson(tmp);
	return 0;
}

// Free list of persons
int freeListPersons(ListPersons_t *list)
{

	Person_t *tmp = list->start;
	if (list->start == NULL)
	{
		fprintf(stderr, "List is empty.\n");
		return -1;
	}
	while (tmp->node.next != NULL)
	{									 // until finding last element tmp at end of list
		Person_t *next = tmp->node.next; // set pointer to next element in list after tmp
		free(tmp->first_name);
		free(tmp->last_name);
		free(tmp);
		tmp = next; // set previously saved element next as new tmp
	}
	printf("Success: Freeing list of persons.\n");
	return 0;
}

// Print a person
int printPerson(Person_t *person)
{
	printf("Arrival: %c\tFirst name: %s\tLast name: %s\t", person->arrival, person->first_name, person->last_name);
	if (person->neighbour[0] == NULL)
	{
		printf("Neighbour 1: None.\t");
	}
	else
	{
		printf("Neighbour 1: %s %s\t", person->neighbour[0]->first_name, person->neighbour[0]->last_name);
	}
	if (person->neighbour[1] == NULL)
	{
		printf("Neighbour 2: None.\t");
	}
	else
	{
		printf("Neighbour 2: %s %s\t", person->neighbour[1]->first_name, person->neighbour[1]->last_name);
	}
	printf("\n");
	return 0;
}

// Print list of persons
int printListPersons(ListPersons_t *list)
{
	if (list->start == NULL)
	{
		fprintf(stderr, "List is empty.\n");
		return -1;
	}
	Person_t *tmp = list->start;
	while (tmp != NULL)
	{ // until end of list
		printPerson(tmp);
		tmp = tmp->node.next; // set next element as tmp
	}
	printf("Final count: %hu\n", list->count);
	printf("Zivil persons count: %hu\n", list->countZivil);
	return 0;
}

// Export Persons
int exportListPersons(ListPersons_t *list)
{

	// Create file

	FILE *fp;
	// fp = fopen("export.txt", "w");
	char name[40];

	time_t now = time(NULL);
	struct tm *timenow = localtime(&now);

	strftime(name, sizeof(name), "Export_%Y-%m-%d_%H-%M-%S", timenow);
	fp = fopen(name, "w");

	// Check if valid pointer has been returned
	if (fp == NULL)
	{
		fprintf(stderr, "fopen was not successful!\n");
		return -1;
	}

	// Print list of persons into file
	Person_t *tmp = list->start;
	while (tmp != NULL)
	{ // until end of list
		fprintf(fp, "Arrival: %c\tFirst name: %s\tLast name: %s\tSeat Number: \tNeighbour 1: \tNeighbour 2: \t",
				tmp->arrival, tmp->first_name, tmp->last_name);
		if (tmp->neighbour[0] == NULL)
		{
			fprintf(fp, "Neighbour 1: None.\t");
		}
		else
		{
			fprintf(fp, "Neighbour 1: %s %s\t", tmp->neighbour[0]->first_name,
					tmp->neighbour[0]->last_name);
		}
		if (tmp->neighbour[1] == NULL)
		{
			fprintf(fp, "Neighbour 2: None.\t");
		}
		else
		{
			fprintf(fp, "Neighbour 2: %s %s\t", tmp->neighbour[1]->first_name,
					tmp->neighbour[1]->last_name);
		}
		fprintf(fp, "\n");
		tmp = tmp->node.next; // set next element as tmp
	}
	fprintf(fp, "Final count: %hu\n", list->count);
	// Flush file
	if (fflush(fp) != 0)
	{
		fprintf(stderr, "fflush was not successful!\n");
		return -1;
	}
	return 0;
}
