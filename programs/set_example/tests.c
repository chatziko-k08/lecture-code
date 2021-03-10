#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTSet.h"


void strings(void) {
	char* s1 = "FOO";
	char* s2 = "BAR";

	Set set = set_create((CompareFunc)strcmp, NULL);
	set_insert(set, s1);
	set_insert(set, s2);

	TEST_ASSERT(set_size(set) == 2);

	char* value1 = set_find(set, "FOO");
	char* value2 = set_find(set, "BAR");

	TEST_ASSERT(s1 == value1);
	TEST_ASSERT(s2 == value2);

	set_destroy(set);
}

int compare_ints(int* a, int* b) {
	return *a - *b;
}

void integers(void) {
	int a1 = 5;
	int a2 = 42;

	Set set = set_create((CompareFunc)compare_ints, NULL);

	// ΠΡΟΣΟΧΗ: προσθέτουμε στο set pointers προς τοπικές μεταβλητές! Αυτό είναι ok αν το set
	// χρησιμοποιείται ΜΟΝΟ όσο οι μεταβλητές βρίσκονται στο scope! (δλαδή μέχρι το τέλος της κλήσης της συνάρτησης)
	set_insert(set, &a1);
	set_insert(set, &a2);

	TEST_ASSERT(set_size(set) == 2);

	int b1 = 5;
	int b2 = 42;
	int* value1 = set_find(set, &b1);
	int* value2 = set_find(set, &b2);

	TEST_ASSERT(value1 == &a1);
	TEST_ASSERT(value2 == &a2);

	set_destroy(set);
}

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void integers_loop(void) {
	// Χρησιμοποιούμε destroy_value = free ώστε να γίνονται αυτόματα free οι τιμές που αφαιρούνται
	Set set = set_create((CompareFunc)compare_ints, free);

	// Για να αποθηκεύσουμε 100 διαφορετικούς ακεραίους
	// πρέπει κάθε φορά να δημιουργήσουμε έναν νέο ακέραιο.
	for (int i = 0; i < 100; i++)
		set_insert(set, create_int(i));

	// set_min and set_next
	int i = 0;
	for (SetNode node = set_first(set); node != SET_EOF; node = set_next(set, node)) {
		int* value = set_node_value(set, node);
		TEST_ASSERT(*value == i++);
	}

	// set_max and set_previous
	i = 99;
	for (SetNode node = set_last(set); node != SET_BOF; node = set_previous(set, node)) {
		int* value = set_node_value(set, node);
		TEST_ASSERT(*value == i--);
	}

	// destroy, με free_values = true για να κάνουμε free και τα περιεχόμενα
	set_destroy(set);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "strings", strings },
	{ "integers", integers },
	{ "integers_loop", integers_loop },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};