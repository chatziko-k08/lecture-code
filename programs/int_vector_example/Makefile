# Λίστα με τα executables που θέλουμε να κάνουμε compile
PROGS = int_vector_example

# Λίστα από .o files που χρειάζεται κάθε πρόγραμμα (μια μεταβλητή ανά executable)
#
# Χρειαζόμαστε μια υλοοποίηση του ADTIntVector: χρησιμοποιούμε το modules/UsingADTVector/ADTIntVector.o
# Το module όμως αυτό χρησιμοποιεί ADTVector, οπότε χρειαζόμαστε και το modules/UsingDynamicArray/ADTVector.o
#
int_vector_example_OBJS = main.o $(MODULES)/UsingADTVector/ADTIntVector.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Ο βασικός κορμός του Makefile
include ../../common.mk