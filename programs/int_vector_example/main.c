#include <stdio.h>

#include "ADTIntVector.h"


int main() {
	IntVector vec = int_vector_create(0);

	// Το IntVector επιτρέπει να προσθέτουμε εύκολα ακεραίους χωρίς malloc
	for(int i = 0; i < 100; i++)
		int_vector_insert(vec, i);

	// Κάθε στοιχείο είναι int
	for(int i = 0; i < 100; i++)
		printf("%d\n", int_vector_get_at(vec, i));

	int_vector_destroy(vec);
}
