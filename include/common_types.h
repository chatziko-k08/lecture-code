#pragma once // #include το πολύ μία φορά

// Τύποι που χρησιμοποιούνται σε πολλά modules


// Pointer προς ένα αντικείμενο οποιουδήποτε τύπου. Απλά είναι πιο ευανάγνωστο από το "void*" που μοιάζει με το "void"
typedef void* Pointer;

// Δείκτης σε συνάρτηση που συγκρίνει 2 στοιχεία a και b
typedef int (*CompareFunc)(const Pointer a, const Pointer b);