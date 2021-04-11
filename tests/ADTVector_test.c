//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Vector.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTVector.h"


void test_create(void) {
	Vector vec = vector_create(0, NULL);
	vector_set_destroy_value(vec, NULL);

	TEST_ASSERT(vector_size(vec) == 0);

	vector_destroy(vec);
}

void test_insert_last(void) {
	Vector vec = vector_create(0, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));		// Στο vector θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία ώστε να συμβούν πολλαπλά resizes
	for (int i = 0; i < 1000; i++) {
		vector_insert_last(vec, &array[i]);
		TEST_ASSERT(vector_size(vec) == i+1);		// Το size ενημερώθηκε;
		TEST_ASSERT(vector_get_at(vec, i) == &array[i]);	// Μπορούμε να κάνουμε at το στοιχείο που μόλις βάλαμε;
	}

	// Δοκιμή ότι μετά τα resizes τα στοιχεία είναι ακόμα προσπελάσιμα
	for (int i = 0; i < 1000; i++)
		TEST_ASSERT(vector_get_at(vec, i) == &array[i]);

	vector_destroy(vec);
	free(array);
}

void test_remove_last(void) {
	Vector vec = vector_create(1000, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// replace για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το replace)
	for (int i = 0; i < 1000; i++)
		vector_set_at(vec, i, &array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for (int i = 999; i >= 0; i--) {
		TEST_ASSERT(vector_get_at(vec, i) == &array[i]);
		vector_remove_last(vec);
		TEST_ASSERT(vector_size(vec) == i);
	}

	vector_destroy(vec);
	free(array);
}

void test_get_set_at(void) {
	Vector vec = vector_create(0, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// insert πολλαπλά NULL, θα τα αλλάξουμε μετά με replace
	for (int i = 0; i < 1000; i++)
		vector_insert_last(vec, NULL);

	for (int i = 0; i < 1000; i++) {
		TEST_ASSERT(vector_get_at(vec, i) == NULL);
		vector_set_at(vec, i, &array[i]);
		TEST_ASSERT(vector_get_at(vec, i) == &array[i]);
	}

	vector_destroy(vec);
	free(array);
}

void test_iterate(void) {
	Vector vec = vector_create(0, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));		// Στο vector θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// first/last σε κενό vector
	TEST_ASSERT(vector_first(vec) == VECTOR_BOF);
	TEST_ASSERT(vector_last(vec) == VECTOR_EOF);

	// εισαγωγή στοιχείων
	for (int i = 0; i < 1000; i++)
		vector_insert_last(vec, &array[i]);

	int i = 0;
	for (VectorNode node = vector_first(vec); node != VECTOR_EOF; node = vector_next(vec, node))
		TEST_ASSERT(vector_node_value(vec, node) == &array[i++]);
	TEST_ASSERT(i == N);

	for (VectorNode node = vector_last(vec); node != VECTOR_BOF; node = vector_previous(vec, node))
		TEST_ASSERT(vector_node_value(vec, node) == &array[--i]);
	TEST_ASSERT(i == 0);

	vector_destroy(vec);
	free(array);
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_find(void) {
	Vector vec = vector_create(1000, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// replace για προσθήκη δεδομένων
	for (int i = 0; i < 1000; i++) {
		array[i] = i;
		vector_set_at(vec, i, &array[i]);
	}

	for (int i = 0; i < 1000; i++) {
		int* p = vector_find(vec, &i, compare_ints);
		TEST_ASSERT(*p == i);

		VectorNode node = vector_find_node(vec, &i, compare_ints);
		TEST_ASSERT(*(int*)vector_node_value(vec, node) == i);
	}

	int not_exists = -12;
	TEST_ASSERT(vector_find(vec, &not_exists, compare_ints) == NULL);
	TEST_ASSERT(vector_find_node(vec, &not_exists, compare_ints) == VECTOR_EOF);

	vector_destroy(vec);
	free(array);
}

void test_destroy(void) {
	// Απλά εκτελούμε την destroy, για να ελέγξουμε αν όντως δουλεύει
	// σωστά τρέχουμε το test με valgrind.

	Vector vec = vector_create(1, free);

	vector_set_at(vec, 0, malloc(1));
	vector_insert_last(vec, malloc(1));
	vector_remove_last(vec);

	vector_destroy(vec);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "vector_create", test_create },
	{ "vector_insert_last", test_insert_last },
	{ "vector_remove_last", test_remove_last },
	{ "vector_get_set_at", test_get_set_at },
	{ "vector_iterate", test_iterate },
	{ "vector_find", test_find },
	{ "vector_destroy", test_destroy },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};