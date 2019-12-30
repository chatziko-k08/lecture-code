///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Vector μέσω Dynamic Array.
//
///////////////////////////////////////////////////////////

#include "stdlib.h"
#include "assert.h"

#include "ADTVector.h"


// Το αρχικό μέγεθος που δεσμεύουμε
#define VECTOR_MIN_CAPACITY 10	

// Ενα Vector είναι pointer σε αυτό το struct
struct vector {
	Pointer* array;		// Τα δεδομένα. Θέλουμε ένα πίνακα σε Pointer, οπότε Pointer*
	int size;			// Πόσα στοιχεία έχουμε προσθέσει
	int capacity;		// Πόσο χώρο έχουμε δεσμεύσει (το μέγεθος του array). Πάντα capacity >= size, αλλά μπορεί να έχουμε
};


Vector vector_create(int size) {
	// αρχικά δημιουργούμε το struct
	Vector vec = malloc(sizeof(*vec));

	// Αρχικά το vector περιέχει size μη-αρχικοποιημένα στοιχεία, αλλά εμείς δεσμεύουμε xώρο για
	// τουλάχιστον VECTOR_MIN_CAPACITY για να αποφύγουμε τα πολλαπλά resizes
	vec->size = size;
	vec->capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;
	vec->array = malloc(vec->capacity * sizeof(Pointer));

	return vec;
}

int vector_size(Vector vec) {
	return vec->size;
}

Pointer vector_get(Vector vec, int pos) {
	// Το assert(foo) τερματίζει το πρόγραμμα αν το foo είναι false. Είναι χρήσιμο για ελέγχους που πάντα
	// πρέπει να επιτυγχάνουν κατά τη σωστή κλήση μιας συνάρτησης. Ολοι οι έλεγχοι απενεργοποιούνται αν κάνουμε
	// compile με gcc -DNDEBUG ... το οποίο είναι χρήσιμο για μεγαλύτερη ταχύτητα, αφού σιγουρευτούμε για την ορθότητα του κώδικα.
	assert(pos < vec->size);

	return vec->array[pos];
}

void vector_set(Vector vec, int pos, Pointer value) {
	assert(pos < vec->size);

	vec->array[pos] = value;
}

void vector_insert(Vector vec, Pointer value) {
	// Προσθέτουμε στοιχείο οπότε ο πίνακας μεγαλώνει
	vec->size++;

	// Μεγαλώνουμε τον πίνακα (αν χρειαστεί), ώστε να χωράει τουλάχιστον size στοιχεία
	// Διπλασιάζουμε κάθε φορά το capacity (σημαντικό για την πολυπλοκότητα!)
	if(vec->capacity < vec->size) {
		vec->capacity *= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(Pointer));
	}

	vec->array[vec->size-1] = value;
}

Pointer vector_remove(Vector vec) {
	assert(vec->size > 0);

	// Προσοχή: αποθήκευση του στοιχείου για να μη χαθεί από το realloc!
	Pointer value = vec->array[vec->size - 1];

	// Προσθέτουμε στοιχείο οπότε ο πίνακας μικραίνει
	vec->size--;

	// Μικραίνουμε τον πίνακα αν χρειαστεί, ώστε να μην υπάρχει υπερβολική σπατάλη χώρου.
	// Για την πολυπλοκότητα είναι σημαντικό να μειώνουμε το μέγεθος στο μισό, και μόνο
	// αν το capacity είναι τετραπλάσιο του size (δηλαδή το 75% του πίνακα είναι άδειος)
	//
	if(vec->capacity > vec->size * 4 && vec->capacity > 2*VECTOR_MIN_CAPACITY) {
		vec->capacity /= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(Pointer));
	}

	return value;
}

int vector_find(Vector vec, Pointer value, CompareFunc compare) {
	// Διάσχιση του vector
	for(int i = 0; i < vec->size; i++)
		if(compare(vec->array[i], value) == 0)
			return i;		// βρέθηκε

	return -1;				// δεν υπάρχει
}

void vector_destroy(Vector vec) {
	// Πρέπει να κάνουμε free τόσο τον πίνακα όσο και το struct!
	free(vec->array);
	free(vec);			// τελευταίο το vec!
}