//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Stack.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTStack.h"


void test_create(void) {
	Stack stack = stack_create(NULL);

	TEST_CHECK(stack != NULL);
	TEST_CHECK(stack_size(stack) == 0);

	stack_destroy(stack);
}

void test_insert(void) {
	Stack stack = stack_create(NULL);
	int array[1000];					// Στο stack θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for(int i = 0; i < 1000; i++) {
		stack_insert(stack, &array[i]);
		TEST_CHECK(stack_size(stack) == i+1);		// Το μέγεθος πρέπει να μεγαλώσει
		TEST_CHECK(stack_top(stack) == &array[i]);	// Στην κορυφή είναι πάντα το στοιχείο που μόλις βάλαμε!
	}

	stack_destroy(stack);
}

void test_remove(void) {
	Stack stack = stack_create(NULL);
	int array[1000];

	// insert για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το insert)
	for(int i = 0; i < 1000; i++)
		stack_insert(stack, &array[i]);

	// Διαδοχικά remove
	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(stack_remove(stack) == &array[i]);
		TEST_CHECK(stack_size(stack) == i);
	}

	stack_destroy(stack);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "stack_create", test_create },
	{ "stack_insert", test_insert },
	{ "stack_remove", test_remove },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};