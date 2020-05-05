///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Set μέσω B-Tree
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTSet.h"

#define MIN_CHILDREN 3  // Υλοποιούμε το Β-δέντρο ως ένα (3,5)-δέντρο
#define MAX_CHILDREN 5

#define MIN_VALUES (MIN_CHILDREN - 1)
#define MAX_VALUES (MAX_CHILDREN - 1)

typedef struct btree_node* BTreeNode;

// Υλοποιούμε τον ADT Set μέσω B-Tree, οπότε το struct set είναι ένα Β-Δέντρο.
struct set {
	BTreeNode root;             // Η ρίζα του δέντρου , NULL αν είναι κενό δέντρο.
	int size;                   // Μέγεθος, ώστε η set_size() να έχει πολυπλοκότητα Ο(1).
	CompareFunc compare;        // Διάταξη.
	DestroyFunc destroy_value;  // Συνάρτηση που καταστρέφει ένα στοιχείο του set.
};

// Κόμβος του set, περιέχει μία μόνο τιμή. Κάθε btree_node περιέχει πολλά set_nodes!
struct set_node {
	Pointer value;				// Η τιμή του κόμβου.
	BTreeNode owner;			// Ο btree_node στον οποίο ανήκει αυτό το set_node
};

// Το struct btree_node είναι ο κόμβος ενός Β-Δέντρου.
// Δεσμεύουμε MAX_CHILDREN+1 παιδιά και MAX_VALUES+1 τιμές, διότι κατά την εισαγωγή στοιχείων
// ένας κόμβος μπορεί *προσωρινά* να αποκτήσει 1 τιμή παραπάνω από τη μέγιστη.
struct btree_node {
	int count;              // Αριθμός αποθηκευμένων δεδομένων στον κόμβο.
	BTreeNode parent;       
	BTreeNode children[MAX_CHILDREN + 1];    	// Πίνακας με παιδιά.
	struct set_node set_nodes[MAX_VALUES + 1];  // Πίνακας set κόμβων (που περιέχουν τα δεδομένα).
};

static BTreeNode new_root; // Καθολική μεταβλητή για την αποθήκευση της νέας ρίζας, λόγω set_insert ή λόγω set_remove.

// Βοηθητικές συναρτήσεις
static BTreeNode node_create(Pointer value);
static BTreeNode node_find_equal(BTreeNode node, CompareFunc compare, Pointer value, int* index);

static SetNode node_find_min(BTreeNode node);
static SetNode node_find_max(BTreeNode node);
static SetNode node_find_previous(SetNode node, CompareFunc compare);
static SetNode node_find_next(SetNode node, CompareFunc compare);

static void btree_destroy(BTreeNode node, DestroyFunc destroy_value);

static bool is_leaf(BTreeNode node) {
	return (node->children[0] == NULL);
}

/* ======================================= set_remove ====================================== */

// Βοηθητικές συναρτήσεις για την set_remove
static Pointer max_btree_value(BTreeNode node, BTreeNode* mod_node);

static void rotate_right(BTreeNode node, BTreeNode sibling);
static void rotate_left(BTreeNode node, BTreeNode sibling);
static void rebalance(BTreeNode node);
static void node_remove(BTreeNode root, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value);
static void merge(BTreeNode left, BTreeNode right);

static BTreeNode get_right_sibling(BTreeNode node);
static BTreeNode get_left_sibling(BTreeNode node);

// Αν υπάρχει, επιστρέφει τον δεξιό αδερφό του κόμβου, διαφορετικά NULL.
static BTreeNode get_right_sibling(BTreeNode node) {
	BTreeNode parent = node->parent;
	if (parent != NULL && parent->children[parent->count] != node)
		for (int i = 0; i < parent->count; i++)
			if (parent->children[i] == node)
				return parent->children[i+1];

	return NULL;
}

// Αν υπάρχει, επιστρέφει τον αριστερό αδερφό του κόμβου, διαφορετικά NULL.
static BTreeNode get_left_sibling(BTreeNode node) {
	BTreeNode parent = node->parent;
	if (parent != NULL && parent->children[0] != node)
		for (int i = 1; i <= parent->count; i++)
			if (parent->children[i] == node)
				return parent->children[i-1];

	return NULL;
}

// Αναδιαμόρφωσε το υποδέντρο ώστε να ικανοποιεί τις συνθήκες ενός Β-δέντρου.
static void rebalance(BTreeNode node) {
	// Εαν δοθεί κενός ή μη-ελλιπής κόμβος ή η ρίζα, το δέντρο δε χρειάζεται αναδιαμόρφωση.
	if (node == NULL || node->count >= MIN_VALUES || node->parent == NULL)
		return;

	BTreeNode right_sibling = get_right_sibling(node);
	BTreeNode left_sibling  = get_left_sibling(node);

	// Εαν υπάρχει ο δεξιός αδερφός & έχει περισσότερα δεδομένα από τα ελάχιστα δυνατά, κάνε αριστερή περιστροφή.
	if (right_sibling != NULL && right_sibling->count > MIN_VALUES)
		rotate_left(node, right_sibling);  
	
	// Εαν υπάρχει ο αριστερός αδερφός & έχει περισσότερα δεδομένα από τα ελάχιστα δυνατά, κάνε δεξιά περιστροφή.
	else if (left_sibling != NULL && left_sibling->count > MIN_VALUES)
		rotate_right(node, left_sibling);

	// Εαν υπάρχει ο αριστερός αδερφός, συγχώνευσέ τον με τον ελλιπή κόμβο, παίρνοντας μια διαχωριστική τιμή από τον πατέρα.
	else if (left_sibling != NULL) 
		merge(left_sibling, node);

	else // Εαν υπάρχει ο δεξιός αδερφός, συγχώνευσέ τον με τον ελλιπή κόμβο, παίρνοντας μια διαχωριστική τιμή από τον πατέρα.
		merge(node, right_sibling);
}


// Δεξιά περιστροφή μεταξύ ελλιπούς κόμβου, αριστερού αδερφού & πατέρα.
static void rotate_right(BTreeNode node, BTreeNode left) {
	BTreeNode parent = node->parent;

	int sep_index = 0;     // Βρες τη θέση της διαχωριστικής τιμής στον πατέρα.
	while (parent->children[sep_index+1] != node && ++sep_index);

	// Ολίσθησε τα τις τιμές & τα παιδιά του ελλιπούς κόμβου μία θέση δεξιά.
	for (int i = node->count; i > 0; --i) {
		node->set_nodes[i].value = node->set_nodes[i-1].value;
		node->children[i+1] = node->children[i];
	}
	node->children[1] = node->children[0];

	// Αντίγραψε τη διαχωριστική τιμή από τον πατέρα στον ελλιπή κόμβο.
	node->set_nodes[0].value = parent->set_nodes[sep_index].value;

	++node->count;

	// Μετακίνησε το μεγαλύτερο στοιχείο του αριστερού αδερφού στον πατέρα, στη θέση της διαχωριστικής τιμής που μετακινήσαμε.
	parent->set_nodes[sep_index].value = left->set_nodes[left->count-1].value;

	node->children[0] = left->children[left->count];  // Μετακίνησε το μεγαλύτερο παιδί του αριστερού αδερφού στον ελλιπή κόμβο.
	if (!is_leaf(node))
		node->children[0]->parent = node;

	// Αφαίρεσε το στοιχείο που μετακινήθηκε από τον αριστερό αδερφό στον πατέρα.
	--left->count;
}


// Αριστερή περιστροφή μεταξύ ελλιπούς κόμβου, δεξιού αδερφού & πατέρα.
static void rotate_left(BTreeNode node, BTreeNode right) {
	BTreeNode parent = node->parent;

	int sep_index = 0;     // Βρες τη θέση της διαχωριστικής τιμής στον πατέρα.
	while (parent->children[sep_index] != node && ++sep_index);

	// Αντίγραψε τη διαχωριστική τιμή από τον πατέρα στον ελλιπή κόμβο.
	node->set_nodes[node->count++].value = parent->set_nodes[sep_index].value;

	// Μετακίνησε το μικρότερο στοιχείο του δεξιού αδερφού στον πατέρα, στη θέση της διαχωριστικής τιμής που μετακινήσαμε.
	parent->set_nodes[sep_index].value = right->set_nodes[0].value;

	node->children[node->count] = right->children[0];  // Μετακίνησε το μεγαλύτερο παιδί του δεξιού αδερφού στον ελλιπή κόμβο.
	if (node->children[node->count] != NULL)
		node->children[node->count]->parent = node;

	// Ολίσθησε τα δεδομένα του δεξιού αδερφού μία θέση αριστερά.
	for (int i = 0; i < right->count-1; i++) {
		right->set_nodes[i].value = right->set_nodes[i+1].value;
		right->children[i] = right->children[i+1];
	}
	right->children[right->count-1] = right->children[right->count];

	// Αφαίρεσε το στοιχείο που μετακινήθηκε από τον δεξιό αδερφό στον πατέρα.
	--right->count;
}

// Συγχωνεύει τον δεξιό κόμβο στον αριστερό, παίρνοντας τη διαχωριστική τιμή από τον πατέρα.
// Ο δεξιός κόμβος διαγράφεται.
static void merge(BTreeNode left, BTreeNode right) {

	BTreeNode parent = left->parent;

	int sep_index = 0;    // Βρες τη θέση της διαχωριστικής τιμής στον πατέρα.
	while (parent->children[sep_index] != left && ++sep_index);

	// Αντίγραψε τη διαχωριστική τιμή από τον πατέρα στον ελλιπή κόμβο.
	left->set_nodes[left->count].value = parent->set_nodes[sep_index].value;
	++left->count;

	// Εαν ο δεξιός κόμβος δεν είναι φύλλο, ενημέρωσε τον πατέρα όλων των παιδιών του.
	if (!is_leaf(right))
		for (int i = 0; i <= right->count; i++)
			right->children[i]->parent = left;

	// Αντίγραψε όλα τα δεδομένα & τα παιδιά του δεξιού κόμβου στον ελλιπή κόμβο.
	for (int i = 0; i < right->count; i++) {
		left->set_nodes[left->count].value = right->set_nodes[i].value;
		left->children[left->count++] = right->children[i];
	}
	left->children[left->count] = right->children[right->count];

	// Ολίσθησε προς τα αριστερά όλες τις τιμές και τα παιδιά του πατέρα
	// αρχίζοντας από την θέση της διαχωριστικής τιμής που αφαιρέθηκε.
	for (int i = sep_index; i < parent->count-1; i++) {
		parent->set_nodes[i].value = parent->set_nodes[i+1].value;
		parent->children[i+1] = parent->children[i+2];
	}

	--parent->count;  // Η διαχωριστική τιμή αφαιρέθηκε.
	free(right);  	  // Διάγραψε τον κόμβο που συγχωνεύτηκε.

	if (parent->count == 0) { // Εαν μετά τη διαγραφή η ρίζα έμεινε κενή.
		
		free(parent); 	  // Διάγραψε τον κόμβο ρίζας.
		left->parent = NULL;
		new_root = left;  // Ανανέωσε τη στατική μεταβλητή με τη νέα ρίζα.
	}
	else
		rebalance(parent); // Ο πατέρας μπορεί να είναι πλέον ελλιπής. Ισορρόπησε το υποδέντρο του.
}


// Διαγράφει το κόμβο με τιμή ισοδύναμη της value, αν υπάρχει.
// Θέτει το *removed σε true αν έγινε πραγματικά διαγραφή & επιστρέφει την τιμή που διαγράφηκε στο *old_value.
static void node_remove(BTreeNode root, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value) {
	if (root == NULL) {
		*removed = false;   // Κενό δέντρο, δεν υπάρχει η τιμή.
		return;
	}

	int index;    // Βρες τον κόμβο που περιέχει την τιμή.
	BTreeNode node = node_find_equal(root, compare, value, &index);

	if (node == NULL) {
		*removed = false;   // Η τιμή που θέλουμε να διαγράψουμε *δεν υπάρχει* στο δέντρο.
		return;
	}

	// Βρέθηκε ισοδύναμη τιμή στον node, οπότε τον διαγράφουμε. Το πώς θα γίνει αυτό εξαρτάται από το αν έχει παιδιά.
	*removed = true;
	*old_value = node->set_nodes[index].value;

	if (is_leaf(node)) {
		// Άν ο κόμβος είναι φύλλο, διάγραψε την τιμή, αναδιάταξε τα δεδομένα, και αναδιαμόρφωσε το δέντρο.

		for (int i = index; i < node->count-1; i++)  // Ολίσθησε όλα τα δεδομένα 1 θέση αριστερά.
			node->set_nodes[i].value = node->set_nodes[i + 1].value;
 
		--node->count;    // Αφαίρεσε το δεδομένο.

		rebalance(node);  // Αναδιαμόρφωσε το δένδρο.

	} else {  
		// Άν είναι εσωτερικός κόμβος τότε η τιμή που θέλουμε να διαγράψουμε λειτουργεί ως διαχωριστική τιμή.
		// Βρες το μεγαλύτερο στοιχείο του υποδέντρου που ορίζεται ακριβώς πριν τη διαχωριστική τιμή
		// και αντικατέστησε με αυτό τη διαχωριστική τιμή, ώστε να διατηρηθεί η διάταξη στον κόμβο.
		// Η μεγαλύτερη τιμή βρίσκεται σε φύλλο. Αφού διαγράφουμε από φύλλο, είναι πολύ πιθανό να γίνει ελλιπές.
		// Οπότε αναδιαμόρφωσε το δέντρο ξεκινώντας από το φύλλο στο οποίο έγινε η διαγραφή.
		
		BTreeNode mod_node; // Αποθήκευσε το φύλλο στο οποίο έγινε η διαγραφή.

		node->set_nodes[index].value = max_btree_value(node->children[index], &mod_node); // Αντικατέστησε τη διαχωριστική τιμή.

		--mod_node->count;    // Αφαίρεσε το δεδομένο.

		rebalance(mod_node);   // Αναδιαμόρφωσε το δέντρο.
	}
}


// Βρες και επίστρεψε τη μεγαλύτερη τιμή στο υποδέντρο που ορίζεται από τον κόμβο node.
// Επίστρεψε τον κόμβο-φύλλο στο οποίο βρίσκεται μέσω της mod_node.
static Pointer max_btree_value(BTreeNode node, BTreeNode* mod_node) {

	if (!is_leaf(node))  // Ο node είναι εσωτερικός κόμβος, οπότε ψάξε στο δεξιότερο παιδί του.
		return max_btree_value(node->children[node->count], mod_node);

	Pointer max = node->set_nodes[node->count-1].value;  // Αποθήκευσε τη μέγιστη τιμή.

	*mod_node = node; // Αποθήκευσε τον κόμβο-φύλλο.
	return max;
}

/* ================================= set_remove_end ======================================== */

/* =================================== set_insert ========================================== */

// Βοηθητικές συναρτήσεις για την set_insert
static void node_insert(BTreeNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value); // TODO : Comments
static void split(BTreeNode node, CompareFunc compare);
static void insert_at_parent(BTreeNode node, BTreeNode right, Pointer median, CompareFunc compare);

static BTreeNode split_in_half(BTreeNode node);


// Αν υπάρχει κόμβος με τιμή ισοδύναμη της value, αλλάζει την τιμή του σε value, διαφορετικά προσθέτει νέο κόμβο με τιμή value.
// Θέτει το *inserted σε true αν έγινε προσθήκη, ή false αν έγινε ενημέρωση.
// Θέτει την μεταβλητή new_root, εαν άλλαξε η ρίζα του δέντρου μετά την προσθήκη.
static void node_insert(BTreeNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value) {

	if (node == NULL) {    // Αν το υποδέντρο είναι κενό, δημιούργησε νέο κόμβο ο οποίος γίνεται ρίζα του υποδέντρου
		*inserted = true;   // Έγινε η προσθήκη
		new_root = node_create(value);
		return;
	}

	int index;
	for (index = 0; index < node->count; index++) {
		int compare_res = compare(value, node->set_nodes[index].value);

		if (compare_res == 0) { // Η ίδια τιμή υπάρχει ήδη στο δέντρο.
			*inserted = false;    
			*old_value = node->set_nodes[index].value;
			node->set_nodes[index].value = value;
			return;
		}

		if (compare_res < 0)  // Βρέθηκε κάποια τιμή που θα μπορούσε να είναι η διαχωριστική.
			break;
	}

	if (!is_leaf(node)) // Εαν ο κόμβος είναι εσωτερικός, ψάξε στο αριστερό παιδί που ορίζει η διαχωριστική τιμή.
		node_insert(node->children[index], compare, value, inserted, old_value);
	else {
		// Φτάσαμε σε φύλλο, επομένως μπορούμε να κάνουμε την προσθήκη.
		for (int i = node->count-1; i >= index; --i)
			node->set_nodes[i+1].value = node->set_nodes[i].value;  // Ολίσθησε προς τα δεξιά όλα τα στοιχεία του φύλλου αρχίζοντας από τη θέση όπου θα γίνει η προσθήκη.
	
		node->set_nodes[index].value = value;  // Κάνε την προσθήκη.
		*inserted = true;
		++node->count;

		if (node->count > MAX_VALUES) // Το φύλλο έχει περισσότερες από τις επιτρεπτές τιμές, οπότε κάνε split.
			split(node, compare);
	}
}

// Εαν ο κόμβος node έχει υπερχειλήσει, τον χωρίζει σε 2 κόμβους.
// Στέλνει τη μεσαία από τις τιμές του κόμβου node στον πατέρα του.
static void split(BTreeNode node, CompareFunc compare) {
	// Εαν ο κόμβος έχει ξεπεράσει το μέγιστο όριο τιμών.
	if (node->count > MAX_VALUES) {
		Pointer median = node->set_nodes[MAX_CHILDREN/2].value;  // Αποθήκευσε τη μέση τιμή.
		BTreeNode right = split_in_half(node);          // Χώρισε τον κόμβο στα 2.
		insert_at_parent(node, right, median, compare); // Μεταβίβασε τη μέση τιμή στον πατέρα.
	}
}

// Χωρίζει τον κόμβο node σε 2 κόμβους, με τον καθένα να έχει από MAX_CHILDREN/2 (=2) τιμές.
// Επιστρέφει τον δεξιό κόμβο που δημιούργησε.
static BTreeNode split_in_half(BTreeNode node) {
	
	BTreeNode right = node_create(NULL);
	right->parent = node->parent;     // Οι 2 κόμβοι έχουν τον ίδιο πατέρα.

	for (int i = 0; i < MAX_CHILDREN/2; i++) {
		// Μετακίνησε MAX_CHILDREN/2 τιμές και παιδιά από τον αριστερό κόμβο στον δεξιό.
		right->set_nodes[i].value = node->set_nodes[i + MAX_CHILDREN/2 + 1].value;
		right->children[i] = node->children[i + MAX_CHILDREN/2 + 1];

		if (right->children[i] != NULL)               // Εαν ο κόμβος έχει παιδιά
			right->children[i]->parent = right; // ανανέωσε τον πατέρα τους.

		++right->count; // Έγινε προσθήκη τιμής στον δεξιό κόμβο.
		--node->count;  // Και αφαίρεση στον αριστερό.
	}

	// Μετακίνησε και το τελευταίο παιδί του αριστερού κόμβου στον δεξιό.
	right->children[MAX_CHILDREN/2] = node->children[MAX_CHILDREN];
	if (right->children[MAX_CHILDREN/2] != NULL)
		right->children[MAX_CHILDREN/2]->parent = right;

	return right;
}

// Πραγματοποιεί την προσθήκη της τιμής median στον πατέρα του κόμβου node.
// Εαν ο κόμβος node είναι η ρίζα, τότε δημιουργεί και θέτει την καινούργια ρίζα στη μεταβλητή new_root.
static void insert_at_parent(BTreeNode node, BTreeNode right, Pointer median, CompareFunc compare) {
	BTreeNode parent = node->parent;

	--node->count;  // Αφαιρέσαμε την τιμή από τον node.

	if (parent == NULL){    // O node είναι η ρίζα
		new_root = node_create(median);     // Δημιούργησε καινούργια ρίζα η οποία θα έχει για παιδιά τους node, right.
		right->parent = node->parent = new_root;
		new_root->children[0] = node;
		new_root->children[1] = right;
	}
	else {
		int index;  // Βρες τη θέση εισαγωγής της τιμής στον πατέρα.
		for (index = 0; index < parent->count; index++)
			if (compare(median, parent->set_nodes[index].value) < 0)
				break;
			
		// Ολίσθησε προς τα δεξιά όλες τις τιμές και τα παιδιά του πατέρα αρχίζοντας από τη θέση όπου θα γίνει η προσθήκη.
		for (int i = parent->count-1; i >= index; --i) {
			parent->set_nodes[i+1].value = parent->set_nodes[i].value;
			parent->children[i+2] = parent->children[i+1];
		}
				
		++parent->count;        // Κάνε την προσθήκη.
		parent->set_nodes[index].value = median;
		parent->children[index+1] = right; // Πρόσθεσε τον δεξιό κόμβο που δημιουργήθηκε ως δεξιό παιδί της (νέας) διαχωριστικής τιμής.

		if (parent->count > MAX_VALUES)  // Έλεγξε εαν υπερχείλησε ο πατέρας λόγω της προσθήκης.
			split(parent, compare);
	}
}

/* ================================= set_insert_end ======================================== */

// Δημιουργεί και επιστρέφει έναν κόμβο χωρίς παιδιά ή πατέρα (όλα τα πεδία είναι NULL).
// Εαν το value *δεν* είναι NULL τότε αρχικοποιεί το values[0] του κόμβου με το value.
static BTreeNode node_create(Pointer value) {

	struct btree_node* node = calloc(1, sizeof(struct btree_node));

	if (value != NULL) {
		node->set_nodes[0].value = value;
		node->count = 1;
	}

	// Ορίζουμε τον owner σε όλα τα set_nodes που περιέχει αυτό το btree_node
	for (int i = 0; i < MAX_VALUES+1; i++)
		node->set_nodes[i].owner = node;

	return node;  // Το BTreeNode είναι struct btree_node * .
}

// Επιστρέφει τον κόμβο με τιμή ίση με value στο υποδέντρο με ρίζα node, διαφορετικά NULL.
// Αν δωθεί NULL ως index, δεν το χρησιμοποιούμε.
static BTreeNode node_find_equal(BTreeNode node, CompareFunc compare, Pointer value, int* index) {
	if (node == NULL || node->count == 0)
		return NULL;  // Κενός κόμβος, η τιμή δεν υπάρχει.

	int i;  // Ορίζει τη διαχωριστική τιμή σε σχέση με την οποιά αναζητούμε την τιμή.
	for (i = 0; i < node->count; i++) {
		int compare_res = compare(value, node->set_nodes[i].value); // Aποθήκευση για να μην καλέσουμε την compare 2 φορές.

		if (compare_res == 0) {
			if (index != NULL)
				*index = i;

			return node;  // Η τιμή βρέθηκε στον τρέχοντα κόμβο.
		}

		if (compare_res < 0)                                 // Αφού η τιμή που αναζητούμε είναι μικρότερη της διαχωριστικής τιμής
			return node_find_equal(node->children[i], compare, value, index);  // τότε βρίσκεται στο αριστερό παιδί που ορίζει η διαχωριστική τιμή.
	}
	// Η τιμή βρίσκεται στο δεξιότερο παιδί, αφού είναι μεγαλύτερη από όλες τις τιμές του κόμβου.
	return node_find_equal(node->children[node->count], compare, value, index);  
}

// Επιστρέφει τον μικρότερο set κόμβο του υποδέντρου με ρίζα node.
static SetNode node_find_min(BTreeNode node) {
	if (node == NULL)
		return NULL;

	return node->children[0] != NULL
		? node_find_min(node->children[0])		// Υπάρχει το πιο αριστερό υποδέντρο, η μικρότερη τιμή βρίσκεται εκεί.
		: &node->set_nodes[0];					// Αλλιώς ο μικρότερος set κόμβος είναι ο πρώτος σε αυτόν τον btree κόμβο
}

// Επιστρέφει τον μεγαλύτερο κόμβο του υποδέντρου με ρίζα node.
static SetNode node_find_max(BTreeNode node) {
	if (node == NULL)
		return NULL;

	return node->children[node->count] != NULL
		? node_find_max(node->children[node->count] )	// Υπάρχει το πιο δεξί υποδέντρο, η μεγαλύτερη τιμή βρίσκεται εκεί.
		: &node->set_nodes[node->count-1];				// Αλλιώς ο μεγαλύτερος set κόμβος είναι ο τελευταίος σε αυτόν τον btree κόμβο
}

// Καταστρέφει όλο το υποδέντρο με ρίζα node.
static void btree_destroy(BTreeNode node, DestroyFunc destroy_value) {
	if (node == NULL)
		return;
	
	// Πρώτα κατάστρεψε τα παιδιά.
	for (int i = 0; i <= node->count; i++)
		btree_destroy(node->children[i], destroy_value);

	if (destroy_value != NULL) {
		for (int i = 0; i < node->count; i++)
			destroy_value(node->set_nodes[i].value);  // Κατάστρεψε τις τιμές.
	}

	free(node);  // Αποδέσμευσε το node.
}


// Επιστρέφει τον προηγούμενο (στη σειρά διάταξης) του set_node,
// ή NULL αν ο node είναι ο μικρότερος του υποδέντρου.
static SetNode node_find_previous(SetNode set_node, CompareFunc compare) {  
	// Βρίσκουμε σε ποιον btree_node ανήκει ο set_node, και το index του μέσα σε αυτόν
	BTreeNode btree_node = set_node->owner;
	int index;
	for (index = 0; index < MAX_VALUES && &btree_node->set_nodes[index] != set_node; index++)
		;
	assert(index < MAX_VALUES);		// βρέθηκε

	if (!is_leaf(btree_node))								// Αν είναι εσωτερικός κόμβος, επέστρεψε τον μέγιστο κόμβο 
		return node_find_max(btree_node->children[index]);	// από το αριστερό παιδί της διαχωριστική τιμής, που είναι ο set_node.

	// Ο κόμβος είναι φύλλο.

	if (index == 0) {    			// Ο set_node είναι πρώτος μέσα στο btree node
		// Ψάξε για κάποιον πρόγονο του κόμβου που να έχει τουλάχιστον 1 τιμή μικρότερη από την set_node.
		while (btree_node->parent != NULL && compare(set_node->value, btree_node->parent->set_nodes[0].value) < 0)
			btree_node = btree_node->parent;

		if (btree_node->parent == NULL)  // Φτάσαμε στη ρίζα, οπότε ο set_node είναι η μικρότερη τιμή του δέντρου.
			return NULL;

		for (int i = btree_node->parent->count-1; i >= 0 ; --i)
			if (compare(set_node->value, btree_node->parent->set_nodes[i].value) > 0)
				return &btree_node->parent->set_nodes[i]; 		// Βρες & επέστρεψε τον set node του προγόνου, που είναι αμέσως μικρότερος από το set_node.
	}

	// Επέστρεψε τον αμέσως προηγούμενο set_node του φύλλου.
	return &btree_node->set_nodes[index-1];
}

// Επιστρέφει τον επόμενο (στη σειρά διάταξης) του κόμβου set_node,
// ή NULL αν ο node είναι ο μεγαλύτερος του υποδέντρου.
static SetNode node_find_next(SetNode set_node, CompareFunc compare) {
	// Βρίσκουμε σε ποιον btree_node ανήκει ο set_node, και το index του μέσα σε αυτόν
	BTreeNode btree_node = set_node->owner;
	int index;
	for (index = 0; index < MAX_VALUES && &btree_node->set_nodes[index] != set_node; index++)
		;
	assert(index < MAX_VALUES);		// βρέθηκε

	if (!is_leaf(btree_node))		// Αν είναι εσωτερικός κόμβος, βρες και επέστρεψε τον μικρότερο κόμβο του αντίστοιχου παιδιού.
		return node_find_min(btree_node->children[index+1]);

	// Ο κόμβος είναι φύλλο.

	if (index == btree_node->count-1) {			// Ο set_node είναι τελευταίος μέσα στο btree node
		// Ψάξε για κάποιον πρόγονο του κόμβου που να έχει τουλάχιστον 1 τιμή μεγαλύτερη από την node.
		while (btree_node->parent != NULL && compare(set_node->value, btree_node->parent->set_nodes[btree_node->parent->count-1].value) > 0)
			btree_node = btree_node->parent;

		if (btree_node->parent == NULL)			// Φτάσαμε στη ρίζα, οπότε ο set_node είναι η μεγαλύτερη τιμή του δέντρου.
			return NULL;

		for (int i = 0; i < btree_node->parent->count; i++)
			if (compare(set_node->value, btree_node->parent->set_nodes[i].value) < 0)
				return &btree_node->parent->set_nodes[i];		// Βρες & επέστρεψε την τιμή του προγόνου, που είναι αμέσως μεγαλύτερη από την set_node.
	}
	
	// Επέστρεψε τον αμέσως προηγούμενο set_node του φύλλου.
	return &btree_node->set_nodes[index+1];
}


//// Συναρτήσεις του ADT Set.

Set set_create(CompareFunc compare, DestroyFunc destroy_value) {
	assert(compare != NULL);

	Set set = malloc(sizeof(*set));
	set->root = NULL;     // Kενό δέντρο.
	set->size = 0;
	set->compare = compare;
	set->destroy_value = destroy_value;

	return set;
}

int set_size(Set set) {
	return set->size;
}

Pointer set_find(Set set, Pointer value) {
	SetNode node = set_find_node(set, value);
	return node ? node->value : NULL;
}

bool set_remove(Set set, Pointer value) {

	bool removed;
	Pointer old_value = NULL;
	
	new_root = NULL;
	node_remove(set->root, set->compare, value, &removed, &old_value);

	if (removed) {
		--set->size;    // Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος.

		if (new_root != NULL)   // Εαν προέκυψε νέα ρίζα λόγω της διαγραφής.
			set->root = new_root;   // Ανανέωσε τη ρίζα του δέντρου.

		else if (set_size(set) == 0) {   // Εαν διαγράφηκε και το τελευταίο στοιχείο της ρίζας.
			free(set->root);   	// Αποδέσμευσε τη μνήμη του κόμβου της πρώην ρίζας.
			set->root = NULL;   // Κενό δέντρο.
		}

		if (set->destroy_value != NULL)
			set->destroy_value(old_value);
	}

	return old_value != NULL;
}

SetNode set_first(Set set) {
	return node_find_min(set->root);
}

SetNode set_last(Set set) {
	return node_find_max(set->root);
}

void set_destroy(Set set) {
	btree_destroy(set->root, set->destroy_value);
	free(set);
}

SetNode set_find_node(Set set, Pointer value) {
	int index;
	BTreeNode node = node_find_equal(set->root, set->compare, value, &index);

	return node ? &node->set_nodes[index] : NULL;
}


void set_insert(Set set, Pointer value) {
	bool inserted;
	Pointer old_value;

	new_root = NULL; // Θέσε τον δείκτη της νέας ρίζας σε NULL, ώστε εαν αλλάξει, αυτό σημαίνει ότι έχουμε νέα ρίζα.
	node_insert(set->root, set->compare, value, &inserted, &old_value);

	// Το size αλλάζει μόνο αν μπει νέος κόμβος. Στα updates κάνουμε destroy την παλιά τιμή
	if (inserted) {
		++set->size;

		if (new_root != NULL)  // Η ρίζα άλλαξε
			set->root = new_root;
	}

	else if (set->destroy_value != NULL)
		set->destroy_value(old_value);
}

SetNode set_previous(Set set, SetNode node) {
	return node_find_previous(node, set->compare);
}

SetNode set_next(Set set, SetNode node) {
	return node_find_next(node, set->compare);
}

Pointer set_node_value(Set set, SetNode node) {
	return node->value;
}

DestroyFunc set_set_destroy_value(Set set, DestroyFunc destroy_value) {
	DestroyFunc old = set->destroy_value;
	set->destroy_value = destroy_value;
	return old;
}

// Συναρτήσεις που δεν υπάρχουν στο public interface αλλά χρησιμοποιούνται στα tests
// Ελέγχουν ότι το δέντρο είναι ένα σωστό AVL.

// LCOV_EXCL_START (δε μας ενδιαφέρει το coverage των test εντολών, και επιπλέον μόνο τα true branches εξετάζονται σε ένα επιτυχημένο test)

// Επιστρέφει το ύψος του υποδέντρου με ρίζα node.
// Θέτει τη μεταβλητή valid σε false εαν κάποιο παιδί του κόμβου έχει διαφορετικό ύψος από τα υπόλοιπα.
static int get_height(BTreeNode node, bool *valid) {
	if (node == NULL)
		return 0;

	const int height = 1 + get_height(node->children[0], valid);
	
	for (int i = 1; i <= node->count; i++) {
		if (height != 1 + get_height(node->children[i], valid))
			*valid = false;
	}

	return height;
}

static bool is_valid_height(BTreeNode root) {
	bool valid = true;
	get_height(root, &valid);
	return valid;
}

// Έλεγξε ότι όλα τα παιδιά του node έχουν τον ίδιο πατέρα.
static bool is_valid_parent(BTreeNode node) {
	if (node == NULL || is_leaf(node))
		return true;

	for (int i = 0; i <= node->count; i++)
		if (node != node->children[i]->parent)
			return false;

	return true;
}

static bool node_is_btree(BTreeNode node, CompareFunc compare) {
	if (node == NULL)
		return true;

	// Ο κόμβος έχει περισσότερες τιμές από όσες πρέπει.
	if (node->count > MAX_VALUES)
		return false;

	// Ο κόμβος *δεν* είναι η ρίζα και έχει λιγότερες τιμές από όσες πρέπει.
	if (node->parent != NULL && node->count < MIN_VALUES)
		return false;

	// Όλες οι τιμές του κόμβου είναι σε σωστή διάταξη.
	for (int i = 0; i < node->count-1; i++) {
		if (compare(node->set_nodes[i].value, node->set_nodes[i+1].value) >= 0)
			return false;
	}

	// Έλεγξε ότι όλα τα παιδιά του δέντρου έχουν έγκυρο ύψος.
	if (node->parent == NULL && !is_valid_height(node))
		return false;

	// Έλεγξε ότι όλα τα παιδιά του node έχουν τον ίδιο πατέρα.
	if (!is_valid_parent(node))
		return false;

	// Για όλες τις τιμές του κόμβου.
	for (int i = 0; i < node->count; i++) {

		SetNode left_max = node_find_max(node->children[i]);  // Μέγιστο παιδί αριστερού υποδέντρου.
		SetNode right_min = node_find_min(node->children[i+1]);  // Ελάχιστο παιδί δεξιού υποδέντρου.

		Pointer left_last  = (node->children[i] != NULL)  // Μεγαλύτερο στοιχείο αριστερού παιδιού.
			? node->children[i]->set_nodes[ node->children[i]->count-1 ].value
			: NULL;

		Pointer right_first = (node->children[i+1] != NULL)  // Μικρότερο στοιχείο δεξιού παιδιού.
			? node->children[i+1]->set_nodes[0].value 
			: NULL;

		Pointer val = node->set_nodes[i].value;  // Τιμή που ελέγχεται.

		bool correct = 
			(left_last   == NULL || compare(left_last, val)   < 0) &&  // Μεγαλύτερη από την μεγαλύτερη τιμή του αριστερού παιδιού.
			(right_first == NULL || compare(right_first, val) > 0) &&  // Μικρότερη από την μικρότερη του δεξιού παιδιού.
			(left_max  == NULL || compare(left_max->value, val) < 0) &&  // Μεγαλύτερη από τη μέγιστη του αριστερού υποδέντρου
			(right_min == NULL || compare(right_min->value, val) > 0) &&  // Μικρότερη από τη ελάχιστη του δεξιού υποδέντρου
			node_is_btree(node->children[i], compare);  // Έλεγξε και το αριστερό υποδέντρο.

		if (i == node->count-1)  // Εαν είναι η τελευταία διαχωριστική τιμή, έλεγξε και το δεξί υποδέντρο.
			correct = correct && node_is_btree(node->children[node->count], compare);

		if (!correct)
			return false;
	}

	return true;
}

bool set_is_proper(Set set) {
	return node_is_btree(set->root, set->compare);
}

// LCOV_EXCL_STOP