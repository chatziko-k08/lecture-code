//
// Unit tests για το io.h module
//

#include <string.h>
#include <stdio.h>

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "io.h"


void test_io_write_vector_to_file() {
	Vector vec = vector_create(0, NULL);
	vector_insert(vec, "foo");
	vector_insert(vec, "bar");

	// δημιουργία ενός προσωρινού αρχείου
	TEST_ASSERT(io_write_vector_to_file("io_tests_temp", vec) > 0);

	vector_destroy(vec);
}

void test_io_read_file_as_vector() {
	Vector vec = io_read_file_as_vector("io_tests_temp");
	TEST_ASSERT(vec != NULL);

	TEST_ASSERT(strcmp(vector_get_at(vec, 0), "foo") == 0);
	TEST_ASSERT(strcmp(vector_get_at(vec, 1), "bar") == 0);

	// διαγραφή του προσωρινού αρχείου
	remove("io_tests_temp");

	TEST_ASSERT(io_read_file_as_vector("a_file_that_doesnt_exist") == NULL);

	vector_destroy(vec);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "io_write_vector_to_file", test_io_write_vector_to_file },
	{ "io_read_file_as_vector", test_io_read_file_as_vector },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};