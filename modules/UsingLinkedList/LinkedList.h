#pragma once

// Ιδιωτικές πληροφορίες της υλοποίησης, μη ορατές στο χρήστη

#include "ADTList.h"

// Ενα List είναι pointer σε αυτό το struct
struct list {
	ListNode dummy;		// χρησιμοποιούμε dummy κόμβο, ώστε ακόμα και η κενή λίστα να έχει έναν κόμβο.
	ListNode last;		// δείκτης στον τελευταίο κόμβο, ή στον dummy (αν η λίστα είναι κενή)
	int size;			// μέγεθος, ώστε η list_size να είναι Ο(1)
};

struct list_node {
	ListNode next;		// Δείκτης στον επόμενο
	Pointer value;		// Η τιμή που αποθηκεύουμε στον κόμβο
};