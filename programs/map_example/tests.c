#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"


void strings() {
	char* s1 = "FOO";
	char* s2 = "BAR";

	Map map = map_create((CompareFunc)strcmp, NULL, NULL);
	map_insert(map, s1, s2);
	map_insert(map, s2, s1);

	TEST_CHECK(map_size(map) == 2);

	char* value1 = map_find(map, "FOO");
	char* value2 = map_find(map, "BAR");

	TEST_ASSERT(s1 == value2);
	TEST_ASSERT(s2 == value1);

	map_destroy(map);
}

int compare_ints(int* a, int* b) {
	return *a - *b;
}

void integers() {
	int a1 = 5;
	int a2 = 42;

	Map map = map_create((CompareFunc)compare_ints, NULL, NULL);

	// ΠΡΟΣΟΧΗ: προσθέτουμε στο map pointers προς τοπικές μεταβλητές! Αυτό είναι ok αν το map
	// χρησιμοποιείται ΜΟΝΟ όσο οι μεταβλητές βρίσκονται στο scope! (δλαδή μέχρι το τέλος της κλήσης της συνάρτησης)
	map_insert(map, &a1, &a2);
	map_insert(map, &a2, &a1);

	TEST_CHECK(map_size(map) == 2);

	int b1 = 5;
	int b2 = 42;
	int* value1 = map_find(map, &b1);
	int* value2 = map_find(map, &b2);

	TEST_ASSERT(value1 == &a2);
	TEST_ASSERT(value2 == &a1);

	map_destroy(map);
}

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void integers_loop() {
	Map map = map_create((CompareFunc)compare_ints, free, free);

	// Για να αποθηκεύσουμε 100 διαφορετικούς ακεραίους
	// πρέπει κάθε φορά να δημιουργήσουμε έναν νέο ακέραιο.
	for(int i = 0; i < 100; i++)
		map_insert(map, create_int(i), create_int(2 * i));

	// find
	for(int i = 0; i < 100; i++) {
		int* value = map_find(map, &i);
		TEST_ASSERT(*value == 2*i);
	}

	map_destroy(map);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "strings", strings },
	{ "integers", integers },
	{ "integers_loop", integers_loop },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};