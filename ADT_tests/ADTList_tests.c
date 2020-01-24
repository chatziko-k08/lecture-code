//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT List.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTList.h"


void test_create(void) {
	List list = list_create(NULL);

	TEST_CHECK(list != NULL);
	TEST_CHECK(list_size(list) == 0);

	list_destroy(list);
}

void test_insert(void) {
	List list = list_create(NULL);
	int array[1000];					// θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία στην αρχή
	for(int i = 0; i < 1000; i++) {
		list_insert(list, LIST_BOF, &array[i]);
		TEST_CHECK(list_size(list) == i+1);									// Το size ενημερώθηκε;
		TEST_CHECK(list_node_value(list, list_first(list)) == &array[i]);	// Μπορούμε να διαβάσουμε το στοιχείο που μόλις βάλαμε;
	}

	// τα στοιχεία πρέπει να έχουν μπει με την αντίστροφη σειρά
	ListNode node = list_first(list);
	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(list_node_value(list, node) == &array[i]);
		node = list_next(list, node);
	}

	// Προσθήκη στη μέση
	node = list_first(list);
	node = list_next(list, node);
	node = list_next(list, node);

	list_insert(list, node, &array[0]);
	TEST_CHECK(list_node_value(list, list_next(list, node)) == &array[0]);

	list_destroy(list);
}

// TODO TESTS MISSING

// void test_remove(void) {
// 	List list = list_create();
// 	int array[1000];

// 	// insert για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το insert)
// 	for(int i = 0; i < 1000; i++)
// 		list_insert(list, &array[i]);

// 	// Διαδοχικά remove ώστε να συμβούν και resizes
// 	for(int i = 999; i >= 0; i--) {
// 		TEST_CHECK(list_remove(list) == &array[i]);
// 		TEST_CHECK(list_size(list) == i);
// 	}

// 	// Η χωρητικότητα πρέπει να έχει επιστρέψει στην αρχική
// 	TEST_CHECK(list->capacity == VECTOR_MIN_CAPACITY);

// 	list_destroy(list);
// }

// void test_get_set(void) {
// 	List list = list_create();
// 	int array[1000];

// 	// insert πολλαπλά NULL, θα τα αλλάξουμε μετά με set
// 	for(int i = 0; i < 1000; i++)
// 		list_insert(list, NULL);

// 	for(int i = 0; i < 1000; i++) {
// 		TEST_CHECK(list_node_value(list, i) == NULL);
// 		list_set(list, i, &array[i]);
// 		TEST_CHECK(list_node_value(list, i) == &array[i]);
// 	}

// 	list_destroy(list);
// }

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "create", test_create },
	{ "insert", test_insert },
	// { remove, test_remove },
	// { "get_set", test_get_set },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};