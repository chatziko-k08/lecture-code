//////////////////////////////////////////////////////////////////
//
// Unit tests για την υλοποίηση του ADT Stack μέσω ADTList.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTStack.h"
#include "ADTQueue.h"


//// Stack /////////////////////////////////////////////////////////////////////////////////////////////

void test_stack_create(void) {
	Stack stack = stack_create();

	TEST_CHECK(stack != NULL);
	TEST_CHECK(stack_size(stack) == 0);

	stack_destroy(stack);
}

void test_stack_insert(void) {
	Stack stack = stack_create();
	int array[1000];					// Στο stack θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for(int i = 0; i < 1000; i++) {
		stack_insert(stack, &array[i]);
		TEST_CHECK(stack_size(stack) == i+1);		// Το μέγεθος πρέπει να μεγαλώσει
		TEST_CHECK(stack_top(stack) == &array[i]);	// Στην κορυφή είναι πάντα το στοιχείο που μόλις βάλαμε!
	}

	stack_destroy(stack);
}

void test_stack_remove(void) {
	Stack stack = stack_create();
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


//// Queue /////////////////////////////////////////////////////////////////////////////////////////////

void test_queue_create(void) {
	Queue queue = queue_create();

	TEST_CHECK(queue != NULL);
	TEST_CHECK(queue_size(queue) == 0);

	queue_destroy(queue);
}

void test_queue_insert(void) {
	Queue queue = queue_create();
	int array[1000];					// Στο queue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for(int i = 0; i < 1000; i++) {
		queue_insert(queue, &array[i]);
		TEST_CHECK(queue_size(queue) == i+1);		// Το size πρέπει να μεγαλώσει
		TEST_CHECK(queue_top(queue) == &array[0]);	// Το στοιχείο στην κορυφή είναι πάντα το array[0] !!
	}

	queue_destroy(queue);
}

void test_queue_remove(void) {
	Queue queue = queue_create();
	int array[1000];

	// insert για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το insert)
	for(int i = 0; i < 1000; i++)
		queue_insert(queue, &array[i]);

	// Διαδοχικά remove, πρέπει να βγουν με την ίδια σειρά που είναι στο array
	for(int i = 0; i < 1000; i++) {
		TEST_CHECK(queue_remove(queue) == &array[i]);
		TEST_CHECK(queue_size(queue) == 999-i);
	}

	queue_destroy(queue);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make run-valgrid


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "stack_create", test_stack_create },
	{ "stack_insert", test_stack_insert },
	{ "stack_remove", test_stack_remove },
	{ "queue_create", test_queue_create },
	{ "queue_insert", test_queue_insert },
	{ "queue_remove", test_queue_remove },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};