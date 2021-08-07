#include "gtest/gtest.h"

extern "C"
{
#include "hospital_structures.h"
#include "persons.h"
}



// This is a dummy-test, which will always be passed. It serves as a template for other testcases
TEST(person_unit_tests, DUMMY)
{
	EXPECT_EQ(1, 1);
}

TEST(person_unit_tests, fillStructPerson_assert_static)
{
	Person_t ref_person = {
		.arrival = 'Z',
		.first_name = "Peter",
		.last_name = "Lustig",
		.neighbour = {NULL, NULL},
		.seat = NULL,
		.node = {.next = NULL, .prev = NULL}};
	Person_t *test_person = fillStructPerson('Z', "Peter", "Lustig");
	EXPECT_EQ(ref_person.arrival, test_person->arrival);
	EXPECT_STREQ(ref_person.first_name, test_person->first_name);
	EXPECT_STREQ(ref_person.last_name, test_person->last_name);
}

TEST(person_unit_tests, createStructPerson_test){
    EXPECT_NE(createStructPerson(), nullptr);
}

TEST(person_unit_tests, createListPersons_test){
    EXPECT_NE(createStructPerson(), nullptr);
}

// Test if -1 is returned, once limit of active persons is reached
TEST(person_unit_tests, addPerson_limit_active_test){
    Person_t *newPerson = createStructPerson();
    ListPersons_t *list = createListPersons();
    list->count = 50;
    EXPECT_EQ(addPerson(list, newPerson), -1);
    freeListPersons(list);
}

// Test if -1 is returned, once limit of seated persons is reached
TEST(person_unit_tests, addPerson_limit_seated_test){
    Person_t *newPerson = createStructPerson();
    ListPersons_t *list = createListPersons();
    newPerson->arrival = 'Z';
    list->countZivil = 25;
    EXPECT_EQ(addPerson(list, newPerson), -1);
    freeListPersons(list);
}

// Test if first person is added successfully
TEST(person_unit_tests, addPerson_first_test){
    Person_t * newPerson = createStructPerson();
    ListPersons_t *list = createListPersons();
    addPerson(list, newPerson);
    EXPECT_EQ(list->start, list->last);
    freeListPersons(list);
}

// Test if it is possible to add: 5 x person of type Z
TEST(person_unit_tests, addPerson_just_Z_test){
    Person_t *newPerson1 = fillStructPerson('Z', "Christian", "Griffith");
    Person_t *newPerson2 = fillStructPerson('Z', "Clara", "Tate");
    Person_t *newPerson3 = fillStructPerson('Z', "Emily", "Lane");
    Person_t *newPerson4 = fillStructPerson('Z', "Luis", "Herrera");
    Person_t *newPerson5 = fillStructPerson('Z', "Callie", "Moore");
    ListPersons_t *list = createListPersons();
    EXPECT_EQ(addPerson(list, newPerson1), 0);
    EXPECT_EQ(addPerson(list, newPerson2), 0);
    EXPECT_EQ(addPerson(list, newPerson3), 0);
    EXPECT_EQ(addPerson(list, newPerson4), 0);
    EXPECT_EQ(addPerson(list, newPerson5), 0);
    printListPersons(list);
    freeListPersons(list);
}

// Test if it is possible to add: 5 x person of type R
TEST(person_unit_tests, addPerson_just_R_test){
    Person_t *newPerson1 = fillStructPerson('R', "Christian", "Griffith");
    Person_t *newPerson2 = fillStructPerson('R', "Clara", "Tate");
    Person_t *newPerson3 = fillStructPerson('R', "Emily", "Lane");
    Person_t *newPerson4 = fillStructPerson('R', "Luis", "Herrera");
    Person_t *newPerson5 = fillStructPerson('R', "Callie", "Moore");
    ListPersons_t *list = createListPersons();
    EXPECT_EQ(addPerson(list, newPerson1), 0);
    EXPECT_EQ(addPerson(list, newPerson2), 0);
    EXPECT_EQ(addPerson(list, newPerson3), 0);
    EXPECT_EQ(addPerson(list, newPerson4), 0);
    EXPECT_EQ(addPerson(list, newPerson5), 0);
    printListPersons(list);
    freeListPersons(list);
}

// Test if it is possible to add: alternating person of type Z and R
TEST(person_unit_tests, addPerson_alternating_R_Z_test){
    ListPersons_t *list = createListPersons();
    Person_t *newPerson1 = fillStructPerson('Z', "Christian", "Griffith");
    Person_t *newPerson2 = fillStructPerson('Z', "Clara", "Tate");
    Person_t *newPerson3 = fillStructPerson('Z', "Emily", "Lane");
    Person_t *newPerson4 = fillStructPerson('Z', "Luis", "Herrera");
    Person_t *newPerson5 = fillStructPerson('R', "Callie", "Moore");
    Person_t *newPerson6 = fillStructPerson('Z', "Maria", "Richardson");
    Person_t *newPerson7 = fillStructPerson('Z', "Agnes", "Underwood");
    Person_t *newPerson8 = fillStructPerson('R', "Allie", "Chapman");
    Person_t *newPerson9 = fillStructPerson('Z', "Garrett", "Frazier");
    Person_t *newPerson10 = fillStructPerson('Z', "John", "Silva");
    EXPECT_EQ(addPerson(list, newPerson1), 0);
    EXPECT_EQ(addPerson(list, newPerson2), 0);
    EXPECT_EQ(addPerson(list, newPerson3), 0);
    EXPECT_EQ(addPerson(list, newPerson4), 0);
    EXPECT_EQ(addPerson(list, newPerson5), 0);
    EXPECT_EQ(addPerson(list, newPerson6), 0);
    EXPECT_EQ(addPerson(list, newPerson7), 0);
    EXPECT_EQ(addPerson(list, newPerson8), 0);
    EXPECT_EQ( addPerson(list, newPerson9), 0);
    EXPECT_EQ(addPerson(list, newPerson10), 0);
    printListPersons(list);
    freeListPersons(list);
}

// Test if person is appended successfully - list empty
TEST(person_unit_tests, appendPerson_empty_test){
    Person_t * newPerson = createStructPerson();
    ListPersons_t *list = createListPersons();
    EXPECT_EQ(appendPerson(newPerson, list), 0);
    freeListPersons(list);
}

// Test if person is appended successfully - list not empty
TEST(person_unit_tests, appendPerson_not_empty_test){
    ListPersons_t *list = createListPersons();
    Person_t * newPerson1 = fillStructPerson('Z', "Peter", "Lustig");
    EXPECT_EQ(appendPerson(newPerson1, list), 0);
    Person_t * newPerson2 = fillStructPerson('Z', "Susi", "Fad");
    EXPECT_EQ(appendPerson(newPerson2, list), 0);
    freeListPersons(list);
}

// Test if person can not be appended if limit is reached
TEST(person_unit_tests, appendPerson_limit_test){
    ListPersons_t *list = createListPersons();
    list->count = 100;
    Person_t * newPerson = fillStructPerson('Z', "Peter", "Lustig");
    EXPECT_EQ(appendPerson(newPerson, list), -1);
    freeListPersons(list);
}


// Test if person is moved successfully
TEST(person_unit_tests, movePerson_return_success_test){
    Person_t * newPerson = fillStructPerson('Z', "Peter", "Lustig");
    ListPersons_t *listActive = createListPersons();
    ListPersons_t *listCompleted = createListPersons();
    addPerson(listActive, newPerson);
    EXPECT_EQ(movePerson(listActive, listCompleted), 0);
    freeListPersons(listActive);
    freeListPersons(listCompleted);
}

// Test movePerson, if -1 is returned, if list ist empty
TEST(person_unit_tests, movePerson_return_failure_test){
    ListPersons_t *listActive = createListPersons();
    ListPersons_t *listCompleted = createListPersons();
    EXPECT_EQ(movePerson(listActive, listCompleted), -1);
    freeListPersons(listActive);
    freeListPersons(listCompleted);
}

// Test freeListPersons, if -1 is returned, if list ist empty
TEST(person_unit_tests, freeListPersons_return_failure_test){
    ListPersons_t *listActive = createListPersons();
    EXPECT_EQ(freeListPersons(listActive), -1);
}

// Test freeListPersons, if 0 is returned
TEST(person_unit_tests, freeListPersons_return_success_test){
    ListPersons_t *listActive = createListPersons();
    Person_t * newPerson = fillStructPerson('Z', "Peter", "Lustig");
    addPerson(listActive, newPerson);
    EXPECT_EQ(freeListPersons(listActive), 0);
}

// Test printing one person
TEST(person_unit_tests, printPerson_test){
    Person_t * Peter = fillStructPerson('Z', "Peter", "Lustig");
    EXPECT_EQ(printPerson(Peter), 0);
    Person_t * Susi = fillStructPerson('Z', "Susi", "Fad");
    Person_t * testNeighbour = fillStructPerson('Z', "Neighbour", "Neighbourson");
    Susi->neighbour[0] = testNeighbour;
    Susi->neighbour[1] = testNeighbour;
    EXPECT_EQ(printPerson(Susi), 0);
}

// Test printing an empty list
TEST(person_unit_tests, printListPersons_return_failure_test){
    ListPersons_t *list = createListPersons();
    EXPECT_EQ(printListPersons(list), -1);
    freeListPersons(list);
}

// Test printing a list
TEST(person_unit_tests, printListPersons_return_success_test){
    ListPersons_t *list = createListPersons();
    Person_t * newPerson1 = fillStructPerson('Z', "Peter", "Lustig");
    Person_t * newPerson2 = fillStructPerson('Z', "Peter", "Lustig");
    Person_t * newPerson3 = fillStructPerson('Z', "Peter", "Lustig");
    newPerson1->neighbour[0] = newPerson2;
    newPerson1->neighbour[1] = newPerson3;
    addPerson(list, newPerson1);
    addPerson(list, newPerson2);
    addPerson(list, newPerson3);
    EXPECT_EQ(printListPersons(list), 0);
    freeListPersons(list);
}




// Test exporting a list
TEST(person_unit_tests, exportListPersons_test){
    ListPersons_t *list = createListPersons();
    Person_t * newPerson1 = fillStructPerson('Z', "Peter", "Lustig");
    Person_t * newPerson2 = fillStructPerson('Z', "Peter", "Lustig");
    Person_t * newPerson3 = fillStructPerson('Z', "Peter", "Lustig");
    newPerson1->neighbour[0] = newPerson2;
    newPerson1->neighbour[1] = newPerson3;
    addPerson(list, newPerson1);
    addPerson(list, newPerson2);
    addPerson(list, newPerson3);
    EXPECT_EQ(exportListPersons(list), 0);
    freeListPersons(list);
}

