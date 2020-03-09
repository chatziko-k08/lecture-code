///////////////////////////////////////////////////////////////////
//
// ADT Stack
//
// Abstract στοίβα. Λειτουργία "First In Last Out", δηλαδή σε κάθε
// remove αφαιρείται το τελευταίο στοιχείο που έγινε insert.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Μία στοίβα αναπαριστάται από τον τύπο Stack. Ο χρήστης δε χρειάζεται να γνωρίζει το περιεχόμενο
// του τύπου αυτού, απλά χρησιμοποιεί τις συναρτήσεις stack_<foo> που δέχονται και επιστρέφουν Stack.
//
// Ο τύπος Stack ορίζεται ως pointer στο "struct stack" του οποίου το περιεχόμενο είναι άγνωστο
// (incomplete struct), και εξαρτάται από την υλοποίηση του ADT Stack.
//
typedef struct stack* Stack;


// Δημιουργεί και επιστρέφει μια νέα στοίβα.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.

Stack stack_create(DestroyFunc destroy_value);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η στοίβα stack

int stack_size(Stack stack);

// Επιστρέφει το στοιχείο στην κορυφή της στοίβας (μη ορισμένο αποτέλεσμα αν η στοίβα είναι κενή)

Pointer stack_top(Stack stack);

// Προσθέτει την τιμή value στην κορυφή της στοίβας stack.

void stack_insert_top(Stack stack, Pointer value);

// Αφαιρεί την τιμή στην κορυφή της στοίβας (μη ορισμένο αποτέλεσμα αν η στοίβα είναι κενή)

void stack_remove_top(Stack stack);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc stack_set_destroy_value(Stack stack, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η στοιβα stack.
// Οποιαδήποτε λειτουργία πάνω στη στοίβα μετά το destroy είναι μη ορισμένη.

void stack_destroy(Stack stack);
