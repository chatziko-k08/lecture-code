// Αποδοτική αναδρομική υλοποίηση της ακολουθίας fibonacci
// απομνημονεύοντας στοιχεία που έχουμε ήδη υπολογίσει.

#include <stdio.h>
#include <stdlib.h>

// Χρησιμοποιούμε ένα specialization του ADTVector που παρέχει Vectors από ακεραίους
#include "ADTIntVector.h"

IntVector memory = NULL;

int fibonacci(int n) {
	if(memory == NULL) {
		// Αρχικοποίηση της μνήμης με τα πρώτα 2 στοιχεία της ακολουθίας
		memory = int_vector_create(0);
		int_vector_insert(memory, 0);
		int_vector_insert(memory, 1);
	}

	// Αν δεν έχουμε ήδη το αποτέλεσμα το υπολογίζουμε και το αποθηκεύουμε
	if(n >= int_vector_size(memory)) {
		int res = fibonacci(n-2) + fibonacci(n-1);
		int_vector_insert(memory, res);
	}

	return int_vector_get_at(memory, n);
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		fprintf(stderr, "usage: fibonacci <n>\n");
		return 1;
	}

	int n = atoi(argv[1]);	
	printf("fibonacci(%d): %d\n", n, fibonacci(n));

	int_vector_destroy(memory);
}
