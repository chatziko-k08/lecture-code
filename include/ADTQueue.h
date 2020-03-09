///////////////////////////////////////////////////////////////////
//
// ADT Queue
//
// Abstract ουρά. Λειτουργία "First In First Out", δηλαδή σε κάθε
// remove αφαιρείται το παλιότερο στοιχείο που έγινε insert.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Μία ουρά αναπαριστάται από τον τύπο Queue. Ο χρήστης δε χρειάζεται να γνωρίζει το περιεχόμενο
// του τύπου αυτού, απλά χρησιμοποιεί τις συναρτήσεις queue_<foo> που δέχονται και επιστρέφουν Queue.
//
// Ο τύπος Queue ορίζεται ως pointer στο "struct queue" του οποίου το περιεχόμενο είναι άγνωστο
// (incomplete struct), και εξαρτάται από την υλοποίηση του ADT Queue.
//
typedef struct queue* Queue;


// Δημιουργεί και επιστρέφει μια νέα ουρά.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.

Queue queue_create(DestroyFunc destroy_value);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά queue

int queue_size(Queue queue);

// Επιστρέφει το στοιχείο στο μπροστινό μέρος της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer queue_front(Queue queue);

// Επιστρέφει το στοιχείο στο πίσω μέρος της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer queue_back(Queue queue);

// Προσθέτει την τιμή value στo πίσω μέρος της ουράς queue.

void queue_insert_back(Queue queue, Pointer value);

// Αφαιρεί την τιμή στο μπροστά μέρος της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

void queue_remove_front(Queue queue);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά queue.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void queue_destroy(Queue queue);
