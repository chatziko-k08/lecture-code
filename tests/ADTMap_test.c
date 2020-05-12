//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Map.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"


// Δημιουργούμε μια ειδική compare συνάρτηση
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	Map map = map_create(compare_ints, NULL, NULL);
	map_set_hash_function(map, hash_int);
	map_set_destroy_key(map, NULL);
	map_set_destroy_value(map, NULL);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);
	TEST_ASSERT(map_size(map) == 0);

	map_destroy(map);
}

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

// Βοηθητική συνάρτηση, κάνει insert και ελέγχει αν έγινε η εισαγωγή
void insert_and_test(Map map, Pointer key, Pointer value) {
	map_insert(map, key, value);
	TEST_ASSERT(map_find(map, key) == value);
}

// Βοηθητική συνάρτηση για το ανακάτεμα του πίνακα τιμών
void shuffle(int* array[], int n) {
	for (int i = 0; i < n; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		int* t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

void test_insert(void) {

	Map map = map_create(compare_ints, free, free);
	map_set_hash_function(map, hash_int);

	int N = 1000;
	int** key_array = malloc(N * sizeof(*key_array));
	int** value_array = malloc(N * sizeof(*value_array));

	for (int i = 0; i < N; i++) {
		key_array[i] = create_int(i);
	}

	// Ανακατεύουμε το key_array ώστε να υπάρχει ομοιόμορφη εισαγωγή τιμών
	shuffle(key_array, N);

	// Δοκιμάζουμε την insert εισάγοντας κάθε φορά νέους κόμβους
	for (int i = 0; i < N; i++) {
		value_array[i] = create_int(i);

		// Εισαγωγή, δοκιμή και έλεγχος ότι ενημερώθηκε το size
		insert_and_test(map, key_array[i], value_array[i]);

		TEST_ASSERT(map_size(map) == (i + 1)); 
	}

	// Προσθέτουμε ένα κλειδί που είναι __ισοδύναμο__ (όχι ίσο) με το κλειδί του πρώτου κόμβο
	// Και ελέγχουμε αν και το key και το value έχουν ενημερωθεί
	int* new_key = create_int(*key_array[0]);
	int* new_value = create_int(99);

	insert_and_test(map, new_key, new_value);

	map_destroy(map);

	// Δοκιμάζουμε ότι insert/replace δουλεύει σωστά και χωρίς αυτόματο free
	Map map2 = map_create(compare_ints, NULL, NULL);
	map_set_hash_function(map2, hash_int);

	int key1 = 0, key2 = 0;
	int value1 = 0, value2 = 0;

	insert_and_test(map2, &key1, &value1);
	insert_and_test(map2, &key1, &value2);
	insert_and_test(map2, &key2, &value2);

	map_destroy(map2);
	free(key_array);
	free(value_array);
}


void test_remove(void) {

	Map map = map_create(compare_ints, free, free);
	map_set_hash_function(map, hash_int);

	int N = 1000;
	int** key_array = malloc(N * sizeof(*key_array));
	int** value_array = malloc(N * sizeof(*value_array));

	for (int i = 0; i < N; i++) {
		key_array[i] = create_int(i);
		value_array[i] = create_int(i);

		map_insert(map, key_array[i], value_array[i]);
		// Ανά τακτά χρονικά διαστήματα διαγράφουμε κάποιο κλειδί που μόλις βάλαμε
		if (i % (N / 20) == 0) 
			TEST_ASSERT(map_remove(map, key_array[i]));
	}

	// Δοκιμάζουμε, πριν διαγράψουμε κανονικά τους κόμβους, ότι η map_remove
	// διαχειρίζεται σωστά ένα κλειδί που δεν υπάρχει στο Map
	int not_exists = 2000;
	TEST_ASSERT(!map_remove(map, &not_exists));

	// Διαγράφουμε όλους τους κόμβους και ελέγχουμε εάν η τιμή που μας επιστρέφει η map_remove είναι σωστή
	for (int i = 0; i < N; i++) {
		// (Αν δεν το έχουμε διαγράψει ήδη)
		if (i % (N / 20) != 0) {
			TEST_ASSERT(map_remove(map, key_array[i]));
		}
	}
	// Ελέγχουμε την συμπεριφορά της remove σε κάτι που έχει ήδη διαγραφεί.
	int key1 = 100;
	TEST_ASSERT(!map_remove(map, &key1));
	map_destroy(map);
	free(key_array);
	free(value_array);
}


void test_find() {

	Map map = map_create(compare_ints, free, free);
	map_set_hash_function(map, hash_int);

	int N = 1000;
	int** key_array = malloc(N * sizeof(*key_array));
	int** value_array = malloc(N * sizeof(*value_array));

	for (int i = 0; i < N; i++) {
		key_array[i] = create_int(i);
		value_array[i] = create_int(i);

		map_insert(map, key_array[i], value_array[i]);
		MapNode found = map_find_node(map, key_array[i]);
		Pointer found_key = map_node_key(map, found);
		Pointer found_val = map_node_value(map, found);

		// Δοκιμάζουμε ότι ο κόμβος που μόλις κάναμε insert έχει το ίδιο Key και Value 
		TEST_ASSERT(found != MAP_EOF); 
		TEST_ASSERT(found_key == key_array[i]);
		TEST_ASSERT(found_val == value_array[i]);
	}	

	// Αναζήτηση στοιχείου που δεν υπάρχει στο map
	int not_exists = 2000;
	TEST_ASSERT(map_find_node(map, &not_exists) == MAP_EOF);
	TEST_ASSERT(map_find(map, &not_exists) == NULL);

	map_destroy(map);
	free(key_array);
	free(value_array);
}

void test_iterate() {
	Map map = map_create(compare_ints, free, free);
	map_set_hash_function(map, hash_int);
	// first σε κενό map
	TEST_ASSERT(map_first(map) == MAP_EOF);
	// Προσθέτουμε Ν ακεραίους, το value κάθε ακεραίου i είναι 2*i
	int N = 1000;
	for (int i = 0; i < N; i++)
		map_insert(map, create_int(i), create_int(2*i));

	// Ελέγχουμε ότι διατρέχοντας το map βρίσκουμε όλους τους ακεραίους από μία φορά τον καθένα
	// Στον πίνακα seen κρατάμε αν έχουμε ήδη δει τον κάθε αριθμό
	bool seen[N];
	for (int i = 0; i < N; i++)
		seen[i] = false;

	for (MapNode node = map_first(map); node != MAP_EOF; node = map_next(map, node)) {
		int* key = map_node_key(map, node);
		int* value = map_node_value(map, node);

		TEST_ASSERT(*key >= 0 && *key < N && !seen[*key]);
		TEST_ASSERT(*value == 2 * *key);

		seen[*key] = true;
	}

	map_destroy(map);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	// { "create", test_create },

	{ "map_create", test_create },
	{ "map_insert", test_insert },
	{ "map_remove", test_remove },
	{ "map_find", 	test_find },
	{ "map_iterate",test_iterate },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 