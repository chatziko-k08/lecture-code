#include <stdlib.h>
#include <string.h>

#include "io.h"


Vector io_read_stream_as_vector(FILE* stream) {
	// TODO: θεωρούμε ότι κάθε γραμμή έχει max 500 χαρακτήρες. Πώς μπορούμε να αφαιρέσουμε αυτόν τον περιορισμό;
	int max_len = 500;
	char line[max_len];

	Vector vec = vector_create(0, free);		// αυτόματο free για κάθε στοιχείο που αφαιρείται

	while(fgets(line, max_len, stream)) {
		// αφαίρεση του newline, αν υπάρχει
		char* newline = strchr(line, '\n');
		if(newline != NULL)
			*newline = '\0';

		// προσθήκη στο vector
		vector_insert_last(vec, strdup(line));
	}

	return vec;
}

Vector io_read_file_as_vector(char* filename) {
	// απλά ανοίγουμε το αρχείο και καλούμε την io_read_stream_as_vector
	FILE* file = fopen(filename, "r");
	if(file == NULL)
		return NULL;

	Vector vec = io_read_stream_as_vector(file);
	fclose(file);

	return vec;
}

int io_write_vector_to_stream(FILE* stream, Vector vec) {
	int written = 0;
	int size = vector_size(vec);

	// διασχίζουμε το vector και τυπώνουμε κάθε γραμμή
	for(int i = 0; i < size; i++) {
		char* line = vector_get_at(vec, i);
		written += fprintf(stream, "%s\n", line);
	}

	return written;
}

int io_write_vector_to_file(char* filename, Vector vec) {
	// απλά ανοίγουμε το αρχείο και καλούμε την io_write_vector_to_stream
	FILE* file = fopen(filename, "w");
	if(file == NULL)
		return 0;

	int written = io_write_vector_to_stream(file, vec);
	fclose(file);

	return written;
}
