////////////////////////////////////////////////////////////////////////
//
// ADT Set
//
// Abstract διατεταγμένο σύνολο. Τα στοιχεία είναι διατεταγμένα με βάση
// τη συνάρτηση compare, και καθένα εμφανίζεται το πολύ μία φορά.
// Παρέχεται γρήγορη αναζήτηση με ισότητα αλλά και με ανισότητα.
//
////////////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Ενα σύνολο αναπαριστάται από τον τύπο Set

typedef struct set* Set;


// Δημιουργεί και επιστρέφει ένα σύνολο, στο οποίο τα στοιχεία συγκρίνονται με βάση
// τη συνάρτηση compare.

Set set_create(CompareFunc compare);

// Επιστρέφει τη συνάρτηση compare του set.

Set set_compare(Set set);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το σύνολο set.

int set_size(Set set);

// Προσθέτει την τιμή value στο σύνολο, αντικαθιστώντας τυχόν προηγούμενη τιμή ισοδύναμη της value.
// Επιστρέφει true αν άλλαξε το μέγεθος του συνόλου (δηλαδή έγινε προσθήκη, όχι αντικατάσταση), αλλιώς false.
//
// ΠΡΟΣΟΧΗ: όσο το value είναι μέλος του set, οποιαδήποτε μεταβολή στην μνήμη που δείχνει έχει απρόβλεπτα αποτελέσματα.

bool set_insert(Set set, Pointer value);

// Αφαιρεί τη μοναδική τιμή ισοδύναμη της value από το σύνολο, αν υπάρχει.
// Επιστρέφει true αν άλλαξε το μέγεθος του συνόλου (βρέθηκε δηλαδή τιμή), αλλιώς false.

bool set_remove(Set set, Pointer value);

// Επιστρέφει true αν το set περιέχει τιμή ισοδύναμη της value (με βάση τη compare) αλλιώς false

bool set_exists(Set set, Pointer value);

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value, ή NULL αν δεν υπάρχει

Pointer set_find(Set set, Pointer value);

// Επιστρέφει τη μεγαλύτερη τιμή του set που να είναι μικρότερη από value με βάση τη σειρά διάταξης
// (δηλαδή την τιμή max { v in set | v < value }), ή NULL αν δεν υπάρχει

Pointer set_find_smaller(Set set, Pointer value);

// Επιστρέφει τη μικρότερη τιμή του set που να είναι μεγαλύτερη από value με βάση τη σειρά διάταξης
// (δηλαδή την τιμή min { v in set | v > value }), ή NULL αν δεν υπάρχει.

Pointer set_find_greater(Set set, Pointer value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το σύνολο.
// Οποιαδήποτε λειτουργία πάνω στο set μετά το destroy είναι μη ορισμένη.

void set_destroy(Set set);


// Διάσχιση του set ////////////////////////////////////////////////////////////
//
// Η διάσχιση γίνεται με τη σειρά διάταξης.

#define SET_START	(SetNode)0
#define SET_END		(SetNode)0

typedef struct set_node* SetNode;

// Επιστρέφουν τον πρώτο και τον τελευταίο κομβο του set, ή SET_START / SET_END αντίστοιχα αν το set είναι κενό

SetNode set_first(Set set);
SetNode set_last(Set set);

// Επιστρέφουν τον επόμενο και τον προηγούμενο κομβο του node, ή SET_END / SET_START
// αντίστοιχα αν ο node δεν έχει επόμενο / προηγούμενο.

SetNode set_next(Set set, SetNode node);
SetNode set_previous(Set set, SetNode node);

// Επιστρέφει το περιεχόμενο του κόμβου node

Pointer set_node_value(Set set, SetNode node);

// Βρίσκει το μοναδικό στοιχείο στο set που να είναι ίσο με value.
// Επιστρέφει τον κόμβο του στοιχείου, ή SET_END αν δεν βρεθεί.

SetNode set_find_node(Set set, Pointer value);
