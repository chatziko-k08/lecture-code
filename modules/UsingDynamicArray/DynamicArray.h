#pragma once

// Ιδιωτικές πληροφορίες της υλοποίησης, μη ορατές στο χρήστη

// Το αρχικό μέγεθος που δεσμεύουμε
#define VECTOR_MIN_CAPACITY 10	

// Ενα Vector είναι pointer σε αυτό το struct
struct vector {
	Pointer* array;		// Τα δεδομένα. Θέλουμε ένα πίνακα σε Pointer, οπότε Pointer*
	int size;			// Πόσα στοιχεία έχουμε προσθέσει
	int capacity;		// Πόσο χώρο έχουμε δεσμεύσει (το μέγεθος του array). Πάντα capacity >= size, αλλά μπορεί να έχουμε
};