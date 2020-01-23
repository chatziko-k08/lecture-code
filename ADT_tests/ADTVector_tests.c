//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Vector.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTVector.h"


void test_create(void) {
	Vector vec = vector_create(0);

	TEST_CHECK(vec != VECTOR_FAIL);
	TEST_CHECK(vector_size(vec) == 0);

	vector_destroy(vec);
}

void test_insert(void) {
	Vector vec = vector_create(0);
	int array[1000];					// Στο vector θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία ώστε να συμβούν πολλαπλά resizes
	for(int i = 0; i < 1000; i++) {
		vector_insert(vec, &array[i]);
		TEST_CHECK(vector_size(vec) == i+1);		// Το size ενημερώθηκε;
		TEST_CHECK(vector_at(vec, i) == &array[i]);	// Μπορούμε να κάνουμε at το στοιχείο που μόλις βάλαμε;
	}

	// Δοκιμή ότι μετά τα resizes τα στοιχεία είναι ακόμα προσπελάσιμα
	for(int i = 0; i < 1000; i++)
		TEST_CHECK(vector_at(vec, i) == &array[i]);

	vector_destroy(vec);
}

void test_remove(void) {
	Vector vec = vector_create(1000);
	int array[1000];

	// replace για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το replace)
	for(int i = 0; i < 1000; i++)
		vector_replace(vec, i, &array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(vector_remove(vec) == &array[i]);
		TEST_CHECK(vector_size(vec) == i);
	}

	// remove σε κενό vector
	TEST_CHECK(vector_remove(vec) == NULL);

	vector_destroy(vec);
}

void test_at_replace(void) {
	Vector vec = vector_create(0);
	int array[1000];

	// at σε κενό vector
	TEST_CHECK(vector_at(vec, 0) == NULL);

	// insert πολλαπλά NULL, θα τα αλλάξουμε μετά με replace
	for(int i = 0; i < 1000; i++)
		vector_insert(vec, NULL);

	for(int i = 0; i < 1000; i++) {
		TEST_CHECK(vector_at(vec, i) == NULL);
		vector_replace(vec, i, &array[i]);
		TEST_CHECK(vector_at(vec, i) == &array[i]);
	}

	// at εκτός μεγέθους κενό vector
	TEST_CHECK(vector_at(vec, -1) == NULL);
	TEST_CHECK(vector_at(vec, 1000) == NULL);

	vector_destroy(vec);
}

void test_iterate(void) {
	Vector vec = vector_create(0);

	// first/last σε κενό vector
	TEST_CHECK(vector_first(vec) == VECTOR_START);
	TEST_CHECK(vector_last(vec) == VECTOR_END);

	// insert πολλαπλά NULL
	for(int i = 0; i < 1000; i++)
		vector_insert(vec, NULL);

	for(VectorNode node = vector_first(vec); node != VECTOR_END; node = vector_next(vec, node))
		TEST_CHECK(vector_node_value(vec, node) == NULL);

	for(VectorNode node = vector_last(vec); node != VECTOR_START; node = vector_previous(vec, node))
		TEST_CHECK(vector_node_value(vec, node) == NULL);

	vector_destroy(vec);
}

int compare_ints(void* a, void* b) {
	return *(int*)a - *(int*)b;
}

void test_find(void) {
	Vector vec = vector_create(1000);
	int array[1000];

	// replace για προσθήκη δεδομένων
	for(int i = 0; i < 1000; i++) {
		array[i] = i;
		vector_replace(vec, i, &array[i]);
	}

	for(int i = 0; i < 1000; i++) {
		int* p = vector_find(vec, &i, compare_ints);
		TEST_CHECK(*p == i);
	}

	int not_exists = -12;
	TEST_CHECK(vector_find(vec, &not_exists, compare_ints) == NULL);

	vector_destroy(vec);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "create", test_create },
	{ "insert", test_insert },
	{ "remove", test_remove },
	{ "at_replace", test_at_replace },
	{ "iterate", test_iterate },
	{ "find", test_find },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};