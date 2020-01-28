#pragma once

#include <stdio.h>

#include "ADTVector.h"

// Module που διευκολύνει το input/output ολόκληρων αρχείων. Ενα τέτοιο module θα μπορούσε να είναι
// κοινόχρηστο ώστε να χρησιμοποιείται από διαφορετικά προγράμματα ή άλλα modules.

// Διαβάζει τα περιεχόμενα του stream και τα επιστρέφει ως ένα Vector που περιέχει
// ένα στοιχείο για κάθε γραμμή του αρχείου (χωρίς την αλλαγή γραμμής "\n")

Vector io_read_stream_as_vector(FILE* stream);

// Διαβάζει τα περιεχόμενα του αρχείου filename και τα επιστρέφει ως ένα Vector που περιέχει
// ένα στοιχείο για κάθε γραμμή του αρχείου (χωρίς την αλλαγή γραμμής "\n")

Vector io_read_file_as_vector(char* filename);

// Γράφει τα περιεχόμενα του string Vector vec στο stream, με αλλαγή γραμμής μετά από
// κάθε στοιχείο. Επιστρέφει τον αριθμό των χαρακτήρων που γράφτηκαν.

int io_write_vector_to_stream(FILE* stream, Vector vec);

// Γράφει τα περιεχόμενα του string Vector vec στο αρχείο filename, με αλλαγή γραμμής
// μετά από κάθε στοιχείο. Επιστρέφει τον αριθμό των χαρακτήρων που γράφτηκαν.

int io_write_vector_to_file(char* filename, Vector vec);
