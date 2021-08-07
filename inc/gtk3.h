#ifndef GTK3_H
#define GTK3_H
#include "hospital_structures.h"
#include <gtk/gtk.h>

/*!
 @brief This struct is used to handle the data for the persons to be treated.
 Furthermore, this structure also contains the data type for the widget for
 the combo box (selection 'Z' or 'R'). Moreover, an active and completed list
 of patients is kept.
 @returns a struct
*/
typedef struct gtk_patient_info
{
	GtkWidget *first_name_entry;
	GtkWidget *last_name_entry;
	GtkWidget *arrival_combobox;
	GtkWidget *patient_list_view;
	ListPersons_t *active_persons;
	ListPersons_t *completed_persons;
	ListRows_t **rows;
} gtk_patient_info_t;

/*
 @brief This enum serves the column numbers to the Tree-View-Model in the GUI.
*/
enum
{
	ID_COLUMN,		   // G_TYPE_INT
	ARRIVAL_COLUMN,	   // G_TYPE_CHAR
	FIRST_NAME_COLUMN, // G_TYPE_STRING
	LAST_NAME_COLUMN,  // G_YTPE_STRING
	N_COLUMNS		   // NUMBER OF COLUMNS
};

/*!
 @brief This callback function acquires the priorization of the patient.
 @param GtkComboBoxText is a simple variant of GtkComboBox that
 hides the model-view complexity for simple text-only use cases.
 @returns the arrival_type 'Z' or 'R';
*/
char get_arrival_type_from_combobox(GtkComboBoxText *combobox);

/*!
  @brief This callback function is triggered by closing the window and
  properly exits the gui the program.
  @param GtkWidget *widget is the window which is to be closed
  @param gpointer data is for optional data and not necassary in our function.
  @returns nothing
*/
int on_destroy(GtkWidget *widget, gpointer data);

/*!
 @brief This callback function is for the evaluation of the new patient and
 the input fields.
 @param GtkButton *button is the "New Patient" button in our gui.
 When this button is pressed, the function is triggered automatically.
 @param gpointer data is used for hand over our struct gtk_patient_info. We can
 use with this parameter the information which were passed.
 gpointer data is used for hand over our struct gtk_patient_info. We can
 use with this parameter the information which were created in gui main.
 @returns 0 in case of success, -1 in case of failure
*/
int on_new_Patient_clicked(GtkButton *button, gpointer data);

/*!
 @brief This callback function transfers always with the button "Next Patient"
 the first person in the list active_persons to the list completed_persons.
 @param GtkButton *button is the "Next Patient" button in our gui.
 When this button is pressed, the function is triggered automatically.
 @param gpointer data is used for hand over our struct gtk_patient_info.
 @returns 0 in case of success, -1 in case of failure
*/

int on_next_Patient_clicked(GtkButton *button, gpointer data);

/*!
 @ This function is the gui and is called from main.c. The reason we did
 it this way is because we initialize our data store in main.c and then call the gui.
 @param argc is the number of arguments in our program is 1.
 @param **argv is an array with the name of the program as the first element by any additional
 arguments that may have been passed. In our case, just the name of the program.
 @param ListPersons_t *active creates a list of active persons.
 @param ListPersons_t *completed creates a list of completed persons.
 @param ListRows_t **rows is a list of 5 rows with each of 5 seats.
 @returns 0 in case of success, -1 in case of failure
*/

int gui_main(int argc, char **argv, ListPersons_t *active, ListPersons_t *completed, ListRows_t **rows);

#endif