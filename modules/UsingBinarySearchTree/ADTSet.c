///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Set μέσω Binary Search Tree (BST)
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTSet.h"

// Κόμβοι του δέντρου, ταυτίζονται με τους κόμβους του set
typedef SetNode BSTNode;		// Ξεχωριστό typedef για να είναι πιο σαφές ότι έχουμε κόμβους BST

struct set_node {				// Ενα SetNode είναι pointer σε αυτό το struct
	BSTNode parent;				// Πατέρας
	BSTNode left, right;		// Παιδιά
	Pointer value;
};

// Ενα Set είναι pointer σε αυτό το struct
struct set {
	BSTNode root;				// η ρίζα, NULL αν είναι κενό δέντρο
	int size;					// μέγεθος, ώστε η set_size να είναι Ο(1)
	CompareFunc compare;		// η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του set
};


// Οι bst_* συναρτήσεις είναι βοηθητικές (κρυφές από το χρήστη) και υλοποιούν τις διάφορες λειτουργίες του Δυαδικού Δέντρου Αναζήτησης.
// Οι set_* συναρτήσεις (πιο μετά στο αρχείο), υλοποιούν τις συναρτήσεις του ADT Set, και είναι απλές, καλώντας τις αντίστοιχες bst_*.
//
// Παρατηρήσεις για τις bst_* συναρτήσεις
// - είναι αναδρομικές, η αναδρομή είναι γενικά πολύ βοηθητική στα δέντρα.
// - όσες συναρτήσεις _τροποποιούν_ το δέντρο, ουσιαστικά ενεργούν στο _υποδέντρο_ με ρίζα τον κόμβο node, και επιστρέφουν τη νέα
//   ρίζα του υποδέντρου μετά την τροποποίηση. Η νέα ρίζα χρησιμοποιείται από την προηγούμενη αναδρομική κλήση.

// Επιστρέφει τον κόμβο με τιμή ίση με value, διαφορετικά NULL

BSTNode bst_find_node(BSTNode node, CompareFunc compare, Pointer value) {
	// κενό υποδέντρο, δεν υπάρχει η τιμή
	if(node == NULL)
		return NULL;
	
	// Το που βρίσκεται ο κόμβος που ψάχνουμε εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβο (node->value)
	//
	int compare_res = compare(value, node->value);			// αποθήκευση για να μην καλέσουμε την compare 2 φορές
	if(compare_res == 0)									// value ισοδύναμη της node->value, βρήκαμε τον κόμβο
		return node;
	else if(compare_res < 0)								// value < node->value, ο κόμβος που ψάχνουμε είναι στο αριστερό υποδέντρο
		return bst_find_node(node->left, compare, value);
	else													// value > node->value, ο κόμβος που ψάχνουμε είνια στο δεξιό υποδέντρο
		return bst_find_node(node->right, compare, value);
}

// Επιστρέφει τον μικρότερο κόμβο του δέντρου με ρίζα node

BSTNode bst_find_min_node(BSTNode node) {
	// Αν υπάρχει αριστερό υποδέντρο, η μικρότερη τιμή βρίσκεται εκεί
	// Διαφορετικά η μικρότερη τιμή είναι στο ίδιο το node
	//
	return node == NULL || node->left == NULL ? node : bst_find_min_node(node->left);
}

// Επιστρέφει τον μεγαλύτερο κόμβο του δέντρου με ρίζα node

BSTNode bst_find_max_node(BSTNode node) {
	// Αν υπάρχει δεξί υποδέντρο, η μεγαλύτερη τιμή βρίσκεται εκεί
	// Διαφορετικά η μεγαλύτερη τιμή είναι στο ίδιο το node
	//
	return node == NULL || node->right == NULL ? node : bst_find_max_node(node->right);
}

// Επιστρέφει τον προηγούμενο του node στη σειρά διάταξης, ή NULL αν ο node είναι ο μικρότερος όλου του δέντρου

BSTNode bst_find_previous_node(BSTNode node, CompareFunc compare) {
	// Αν έχουμε αριστερό παιδί τότε όλο το αριστερό υποδέντρο είναι μικρότεροι κόμβοι.
	// Ο πρηγούμενος είναι ο μεγαλύτερος από αυτούς.
	if(node->left != NULL)
		return bst_find_max_node(node->left);

	// Δεν έχουμε αριστερό παιδί, μπορεί όμως να υπάρχουν μικρότεροι κόμβοι σε άλλα σημεία του υποδέντρου.
	// Προχωράμε προς τη ρίζα, ψάχνοντας για κόμβο που να είναι _δεξί_ παιδί του πατέρα του, άρα ο πατέρας
	// είναι ο ακριβώς προγούμενός του.
	for(; node != NULL; node = node->parent)
		if(node->parent != NULL && node->parent->right == node)
			return node->parent;

	// φτάσαμε στη ρίζα ακολουθώντας μόνο _αριστερά_ links, άρα είμαστε ο αριστερότερος (μικρότερος) κόμβος όλου του δέντρου!
	return NULL;
}

// Επιστρέφει τον επόμενο του node στη σειρά διάταξης, ή NULL αν ο node είναι ο μεγαλύτερος όλου του δέντρου

BSTNode bst_find_next_node(BSTNode node, CompareFunc compare) {
	// Αν έχουμε δεξί παιδί τότε όλο το δεξί υποδέντρο είναι μικρότεροι κόμβοι.
	// Ο επόμενος είναι ο μεγαλύτερος από αυτούς.
	if(node->right != NULL)
		return bst_find_min_node(node->right);

	// Δεν έχουμε δεξί παιδί, μπορεί όμως να υπάρχουν μεγαλύτεροι κόμβοι σε άλλα σημεία του υποδέντρου.
	// Προχωράμε προς τη ρίζα, ψάχνοντας για κόμβο που να είναι _αριστερό_ παιδί του πατέρα του, άρα ο πατέρας
	// είναι ο ακριβώς επόμενός του.
	for(; node != NULL; node = node->parent)
		if(node->parent != NULL && node->parent->left == node)
			return node->parent;

	// φτάσαμε στη ρίζα ακολουθώντας μόνο _δεξιά_ links, άρα είμαστε ο δεξιότερος (μεγαλύτερος) κόμβος όλου του δέντρου!
	return NULL;
}

// Αν υπάρχει κόμβος με τιμή ισοδύναμη της value, αλλάζει την τιμή του σε value, διαφορετικά προσθέτει
// νέο κόμβο με τιμή value. Επιστρέφει τη νέα ρίζα του υποδέντρου, και θέτει το *inserted σε true
// αν έγινε προσθήκη, ή false αν έγινε ενημέρωση.

BSTNode bst_insert(BSTNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value) {
	// Αν το υποδέντρο είναι κενό, δημιουργούμε νέο κόμβο ο οποίος γίνεται ρίζα του υποδέντρου
	if(node == NULL) {
		node = malloc(sizeof(*node));
		node->parent = NULL;	// ρίζα, για την ώρα δεν έχει πατέρα. Μπορεί να αποκτήσει αν το υποδέντρο τοποθετηθεί ως παιδί άλλου δέντρου
		node->left = node->right = NULL;
		node->value = value;

		*inserted = true;		// κάναμε προσθήκη
		return node;			// η νέα ρίζα του υποδέντρου είναι ο νέος κόμβος
	}

	// Το που θα γίνει η προσθήκη εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβου (node->value)
	//
	int compare_res = compare(value, node->value);
	if(compare_res == 0) {
		// βρήκαμε ισοδύναμη τιμή, κάνουμε update
		*inserted = false;
		*old_value = node->value;
		node->value = value;

	} else if(compare_res < 0) {
		// value < node->value, συνεχίζουμε αριστερά. Η ρίζα του αριστερού υποδέντρου
		// ίσως αλλαξει, οπότε ενημερώνουμε το node->left με τη νέα ρίζα!
		node->left = bst_insert(node->left, compare, value, inserted, old_value);
		node->left->parent = node;

	} else {
		// value > node->value, συνεχίζουμε αριστερά. Η ρίζα του δεξιού υποδέντρου
		// ίσως αλλαξει, οπότε ενημερώνουμε το node->right με τη νέα ρίζα!
		node->right = bst_insert(node->right, compare, value, inserted, old_value);
		node->right->parent = node;
	}

	return node;	// η ρίζα του υποδέντρου δεν αλλάζει
}

// Αφαιρεί και αποθηκεύει στο min_node τον μικρότερο κόμβο του υποδέντρου με ρίζα node.
// Επιστρέφει τη νέα ρίζα του υποδέντρου.

BSTNode bst_remove_min_node(BSTNode node, BSTNode* min_node) {
	if(node->left == NULL) {
		// Δεν έχουμε αριστερό υποδέντρο, οπότε ο μικρότερος είναι ο ίδιος ο node
		*min_node = node;

		BSTNode right = node->right;	// αποθήκευση πριν το free
		if(right != NULL)
			right->parent = node->parent;

		return right;					// νέα ρίζα είναι το δεξιό παιδί

	} else {
		// Εχουμε αριστερό υποδέντρο, οπότε η μικρότερη τιμή είναι εκεί. Συνεχίζουμε αναδρομικά
		// και ενημερώνουμε το node->left με τη νέα ρίζα του υποδέντρου.
		node->left = bst_remove_min_node(node->left, min_node);
		return node;
	}
}

// Διαγράφει το κόμβο με τιμή ισοδύναμη της value, αν υπάρχει. Επιστρέφει τη νέα ρίζα του
// υποδέντρου, και θέτει το *removed σε true αν έγινε πραγματικά διαγραφή.

BSTNode bst_remove(BSTNode node, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value) {
	if(node == NULL) {
		*removed = false;		// κενό υποδέντρο, δεν υπάρχει η τιμή
		return NULL;
	}

	int compare_res = compare(value, node->value);
	if(compare_res == 0) {
		// Βρέθηκε ισοδύναμη τιμή στον node, οπότε τον διαγράφουμε. Το πώς θα γίνει αυτό εξαρτάται από το αν έχει παιδιά.
		*removed = true;
		*old_value = node->value;

		if(node->left == NULL) {
			// Δεν υπάρχει αριστερό υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το δεξί παιδί
			BSTNode right = node->right;	// αποθήκευση πριν το free!
			if(right != NULL)
				right->parent = node->parent;

			free(node);
			return right;

		} else if(node->right == NULL) {
			// Δεν υπάρχει δεξί υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το αριστερό παιδί
			BSTNode left = node->left;		// αποθήκευση πριν το free!
			if(left != NULL)
				left->parent = node->parent;

			free(node);
			return left;

		} else {
			// Υπάρχουν και τα δύο παιδιά. Αντικαθιστούμε την τιμή του node με την μικρότερη του δεξιού υποδέντρου, η οποία
			// αφαιρείται. Η συνάρτηση bst_remove_min_node κάνει ακριβώς αυτή τη δουλειά.

			BSTNode min_right;
			node->right = bst_remove_min_node(node->right, &min_right);

			// Σύνδεση του min_right στη θέση του node
			min_right->parent = node->parent;
			min_right->left = node->left;
			min_right->right = node->right;

			free(node);
			return min_right;
		}
	}

	// compare_res != 0, συνεχίζουμε στο αριστερό ή δεξί υποδέντρο, η ρίζα δεν αλλάζει.
	if(compare_res < 0)
		node->left  = bst_remove(node->left,  compare, value, removed, old_value);
	else
		node->right = bst_remove(node->right, compare, value, removed, old_value);

	return node;
}

// Καταστρέφει όλο το υποδέντρο με ρίζα node

void bst_destroy(BSTNode node, DestroyFunc destroy_value) {
	if(node == NULL)
		return;
	
	// πρώτα destroy τα παιδιά, μετά free το node
	bst_destroy(node->left, destroy_value);
	bst_destroy(node->right, destroy_value);

	if(destroy_value != NULL)
		destroy_value(node->value);

	free(node);
}


//// Συναρτήσεις του ADT Set. Γενικά πολύ απλές, αφού καλούν τις αντίστοιχες bst_* ////////////////////////////////////////////

Set set_create(CompareFunc compare, DestroyFunc destroy_value) {
	// δημιουργούμε το stuct
	Set set = malloc(sizeof(*set));
	set->root = NULL;			// κενό δέντρο
	set->size = 0;
	set->compare = compare;
	set->destroy_value = destroy_value;

	return set;
}

int set_size(Set set) {
	return set->size;
}

void set_insert(Set set, Pointer value) {
	bool inserted;
	Pointer old_value;
	set->root = bst_insert(set->root, set->compare, value, &inserted, &old_value);

	// Το size αλλάζει μόνο αν μπει νέος κόμβος. Στα updates κάνουμε destroy την παλιά τιμή
	if(inserted)
		set->size++;
	else if(set->destroy_value != NULL)
		set->destroy_value(old_value);
}

Pointer set_remove(Set set, Pointer value) {
	bool removed;
	Pointer old_value = NULL;
	set->root = bst_remove(set->root, set->compare, value, &removed, &old_value);

	// Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
	if(removed) {
		set->size--;

		if(set->destroy_value != NULL)
			set->destroy_value(old_value);
	}

	return old_value;
}

Pointer set_find(Set set, Pointer value) {
	BSTNode node = bst_find_node(set->root, set->compare, value);
	return node == NULL ? NULL : node->value;
}

void set_destroy(Set set) {
	bst_destroy(set->root, set->destroy_value);
	free(set);
}

SetNode set_first(Set set) {
	return bst_find_min_node(set->root);
}

SetNode set_last(Set set) {
	return bst_find_max_node(set->root);
}

SetNode set_previous(Set set, SetNode node) {
	// TODO: improve
	return bst_find_previous_node(node, set->compare);
}

SetNode set_next(Set set, SetNode node) {
	return bst_find_next_node(node, set->compare);
}

Pointer set_node_value(Set set, SetNode node) {
	return node->value;
}

SetNode set_find_node(Set set, Pointer value) {
	return bst_find_node(set->root, set->compare, value);
}