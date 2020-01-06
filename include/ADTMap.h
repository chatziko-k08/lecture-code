///////////////////////////////////////////////////////////
//
// ADT Map
//
// Abstract map. Παρέχει γρήγορη αντιστοίχιση key => value.
//
///////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Ενα map αναπαριστάται από τον τύπο Map

typedef struct map* Map;

// Δείκτης σε συνάρτηση που επισκέπτεται τα στειχεία του Map
typedef void (*MapVisitFunc)(Map map, Pointer key, Pointer value);


// Δημιουργεί και επιστρέφει ένα map, στο οποίο τα στοιχεία συγκρίνονται με βάση
// τη συνάρτηση compare.

Map map_create(CompareFunc compare);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το map.

int map_size(Map map);

// Επιστρέφει true αν το map περιέχει το συγκεκριμένο key, διαφορετικά false.

bool map_exists(Map map, Pointer key);

// Επιστρέφει την τιμή που έχει αντιστοιχιστεί στο συγκεκριμένο key, ή NULL αν το key δεν υπάρχει.
//
// Προσοχή: η συνάρτηση επιστρέφει NULL είτε όταν το key δεν υπάρχει, είτε όταν υπάρχει και έχει τιμή NULL.
//          Αν χρειάζεται να διαχωρίσουμε τις δύο περιπτώσεις μπορούμε να χρησιμοποιήσουμε την map_exists.

Pointer map_get(Map map, Pointer key);

// Προσθέτει το κλειδί key με τιμή value στο map, αντικαθιστώντας τυχόν προηγούμενη τιμή.
// Επιστρέφει true αν το κλειδί υπήρχε ήδη, διαφορετικά false.

bool map_insert(Map map, Pointer key, Pointer value);

// Αφαιρεί το κλειδί key από το map. Επιστρέφει την τιμή που είχε το κλειδί, ή NULL αν το κλειδί δεν υπήρχε.
//
// Προσοχή: όπως και η map_get, η συνάρτηση επιστρέφει NULL είτε όταν το key δεν υπάρχει, είτε όταν υπάρχει και έχει τιμή NULL.
//          Αν χρειάζεται να διαχωρίσουμε τις δύο περιπτώσεις μπορούμε να χρησιμοποιήσουμε την map_exists.

Pointer map_remove(Map map, Pointer key);

// Καλεί τη visit(map, key, value) για κάθε στοιχείο του map (σε μη καθορισμένη σειρά)

void map_visit(Map map, MapVisitFunc visit);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το map.
// Οποιαδήποτε λειτουργία πάνω στο map μετά το destroy είναι μη ορισμένη.

void map_destroy(Map map);




//// Επιπλέον συναρτήσεις για υλοποιήσεις βασισμένες σε hashing ////////////////////////////

// Τύπος συνάρτησης κατακερματισμού

typedef int (*HashFunc)(const Pointer);

// Υλοποιημένες συναρτήσεις κατακερματισμού για συχνούς τύπους δεδομένων

int hash_string(const Pointer value);		// Χρήση όταν το key είναι char*
int hash_int(const Pointer value);			// Χρήση όταν το key είναι int*
int hash_pointer(const Pointer value);		// Χρήση όταν το key είναι pointer που θεωρείται διαφορετικός από οποιονδήποτε άλλο pointer

// Ορίζει τη συνάρτηση κατακερματισμού hash για το συγκεκριμένο map
// Πρέπει να κληθεί μετά την map_create και πριν από οποιαδήποτε άλλη συνάρτηση.

void map_set_hash(Map map, HashFunc hash);