///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT List μέσω συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include "stdlib.h"
#include "assert.h"

#include "ADTList.h"
#include "LinkedList.h"		// Ιδιωτικές πληροφορίες της υλοποίησης, μη ορατές στο χρήστη



List list_create() {
	// Πρώτα δημιουργούμε το stuct
	List list = (List)malloc(sizeof(*list));
	list->size = 0;

	// Χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και μια άδεια λίστα να έχει ένα κόμβο
	// (απλοποιεί τους αλγορίθμους). Οπότε πρέπει να τον δημιουργήσουμε.
	//
	list->dummy = (ListNode)malloc(sizeof(*list->dummy));
	list->dummy->next = NULL;		// άδεια λίστα, ο dummy δεν έχει επόμενο

	// Σε μια κενή λίστα, τελευταίος κόμβος είναι επίσης ο dummy
	list->last = list->dummy;

	return list;
}

int list_size(List list) {
	return list->size;
}

ListNode list_first(List list) {
	// Ο πρώτος κόμβος είναι ο επόμενος του dummy.
	//
	return list->dummy->next;
}

ListNode list_last(List list) {
	// Προσοχή, αν η λίστα είναι κενή το last δείχνει στον dummy, εμείς όμως θέλουμε να επιστρέψουμε NULL, όχι τον dummy!
	//
	if(list->last == list->dummy)
		return NULL;		// κενή λίστα, επιστρέφουμε NULL
	else
		return list->last;
}

ListNode list_next(List list, ListNode node) {
	assert(node != NULL);
	return node->next;
}

Pointer list_get(List list, ListNode node) {
	assert(node != NULL);
	return node->value;
}

void list_set(List list, ListNode node, Pointer value) {
	assert(node != NULL);
	node->value = value;
}

ListNode list_insert_after(List list, ListNode node, Pointer value) {
	// Αν το node είναι NULL απλά εισάγουμε μετά τον dummy κόμβο!
	// Αυτή ακριβώς είναι η αξία του dummy, δε χρειαζόμαστε ξεχωριστή υλοποίηση.
	if(node == NULL)
		node = list->dummy;

	// Δημιουργία του νέου κόμβου
	ListNode new = (ListNode)malloc(sizeof(*new));
	new->value = value;

	// Σύνδεση του new ανάμεσα στο node και το node->next
	new->next = node->next;
	node->next = new;

	// Ενημέρωση των size & last
	list->size++;
	if(list->last == node)
		list->last = new;

	return new;
}

void list_remove_after(List list, ListNode node) {
	// Αν το node είναι NULL απλά διαγράφουμε μετά τον dummy κόμβο!
	// Αυτή ακριβώς είναι η αξία του dummy, δε χρειαζόμαστε ξεχωριστή υλοποίηση.
	if(node == NULL)
		node = list->dummy;

	// Ο κόμβος προς διαγραφή είναι ο επόμενος του node, ο οποίος πρέπει να υπάρχει
	ListNode removed = node->next;
	assert(removed != NULL);

	// Σύνδεση του node με τον επόμενο του removed
	node->next = removed->next;		// πριν το free!
	free(removed);

	// Ενημέρωση των size & last
	list->size--;
	if(list->last == removed)
		list->last = node;
}

ListNode list_find(List list, Pointer value, CompareFunc compare) {
	// διάσχιση όλης της λίστας
	for(ListNode node = list->dummy->next; node != NULL; node = node->next)
		if(compare(value, node->value) == 0)
			return node;		// βρέθηκε

	return NULL;	// δεν υπάρχει
}

void list_destroy(List list) {
	ListNode node = list->dummy;		// θέλουμε να διαγράψουμε _και_ τον dummy
	while(node != NULL) {
		ListNode next = node->next;		// Αποθήκευση _πριν_ κάνουμε free!
		free(node);
		node = next;
	}
	free(list);
}