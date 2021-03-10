#include <stdlib.h>

// Χρησιμοποιούμε ένα specialization του ADTVector που παρέχει Vectors από ακεραίους
#include "ADTIntVector.h"

IntVector memory = NULL;			// int vector για την αποθήκευση των στοιχείων

// Αποδοτική αναδρομική υλοποίηση της ακολουθίας fibonacci
// απομνημονεύοντας στοιχεία που έχουμε ήδη υπολογίσει.

int fibonacci(int n) {
	if (memory == NULL) {
		// Αρχικοποίηση της μνήμης με τα πρώτα 2 στοιχεία της ακολουθίας
		memory = int_vector_create(0);
		int_vector_insert_last(memory, 0);
		int_vector_insert_last(memory, 1);
	}

	// Αν δεν έχουμε ήδη το αποτέλεσμα το υπολογίζουμε και το αποθηκεύουμε
	if (n >= int_vector_size(memory)) {
		int res = fibonacci(n-2) + fibonacci(n-1);
		int_vector_insert_last(memory, res);
	}

	return int_vector_get_at(memory, n);
}

void fibonacci_destroy(void) {
	int_vector_destroy(memory);
}