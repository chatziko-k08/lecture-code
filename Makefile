# Το Makefile αυτό βρίσκεται στο root ολόκληρου του project. Η μόνη χρησιμοτητά του
# είναι να μπορούμε εύκολα να κάνουμε compile όλα τα τμήματα του project. Εκτελώντας
#   make foo
# στο root directory, εκτελείται η ίδια εντολή σε όλα τα subdirectories που περιέχουν κάποιο Makefile.

# Κοινός κανόνας για οποιοδήποτε target, το όνομα το target είναι διαθέσιμο στη μεταβλητή $@
# Για κάθε subdirectory, έλεγχος αν περιέχει Makefile, και αν ναι τρέχουμε το
# make $@ με --silent για να μην έχουμε τεράστια έξοδο.
%:
	@for dir in * */*; do \
		if [ -f $$dir/Makefile ]; then \
			echo; echo Running make $@ in $$dir; echo; \
			$(MAKE) $@ --silent -C $$dir || exit; \
		fi; \
	done;
	@echo success

# Ορίζουμε το 'all' σαν default target
all:

# Τα παρακάτω είναι μόνο για να αναγνωρίζει τα targets το auto-complete
run:
valgrind:
clean:
