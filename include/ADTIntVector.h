///////////////////////////////////////////////////////////////////
//
// ADT IntVector - vector από ακεραίους
//
// Απλό specialization του ADT Vector ώστε τα στοιχεία που περιέχει
// να είναι ints.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "ADTVector.h"

typedef struct int_vector* IntVector;


// Οι συναρτήσεις είναι ολόιδιες με αυτές του ADTVector, απλά τα στοιχεία είναι τύπου int αντί για Pointer
//
IntVector int_vector_create(int size);
int int_vector_size(IntVector vec);
void int_vector_set(IntVector vec, int pos, int value);
int int_vector_get(IntVector vec, int pos);
void int_vector_insert(IntVector vec, int value);
int int_vector_remove(IntVector vec);
int int_vector_find(Vector vec, int value);
void int_vector_destroy(IntVector vec);
