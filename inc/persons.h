#ifndef PERSONS_H
#define PERSONS_H

#include "hospital_structures.h"

/*!
@brief This function creates an empty struct person.
@param None.
@returns Pointer to a struct person.
*/
Person_t *createStructPerson();

/*!
@brief This function creates the basis for a list of persons.
@param None.
@returns Pointer to a list of persons.
*/
ListPersons_t *createListPersons();

/*!
@brief This function fill an empty struct person with the given values.
@param Arrival type, first name, last name - as received from GUI.
@returns Pointer to a struct person.
*/
Person_t *fillStructPerson(char arrival, char *first_name, char *last_name);

/*!
@brief This function add a person to a list of persons.
@param List of persons, person.
@returns 0 in case of success, -1 in case of failure.
*/
int addPerson(ListPersons_t *list, Person_t *person);

/*!
@brief This function move a person to another list of persons.
@param List of persons (source), list of persons (destination).
@returns 0 in case of success, -1 in case of failure.
*/
int movePerson(ListPersons_t *listActive, ListPersons_t *listCompleted);

/*!
@brief This function appends a person to a list of persons.
@param Person, list of persons.
@returns 0 in case of success, -1 in case of failure.
*/
int appendPerson(Person_t *person, ListPersons_t *list);

/*!
@brief This function frees the memory for a given list of persons.
@param List of persons
@returns 0 in case of success, -1 in case of failure.
*/
int freeListPersons(ListPersons_t *list);

/*!
@brief This function prints on person.
@param Person
@returns 0 in case of success, -1 in case of failure.
*/
int printPerson(Person_t *person);
/*!
@brief This function prints a list of persons.
@param List of persons
@returns 0 in case of success, -1 in case of failure.
*/
int printListPersons(ListPersons_t *list);

/*!
@brief This function exports a given list of persons as a file.
@param List of persons.
@returns 0 in case of success, -1 in case of failure.
*/
int exportListPersons(ListPersons_t *list);

#endif