#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "pair_sum.h"


// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void test_pair_sum(void) {
	Vector numbers = vector_create(0, free);
	for (int i = 0; i < 10; i++)
		vector_insert_last(numbers, create_int(i));

	int a, b;
	TEST_ASSERT(!pair_sum(90, numbers, &a, &b));
	TEST_ASSERT( pair_sum(15, numbers, &a, &b));
	TEST_ASSERT(a + b == 15);

	vector_destroy(numbers);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "pair_sum", test_pair_sum },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};