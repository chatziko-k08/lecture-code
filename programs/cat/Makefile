# Λίστα με objects (.o) για κάθε εκτελέσιμο που θέλουμε να φτιάξουμε.
#
# Για το ADTVector χρησιμοποιούμε την υλοποίηση του k08.a library.
#
cat_OBJS		= cat.o io.o $(LIB)/k08.a
io_test_OBJS	= io_test.o io.o $(LIB)/k08.a

# Ορίσματα που χρησιμοποιούνται από το make run
cat_ARGS = input-file

# Ο βασικός κορμός του Makefile
include ../../common.mk