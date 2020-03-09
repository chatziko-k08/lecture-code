///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT List (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTList.h"
#include "ADTQueue.h"


// Μια ουρά είναι απλά μια λίστα, αλλά το Queue (pointer σε struct queue) είναι διαφορετικός pointer
// από το List (pointer σε struct list). Εχουμε 2 επιλογές:
//
// 1. (Η πιο "καθαρή") Να φτιάξουμε το struct queue ως εξής
//		struct queue {
//			List list;
//		};
//    και στο πεδίο "list" να αποθηκεύσουμε τη λίστα.
//
// 2. (Η πιο "άμεση"). Να χρησιμοποιούμε απευθείας ένα List ως Queue, κάνοντας cast τον pointer.
//    Το cast δεν αλλάζει την τιμή του pointer, απλά επιτρέχει την ανάθεσή του σε μεταβλητή διαφορετικού τύπου.
//
// Στον παρακάτω κώδικα επιλέγουμε την πρώτη λύση (δες το ADTStack.c για τη δεύτερη).

struct queue {
	List list;
};


Queue queue_create(DestroyFunc destroy_value) {
	// Φτιάχνουμε ένα struct και αποθηκεύουμε μέσα μια νέα λίστα
	//
	Queue queue = malloc(sizeof(*queue));
	queue->list = list_create(destroy_value);
	return queue;
}

int queue_size(Queue queue) {
	return list_size(queue->list);
}

Pointer queue_front(Queue queue) {
	return list_node_value(queue->list, list_first(queue->list));
}

Pointer queue_back(Queue queue) {
	return list_node_value(queue->list, list_last(queue->list));
}

void queue_insert_back(Queue queue, Pointer value) {
	list_insert_next(queue->list, list_last(queue->list), value);		// Προσθήκη στο _τέλος_
}

void queue_remove_front(Queue queue) {
	list_remove_next(queue->list, NULL);
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	return list_set_destroy_value(queue->list, destroy_value);
}

void queue_destroy(Queue queue) {
	// Τη μνήμη της λίστας την κάνει free η list_destroy, αλλά το struct που περιέχει
	// τη λίστα το δημιουργήσαμε εμείς, οπότε πρέπει εμείς να το κάνουμε free.
	//
	list_destroy(queue->list);
	free(queue);
}
