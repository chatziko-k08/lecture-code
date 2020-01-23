#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTIntVector.h"


void integers_loop() {
	IntVector vec = int_vector_create(0);

	// Το IntVector επιτρέπει να προσθέτουμε εύκολα ακεραίους χωρίς malloc
	int i;
	for(i = 0; i < 100; i++)
		int_vector_insert(vec, i);

	int value1 = int_vector_at(vec, 0);
	int value2 = int_vector_at(vec, 1);

	// Οι τιμές είναι αυτές που περιμένουμε
	TEST_ASSERT(value1 == 0);
	TEST_ASSERT(value2 == 1);

	// Δεν έχουμε κάνει εμείς malloc, οπότε δεν χρειάζεται και free
	int_vector_destroy(vec);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "integers_loop", integers_loop },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};