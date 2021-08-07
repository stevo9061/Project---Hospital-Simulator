/* MAIN.c
 *   by Nikolaus Rieder, Stefan Bittgen, Eva Grabler
 * Description:
 *   Main execution for the gui
**/

/*--- COMMON LIBRARIES ---*/
#include <stdio.h>
#include <stdlib.h>
/*--- CUSTOM LIBRARIES ---*/
#include "gtk3.h"
#include "persons.h"
#include "seat_rows.h"
#include "hospital_structures.h" // Is listed here for completion, but not necessary since gtk3.h includes it
/*--- MACROS ---*/
#define DEBUG
#define ENTRYS 2
#define LABELS 3
#define BUTTONS 2
#define ROWS 4
#define COLUMNS 2
#define BUF 50
#define MAX_ROWS 5

int add_patient_to_view(Person_t *person, GtkTreeView *tree, gint id)
{
	GtkListStore *storage;
	GtkTreeIter iter;
	if (person == NULL)
	{
		return -1;
	}
	gchar *arrival = (person->arrival == 'R') ? "Rettung" : "Zivil";
	storage = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
	gtk_list_store_append(storage, &iter);
	person->iter = iter;
	gtk_list_store_set(storage, &iter,
					   ID_COLUMN, id,
					   ARRIVAL_COLUMN, arrival,
					   FIRST_NAME_COLUMN, (gchar *)person->first_name,
					   LAST_NAME_COLUMN, (gchar *)person->last_name,
					   -1);
	return 0;
}

int remove_patient_from_view(Person_t *person, GtkTreeView *tree, gint id)
{
	GtkListStore *storage;
	GtkTreeIter iter = person->iter;
	if (person == NULL)
	{
		return -1;
	}
	storage = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
	gtk_list_store_remove(storage, &iter);
	return 0;
}

char get_arrival_type_from_combobox(GtkComboBoxText *combobox)
{
	char arrival_type;
	gchar *active_text = gtk_combo_box_text_get_active_text(combobox); // Needs to be free'd according to Documentation
	gchar *cmp_zivil = "Zivil";										   // local variable
	gchar *cmp_rettung = "Rettung";									   // local variable
	if (g_strcmp0(active_text, cmp_zivil) == 0)
	{
		arrival_type = 'Z';
	}
	else if (g_strcmp0(active_text, cmp_rettung) == 0)
	{
		arrival_type = 'R';
	}
	else
	{
		arrival_type = 0;
	}
	g_free(active_text);
	return arrival_type;
}

int on_destroy(GtkWidget *widget, gpointer data)
{
	g_print("Good Bye!\n");
	gtk_main_quit();
	return 0;
}

/* Evaluation input fields */
int on_new_Patient_clicked(GtkButton *button, gpointer data)
{
	gtk_patient_info_t *patient_info = data;
	char *first_name;
	char *last_name;
	char arrival;
	Person_t *person = NULL;

#ifdef DEBUG
	// Checking if the provided user_data is set correctly, because the widgets could be NULL
	g_print(GTK_IS_WIDGET(patient_info->first_name_entry) ? "First_Name: true\n" : "First_Name: false\n");
	g_print(GTK_IS_WIDGET(patient_info->last_name_entry) ? "Last_Name: true\n" : "Last_Name: false\n");
	g_print(GTK_IS_WIDGET(patient_info->arrival_combobox) ? "Arrival_Combobox: true\n" : "Arrival_Combobox: false\n");
#endif

	first_name = (char *)gtk_entry_get_text(GTK_ENTRY(patient_info->first_name_entry));
	last_name = (char *)gtk_entry_get_text(GTK_ENTRY(patient_info->last_name_entry));
	arrival = get_arrival_type_from_combobox(GTK_COMBO_BOX_TEXT(patient_info->arrival_combobox));
	if (arrival == 0)
	{
		fprintf(stderr, "No arrival type was set.\n");
		return -1;
	}

#ifdef DEBUG
	g_print("Entry-Field Evaluation:\n");
	g_print("First Name: %s\n", first_name);
	g_print("Last Name: %s\n", last_name);
	g_print("Arrival: %c\n", arrival);
#endif
	person = fillStructPerson(arrival, first_name, last_name);
	if (addPerson(patient_info->active_persons, person) < 0)
	{
		fprintf(stderr, "Failed to add Person.\n");
		return -1;
	}
	if (person->arrival == 'Z')
	{
		if (selectRow(patient_info->rows, person) < 0)
		{
			fprintf(stderr, "Failed to select row.\n");
			return -1;
		}
	}
	add_patient_to_view(person,
						GTK_TREE_VIEW(patient_info->patient_list_view),
						(gint)patient_info->active_persons->count);
	g_print("Person hinzugefügt.\n");
	gtk_entry_set_text(GTK_ENTRY(patient_info->first_name_entry), "Type here ...");
	gtk_entry_set_text(GTK_ENTRY(patient_info->last_name_entry), "Type here ...");
	gtk_combo_box_set_active(GTK_COMBO_BOX(patient_info->arrival_combobox), -1); // Resets the combo-box to 'No active item'

	return 0;
}

int on_next_Patient_clicked(GtkButton *button, gpointer data)
{
	gtk_patient_info_t *patient_info = data;
	movePerson(patient_info->active_persons, patient_info->completed_persons);
	clearSeat(patient_info->completed_persons->last);

	remove_patient_from_view(patient_info->completed_persons->last,
							 GTK_TREE_VIEW(patient_info->patient_list_view),
							 (gint)patient_info->active_persons->count + 1);

	return 0;
}
/*
static void on_arrival_combobox_changed(GtkComboBox *combobox, gpointer data)
{
	g_print("Arrival Type: %c\n", get_arrival_type_from_combobox(combobox));
}
*/

int gui_main(int argc, char **argv, ListPersons_t *active, ListPersons_t *completed, ListRows_t **rows)
{
	gtk_patient_info_t patient_info = {
		.active_persons = active,
		.completed_persons = completed,
		.rows = rows,
		.first_name_entry = NULL,
		.last_name_entry = NULL,
		.arrival_combobox = NULL,
		.patient_list_view = NULL};
	GtkWidget *win;
	GdkPixbuf *pic;
	GtkGrid *table; //Pack widgets in rows and columns
	GtkLabel *label[LABELS];
	GtkEntry *entry[ENTRYS]; //    Textfields - GtkEntry
	GtkButton *entry_button[BUTTONS];
	GtkWidget *hbox_buttons; //Base class for all widgets
	GtkWidget *vbox;
	GError *err = NULL;
	GtkWidget *arrival_combobox; // the combo-box widget, it is easier to cast to other types from Widget instead of other way around
	// Initialize List-Storage for the patient-list to be displayed in the GUI
	GtkWidget *tree;
	GtkWidget *viewport;
	GtkWidget *scroll_window;
	GtkListStore *patient_list_store;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkAdjustment *v_adjust;
	GtkAdjustment *h_adjust;

	gtk_init(&argc, &argv);

	// Initialize List-Storage for the patient-list to be displayed in the GUI
	patient_list_store = gtk_list_store_new(N_COLUMNS,
											G_TYPE_INT,
											G_TYPE_STRING,
											G_TYPE_STRING,
											G_TYPE_STRING);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(patient_list_store));
	h_adjust = gtk_adjustment_new(0.0, 0.0, 5.0, 1.0, 2.0, 10.0);
	v_adjust = gtk_adjustment_new(0.0, 0.0, 5.0, 1.0, 2.0, 10.0);
	viewport = gtk_viewport_new(h_adjust, v_adjust);
	scroll_window = gtk_scrolled_window_new(h_adjust, v_adjust);
	gtk_container_set_border_width(GTK_CONTAINER(scroll_window), 10);
	gtk_widget_set_size_request(scroll_window, 400, 200);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	g_object_unref(G_OBJECT(patient_list_store)); // Storage can be unreferenced because tree-view keeps own model
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("ID",
													  renderer,
													  "text", ID_COLUMN,
													  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Arrival",
													  renderer,
													  "text", ARRIVAL_COLUMN,
													  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("First name",
													  renderer,
													  "text", FIRST_NAME_COLUMN,
													  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Last name",
													  renderer,
													  "text", LAST_NAME_COLUMN,
													  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	patient_info.patient_list_view = tree;

	// Load a graphic into a pixbuf
	const gchar *const icon_name = "Icon.png";
	pic = gdk_pixbuf_new_from_file(icon_name, &err);
	(pic == NULL) ? g_print("The following error is occurred: %s\n", err->message)
				  : gtk_window_set_default_icon(pic);

	// Create window with the following properties:
	win = g_object_new(GTK_TYPE_WINDOW,
					   "title", "Hospital Simulator",
					   "default-width", 380,
					   "default-height", 200,
					   "resizable", TRUE,
					   "window-position", GTK_WIN_POS_CENTER,
					   "border-width", 10,
					   "icon", pic,
					   NULL);
	// Create a table
	table = GTK_GRID(gtk_grid_new());			 //Creates a new Grid-Widget
	gtk_grid_set_row_spacing(table, ROWS);		 //Sets the amount of space between rows of grid .
	gtk_grid_set_column_spacing(table, COLUMNS); //Sets the amount of space between columns of grid .
	// Create a horizontal box
	hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); //Creates a new GTK-Box -> GtkBox — A container for packing widgets in a single row or column
	vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	// Creates ENTRYS number fields
	for (int i = 0; i < ENTRYS; i++)
	{
		entry[i] = GTK_ENTRY(gtk_entry_new());
		gtk_entry_set_text(entry[i], "Type here ...");
		gtk_entry_set_max_length(entry[i], BUF);
	}

	// Create labels for text fields
	label[0] = g_object_new(GTK_TYPE_LABEL,
							"justify", GTK_JUSTIFY_LEFT,
							"label", "First Name",
							NULL);
	label[1] = g_object_new(GTK_TYPE_LABEL,
							"justify", GTK_JUSTIFY_LEFT,
							"label", "Last Name",
							NULL);
	label[2] = g_object_new(GTK_TYPE_LABEL,
							"label", "Arrival",
							NULL);

	// Create buttons to evaluate the text fields
	entry_button[0] = g_object_new(GTK_TYPE_BUTTON, "label", "New Patient", NULL);
	entry_button[1] = g_object_new(GTK_TYPE_BUTTON, "label", "Next Patient", NULL);

	gchar *arrival_type_string[] = {"Zivil", "Rettung"};
	arrival_combobox = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(arrival_combobox),
							  NULL,
							  arrival_type_string[0]);
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(arrival_combobox),
							  NULL,
							  arrival_type_string[1]);

	// Initialize struct for all the relevant patient_information widgets
	// MUST BE DONE HERE, after initialization of widgets, otherwise the data will be incorrect.
	patient_info.first_name_entry = GTK_WIDGET(entry[0]);
	patient_info.last_name_entry = GTK_WIDGET(entry[1]);
	patient_info.arrival_combobox = arrival_combobox;

	// Create Signalhandler for closing program
	g_signal_connect(win, "destroy",
					 G_CALLBACK(on_destroy), NULL);

	// Signalhandler for the Buttons
	g_signal_connect(entry_button[0], "clicked", G_CALLBACK(on_new_Patient_clicked), &patient_info);
	//	g_signal_connect(entry_button[1], "clicked", G_CALLBACK(entry_loeschen), NULL);
	g_signal_connect(entry_button[1], "clicked", G_CALLBACK(on_next_Patient_clicked), &patient_info);

	// Signalhandler for combobox
	// Not used at the moment
	// g_signal_connect(arrival_combobox, "changed", G_CALLBACK(on_arrival_combobox_changed), &patient_info);

	/* Great packing of the widgets begins
*  Adds a widget to the grid.
*  The position of child is determined by left and top .
*  The number of “cells” that child will occupy is determined by width and height .
*/
	// First name
	gtk_grid_attach(table, GTK_WIDGET(label[0]), 0, 0, 1, 1);
	gtk_grid_attach(table, GTK_WIDGET(entry[0]), 1, 0, 1, 1);
	// Last name
	gtk_grid_attach(table, GTK_WIDGET(label[1]), 0, 2, 1, 1);
	gtk_grid_attach(table, GTK_WIDGET(entry[1]), 1, 2, 1, 1);
	// Arrival
	gtk_grid_attach(table, GTK_WIDGET(label[2]), 0, 3, 1, 1);
	gtk_grid_attach(table, arrival_combobox, 1, 3, 1, 1);

	gtk_grid_attach(table, GTK_WIDGET(hbox_buttons), 0, 4, 2, 1);

	/* NOTE GRID-Helper
	1st Number is left
	LEFT_TO_RIGHT
	~~~~~~~~~~~~~~~~~
	| 0 1 2 3 4 5 6 |
	| 0 1 2 3 4 5 6 |
	| 0 1 2 3 4 5 6 |
	| 0 1 2 3 4 5 6 |
	| 0 1 2 3 4 5 6 |
	| 0 1 2 3 4 5 6 |
	~~~~~~~~~~~~~~~~~
	2nd Number is top
	TOP_TO_BOTTOM
	~~~~~~~~~~~~~~~~~
	| 0 0 0 0 0 0 0 |
	| 1 1 1 1 1 1 1 |
	| 2 2 2 2 2 2 2 |
	| 3 3 3 3 3 3 3 |
	| 4 4 4 4 4 4 4 |
	| 5 5 5 5 5 5 5 |
	| 6 6 6 6 6 6 6 |
	~~~~~~~~~~~~~~~~~
	*/

	/* Adds child to box , packed with reference to the start of box .
*  The child is packed after any other child packed with reference to the start of box .
*/

	gtk_box_pack_start(GTK_BOX(hbox_buttons), GTK_WIDGET(entry_button[0]), FALSE, FALSE, 0); //New Button

	gtk_box_pack_start(GTK_BOX(hbox_buttons), GTK_WIDGET(entry_button[1]), FALSE, FALSE, 0); //Next Button

	gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(table), FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(vbox), scroll_window, FALSE, FALSE, 10);

	/* Adds widget to container . Typically used for simple containers such as GtkWindow,
 * GtkFrame, or GtkButton; for more complicated layout containers such as GtkBox or GtkGrid,
 * this function will pick default packing parameters that may not be correct.
 *
*/
	gtk_container_add(GTK_CONTAINER(scroll_window), viewport);
	gtk_container_add(GTK_CONTAINER(viewport), tree);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	/* Show the window */
	gtk_widget_show_all(GTK_WIDGET(win));

	/* Mainloop of gtk. Program awaits interaction  */
	gtk_main();
	g_print("The Main loop has been terminated.\n");

	return 0;
}
