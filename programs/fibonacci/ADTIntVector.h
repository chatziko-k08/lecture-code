///////////////////////////////////////////////////////////////////
//
// ADT IntVector - vector από ακεραίους
//
// Απλό specialization του ADT Vector ώστε τα στοιχεία που περιέχει
// να είναι ints.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include <limits.h>

#include "ADTVector.h"

typedef struct int_vector* IntVector;


// Οι συναρτήσεις είναι ολόιδιες με αυτές του ADTVector, documentation υπάρχει στο ADTVector.h
//
// Η μόνη διαφορά είναι ότι τα στοιχεία είναι τύπου int αντί για Pointer.
// Η συνάρτηση vector_find επιστρέφει INT_MIN αν δεν βρεθεί το στοιχείο.


IntVector int_vector_create(int size);

int int_vector_size(IntVector vec);

void int_vector_insert_last(IntVector vec, int value);

void int_vector_remove_last(IntVector vec);

void int_vector_set_at(IntVector vec, int pos, int value);

int int_vector_get_at(IntVector vec, int pos);

int int_vector_find(IntVector vec, int value);

void int_vector_destroy(IntVector vec);
