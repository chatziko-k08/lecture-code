# common.mk
#
# Το αρχείο αυτό επιτρέπει την εύκολη δημιουργία Makefiles με πολλαπλά targets
# αποφεύγοντας την επανάληψη των ίδιων εντολών. Χρησιμοποιείται φτιάχνοντας
# ένα ή περισσότερα Makefiles, και μέσα στο κάθε Makefile:
#
# 1. προσθέτουμε για κάθε εκτελέσιμο <foo> μια μεταβλητή <foo>_OBJS με
#    όλα τα objects (.o) που χρειάζονται για το πρόγραμμα. Πχ
#    myprogram_OBJS = main.o module1.o module2.o
#
# 2. Κάνουμε: include common.mk
#
# Για κάθε εκτελέσιμο <foo> παράγονται τα εξής targets:
#   <foo>           Κάνει compile το <foo>
#   run-<foo>       Κάνει compile και εκτελεί το <foo>
#   valgrind-<foo>  Κάνει compile και εκτελεί το <foo> μέσω valgrind
#
# Και επιπλέον παράγονται τα παρακάτω γενικά targets:
#   all             Κάνει depend σε όλα τα targets <foo>
#   run             Κάνει depend σε όλα τα targets run-<foo>
#   valgrind        Κάνει depend σε όλα τα targets valgrind-<foo>
#   clean           Διαγράφει όλα τα αρχεία που παράγονται από το make
#
# Το αρχείο αυτό χρησιμοποιεί κάποια advanced features του GNU make, ΔΕΝ απαιτείται
# να κατανοήσετε όλες τις εντολές για να το χρησιμοποιήσετε (όχι ότι είναι δύσκολο
# να το κάνετε αν θέλετε). Επίσης μπορείτε κάλλιστα τα χρησιμοποιήσετε τα απλούστερα
# Makefiles που είδαμε στο μάθημα, αν προτιμάτε.


## Μεταβλητές ###########################################################

# Paths
MY_PATH := $(dir $(lastword $(MAKEFILE_LIST)))
MODULES := $(MY_PATH)modules
INCLUDE := $(MY_PATH)include

# Compiler options
#   -g         Δημιουργεί εκτελέσιμο κατάλληλο για debugging
#   -I<dir>    Λέει στον compiler να ψάξει στο συγκεκριμένο directory για include files
#   -Wall      Ενεργοποιεί όλα τα warnings
#   -MDD       Δημιουργεί ένα .d αρχείο με τα dependencies, το οποίο μπορούμε να κάνουμε include στο Makefile
#
CFLAGS += -g -Wall -MMD -I$(INCLUDE)

# Linker options
#   -lm        Link με τη math library
#
LDFLAGS += -lm

# Λίστα με όλα τα εκτελέσιμα <prog> για τα οποία υπάρχει μια μεταβλητή <prog>_OBJS
PROGS := $(subst _OBJS,,$(filter %_OBJS,$(.VARIABLES)))

# Μαζεύουμε όλα τα objects σε μία μεταβλητή
OBJS := $(foreach prog, $(PROGS), $($(prog)_OBJS))

# Για κάθε .o ο gcc παράγει ένα .d, τα αποθηκεύουμε εδώ
DEPS := $(patsubst %.o,%.d,$(OBJS))

# Για κάθε target <prog> φτιάχνουμε 2 targets run-<prog> και valgrind-<prog>.
# Στις παρακάτω μεταβλητές φτιάχνουμε μια λίστα με όλα αυτά τα targets
# Το "?=" σημαίνει "ανάθεση αν η μεταβλητή δεν έχει ήδη τιμή". Αυτό επιτρέπει
# στο Makefile να ορίσει ποια targets θέλει να φτιαχτούν, πριν το include common.mk
#
RUN_TARGETS ?= $(foreach prog, $(PROGS), run-$(prog))
VAL_TARGETS ?= $(foreach prog, $(PROGS), valgrind-$(prog))


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

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target run-<prog> που το εκτελεί,
# και ένα valgrind-<prog> που το εκτελεί μέσω valgrind.
#
$(RUN_TARGETS): $$(subst run-,,$$@)
	./$(subst run-,,$@)

$(VAL_TARGETS): $$(subst valgrind-,,$$@)
	valgrind --error-exitcode=1 --leak-check=full ./$(subst valgrind-,,$@) -E

run:      $(RUN_TARGETS)
valgrind: $(VAL_TARGETS)

# Δηλώνουμε ότι οι παρακάτω κανόνες είναι εικονικοί, δεν παράγουν αρχεία. Θέλουμε δηλαδή
# το "make clean" να εκτελεστεί ακόμα και αν συμπτωματικά υπάρχει ένα αρχείο "clean" στο
# τρέχον directory.
#
.PHONY: clean run valgrind $(RUN_TARGETS) $(VAL_TARGETS)
