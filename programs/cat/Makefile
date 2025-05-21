# Λίστα με objects (.o) για κάθε εκτελέσιμο που θέλουμε να φτιάξουμε.
#
# Για το ADTVector χρησιμοποιούμε την υλοποίηση με Dynamic Array
#
cat_OBJS		= cat.o io.o $(MODULES)/UsingDynamicArray/ADTVector.o
io_test_OBJS	= io_test.o io.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Ορίσματα που χρησιμοποιούνται από το make run
cat_ARGS = input-file

# Ο βασικός κορμός του Makefile
include ../../common.mk
