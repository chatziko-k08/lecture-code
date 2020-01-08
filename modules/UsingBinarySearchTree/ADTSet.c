///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Set μέσω Binary Search Tree
//
///////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTSet.h"

// Κόμβοι του δέντρου
typedef struct bst_node* BSTNode;
struct bst_node {
	BSTNode left, right;	// Δείκτες στα παιδιά
	Pointer value;
};

// Ενα Set είναι pointer σε αυτό το struct
struct set {
	BSTNode root;			// η ρίζα, NULL αν είναι κενό δέντρο
	int size;				// μέγεθος, ώστε η set_size να είναι Ο(1)
	CompareFunc compare;	// η διάταξη
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
	if(node == NULL)
		return NULL;

	// Αν υπάρχει αριστερό υποδέντρο, η μικρότερη τιμή βρίσκεται εκεί
	// Διαφορετικά η μικρότερη τιμή είναι στο ίδιο το node
	//
	return node->left == NULL ? node : bst_find_min_node(node->left);
}

// Επιστρέφει τον μεγαλύτερο κόμβο του δέντρου με ρίζα node

BSTNode bst_find_max_node(BSTNode node) {
	if(node == NULL)
		return NULL;

	// Αν υπάρχει δεξί υποδέντρο, η μεγαλύτερη τιμή βρίσκεται εκεί
	// Διαφορετικά η μεγαλύτερη τιμή είναι στο ίδιο το node
	//
	return node->right == NULL ? node : bst_find_max_node(node->right);
}

// Επιστρέφει τον κόμβο με την προηγούμενη τιμή από τη value με βάση τη σειρά διάταξης
// (δηλαδή τη μεγαλύτερη τιμή, ανάμεσα σε αυτές που είναι αυστηρά μικρότερες της value)

BSTNode bst_find_previous_node(BSTNode node, CompareFunc compare, Pointer value) {
	if(node == NULL)
		return NULL;
	
	if(compare(value, node->value) <= 0) {
		// value <= node->value, αλλά εμείς θέλουμε τιμές αυστηρά _μικρότερες_ της value.
		// Οπότε συνεχίζουμε στο δεξί υποδέντρο.
		return bst_find_previous_node(node->left, compare, value);

	} else {
		// value > node->value, οπότε η node->value είναι ανάμεσα στις τιμές που ψάχνουμε. Αλλά
		// εμείς ψάχνουμε τη _μεγαλύτερη_ από αυτές τις τιμές. Αν υπάρχει, θα βρίσκεται
		// στο δεξί υποδέντρο, οπότε ψάχνουμε εκεί.
		BSTNode larger = bst_find_previous_node(node->right, compare, value);

		// Αν βρήκαμε κόμβο στο δεξί υποδέντρο τότε έχει σίγουρα τιμή μεγαλύτερη της node->value,
		// οπότε τον προτιμάμε. Διαφορετικά επιστρέφουμε τον node.
		return larger != NULL ? larger : node;
	}
}

// Επιστρέφει τον κόμβο με την επόμενη τιμή από τη value με βάση τη σειρά διάταξης
// (δηλαδή τη μικρότερη τιμή, ανάμεσα σε αυτές που είναι αυστηρά μεγαλύτερες της value)

BSTNode bst_find_next_node(BSTNode node, CompareFunc compare, Pointer value) {
	if(node == NULL)
		return NULL;
	
	if(compare(value, node->value) >= 0) {
		// value >= node->value, αλλά εμείς θέλουμε τιμές αυστηρά _μεγαλύτερες_ της value.
		// Οπότε συνεχίζουμε στο δεξί υποδέντρο.
		return bst_find_previous_node(node->right, compare, value);

	} else {
		// value < node->value, οπότε η node->value είναι ανάμεσα στις τιμές που ψάχνουμε. Αλλά
		// εμείς ψάχνουμε τη _μικρότερη_ από αυτές τις τιμές. Αν υπάρχει, θα βρίσκεται
		// στο αριστερό υποδέντρο, οπότε ψάχνουμε εκεί.
		BSTNode smaller = bst_find_next_node(node->left, compare, value);

		// Αν βρήκαμε κόμβο στο αριστερό υποδέντρο τότε έχει σίγουρα τιμή μικρότερη της node->value,
		// οπότε τον προτιμάμε. Διαφορετικά επιστρέφουμε τον node.
		return smaller != NULL ? smaller : node;
	}
}

// Αν υπάρχει κόμβος με τιμή ισοδύναμη της value, αλλάζει την τιμή του σε value, διαφορετικά προσθέτει
// νέο κόμβο με τιμή value. Επιστρέφει τη νέα ρίζα του υποδέντρου, και θέτει το *inserted σε true
// αν έγινε προσθήκη, ή false αν έγινε ενημέρωση.

BSTNode bst_insert(BSTNode node, CompareFunc compare, Pointer value, bool* inserted) {
	// Αν το υποδέντρο είναι κενό, δημιουργούμε νέο κόμβο ο οποίος γίνεται ρίζα του υποδέντρου
	if(node == NULL) {
		node = malloc(sizeof(*node));
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
		node->value = value;
		*inserted = false;

	} else if(compare_res < 0) {
		// value < node->value, συνεχίζουμε αριστερά. Η ρίζα του αριστερού υποδέντρου
		// ίσως αλλαξει, οπότε ενημερώνουμε το node->left με τη νέα ρίζα!
		node->left = bst_insert(node->left, compare, value, inserted);

	} else {
		// value > node->value, συνεχίζουμε αριστερά. Η ρίζα του δεξιού υποδέντρου
		// ίσως αλλαξει, οπότε ενημερώνουμε το node->right με τη νέα ρίζα!
		node->right = bst_insert(node->right, compare, value, inserted);
	}

	return node;	// η ρίζα του υποδέντρου δεν αλλάζει
}

// Διαγράφει τον μικρότερο κόμβο του υποδέντρου με ρίζα node, αφού αντιγράψει την τιμή του
// στον κόμβο copy. Επιστρέφει τη νέα ρίζα του υποδέντρου.

BSTNode bst_remove_min_node(BSTNode node, BSTNode copy) {
	if(node->left == NULL) {
		// Δεν έχουμε αριστερό υποδέντρο, οπότε ο μικρότερος είναι ο ίδιος ο node
		copy->value = node->value;		// αντιγραφή
		BSTNode right = node->right;	// αποθήκευση πριν το free
		free(node);
		return right;					// νέα ρίζα είναι το δεξιό παιδί

	} else {
		// Εχουμε αριστερό υποδέντρο, οπότε η μικρότερη τιμή είναι εκεί. Συνεχίζουμε αναδρομικά
		// και ενημερώνουμε το node->left με τη νέα ρίζα του υποδέντρου.
		node->left = bst_remove_min_node(node->left, copy);
		return node;
	}
}

// Διαγράφει το κόμβο με τιμή ισοδύναμη της value, αν υπάρχει. Επιστρέφει τη νέα ρίζα του
// υποδέντρου, και θέτει το *removed σε true αν έγινε πραγματικά διαγραφή.

BSTNode bst_remove(BSTNode node, CompareFunc compare, Pointer value, bool* removed) {
	if(node == NULL) {
		*removed = false;		// κενό υποδέντρο, δεν υπάρχει η τιμή
		return NULL;
	}

	int compare_res = compare(value, node->value);
	if(compare_res == 0) {
		// Βρέθηκε ισοδύναμη τιμή στον node, οπότε τον διαγράφουμε. Το πώς θα γίνει αυτό εξαρτάται από το αν έχει παιδιά.
		*removed = true;

		if(node->left == NULL) {
			// Δεν υπάρχει αριστερό υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το δεξί παιδί
			BSTNode right = node->right;	// αποθήκευση πριν το free!
			free(node);
			return right;

		} else if(node->right == NULL) {
			// Δεν υπάρχει δεξί υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το αριστερό παιδί
			BSTNode left = node->left;		// αποθήκευση πριν το free!
			free(node);
			return left;

		} else {
			// Υπάρχουν και τα δύο παιδιά. Αντικαθιστούμε την τιμή του node με την μικρότερη του δεξιού υποδέντρου, η οποία
			// αφαιρείται. Η συνάρτηση bst_remove_min_node κάνει ακριβώς αυτή τη δουλειά.
			node->right = bst_remove_min_node(node->right, node);
			return node;
		}
	}

	// compare_res != 0, συνεχίζουμε στο αριστερό ή δεξί υποδέντρο, η ρίζα δεν αλλάζει.
	if(compare_res < 0)
		node->left  = bst_remove(node->left,  compare, value, removed);
	else
		node->right = bst_remove(node->right, compare, value, removed);

	return node;
}

// Καλεί visit(set, value) για κάθε κόμβο του δέντρου, σε σειρά διάταξης.

void bst_visit(BSTNode node, Set set, SetVisitFunc visit) {
	if(node == NULL)
		return;

	// Επίσκεψη πρώτα αριστερά, μετά τον ίδιο τον κόμβο, και τέλος δεξιά
	bst_visit(node->left, set, visit);
	visit(set, node->value);
	bst_visit(node->right, set, visit);
}

// Καταστρέφει όλο το υποδέντρο με ρίζα node

void bst_destroy(BSTNode node) {
	if(node == NULL)
		return;
	
	// πρώτα destroy τα παιδιά, μετά free το node
	bst_destroy(node->left);
	bst_destroy(node->right);
	free(node);
}

// Επιστρέφει την τιμή του node, ή NULL αν ο ίδιος ο κόμβος είναι NULL
// Η συνάρτηση αυτή απλοποιεί αρκετές set_* συναρτήσεις.

Pointer bst_node_value(BSTNode node) {
	return node != NULL ? node->value : NULL;
}


//// Συναρτήσεις του ADT Set. Γενικά πολύ απλές, αφού καλούν τις αντίστοιχες bst_* ////////////////////////////////////////////

Set set_create(CompareFunc compare) {
	// δημιουργούμε το stuct
	Set set = malloc(sizeof(*set));
	set->root = NULL;			// κενό δέντρο
	set->size = 0;
	set->compare = compare;

	return set;
}

int set_size(Set set) {
	return set->size;
}

bool set_exists(Set set, Pointer value) {
	return bst_find_node(set->root, set->compare, value) != NULL;
}

Pointer set_get(Set set, Pointer value) {
	return bst_node_value( bst_find_node(set->root, set->compare, value) );
}

Pointer set_min(Set set) {
	return bst_node_value( bst_find_min_node(set->root) );
}

Pointer set_max(Set set) {
	return bst_node_value( bst_find_max_node(set->root) );
}

Pointer set_previous(Set set, Pointer value) {
	return bst_node_value( bst_find_previous_node(set->root, set->compare, value) );
}

Pointer set_next(Set set, Pointer value) {
	return bst_node_value( bst_find_next_node(set->root, set->compare, value) );
}

bool set_insert(Set set, Pointer value) {
	bool inserted;
	set->root = bst_insert(set->root, set->compare, value, &inserted);

	// Το size αλλάζει μόνο αν μπει νέος κόμβος, όχι στα updates!
	if(inserted)
		set->size++;

	return inserted;
}

bool set_remove(Set set, Pointer value) {
	bool removed;
	set->root = bst_remove(set->root, set->compare, value, &removed);

	// Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
	if(removed)
		set->size--;

	return removed;
}

void set_visit(Set set, SetVisitFunc visit) {
	bst_visit(set->root, set, visit);
}

void set_destroy(Set set) {
	bst_destroy(set->root);
	free(set);
}
