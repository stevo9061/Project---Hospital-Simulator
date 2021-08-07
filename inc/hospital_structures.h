#ifndef HOSPITAL_STRUCTURES_H
#define HOSPITAL_STRUCTURES_H
#include <gtk/gtk.h>

/*
 * @brief This is a helper struct for the linked list in 'Person'
 * @param struct Person *next is one structure from Person_t who shows to the next node
 * @param struct Person *prev is one structure from Person_t  who shows to the previous node
 */
struct node_p
{
	struct Person *next;
	struct Person *prev;
};

/*
 * @brief This is a helper struct for the linked list in 'Seat'
 * @param struct Seat *next is one structure from Seat_t who shows to the next node
 * @param struct Seat *prev is one structure from Seat_t  who shows to the previous node
 */
struct node_s
{
	struct Seat *next;
	struct Seat *prev;
};

/*
 * @brief This is 'Person' struct
 * @param char arrival is the prioritization of our patients (valid is 'R' and 'Z')
 * @param char *first_name is the first name of the person
 * @param char *last_name is the last name of the person
 * @param struct Person *neighbour[2] is the storage of left & right neighbour
 * @param struct Seat *seat is the reference to seat, if any
 * @param struct ListRows *row is the reference to the row of the seat, if any
 * @param struct node_p node is a reference to node->next and node->prev
 * @param GtkTreeIter iter holds a reference to the row inside the GUI.
 */
typedef struct Person
{
	char arrival;
	char *first_name;
	char *last_name;
	struct Person *neighbour[2];
	struct Seat *seat;	  // reference to seat, if any
	struct ListRows *row; // reference to row of the seat
	struct node_p node;
	GtkTreeIter iter;
} Person_t;

/*
 * @brief This is 'Seat' struct
 * @param struct Person *occupied is to check if occupied != NULL then it is occupied
 * @param struct node_s node is a reference to node->next and node->prev
 */
typedef struct Seat
{
	struct Person *occupied;
	struct node_s node;
} Seat_t;

/*
 * @brief This is 'ListPersons' struct
 * @param unsigned short count is to validate the persons in a specific row
 * @param struct Person *start is the top of the list
 * @param struct Person *last is the end of the list
 */
typedef struct ListPersons
{
	unsigned short count;
	unsigned short countZivil;
	struct Person *start;
	struct Person *last;
} ListPersons_t;

/*
 * @brief This is 'ListRows' struct
 * @param struct Seat *start is the top of the list
 * @param struct Seat *last is the end of the list
 */
typedef struct ListRows
{
	unsigned short count;
	struct Seat *start;
	struct Seat *last;
} ListRows_t;
#endif