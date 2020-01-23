#include "stdlib.h"

#include "ADTIntVector.h"

// Στο module αυτό υλοποιούμε ένα IntVector χρησιμοποιώντας μια **ήδη υπάρχουσα υλοποίηδη του Vector**.
// Ολες οι int_vector_* συναρτήσεις είναι απλές και απλά καλούν τις αντίστοιχες συναρτήσεις vector_*.
//
// Για να προσθέσουμε ένα int value στο vector:
// - δεσμεύουμε μνήμη με malloc
// - αντιγράφουμε εκεί το value
// - προσθέρουμε στο vector τον pointer στην μνήμη που δεσμεύσαμε.
//
// Για ευκολία χρησιμοποιούμε pointer cast, επεξήγηση υπάρχει στο modules/UsingADTList/ADTStack.h που χρησιμοποιεί την ίδια τεχνική.


IntVector int_vector_create(int size) {
	Vector vec = vector_create(size);

	// Ο πίνακας περιέχει size μη-αρχικοποιημένους ακεραίους, δεσμεύουμε μνήμη για αυτούς
	for(int i = 0; i < size; i ++)
		vector_replace(vec, i, malloc(sizeof(int)));

	return (IntVector)vec;
}

int int_vector_size(IntVector vec) {
	return vector_size((Vector)vec);	// trivial
}

void int_vector_replace(IntVector vec, int pos, int value) {
	// Αλλάζουμε την τιμή ενός υπάρχοντος στοιχείου, έχουμε ήδη μνήμη για αυτό! (από το αντίστοιχο insert)
	int *p = vector_at((Vector)vec, pos);		// το vector περιέχει int pointers
	*p = value;							// αλλαγή τιμής
}

int int_vector_at(IntVector vec, int pos) {
	int *p = vector_at((Vector)vec, pos);		// το vector περιέχει int pointers
	return *p;
}

void int_vector_insert(IntVector vec, int value) {
	// Νέο στοιχείο, χρειαζόμαστε μνήμη!
	int *p = malloc(sizeof(int));
	*p = value;
	vector_insert((Vector)vec, p);		// προσθήκη του int pointer στο vector
}

int int_vector_remove(IntVector vec) {
	// Αφαίρεση στοιχείου, πρέπει να κάνουμε free!
	int *p = vector_remove((Vector)vec);
	int value = *p;				// Αποθήκευση πριν κάνουμε free, μετά θα είναι αργά!
	free(p);
	return value;
}

int compare(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int int_vector_find(Vector vec, int value) {
	int* res = vector_find((Vector)vec, &value, compare);
	return *res;
}

void int_vector_destroy(IntVector vec) {
	// Το vector_destroy θα ελευθερώσει τη μνήμη του ίδιου του vector, αλλά τους pointers που βάλαμε
	// τους κάναμε malloc εμείς, οπότε εμείς πρέπει να τους κάνουμε και free!
	for(int i = 0; i < vector_size((Vector)vec); i++)
		free(vector_at((Vector)vec, i));

	vector_destroy((Vector)vec);
}
