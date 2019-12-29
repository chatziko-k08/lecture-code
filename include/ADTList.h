///////////////////////////////////////////////////////////////////
//
// ADT List
//
// Abstract λίστα. Παρέχει σειριακή πρόσβαση στα στοιχεία, και
// προσθήκη/αφαίρεση σε οποιοδήποτε σημείο της λίστας.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Λίστες και κόμβοι αναπαριστώνται από τους τύπους List και ListNode. Ο χρήστης δε χρειάζεται να γνωρίζει το περιεχόμενο
// των τύπων αυτών, απλά χρησιμοποιεί τις συναρτήσεις list_<foo> που δέχονται και επιστρέφουν List / ListNode.
//
// Οι τύποι αυτοί ορίζινται ως pointers στα "struct list" και "struct list_node" των οποίων το
// περιεχόμενο είναι άγνωστο (incomplete structs), και εξαρτάται από την υλοποίηση του ADT List.
//
typedef struct list* List;
typedef struct list_node* ListNode;



// Δημιουργεί και επιστρέφει μια νέα λίστα.

List list_create();

// Επιστρέψει τον αριθμό στοιχείων που περιέχει η λίστα.

int list_size(List list);

// Επιστρέφει τον πρώτο κόμβο, ή NULL αν η λίστα είναι άδεια

ListNode list_first(List list);

// Επιστρέφει τον τελευταίο κόμβο, ή NULL αν η λίστα είναι άδεια

ListNode list_last(List list);

// Επιστρέφει τον κόμβο μετά από τον node, ή NULL αν ο node είναι ο τελευταίος

ListNode list_next(List list, ListNode node);

// Επιστρέφει το περιεχόμενο του κόμβου node

Pointer list_get(List list, ListNode node);

// Αλλάζει το περιεχόμενο του κόμβου node σε value

void list_set(List list, ListNode node, Pointer value);

// Προσθέτει έναν νέο κόμβο μετά τον node, ή στην αρχή αν node == NULL, με περιεχόμενο value.

ListNode list_insert_after(List list, ListNode node, Pointer value);

// Αφαιρεί τον επόμενο κόμβο από τον node, ή τον πρώτο κόμβο αν node == NULL

void list_remove_after(List list, ListNode node);

// Επιστρέφει τον πρώτο κόμβο του οποίου η τιμή είναι ίση με value
// (με βάση τη συνάρτηση compare), ή NULL αν δε βρεθεί κανένας.

ListNode list_find(List list, Pointer value, CompareFunc compare);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η λίστα list.
// Οποιαδήποτε λειτουργία πάνω στη λίστα μετά το destroy είναι μη ορισμένη.

void list_destroy(List list);