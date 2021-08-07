#include "gtest/gtest.h"
#include "gtk3.h"

extern "C"



// This is a dummy-test, which will always be passed. It serves as a template for other testcases
TEST(gui_unit_tests, DUMMY)
{
	EXPECT_EQ(1, 1);
}

TEST(gui_unit_tests, get_arrival_type_assert){

  /*      gtk_patient_info_t ref_guitest = {
        .first_name_entry = NULL,
        .last_name_entry = NULL,
        .arrival_combobox = NULL,
        .active_persons = NULL,
        .completed_persons = NULL,
        .rows = NULL};

        gchar *arrival_type_string[] = {"Zivil", "Rettung"};
        arrival_combobox = gtk_combo_box_text_new();
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(arrival_combobox), NULL, arrival_type_string[0]);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(arrival_combobox),NULL,arrival_type_string[1]);
        ref_guitest.arrival_combobox = "Zivil";

    EXPECT_STREQ(ref_guitest.arrival_combobox,arrival_type_string[0]);
*/

}