//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Set.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTSet.h"


//
//  Βοηθητικές συναρτήσεις 
//

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

// Δημιουργούμε μια ειδική compare συνάρτηση
int compare_ints(void *a, void *b) {
	return *(int*)a - *(int*)b;
}

// Έλεγχος της insert σε λιγότερο χώρο
void insert_and_test(Set set, Pointer value) {

	set_insert(set, value);
	TEST_CHECK(set_find(set, value) == value);
}

// Βοηθητική συνάρτηση για το ανακάτεμα του πίνακα τιμών
void shuffle(int* array[], int n) {
	if (n < 1)
		return;

	for (int i = 0; i < n; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		int* t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

//
//  Test συναρτήσεις 
//

void test_create() {

	// Δημιουργούμε ένα κενό set (χωρίς συνάρτηση αποδεύσμευσης)
	Set set = set_create(compare_ints, NULL);

	TEST_CHECK(set != NULL);
	TEST_CHECK(set_size(set) == 0);

	set_destroy(set);
}

void test_insert() {

	Set set = set_create(compare_ints, free);

	int N = 1000;

	int* value_array[N];

	// Δοκιμάζουμε την insert με νέες τιμές κάθε φορά και με αυτόματο free
	for (int i = 0; i < N; i++) {
		
		value_array[i] = create_int(i);

		insert_and_test(set, value_array[i]);

		TEST_CHECK(set_size(set) == (i + 1));

	}

	// Δοκιμάζουμε την insert με τιμές που υπάρχουν ήδη στο Set
	// και ελέγχουμε ότι δεν ενημερώθηκε το size (καθώς δεν προστέθηκε νέος κόμβος)
	int* new_value = create_int(0);
	insert_and_test(set, new_value);

	TEST_CHECK(set_size(set) == N);

	set_destroy(set);

	// Δοκιμάζουμε την insert χωρίς αυτόματο free
	Set set2 = set_create(compare_ints, NULL);
	
	int local_value1 = 0, local_value2 = 1, local_value3 = 1;

	insert_and_test(set2, &local_value1);
	insert_and_test(set2, &local_value2);
	insert_and_test(set2, &local_value3);		// ισοδύναμη τιμή => replace
	
	set_destroy(set2);

}


void test_remove() {

	Set set = set_create(compare_ints, free);

	int N = 1000;

	int* value_array[N];

	for (int i = 0; i < N; i++) {
		value_array[i] = create_int(i);
	}

	// Ανακατεύουμε το value_array ώστε να υπάρχει ομοιόμορφη εισαγωγή τιμών
	// Πχ εάν εισάγουμε δείκτες με αύξουσα σειρά τιμών, τότε εάν το Set υλοποιείται με BST,
	// οι κόμβοι θα προστίθενται μόνο δεξιά της ρίζας, άρα και η set_remove δεν θα δοκιμάζεται πλήρως
	shuffle(value_array, N);

	for (int i = 0; i < N; i++) {
		set_insert(set, value_array[i]);
	}
	

	// Δοκιμάζουμε, πριν διαγράψουμε κανονικά τους κόμβους, ότι η map_remove διαχειρίζεται 
	// σωστά ένα κλειδί που δεν υπάρχει στο Map και γυρνάει NULL 
	int not_exists = 2000;
	Pointer removed_value = set_remove(set, &not_exists);
	TEST_CHECK(removed_value == NULL);

	// Διαγράφουμε όλους τους κόμβους και ελέγχουμε εάν η τιμή που μας επιστρέφει η map_remove είναι σωστή
	for (int i = 0; i < N; i++) {

		Pointer value = set_remove(set, value_array[i]);
		TEST_CHECK(value == value_array[i]);
	}

	set_destroy(set);

	// Δοκιμάζουμε τη remove χωρίς αυτόματο free
	Set set2 = set_create(compare_ints, NULL);
	
	int local_value1 = 0;

	insert_and_test(set2, &local_value1);
	TEST_CHECK(set_remove(set2, &local_value1) == &local_value1);
	TEST_CHECK(set_size(set2) == 0);
	
	set_destroy(set2);
}


void test_find() {
	
	Set set = set_create(compare_ints, free);

	int N = 1000;

	int* value_array[N];

	for (int i = 0; i < N; i++) {
		value_array[i] = create_int(i);
	}

	// Παρόμοια με την set_remove, εάν το δέντρο δεν είναι σωστά ισορροπημένο, οι συναρτήσεις εύρεσης
	// στοιχείων δεν θα ελέγχονται πλήρως
	shuffle(value_array, N);

	for (int i = 0; i < N; i++) {

		set_insert(set, value_array[i]);

		SetNode found_node 	= set_find_node(set, value_array[i]);
		Pointer found_value = set_node_value(set, found_node);

		TEST_CHECK(found_node != SET_EOF);
		TEST_CHECK(found_value == value_array[i]);

	}

	// Αναζήτηση στοιχείου που δεν υπάρχει στο set
	int not_exists = 2000;
	TEST_CHECK(set_find_node(set, &not_exists) == SET_EOF);
	TEST_CHECK(set_find(set, &not_exists) == NULL);

	// Αναζήτηση μέγιστων/ελάχιστων στοιχείων
	// Συγκρίνουμε τις τιμές των δεικτών και όχι τους ίδιους τους δείκτες, καθώς
	// δεν γνωρίζουμε την θέση τους μετά απο το ανακάτεμα του πίνακα, αλλά γνωρίζουμε
	// ποιές τιμές υπάρχουν στο Set. Στη συγκεκριμένη περίπτωση, γνωρίζουμε ότι Set = {0, 1, ..., N-1}
	SetNode first_node = set_first(set);
	Pointer first_value = set_node_value(set, first_node);
	TEST_CHECK((*(int *)first_value) == 0);

	SetNode next = set_next(set, first_node);
	Pointer next_value = set_node_value(set, next);
	TEST_CHECK((*(int *)next_value) == 1);

	SetNode last_node = set_last(set);
	Pointer last_node_value = set_node_value(set, last_node);
	TEST_CHECK((*(int *)last_node_value) == N-1);

	SetNode prev = set_previous(set, last_node);
	Pointer prev_value = set_node_value(set, prev);
	TEST_CHECK((*(int *)prev_value) == N-2);

	// Ελέγχουμε και ότι βρίσκουμε σωστά τις τιμές από ενδιάμεσους κόμβους
	SetNode middle_node = set_find_node(set, value_array[N/2]);
	SetNode middle_node_prev = set_previous(set, middle_node);

	Pointer middle_node_value = set_node_value(set, middle_node);
	Pointer middle_node_value_prev = set_node_value(set, middle_node_prev);
	
	TEST_CHECK(*(int *)middle_node_value == *(int *)middle_node_value_prev + 1);


	set_destroy(set);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "set_create", test_create },
	{ "set_insert", test_insert },
	{ "set_remove", test_remove },
	{ "set_find", 	test_find 	},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};