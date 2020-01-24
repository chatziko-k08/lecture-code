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

// Οι σταθερές αυτές συμβολίζουν εικονικούς κόμβους _πριν_ τον πρώτο και _μετά_ τον τελευταίο
#define LIST_BOF (ListNode)0
#define LIST_EOF (ListNode)0


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

// Προσθέτει έναν νέο κόμβο __μετά__ τον node, ή στην αρχή αν node == LIST_BOF, με περιεχόμενο value.

ListNode list_insert(List list, ListNode node, Pointer value);

// Αφαιρεί τον __επόμενο__ κόμβο από τον node, ή τον πρώτο κόμβο αν node == LIST_BOF

void list_remove(List list, ListNode node);

// Επιστρέφει την πρώτη τιμή που είναι ισοδύναμη με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν υπάρχει

Pointer list_find(List list, Pointer value, CompareFunc compare);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η λίστα list.
// Οποιαδήποτε λειτουργία πάνω στη λίστα μετά το destroy είναι μη ορισμένη.
// Αν free_values == true τότε καλείται free(value) για κάθε στοιχείο της λίστας.

void list_destroy(List list, bool free_values);


// Διάσχιση της λίστας /////////////////////////////////////////////
//
// Επιστρέφουν τον πρώτο και τον τελευταίο κομβο της λίστας, ή LIST_BOF / LIST_EOF αντίστοιχα αν η λίστα είναι κενή

ListNode list_first(List list);
ListNode list_last(List list);

// Επιστρέφει τον κόμβο μετά από τον node, ή LIST_EOF αν ο node είναι ο τελευταίος

ListNode list_next(List list, ListNode node);

// Επιστρέφει το περιεχόμενο του κόμβου node

Pointer list_node_value(List list, ListNode node);

// Βρίσκει τo πρώτo στοιχείο που είναι ισοδύναμο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τον κόμβο του στοιχείου, ή LIST_EOF αν δεν βρεθεί.

ListNode list_find_node(List list, Pointer value, CompareFunc compare);