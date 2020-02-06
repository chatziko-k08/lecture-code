# Λίστα με objects (.o) για κάθε εκτελέσιμο που θέλουμε να φτιάξουμε.
#
# Για το ADTMap χρησιμοποιούμε την υλοποίηση του k08.a library.
#
pair_sum_test_OBJS = pair_sum_test.o pair_sum.o $(LIB)/k08.a

# Ο βασικός κορμός του Makefile
include ../../common.mk

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a