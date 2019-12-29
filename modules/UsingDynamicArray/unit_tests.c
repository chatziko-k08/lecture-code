//////////////////////////////////////////////////////////////////
//
// Unit tests για την υλοποίηση του ADT Vector μέσω Dynamic Array.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTVector.h"

// Στα unit tests θέλουμε να ελέγξουμε και "εσωτερικά" στοιχεία της υλοποίησης, (πχ το πώς μεταβάλλεται
// η χωρητικότητα του dynamic array) παρόλο που δεν είναι ορατά στον χρήστη του module.
#include "DynamicArray.h"


void test_create(void) {
	Vector vec = vector_create(0);

	TEST_CHECK(vec != NULL);
	TEST_CHECK(vector_size(vec) == 0);
	TEST_CHECK(vec->capacity == VECTOR_MIN_CAPACITY);

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

	// Με αρχική χωρητικότητα 10, πρέπει να έχουν γίνει 8 διπλασιασμοί (2^8 = 128)
	TEST_CHECK(vec->capacity == VECTOR_MIN_CAPACITY * 128);

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

	// Η χωρητικότητα πρέπει να έχει μικρύνει
	TEST_CHECK(vec->capacity < 2*VECTOR_MIN_CAPACITY);

	vector_destroy(vec);
}

void test_get_set(void) {
	Vector vec = vector_create(0);
	int array[1000];

	// insert πολλαπλά NULL, θα τα αλλάξουμε μετά με set
	for(int i = 0; i < 1000; i++)
		vector_insert(vec, NULL);

	for(int i = 0; i < 1000; i++) {
		TEST_CHECK(vector_get(vec, i) == NULL);
		vector_set(vec, i, &array[i]);
		TEST_CHECK(vector_get(vec, i) == &array[i]);
	}

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
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};