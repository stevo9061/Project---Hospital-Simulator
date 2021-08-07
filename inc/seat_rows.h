#ifndef SEAT_ROWS_H
#define SEAT_ROWS_H
#include "hospital_structures.h"
#ifndef MAX_ROWS
#define MAX_ROWS 5
#endif

/*!
@brief This function creates a row
@param None.
@returns Pointer to a struct newRow
*/
ListRows_t *createRow();

/*!
@brief This function creates a seat
@param None.
@returns Pointer to a struct newSeat
*/
Seat_t *createSeat();

/*!
@brief This function occupies a seat in the row with the given person
@param None
@returns 0 for success, 1 for failure
*/
int occupySeat(ListRows_t *row, Person_t *person);

/*!
@brief This functions clears the occupation of a seat from the given person.
@param Person_t *person - The person of which the reference should be free'd
@returns 0 for success, 1 for failure (can be ignored for type 'R') 
*/
int clearSeat(Person_t *person);

/*!
@brief This function selects the row with the lowest person count
 and occupy a seat in that row.
@param ListRows_t *rows[Max_ROWS] - These are the rows through which will be traversed
@param Person_t *person - This is the person who will be assigned
@returns 0 for success, 1 for failure
*/
// int assignSeat(ListRows_t *row, Person_t *person);
int selectRow(ListRows_t *rows[MAX_ROWS], Person_t *person);

/*!
@brief This function is to free the whole row
@param ListRows_t *row - The list which will be deleted.
@returns 0 for success
*/
int freeRow(ListRows_t *row);
// int freeSeat(Seat_t *seat); // NOTE not needed

#endif