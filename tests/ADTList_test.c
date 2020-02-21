//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT List.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTList.h"


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα με NULL δείκτη συνάρτησης delete_value
	List list = list_create(NULL);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_CHECK(list != NULL);
	TEST_CHECK(list_size(list) == 0);

	list_destroy(list);
}


void test_insert(void) {

	List list = list_create(NULL);
	
	// Θα προσθέτουμε, μέσω της insert, δείκτες ως προς τα στοιχεία του π΄ίνακα
	int array[1000];					


	for(int i = 0; i < 1000; i++) {

		// LIST_BOF για εισαγωγή στην αρχή
		list_insert(list, LIST_BOF, &array[i]);
		
		// Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
		TEST_CHECK(list_size(list) == (i + 1));	

		// Ελέγχουμε εάν ο πρώτος κόμβος περιέχει σαν τιμή τον δείκτη που μόλις κάναμε insert								
		TEST_CHECK(list_node_value(list, list_first(list)) == &array[i]);	
	}

	// Ελέγχουμε εάν τα στοιχεία έχουν μπει με την αντίστροφη σειρά
	ListNode node = list_first(list);

	for(int i = 999; i >= 0; i--) {
		TEST_CHECK(list_node_value(list, node) == &array[i]);
		node = list_next(list, node);
	}

	// Εισαγωγή σε ενδιάμεσο κόμβο: προσθέτουμε το NULL σαν δεύτερο κόμβο
	ListNode first_node = list_first(list);
	list_insert(list, first_node, NULL);
	TEST_CHECK(list_node_value(list, list_next(list, first_node)) == NULL);

	list_destroy(list);
}

void test_remove(void) {

	// Δημιουργία λίστας που καλεί αυτόματα τη free σε κάθε στοιχείο που αφαιρείται
	List list = list_create(free);

	int* array[1000];

	// Χρησιμοποιούμε την insert για να γεμίσουμε την λίστα, αφού την έχουμε δοκιμάσει ήδη στην test_insert()
	for(int i = 0; i < 1000; i++) {
		
		// Δημιουργούμε δυναμικά δεσμευμένα αντικείμενα για να δοκιμάσουμε την destroy_function
		array[i]  = malloc(sizeof(int));
		*array[i] = i;
		list_insert(list, LIST_BOF, array[i]);
	}


	for(int i = 999; i >= 0; i--) {
		// Διαγράφουμε απο την αρχή και ελέγχουμε εάν η τιμή του πρώτου κόμβου 
		// ήταν η ίδια με αυτή που κάναμε insert παραπάνω
		TEST_CHECK(list_remove(list, LIST_BOF) == array[i]);

		// Ελέγχουμε ότι ενημερώνεται (μειώνεται) το size/μέγεθος της λίστας
		TEST_CHECK(list_size(list) == i);
	}

	// Ξαναγεμίζουμε την λίστα για να δοκιμάσουμε την διαγραφή απο ενδιάμεσο κόμβο
	for(int i = 0; i < 1000; i++) {
		array[i]  = malloc(sizeof(int));
		*array[i] = i;
		list_insert(list, LIST_BOF, array[i]);
	}	

	// Δοκιμάζουμε την διαγραφή κόμβων ενδιάμεσα της λίστας, και συγκεκριμένα του δεύτερου κόμβου απο την αρχή
	ListNode first_node = list_first(list);

	// Αποθηκεύουμε την τιμή του δεύτερου κόμβου και την συγκρίνουμε με αυτή 
	// που θα μας γυρίσει η list_remove μετά την διαγραφή του
	Pointer next_node_value = list_node_value(list, list_next(list, first_node));
	Pointer removed_value = list_remove(list, first_node);

	TEST_CHECK(next_node_value == removed_value);

	list_destroy(list);
}


// Σύγκριση δύο int pointers
int compare_ints(void *a, void *b) {
	return *(int*)a - *(int*)b;
}


void test_find() {
	List list = list_create(NULL);
	int array[1000];

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα , για να ελέγξουμε την test_find
	for(int i = 0; i < 1000; i++) {
		array[i] = i;
		list_insert(list, LIST_BOF, &array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for(int i = 0; i < 1000; i++) {
		int* value = list_find(list, &i, compare_ints); 
		TEST_CHECK(value == &array[i]);
	}

	// Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
	// αν η list_find γυρνάει σωστά NULL pointer
	int not_exists = -1;
	TEST_CHECK(list_find(list, &not_exists, compare_ints) == NULL);

	list_destroy(list);
}


void test_find_node() {

	List list = list_create(NULL);

	int array[1000];

	// Εισαγωγή τιμών στον πίνακα
	for(int i = 0; i < 1000; i++) {
		array[i] = i;
		list_insert(list, LIST_BOF, &array[i]);
	}

	// Ξεκινάμε από την αρχή της λίστας
	ListNode node = list_first(list);

	for(int i = 999; i >= 0; i--) {
		// Ελέγχουμε ότι η list_find_node βρίσκει σωστά τον πρώτο κόμβο με value τον δείκτη &array[i].
		// Σε αυτή την λίστα, δοκιμάζουμε ότι ο πρώτος κόμβος περιέχει τον δείκτη &array[999], 
		// o δεύτερος τον &array[998] κοκ
		ListNode found_node = list_find_node(list, &i, compare_ints); 
		TEST_CHECK(found_node == node);
		TEST_CHECK(list_node_value(list, found_node) == &array[i]);

		// Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
		node = list_next(list, node);
	}

	list_destroy(list);
}



// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "create", test_create },
	{ "insert", test_insert },
	{ "remove", test_remove },
	{ "find", test_find },
	{ "find_node", test_find_node },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};