# Λίστα με objects (.o) για κάθε εκτελέσιμο που θέλουμε να φτιάξουμε.
#
# Για το ADTSet χρησιμοποιούμε την υλοποίηση του k08.a library.
#
set_example_OBJS = tests.o $(LIB)/k08.a

# Ο βασικός κορμός του Makefile
include ../../common.mk

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a