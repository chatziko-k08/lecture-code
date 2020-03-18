///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Vector μέσω Dynamic Array.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTVector.h"


// Το αρχικό μέγεθος που δεσμεύουμε
#define VECTOR_MIN_CAPACITY 10

// Ένα VectorNode είναι pointer σε αυτό το struct. (το struct περιέχει μόνο ένα στοιχείο, οπότε θα μπροούσαμε και να το αποφύγουμε, αλλά κάνει τον κώδικα απλούστερο)
struct vector_node {
	Pointer value;				// Η τιμή του κόμβου.
};

// Ενα Vector είναι pointer σε αυτό το struct
struct vector {
	VectorNode array;			// Τα δεδομένα, πίνακας από struct vector_node
	int size;					// Πόσα στοιχεία έχουμε προσθέσει
	int capacity;				// Πόσο χώρο έχουμε δεσμεύσει (το μέγεθος του array). Πάντα capacity >= size, αλλά μπορεί να έχουμε
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};


Vector vector_create(int size, DestroyFunc destroy_value) {
	// Αρχικά το vector περιέχει size μη-αρχικοποιημένα στοιχεία, αλλά εμείς δεσμεύουμε xώρο για
	// τουλάχιστον VECTOR_MIN_CAPACITY για να αποφύγουμε τα πολλαπλά resizes
	int capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;

	// Δέσμευση μνήμης, για το struct και το array.
	Vector vec = malloc(sizeof(*vec));
	VectorNode array = calloc(capacity, sizeof(*array));		// αρχικοποίηση σε 0 (NULL)

	// Είναι γενικά καλή πρακτική (ειδικά σε modules γενικής χρήσης), να ελέγχουμε αν η μνήμη δεσμεύτηκε με επιτυχία
	// LCOV_EXCL_START (αγνοούμε από το coverage report, είναι δύσκολο να τεστάρουμε αποτυχίες της malloc)
	if (vec == NULL || array == NULL) {
		free(vec);		// free αν καταφέραμε να δεσμεύσουμε κάποιο από τα δύο.
		free(array);	// Αν όχι το free(NULL) απλά δεν κάνει τίποτα.
		return VECTOR_FAIL;
	}
	// LCOV_EXCL_STOP

	vec->size = size;
	vec->capacity = capacity;
	vec->array = array;
	vec->destroy_value = destroy_value;

	return vec;
}

int vector_size(Vector vec) {
	return vec->size;
}

Pointer vector_get_at(Vector vec, int pos) {
	assert(pos >= 0 && pos < vec->size);	// LCOV_EXCL_LINE (αγνοούμε το branch από τα coverage reports, είναι δύσκολο να τεστάρουμε το false γιατί θα κρασάρει το test)
	return vec->array[pos].value;
}

void vector_set_at(Vector vec, int pos, Pointer value) {
	assert(pos >= 0 && pos < vec->size);	// LCOV_EXCL_LINE

	// Αν υπάρχει συνάρτηση destroy_value, την καλούμε για το στοιχείο που αντικαθίσταται
	if (value != vec->array[pos].value && vec->destroy_value != NULL)
		vec->destroy_value(vec->array[pos].value);

	vec->array[pos].value = value;
}

void vector_insert_last(Vector vec, Pointer value) {
	// Μεγαλώνουμε τον πίνακα (αν χρειαστεί), ώστε να χωράει τουλάχιστον size στοιχεία
	// Διπλασιάζουμε κάθε φορά το capacity (σημαντικό για την πολυπλοκότητα!)
	if (vec->capacity == vec->size) {
		VectorNode new_array = realloc(vec->array, 2 * vec->capacity * sizeof(*new_array));

		// Είναι γενικά καλή πρακτική (ειδικά σε modules γενικής χρήσης), να ελέγχουμε αν η μνήμη δεσμεύτηκε με επιτυχία
		// LCOV_EXCL_START (αγνοούμε από το coverage report, είναι δύσκολο να τεστάρουμε αποτυχίες της malloc)
		if (new_array == NULL)
			return;			// αποτυχία, επιστρέφουμε χωρίς καμία τροποποίηση στο υπάρχον vector
		// LCOV_EXCL_STOP

		// Έχουμε πλέον μεγαλύτερη μνήμη που περιέχει τα προηγούμενα περιεχόμενα.
		// Προσοχή: δε πρέπει να κάνουμε free τον παλιό pointer, το κάνει η realloc
		vec->capacity *= 2;
		vec->array = new_array;
	}

	// Μεγαλώνουμε τον πίνακα και προσθέτουμε το στοιχείο
	vec->array[vec->size].value = value;
	vec->size++;
}

void vector_remove_last(Vector vec) {
	assert(vec->size != 0);		// LCOV_EXCL_LINE

	// Αν υπάρχει συνάρτηση destroy_value, την καλούμε για το στοιχείο που αφαιρείται
	if (vec->destroy_value != NULL)
		vec->destroy_value(vec->array[vec->size - 1].value);

	// Αφαιρούμε στοιχείο οπότε ο πίνακας μικραίνει
	vec->size--;

	// Μικραίνουμε τον πίνακα αν χρειαστεί, ώστε να μην υπάρχει υπερβολική σπατάλη χώρου.
	// Για την πολυπλοκότητα είναι σημαντικό να μειώνουμε το μέγεθος στο μισό, και μόνο
	// αν το capacity είναι τετραπλάσιο του size (δηλαδή το 75% του πίνακα είναι άδειος)
	//
	if (vec->capacity > vec->size * 4 && vec->capacity > 2*VECTOR_MIN_CAPACITY) {
		VectorNode new_array = realloc(vec->array, vec->capacity/2 * sizeof(*new_array));

		// Είναι γενικά καλή πρακτική (ειδικά σε modules γενικής χρήσης), να ελέγχουμε αν η μνήμη δεσμεύτηκε με επιτυχία,
		// η realloc μπορεί να αποτύχει ακόμα και όταν μικραίνουμε τη μνήμη! Ακόμα όμως και αν αποτύχει η μείωση μνήμης,
		// μπορούμε να εκτελέσομυε κανονικά το remove, απλά θα συνεχίσουμε να δεσμεύουμε παραπάνω μνήμη (στο επόμενο remove θα ξαναπροσπαθήσουμε).
		//
		// LCOV_EXCL_START (αγνοούμε από το coverage report, είναι δύσκολο να τεστάρουμε αποτυχίες της malloc)
		if (new_array != NULL) {
			vec->capacity /= 2;
			vec->array = new_array;
		}
		// LCOV_EXCL_STOP
	}
}

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare) {
	// Διάσχιση του vector
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return vec->array[i].value;		// βρέθηκε

	return NULL;				// δεν υπάρχει
}

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value) {
	DestroyFunc old = vec->destroy_value;
	vec->destroy_value = destroy_value;
	return old;
}

void vector_destroy(Vector vec) {
	// Αν υπάρχει συνάρτηση destroy_value, την καλούμε για όλα τα στοιχεία
	if (vec->destroy_value != NULL)
		for (int i = 0; i < vec->size; i++)
			vec->destroy_value(vec->array[i].value);

	// Πρέπει να κάνουμε free τόσο τον πίνακα όσο και το struct!
	free(vec->array);
	free(vec);			// τελευταίο το vec!
}


// Συναρτήσεις για διάσχιση μέσω node /////////////////////////////////////////////////////

VectorNode vector_first(Vector vec) {
	if (vec->size == 0)
		return VECTOR_BOF;
	else	
		return &vec->array[0];
}

VectorNode vector_last(Vector vec) {
	if (vec->size == 0)
		return VECTOR_EOF;
	else
		return &vec->array[vec->size-1];
}

VectorNode vector_next(Vector vec, VectorNode node) {
	if (node == &vec->array[vec->size-1])
		return VECTOR_EOF;
	else
		return node + 1;
}

VectorNode vector_previous(Vector vec, VectorNode node) {
	if (node == &vec->array[0])
		return VECTOR_EOF;
	else
		return node - 1;
}

Pointer vector_node_value(Vector vec, VectorNode node) {
	return node->value;
}

VectorNode vector_find_node(Vector vec, Pointer value, CompareFunc compare) {
	// Διάσχιση του vector
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return &vec->array[i];		// βρέθηκε

	return VECTOR_EOF;				// δεν υπάρχει
}