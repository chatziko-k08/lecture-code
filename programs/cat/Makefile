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

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory.
# Αυτό γίνεται ακόμα και αν υπάρχει ήδη το k08.a (.PHONY), γιατί ίσως πρέπει να ενημερωθεί.
.PHONY: $(LIB)/k08.a
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a