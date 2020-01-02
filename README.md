## code

Ο βασικός κώδικας για το μάθημα Δομές Δεδομένων και Τεχνικές Προγραμματισμού.

### Project structure

- `include`

  Κοινόχρηστα include files, μπορούν να χρησιμοποιηθούν από οποιοδήποτε πρόγραμμα ή module.
  Περιγράφουν τις λειτουργίες κάθε Abstract Data Type. Πχ ένα πρόγραμμα
  που θέλει να χρησιμοποιήσει ένα `Vector` κάνει `#include "ADTVector.h"`.

- `modules`

  Δημόσια modules, παρέχουν υλοποιήσεις των διαφόρων ADTs. Κατηγοριοποιούνται με βάση τη
  δομή δεδομένων που υλοποιεί το ADT, πχ το `DynamicArray/ADTVector.c` υλοποιεί ένα `Vector`
  μέσω Dynamic Array.

- `ADT_tests`

  Tests για κάθε ADT. Οποιαδήποτε υλοποίηση ενός ADT οφείλει να περνάει το αντίστοιχο test.
  Για να φτιάξουμε ένα εκτελέσιμο κάνουμε link
  το test με την υλοποίηση που θέλουμε να ελέγξουμε, πχ
  το `ADTVector_tests.o` με το `DynamicArray/ADTVector.o`.

- `programs`

  Εκτελέσιμα προγράμματα. Ενα πρόγραμμα που χρησιμοποιεί κάποιο ADT, πχ το `Vector`, θα πρέπει
  1. να κάνει `#include "ADTVector.h"`
  2. να γίνει linked με το `ADTVector.ο` κάποιας υλοποίησης του `Vector`, πχ με το `modules/UsingDynamicArray/ADTVector.o`.