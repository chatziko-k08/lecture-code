# Διαδρομή προς το το βασικό directory
ROOT = ../..

# Το όνομα του executable προγράμματος που θέλουμε να κάνουμε compile
PROGRAM = int_vector_example

# Λίστα από .o files που χρειάζεται το πρόγραμμα
#
# Χρειαζόμαστε μια υλοοποίηση του ADTIntVector: χρησιμοποιούμε το modules/UsingADTVector/ADTIntVector.o
# Το module όμως αυτό χρησιμοποιεί ADTVector, οπότε χρειαζόμαστε και το modules/UsingDynamicArray/ADTVector.o
#
OBJS = main.o $(MODULES)/UsingADTVector/ADTIntVector.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Ο βασικός κορμός του Makefile
include $(ROOT)/common.make