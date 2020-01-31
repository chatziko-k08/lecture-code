//
// Unit tests για το fibonacci.h module
//

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "fibonacci.h"


void test_fibonacci(void) {
	TEST_CHECK(fibonacci(0) == 0);
	TEST_CHECK(fibonacci(1) == 1);
	TEST_CHECK(fibonacci(2) == 1);
	TEST_CHECK(fibonacci(3) == 2);
	TEST_CHECK(fibonacci(4) == 3);
	TEST_CHECK(fibonacci(5) == 5);

	TEST_CHECK(fibonacci(20) == 6765);
	TEST_CHECK(fibonacci(30) == 832040);
	TEST_CHECK(fibonacci(45) == 1134903170);

	fibonacci_destroy();
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "fibonacci", test_fibonacci },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};