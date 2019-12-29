### Δομή

- `include`

  Δημόσια include files που μπορούν να χρησιμοποιηθούν από οποιοδήποτε πρόγραμμα.
  Περιγράφουν τις λειτουργίες κάθε Abstract Data Type. Πχ ένα πρόγραμμα
  που θέλει να χρησιμοποιήσει ένα `Vector` κάνει `#include` το
  `include/ADTVector.h`.

- `modules`

  Υλοποιήσεις των διαφόρων ADTs. Κατηγοριοποιούνται με βάση τη δομή δεδομένων
  που υλοποιεί το ADT, πχ το `modules/DynamicArray/ADTVector.c` υλοποιεί ένα `Vector`
  μέσω Dynamic Array.

- `programs`

  Εκτελέσιμα προγράμματα. Ενα πρόγραμμα που χρησιμοποιεί κάποιο ADT, πχ το `Vector`, θα πρέπει
  1. να κάνει `#include "ADTVector.h"`
  2. να γίνει linked με το `ADTVector.ο` κάποιας υλοποίησης του `Vector`, πχ με το `modules/UsingDynamicArray/ADTVector.o`.