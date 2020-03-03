///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω σωρού.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTVector.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Vector

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Vector vector;				// Τα δεδομένα, σε Vector ώστε να έχουμε μεταβλητό μέγεθος χωρίς κόπο
	CompareFunc compare;		// η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};


static Pointer node_value(PriorityQueue pqueue, int node) {
	// τα node ids είναι 1-based, το node αποθηκεύεται στη θέση node - 1
	return vector_get_at(pqueue->vector, node - 1);
}

static void node_swap(PriorityQueue pqueue, int node1, int node2) {
	// τα node ids είναι 1-based, το node αποθηκεύεται στη θέση node - 1
	Pointer value1 = node_value(pqueue, node1);
	Pointer value2 = node_value(pqueue, node2);
	vector_set_at(pqueue->vector, node1 - 1, value2);
	vector_set_at(pqueue->vector, node2 - 1, value1);
}

static void heapify_up(PriorityQueue pqueue, int node) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
	if(node == 1)
		return;

	int parent = node / 2;		// Ο πατέρας του κόμβου. Τα node ids είναι 1-based

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if(pqueue->compare(node_value(pqueue, parent), node_value(pqueue, node)) < 0) {
		node_swap(pqueue, parent, node);
		heapify_up(pqueue, parent);
	}
}

static void heapify_down(PriorityQueue pqueue, int node) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	int left_child = 2 * node;
	int right_child = left_child + 1;

	int size = pqueue_size(pqueue);
	if(left_child > size)
		return;

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	int max_child = left_child;
	if(right_child <= size && pqueue->compare(node_value(pqueue, left_child), node_value(pqueue, right_child)) < 0)
			max_child = right_child;

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if(pqueue->compare(node_value(pqueue, node), node_value(pqueue, max_child)) < 0) {
		node_swap(pqueue, node, max_child);
		heapify_down(pqueue, max_child);
	}
}


// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_ΕΧCL_LINE

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	// Δημιουργία του vector που αποθηκεύει τα στοιχεία. ΔΕΝ περνάμε την destroy_value στο vector!
	// Αν την περάσουμε θα καλείται όταν κάνουμε swap 2 στοιχεία, το οποίο δεν το επιθυμούμε.
	pqueue->vector = vector_create(0, NULL);

	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return vector_size(pqueue->vector);
}

Pointer pqueue_top(PriorityQueue pqueue) {
	return node_value(pqueue, 1);		// root
}

void pqueue_insert(PriorityQueue pqueue, Pointer value) {
	// Προσθέτουμε την τιμή στο τέλος και καλούμε την heapify_up για να επισκευάσει
	// το σωρό, με όρισμα τον τελευταίο κόμβο (το 1-based id του είναι όσο το νέο μέγεθος του σωρού)
	vector_insert(pqueue->vector, value);
	heapify_up(pqueue, pqueue_size(pqueue));
}

Pointer pqueue_remove(PriorityQueue pqueue) {
	int last_node = pqueue_size(pqueue);
	if(last_node == 0)
		return NULL;		// κενός σωρός

	// Αποθήκευση πριν αφαιρεθεί, και destroy
	Pointer top = pqueue_top(pqueue);
	if(pqueue->destroy_value != NULL)
		pqueue->destroy_value(top);

	// Αντικαθιστούμε τον πρώτο κόμβο με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(pqueue, 1, last_node);
	vector_remove(pqueue->vector);

	// Επισκευή του σωρού καλώντας τη heapify_down για τη ρίζα
	heapify_down(pqueue, 1);

	return top;
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
	vector_set_destroy_value(pqueue->vector, pqueue->destroy_value);	// ώστε να κληθεί η destroy_value για όλα τα στοιχεία
	vector_destroy(pqueue->vector);
	free(pqueue);
}