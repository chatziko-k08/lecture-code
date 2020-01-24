#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTVector.h"


void strings() {
	char* s1 = "FOO";
	char* s2 = "BAR";

	Vector vec = vector_create(0);
	vector_insert(vec, s1);
	vector_insert(vec, s2);

	TEST_CHECK(vector_size(vec) == 2);

	char* value1 = vector_get_at(vec, 0);
	char* value2 = vector_get_at(vec, 1);

	TEST_ASSERT(strcmp(s1, value1) == 0);
	TEST_ASSERT(strcmp(s2, value2) == 0);

	vector_destroy(vec);
}

void integers() {
	int a1 = 5;
	int a2 = 42;

	Vector vec = vector_create(0);

	// ΠΡΟΣΟΧΗ: προσθέτουμε στο vector pointers προς τοπικές μεταβλητές! Αυτό είναι ok αν το vector
	// χρησιμοποιείται ΜΟΝΟ όσο οι μεταβλητές βρίσκονται στο scope! (δλαδή μέχρι το τέλος της κλήσης της συνάρτησης)
	vector_insert(vec, &a1);
	vector_insert(vec, &a2);

	TEST_CHECK(vector_size(vec) == 2);

	int* value1 = vector_get_at(vec, 0);
	int* value2 = vector_get_at(vec, 1);

	TEST_ASSERT(*value1 == a1);
	TEST_ASSERT(*value2 == a2);

	vector_destroy(vec);
}

void integers_loop1() {
	Vector vec = vector_create(0);

	// ΠΡΟΣΟΧΗ: προσθέτουμε στο vector 100 φορές έναν pointer στο i
	// Δεν έχουμε αποθηκεύσει 100 διαφορετικές τιμές αλλά 100 ολόιδιους pointers!!
	int i;
	for(i = 0; i < 100; i++)
		vector_insert(vec, &i);

	// Διαβάζοντας οποιαδήποτε τιμή παίρνουμε πίσω έναν pointer στο i
	int* value1 = vector_get_at(vec, 0);
	int* value2 = vector_get_at(vec, 1);

	// και οι δύο pointers δείχνουν στον i
	TEST_ASSERT(value1 == &i);
	TEST_ASSERT(value2 == &i);

	// οπότε προφανώς και τα περιεχόμενά τους είναι ίδια με το i
	TEST_ASSERT(*value1 == i);
	TEST_ASSERT(*value2 == i);

	vector_destroy(vec);
}

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void integers_loop2() {
	Vector vec = vector_create(0);

	// Για να αποθηκεύσουμε 100 διαφορετικούς ακεραίους (_όχι_ όπως στο integers_loop1),
	// πρέπει κάθε φορά να δημιουργήσουμε έναν νέο ακέραιο.
	for(int i = 0; i < 100; i++)
		vector_insert(vec, create_int(i));

	int* value1 = vector_get_at(vec, 0);
	int* value2 = vector_get_at(vec, 1);

	// Οι τιμές είναι αυτές που περιμένουμε
	TEST_ASSERT(*value1 == 0);
	TEST_ASSERT(*value2 == 1);

	// Ο,τι κάναμε malloc πρέπει να το κάνουμε free, αλλιώς θα έχουμε memory leaks! (απόδειξη: make valgrind)
	while(vector_size(vec))
		free(vector_remove(vec));		// αφαίρεση στοιχείου και free, μέχρι να αδειάσει ο πίνακας

	vector_destroy(vec);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "strings", strings },
	{ "integers", integers },
	{ "integers_loop1", integers_loop1 },
	{ "integers_loop2", integers_loop2 },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};