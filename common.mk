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
#   coverage-<foo>  Κάνει compile και εκτελεί το <foo>, και παράγε coverage report
#
# Και επιπλέον παράγονται τα παρακάτω γενικά targets:
#   all             Κάνει depend σε όλα τα targets <foo>
#   run             Κάνει depend σε όλα τα targets run-<foo>
#   valgrind        Κάνει depend σε όλα τα targets valgrind-<foo>
#   coverage        Εκτελεί το run, και παράγει ένα εννιαίο coverage report
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
CFLAGS += -g -Wall -Werror -MMD -I$(INCLUDE)

# Linker options
#   -lm        Link με τη math library
#
LDFLAGS += -lm

# Αν στα targets με τα οποία έχει κληθεί το make (μεταβλητή MAKECMDGOALS) υπάρχει κάποιο
# coverage*, τότε προσθέτουμε το --coverage στα compile & link flags
#
ifneq (,$(findstring coverage,$(MAKECMDGOALS)))
	CFLAGS += --coverage
	LDFLAGS += --coverage
endif

# Λίστα με όλα τα εκτελέσιμα <prog> για τα οποία υπάρχει μια μεταβλητή <prog>_OBJS
PROGS := $(subst _OBJS,,$(filter %_OBJS,$(.VARIABLES)))

# Μαζεύουμε όλα τα objects σε μία μεταβλητή
OBJS := $(foreach prog, $(PROGS), $($(prog)_OBJS))

# Για κάθε .o ο gcc παράγει ένα .d, τα αποθηκεύουμε εδώ
DEPS := $(patsubst %.o,%.d,$(OBJS))

# Λίστα με coverage-related αρχεία που παράγονται κατά το compile & execute με --coverage (.gcda .gcno)
COV_FILES := $(patsubst %.o,%.gcda,$(OBJS)) $(patsubst %.o,%.gcno,$(OBJS))

# Για κάθε target <prog> φτιάχνουμε 3 targets run-<prog>, valgrind-<prog>, coverage-<prog>
# Στις παρακάτω μεταβλητές φτιάχνουμε μια λίστα με όλα αυτά τα targets
# Το "?=" σημαίνει "ανάθεση αν η μεταβλητή δεν έχει ήδη τιμή". Αυτό επιτρέπει
# στο Makefile να ορίσει ποια targets θέλει να φτιαχτούν, πριν το include common.mk
#
RUN_TARGETS ?= $(foreach prog, $(PROGS), run-$(prog))
VAL_TARGETS ?= $(foreach prog, $(PROGS), valgrind-$(prog))
COV_TARGETS ?= $(foreach prog, $(PROGS), coverage-$(prog))


## Κανόνες ###########################################################

# Default target, κάνει compile όλα τα εκτελέσιμα
all: $(PROGS)

# Αυτό χρειάζεται για να μπορούμε να χρησιμοποιήσουμε τα $$(...) και $$@ στης λίστα των dependencies
.SECONDEXPANSION:

# Για κάθε εκτελέσιμο <program>, δημιουργούμε έναν κανόνα που δηλώνει τα περιεχόμενα του
# <program>_OBJS ως depedencies του <program>. Το $@ περιέχει το όνομα του target,
# αλλά για να το χρησιμοποιήσουμε στη λίστα των dependencies χρειάζεται $$@ και .SECONDEXPANSION
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
	@rm -f $(PROGS) $(OBJS) $(DEPS) $(COV_FILES)
	@rm -rf coverage

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target run-<prog> που το εκτελεί
$(RUN_TARGETS): $$(subst run-,,$$@)
	./$(subst run-,,$@)

# Το make run εκτελεί όλα τα run targets
run: $(RUN_TARGETS)

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target valgrind-<prog> που το εκτελεί μέσω valgrind
$(VAL_TARGETS): $$(subst valgrind-,,$$@)
	valgrind --error-exitcode=1 --leak-check=full ./$(subst valgrind-,,$@) -E

valgrind: $(VAL_TARGETS)

# Βοηθητικό target που εκτελεί το lcov. Χρησιμοποιείται ως dependency στα coverage-* targets
lcov:
	@mkdir -p coverage
	lcov --rc lcov_branch_coverage=1 --capture --directory=$(MY_PATH) --output-file coverage/lcov.info
	lcov --rc lcov_branch_coverage=1 --remove coverage/lcov.info '*acutest.h' --output-file coverage/lcov.info
	cd coverage && genhtml --rc lcov_branch_coverage=1 lcov.info
	@echo "To see the report open the file below in your brower:"
	@echo "$$PWD/coverage/index.html"

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target coverage-<prog> που το εκτελεί και μετά φτιάχνει coverage report
$(COV_TARGETS): clean $$(subst coverage-,run-,$$@) lcov

coverage: clean run lcov

# Δηλώνουμε ότι οι παρακάτω κανόνες είναι εικονικοί, δεν παράγουν αρχεία. Θέλουμε δηλαδή
# το "make clean" να εκτελεστεί ακόμα και αν συμπτωματικά υπάρχει ένα αρχείο "clean" στο
# τρέχον directory.
#
.PHONY: clean run valgrind coverage lcov $(RUN_TARGETS) $(VAL_TARGETS) $(COV_TARGETS)
