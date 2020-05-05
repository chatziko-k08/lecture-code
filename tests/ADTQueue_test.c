//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTQueue.h"


void test_create(void) {
	Queue queue = queue_create(NULL);
	queue_set_destroy_value(queue, NULL);

	TEST_ASSERT(queue != NULL);
	TEST_ASSERT(queue_size(queue) == 0);

	queue_destroy(queue);
}

void test_insert(void) {
	Queue queue = queue_create(NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					// Στο queue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for (int i = 0; i < 1000; i++) {
		queue_insert_back(queue, &array[i]);
		TEST_ASSERT(queue_size(queue) == i+1);			// Το size πρέπει να μεγαλώσει
		TEST_ASSERT(queue_front(queue) == &array[0]);	// Το μπροστινό στοιχείο στην κορυφή είναι πάντα το array[0]
		TEST_ASSERT(queue_back(queue) == &array[i]);		// Το πίσω στοιχείο είναι αυτό που μόλις βάλαμε
	}

	queue_destroy(queue);
	free(array);
}

void test_remove(void) {
	Queue queue = queue_create(NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// insert για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το insert)
	for (int i = 0; i < 1000; i++)
		queue_insert_back(queue, &array[i]);

	// Διαδοχικά remove, πρέπει να βγουν με την ίδια σειρά που είναι στο array
	for (int i = 0; i < 1000; i++) {
		TEST_ASSERT(queue_front(queue) == &array[i]);
		queue_remove_front(queue);
		TEST_ASSERT(queue_size(queue) == 999-i);
	}

	queue_destroy(queue);
	free(array);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "queue_create", test_create },
	{ "queue_insert_back", test_insert },
	{ "queue_remove_front", test_remove },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};