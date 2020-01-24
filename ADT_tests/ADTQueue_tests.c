//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTQueue.h"


void test_create(void) {
	Queue queue = queue_create();

	TEST_CHECK(queue != NULL);
	TEST_CHECK(queue_size(queue) == 0);

	queue_destroy(queue, false);
}

void test_insert(void) {
	Queue queue = queue_create();
	int array[1000];					// Στο queue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for(int i = 0; i < 1000; i++) {
		queue_insert(queue, &array[i]);
		TEST_CHECK(queue_size(queue) == i+1);		// Το size πρέπει να μεγαλώσει
		TEST_CHECK(queue_top(queue) == &array[0]);	// Το στοιχείο στην κορυφή είναι πάντα το array[0] !!
	}

	queue_destroy(queue, false);
}

void test_remove(void) {
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

	queue_destroy(queue, false);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "queue_create", test_create },
	{ "queue_insert", test_insert },
	{ "queue_remove", test_remove },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};