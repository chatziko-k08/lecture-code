// Υλοποίηση της εντολής cat του unix.
//    cat [FILE1] [FILE2] ...
//
// Διαβάζει και τυπώνει τα περιεχόμενα του κάθε αρχείου που δίνεται ως είσοδος
// (με τη διαφορά ότι διαβάζει ολόκληρο το αρχείο πριν αρχίσει να γράφει).
// Αν δε δοθεί αρχείο, ή όταν το αρχείο είναι ίσο με "-", διαβάζει από το stdin.

#include <string.h>

#include "io.h"


void process_file(char* filename) {
	// Διαβάζουμε το αρχείο ή την είσοδο. Το io.h module κάνει όλη τη δουλειά.
	Vector vec = strcmp(filename, "-") == 0
		? io_read_stream_as_vector(stdin)
		: io_read_file_as_vector(filename);

	if(vec == NULL) {
		fprintf(stderr, "cat: %s: cannot read file\n", filename);
		return;
	}

	io_write_vector_to_stream(stdout, vec);
	vector_destroy(vec);
}

int main(int argc, char* argv[]) {
	for(int i = 1; i < argc; i++)
		process_file(argv[i]);

	// χωρίς κανένα αρχείο διαβάζουμε από την είσοδο
	if(argc == 1)
		process_file("-");
}
