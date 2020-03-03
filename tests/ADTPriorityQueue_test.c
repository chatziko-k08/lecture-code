//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Priority Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTPriorityQueue.h"

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_create(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL);
	pqueue_set_destroy_value(pqueue, NULL);

	TEST_CHECK(pqueue != NULL);
	TEST_CHECK(pqueue_size(pqueue) == 0);

	pqueue_destroy(pqueue);
}

void test_insert(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL);
	int N = 1000;
	int array[N];					// Στο pqueue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert N στοιχεία
	for(int i = 0; i < N; i++) {
		array[i] = i;
		pqueue_insert(pqueue, &array[i]);
		TEST_CHECK(pqueue_size(pqueue) == i+1);			// Το size ενημερώθηκε;
		TEST_CHECK(pqueue_max(pqueue) == &array[i]);	// Εισαγωγή σε αύξουσα σειρά, το στοιχείο που μόλις βάλαμε πρέπει να είναι στην κορυφή
	}

	pqueue_destroy(pqueue);
}

void test_remove(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free);

	// προσθήκη δεδομένων
	int N = 1000;
	for(int i = 0; i < N; i++)
		pqueue_insert(pqueue, create_int(i));

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for(int i = N-1; i >= 0; i--) {
		int* value = pqueue_max(pqueue);
		TEST_CHECK(*value == i);
		TEST_CHECK(pqueue_remove_max(pqueue) == value);
		TEST_CHECK(pqueue_size(pqueue) == i);
	}

	pqueue_destroy(pqueue);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "pqueue_create", test_create },
	{ "pqueue_insert", test_insert },
	{ "pqueue_remove_max", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};