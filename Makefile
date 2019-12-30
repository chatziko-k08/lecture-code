# Το make foo στο root directory απλά τρέχει την ίδια εντολή σε όλα τα
# subdirectories όπου υπάρχει κάποιο Makefile

# Κοινός κανόνας για οποιοδήποτε target, το όνομα το target είναι διαθέσιμο στη μεταβλητή $@
%:
	# Για κάθε subdirectory, έλεγχος να υπάρχει Makefile, και αν ναι τρέχουμε το make $@
	@for dir in **/*; do \
		if [ -f $$dir/Makefile ]; then \
			echo; echo Running make $@ in $$dir; echo; \
			$(MAKE) $@ -C $$dir || exit; \
		fi; \
	done;
	@echo success

# Ορίζουμε το 'all' σαν default target
all:
