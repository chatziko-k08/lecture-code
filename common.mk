# common.mk
#
# Ο βασικός κορμός όλων των Makefiles, ώστε να μην χρειάζεται επανάληψη των ίδιων εντολών.
# Κάθε Makefile αρκεί να ορίσει τις μεταβλητές PROGS και <program>_OBJS, και να κάνει
# include το common.mk.

## Μεταβλητές ###########################################################

# Paths
MY_PATH := $(dir $(lastword $(MAKEFILE_LIST)))
MODULES := $(MY_PATH)modules
INCLUDE := $(MY_PATH)include

# Compiler options
#   -g         Δημιουργεί εκτελέσιμο κατάλληλο για debugging
#   -I<dir>    Λέει στον compiler να ψάξει στο συγκεκριμένο directory για include files
#   -Wall      Ενεργοποιεί όλα τα warnings
#   -MDD -MP   Δημιουργεί ένα .d αρχείο με τα dependencies, το οποίο μπορούμε να κάνουμε include στο Makefile
#
CFLAGS += -g -Wall -MMD -MP -I$(INCLUDE)

# Linker options
#   -lm        Link με τη math library
#
LDFLAGS += -lm

# Μαζεύουμε όλα τα objects σε μία μεταβλητή
OBJS = $(foreach prog, $(PROGS), $($(prog)_OBJS))

# Για κάθε .o ο gcc παράγει ένα .d, τα αποθηκεύουμε εδώ
DEPS = $(patsubst %.o,%.d,$(OBJS))


## Κανόνες ###########################################################

# Default target, κάνει compile όλα τα εκτελέσιμα
all: $(PROGS)

# Αυτό χρειάζεται για να μπορούμε να χρησιμοποιήσουμε το $@ στον παρακάτω κανόνα
.SECONDEXPANSION:

# Για κάθε εκτελέσιμο <program>, δημιουργούμε έναν κανόνα που δηλώνει τα περιεχόμενα του
# <program>_OBJS ως depedencies του <program>. Το $@ περιέχει το όνομα του target,
# αλλά για να το χρησιμοποιήσουμε στη λίστα των dependencies χρειάζεται $$ και .SECONDEXPANSION
#
$(PROGS): $$($$@_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Κάνουμε include τα .d αρχεία που παράγει ο gcc (το "-" αγνοεί την εντολή αν αποτύχει)
# Ενα αρχείο foo.d περιέχει όλα τα αρχεία (.c και .h) που χρειάστηκε o gcc για να κάνει compile
# το foo.o, στη μορφή του Makefile. Οπότε κάνοντας include το foo.d δηλώνουμε ότι αν οποιοδήποτε
# από τα αρχεία αυτά αλλάξει, πρέπει να ξανακάνουμε compile το foo.o.
#
-include $(DEPS)

# Το make clean καθαρίζει οτιδήποτε φτιάχνεται από αυτό το Makefile
clean:
	rm -f $(PROGS) $(OBJS) $(DEPS)

# Τα run και run-valgrind εκτελούν από default το πρώτο από τα PROGS
# (αυτό μπορεί να αλλάξει ορίζοντας τις μεταβλητές RUN, RUN_VALGRIND πριν το include common.mk
#
RUN ?= ./$(firstword $(PROGS))
RUN_VALGRIND ?= $(RUN) -E

run: $(RUN)
	$(RUN)

run-valgrind: $(RUN)
	valgrind --error-exitcode=1 --leak-check=full $(RUN_VALGRIND)

# Δηλώνουμε ότι οι παρακάτω κανόνες είναι εικονικοί, δεν παράγουν αρχεία. Θέλουμε δηλαδή
# το "make clean" να εκτελεστεί ακόμα και αν συμπτωματικά υπάρχει ένα αρχείο "clean" στο
# τρέχον directory.
#
.PHONY: clean run run-valgrind
