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

// Δείκτης σε συνάρτηση που επισκέπτεται τα στoιχεία του συνόλου
typedef void (*SetVisitFunc)(Set set, Pointer value);


// Δημιουργεί και επιστρέφει ένα σύνολο, στο οποίο τα στοιχεία συγκρίνονται με βάση
// τη συνάρτηση compare.

Set set_create(CompareFunc compare);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το σύνολο set.

int set_size(Set set);

// Επιστρέφει true αν το set περιέχει τιμή ίση με value (με βάση τη compare) αλλιώς false

bool set_exists(Set set, Pointer value);

// Επιστρέφει την μονιαδική τιμή που είναι ίση με value, ή NULL αν δεν υπάρχει

Pointer set_get(Set set, Pointer value);

// Επιστρέφει την μικρότερη τιμή του συνόλου, ή NULL αν το σύνολο είναι άδειο.

Pointer set_min(Set set);

// Επιστρέφει την μεγαλύτερη τιμή του συνόλου, ή NULL αν το σύνολο είναι άδειο.

Pointer set_max(Set set);

// Επιστρέφει την προηγούμενη τιμή από τη value με βάση τη σειρά διάταξης
// (δηλαδή την τιμή max { v in set | v < value }), ή NULL αν δεν υπάρχει

Pointer set_previous(Set set, Pointer value);

// Επιστρέφει την επόμενη τιμή από τη value με βάση τη σειρά διάταξης
// (δηλαδή την τιμή min { v in set | v > value }), ή NULL αν δεν υπάρχει.

Pointer set_next(Set set, Pointer value);

// Προσθέτει την τιμή value στο σύνολο, αντικαθιστώντας τυχόν προηγούμενη τιμή ίση με value.
// Επιστρέφει true αν υπήρχε προηγούμενη ίση τιμή, αλλιώς false.

bool set_insert(Set set, Pointer value);

// Αφαιρεί τη μοναδική τιμή ίση με value από το σύνολο. Επιστρέφει την τιμή αυτή, ή NULL αν δεν υπάρχει.

Pointer set_remove(Set set, Pointer value);

// Καλεί τη visit(set, value) για κάθε στοιχείο του συνόλου, στη σειρά διάταξης.

void set_visit(Set set, SetVisitFunc visit);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το σύνολο.
// Οποιαδήποτε λειτουργία πάνω στο set μετά το destroy είναι μη ορισμένη.

void set_destroy(Set set);
