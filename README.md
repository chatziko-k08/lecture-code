## lecture-code

Ο βασικός κώδικας που χρησιμοποιείται στις διαλέξεις του μαθήματος [Δομές Δεδομένων και Τεχνικές Προγραμματισμού](https://k08.chatzi.org).

### Παράδειγμα χρήσης

```bash
cd programs/vector_example

make cat
./cat Makefile

make io.test
./io.test

make clean
```

### Project structure

- `include`

  Κοινόχρηστα include files (μπορούν να χρησιμοποιηθούν από οποιοδήποτε πρόγραμμα ή module).
  Περιγράφουν τις λειτουργίες κάθε Abstract Data Type. Πχ ένα πρόγραμμα
  που θέλει να χρησιμοποιήσει ένα `Vector` κάνει `#include "ADTVector.h"`.

- `lib`

  Κοινόχρηστες βιβλιοθήκες. Εκτελώντας `make` σε αυτό το directory παράγεται η βιβλιοθήκη
  `k08.a` η οποία περιέχει υλοποιήσεις από όλα τα ADTs.

- `programs`

  Εκτελέσιμα προγράμματα. Ενα πρόγραμμα που χρησιμοποιεί κάποιο ADT, πχ το `Vector`, θα πρέπει
  1. να κάνει `#include "ADTVector.h"`
  2. να γίνει link με το `lib/k08.a` (ή με κάποια συγκεκριμένη υλοποίηση του ADT)

- `modules`

  Κοινόχρηστα modules, παρέχουν υλοποιήσεις των διαφόρων ADTs. Κατηγοριοποιούνται με βάση τη
  δομή δεδομένων που υλοποιεί το ADT, πχ το `UsingDynamicArray/ADTVector.c` υλοποιεί ένα `Vector`
  μέσω Dynamic Array.

- `tests`

  Tests για κοινόχρηστα modules (ένα για κάθε ADT). Οποιαδήποτε υλοποίηση ενός ADT οφείλει να
  περνάει το αντίστοιχο test. Για να φτιάξουμε ένα εκτελέσιμο κάνουμε link
  το test με την υλοποίηση που θέλουμε να ελέγξουμε, πχ
  το `ADTVector.test.o` με το `UsingDynamicArray/ADTVector.o`.