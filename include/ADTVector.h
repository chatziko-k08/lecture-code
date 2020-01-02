///////////////////////////////////////////////////////////////////
//
// ADT Vector
//
// Abstract "array" μεταβλητού μεγέθους. Παρέχει πρόσβαση σε
// οποιοδήποτε στοιχείο (μέσω της θέσης του), και προσθήκη/αφαίρεση
// στοιχείων στο τέλος του vector.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Ένα vector αναπαριστάται από τον τύπο Vector. Ο χρήστης δε χρειάζεται να γνωρίζει το περιεχόμενο
// του τύπου αυτού, απλά χρησιμοποιεί τις συναρτήσεις vector_<foo> που δέχονται και επιστρέφουν Vector.
//
// Ο τύπος Vector ορίζεται ως pointer στο "struct vector" του οποίου το περιεχόμενο είναι άγνωστο
// (incomplete struct), και εξαρτάται από την υλοποίηση του ADT Vector.
//
typedef struct vector* Vector;


// Δημιουργεί και επιστρέφει ένα νεό vector μεγέθους size (τα στοιχεία δεν είναι αρχικοποιημένα).
// Αν δεν υπάρχει διαθέσιμη μνήμη επιστρέφει NULL.

Vector vector_create(int size);

// Επιστρέψει τον αριθμό στοιχείων που περιέχει το vector vec.

int vector_size(Vector vec);

// Προσθέτει την τιμή value στη θέση pos του Vector vec.
// ΔΕΝ μεταβάλλει το μέγεθος του vector, αν pos >= size το αποτέλεσμα δεν είναι ορισμένο.

void vector_set(Vector vec, int pos, Pointer value);

// Επιστρέφει την τιμή στη θέση pos του vector vec, ή NULL αν pos >= size.

Pointer vector_get(Vector vec, int pos);

// Προσθέτει την τιμή value στο τέλος του vector vec. Το μέγεθος του vector μεγαλώνει κατά 1.
// Επιστρέφει 1 αν το insert πετύχει, διαφορετικά 0 (μοναδική περίπτωση αποτυχίας είναι όταν
// απαιτείται δέσμευση μνήμης και δεν υπάρχει διαθέσιμη)

int vector_insert(Vector vec, Pointer value);

// Αφαιρεί και επιστρέψει την τιμή της τελευταίας θέσης του vector.
// Το μέγεθος του vector μικραίνει κατά 1.
// Αν size == 0 επιστρέφει NULL.

Pointer vector_remove(Vector vec);

// Βρίσκει το πρώτο στοιχείο στο vector που να είναι ίσο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τη θέση του στοιχείου, ή -1 αν δεν βρεθεί.

int vector_find(Vector vec, Pointer value, CompareFunc compare);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το vector vec.
// Οποιαδήποτε λειτουργία πάνω στο vector μετά το destroy είναι μη ορισμένη.

void vector_destroy(Vector vec);
