#include <stdlib.h>

#include "ADTMap.h"
#include "pair_sum.h"


int compare_ints(int* a, int* b) {
	return *a - *b;
}

bool pair_sum(int target, Vector numbers, int* res_a, int* res_b) {
	// Στο map θα αποθηκεύουμε τα στοιχεία του vector. Δεν κάνουμε malloc
	// οπότε δε θέλουμε και free, εμείς αποθηκεύουμε στοιχεία που έχουν δημιουργηθεί αλλού!
	Map seen = map_create((CompareFunc)compare_ints, NULL, NULL);
	bool result = false;

	int size = vector_size(numbers);
	for (int i = 0; i < size; i++) {
		int* a = vector_get_at(numbers, i);
		int b = target - *a;

		if (map_find(seen, &b) != NULL) {
			*res_a = *a;
			*res_b = b;
			result = true;
			break;
		}

		map_insert(seen, a, a);		// Για value θέλουμε μια οποιαδήποτε μη-NULL τιμή, οπότε το θέτουμε και αυτό ίσο με a
	}

	map_destroy(seen);
	return result;
}