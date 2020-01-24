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
	Pointer* array;				// Τα δεδομένα. Θέλουμε ένα πίνακα σε Pointer, οπότε Pointer*
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
	Pointer* array = malloc(capacity * sizeof(*array));

	// Είναι γενικά καλή πρακτική (ειδικά σε modules γενικής χρήσης), να ελέγχουμε αν η μνήμη δεσμεύτηκε με επιτυχία
	// LCOV_EXCL_START (αγνοούμε από το coverage report, είναι δύσκολο να τεστάρουμε αποτυχίες της malloc)
	if(vec == NULL || array == NULL) {
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
	if(pos >= 0 && pos < vec->size)
		return vec->array[pos];
	else
		return NULL;
	
}

void vector_set_at(Vector vec, int pos, Pointer value) {
	assert(pos >= 0 && pos < vec->size);	// LCOV_EXCL_LINE (αγνοούμε το branch από τα coverage reports, είναι δύσκολο να τεστάρουμε το false γιατί θα κρασάρει το test)

	vec->array[pos] = value;
}

bool vector_insert(Vector vec, Pointer value) {
	// Μεγαλώνουμε τον πίνακα (αν χρειαστεί), ώστε να χωράει τουλάχιστον size στοιχεία
	// Διπλασιάζουμε κάθε φορά το capacity (σημαντικό για την πολυπλοκότητα!)
	if(vec->capacity == vec->size) {
		vec->capacity *= 2;
		Pointer new_array = realloc(vec->array, vec->capacity * sizeof(Pointer));

		// Είναι γενικά καλή πρακτική (ειδικά σε modules γενικής χρήσης), να ελέγχουμε αν η μνήμη δεσμεύτηκε με επιτυχία
		// LCOV_EXCL_START (αγνοούμε από το coverage report, είναι δύσκολο να τεστάρουμε αποτυχίες της malloc)
		if(new_array == NULL)
			return false;			// αποτυχία, επιστρέφουμε χωρίς καμία τροποποίηση στο υπάρχον vector
		// LCOV_EXCL_STOP

		// Έχουμε πλέον μεγαλύτερη μνήμη που περιέχει τα προηγούμενα περιεχόμενα.
		// Προσοχή: δε πρέπει να κάνουμε free τον παλιό pointer, το κάνει η realloc
		vec->array = new_array;
	}

	// Μεγαλώνουμε τον πίνακα και προσθέτουμε το στοιχείο
	vec->array[vec->size] = value;
	vec->size++;

	return true;
}

Pointer vector_remove(Vector vec) {
	if(vec->size == 0)
		return NULL;

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

	// Αν υπάρχει συνάρτηση destroy_value, την καλούμε για το στοιχείο που αφαιρείται
	if(vec->destroy_value != NULL)
		vec->destroy_value(value);

	return value;
}

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare) {
	// Διάσχιση του vector
	for(int i = 0; i < vec->size; i++)
		if(compare(vec->array[i], value) == 0)
			return vec->array[i];		// βρέθηκε

	return NULL;				// δεν υπάρχει
}

void vector_destroy(Vector vec) {
	// Αν υπάρχει συνάρτηση destroy_value, την καλούμε για όλα τα στοιχεία
	if(vec->destroy_value != NULL)
		for(int i = 0; i < vec->size; i++)
			vec->destroy_value(vec->array[i]);

	// Πρέπει να κάνουμε free τόσο τον πίνακα όσο και το struct!
	free(vec->array);
	free(vec);			// τελευταίο το vec!
}


// Συναρτήσεις για διάσχιση μέσω node /////////////////////////////////////////////////////

VectorNode vector_first(Vector vec) {
	if(vec->size == 0)
		return VECTOR_BOF;
		
	Pointer* p = &vec->array[0];
	return (VectorNode)p;
}

VectorNode vector_last(Vector vec) {
	if(vec->size == 0)
		return VECTOR_EOF;

	Pointer* p = &vec->array[vec->size-1];
	return (VectorNode)p;
}

VectorNode vector_next(Vector vec, VectorNode node) {
	Pointer* p = (Pointer*)node;

	if(p == &vec->array[vec->size-1])
		return VECTOR_EOF;
	else
		return (VectorNode)(p + 1);
}

VectorNode vector_previous(Vector vec, VectorNode node) {
	Pointer* p = (Pointer*)node;

	if(p == &vec->array[0])
		return VECTOR_EOF;
	else
		return (VectorNode)(p - 1);
}

Pointer vector_node_value(Vector vec, VectorNode node) {
	Pointer* p = (Pointer*)node;
	return *p;
}