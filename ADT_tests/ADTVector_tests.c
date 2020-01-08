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

	TEST_CHECK(vec != VECTOR_NONE);
	TEST_CHECK(vector_size(vec) == 0);

	vector_destroy(vec);
}

void test_insert(void) {
	Vector vec = vector_create(0);
	int array[1000];					// Στο vector θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία ώστε να συμβούν πολλαπλά resizes
	for(int i = 0; i < 1000; i++) {
		vector_insert(vec, &array[i]);
		TEST_CHECK(vector_size(vec) == i+1);			// Το size ενημερώθηκε;
		TEST_CHECK(vector_get(vec, i) == &array[i]);	// Μπορούμε να κάνουμε get το στοιχείο που μόλις βάλαμε;
	}

	// Δοκιμή ότι μετά τα resizes τα στοιχεία είναι ακόμα προσπελάσιμα
	for(int i = 0; i < 1000; i++)
		TEST_CHECK(vector_get(vec, i) == &array[i]);

	vector_destroy(vec);
}

void test_remove(void) {
	Vector vec = vector_create(1000);
	int array[1000];

	// set για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το set)
	for(int i = 0; i < 1000; i++)
		vector_set(vec, i, &array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(vector_remove(vec) == &array[i]);
		TEST_CHECK(vector_size(vec) == i);
	}

	// remove σε κενό vector
	TEST_CHECK(vector_remove(vec) == NULL);

	vector_destroy(vec);
}

void test_get_set(void) {
	Vector vec = vector_create(0);
	int array[1000];

	// get σε κενό vector
	TEST_CHECK(vector_get(vec, 0) == NULL);

	// insert πολλαπλά NULL, θα τα αλλάξουμε μετά με set
	for(int i = 0; i < 1000; i++)
		vector_insert(vec, NULL);

	for(int i = 0; i < 1000; i++) {
		TEST_CHECK(vector_get(vec, i) == NULL);
		vector_set(vec, i, &array[i]);
		TEST_CHECK(vector_get(vec, i) == &array[i]);
	}

	// get εκτός μεγέθους κενό vector
	TEST_CHECK(vector_get(vec, -1) == NULL);
	TEST_CHECK(vector_get(vec, 1000) == NULL);

	vector_destroy(vec);
}

int compare_ints(void* a, void* b) {
	return *(int*)a - *(int*)b;
}

void test_find(void) {
	Vector vec = vector_create(1000);
	int array[1000];

	// set για προσθήκη δεδομένων
	for(int i = 0; i < 1000; i++) {
		array[i] = i;
		vector_set(vec, i, &array[i]);
	}

	for(int i = 0; i < 1000; i++)
		TEST_CHECK(vector_find(vec, &i, compare_ints) == i);

	int not_exists = -12;
	TEST_CHECK(vector_find(vec, &not_exists, compare_ints) == -1);

	vector_destroy(vec);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make run-valgrid


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "create", test_create },
	{ "insert", test_insert },
	{ "remove", test_remove },
	{ "get_set", test_get_set },
	{ "find", test_find },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};