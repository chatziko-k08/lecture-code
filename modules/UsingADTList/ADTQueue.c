///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT List (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include "stdlib.h"
#include "assert.h"

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
//    Το cast δεν αλλάζει την τιμή του pointer
//
// Στον παρακάτω κώδικα επιλέγουμε την πρώτη λύση (δες το ADTStack.c για τη δεύτερη).

struct queue {
	List list;
};


Queue queue_create() {
	// Φτιάχνουμε ένα struct και αποθηκεύουμε μέσα μια νέα λίστα
	//
	Queue queue = (Queue)malloc(sizeof(*queue));
	queue->list = list_create();
	return queue;
}

int queue_size(Queue queue) {
	return list_size(queue->list);
}

Pointer queue_top(Queue queue) {
	return list_get(queue->list, list_first(queue->list));
}

void queue_insert(Queue queue, Pointer value) {
	list_insert_after(queue->list, list_last(queue->list), value);		// Προσθήκη στο _τέλος_
}

Pointer queue_remove(Queue queue) {
	Pointer result = queue_top(queue);	// Αποθήκευση πριν κάνουμε remove
	list_remove_after(queue->list, NULL);
	return result;
}

void queue_destroy(Queue queue) {
	// Τη μνήμη της λίστας την κάνει free η list_destroy, αλλά το struct που περιέχει
	// τη λίστα το δημιουργήσαμε εμείς, οπότε πρέπει εμείς να το κάνουμε free.
	//
	list_destroy(queue->list);
	free(queue);
}
