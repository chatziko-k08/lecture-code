//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT IntVector.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTIntVector.h"


void test_create(void) {
	IntVector vec = int_vector_create(0);

	TEST_CHECK(int_vector_size(vec) == 0);

	int_vector_destroy(vec);
}

void test_insert(void) {
	IntVector vec = int_vector_create(0);

	// insert 1000 στοιχεία ώστε να συμβούν πολλαπλά resizes
	for(int i = 0; i < 1000; i++) {
		int_vector_insert(vec, i);
		TEST_CHECK(int_vector_size(vec) == i+1);	// Το size ενημερώθηκε;
		TEST_CHECK(int_vector_get_at(vec, i) == i);	// Μπορούμε να κάνουμε at το στοιχείο που μόλις βάλαμε;
	}

	// Δοκιμή ότι μετά τα resizes τα στοιχεία είναι ακόμα προσπελάσιμα
	for(int i = 0; i < 1000; i++)
		TEST_CHECK(int_vector_get_at(vec, i) == i);

	int_vector_destroy(vec);
}

void test_remove(void) {
	IntVector vec = int_vector_create(1000);

	// replace για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το replace)
	for(int i = 0; i < 1000; i++)
		int_vector_set_at(vec, i, i);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(int_vector_remove(vec) == i);
		TEST_CHECK(int_vector_size(vec) == i);
	}

	// remove σε κενό vector
	TEST_CHECK(int_vector_remove(vec) == INT_MIN);

	int_vector_destroy(vec);
}

void test_get_set_at(void) {
	IntVector vec = int_vector_create(0);

	// insert πολλαπλά 0, θα τα αλλάξουμε μετά με replace
	for(int i = 0; i < 1000; i++)
		int_vector_insert(vec, 0);

	for(int i = 0; i < 1000; i++) {
		TEST_CHECK(int_vector_get_at(vec, i) == 0);
		int_vector_set_at(vec, i, i);
		TEST_CHECK(int_vector_get_at(vec, i) == i);
	}

	// at εκτός μεγέθους
	TEST_CHECK(int_vector_get_at(vec, -1) == INT_MIN);
	TEST_CHECK(int_vector_get_at(vec, 1000) == INT_MIN);

	int_vector_destroy(vec);
}

void test_find(void) {
	IntVector vec = int_vector_create(1000);

	// replace για προσθήκη δεδομένων
	for(int i = 0; i < 1000; i++)
		int_vector_set_at(vec, i, i);

	for(int i = 0; i < 1000; i++) {
		int p = int_vector_find(vec, i);
		TEST_CHECK(p == i);
	}

	TEST_CHECK(int_vector_find(vec, -12) == INT_MIN);

	int_vector_destroy(vec);
}

void test_destroy() {
	// Απλά εκτελούμε την destroy, για να ελέγξουμε αν όντως δουλεύει
	// σωστά τρέχουμε το test με valgrind.

	IntVector vec = int_vector_create(1);

	int_vector_set_at(vec, 0, 1);
	int_vector_insert(vec, 1);
	int_vector_remove(vec);

	int_vector_destroy(vec);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "create", test_create },
	{ "insert", test_insert },
	{ "remove", test_remove },
	{ "get_set_at", test_get_set_at },
	{ "find", test_find },
	{ "destroy", test_destroy },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};