# Λίστα με objects (.o) για κάθε εκτελέσιμο που θέλουμε να φτιάξουμε.
#
# Χρειαζόμαστε την υλοποίηση του ADTIntVector, και το k08.a library για το ADTVector.
#
fibonacci_test_OBJS = fibonacci_test.o fibonacci.o ADTIntVector.o $(LIB)/k08.a

ADTIntVector_test_OBJS = ADTIntVector_test.o ADTIntVector.o $(LIB)/k08.a

# Ο βασικός κορμός του Makefile
include ../../common.mk

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory.
# Αυτό γίνεται ακόμα και αν υπάρχει ήδη το k08.a (.PHONY), γιατί ίσως πρέπει να ενημερωθεί.
.PHONY: $(LIB)/k08.a
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a