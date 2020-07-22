//
// Unit tests για το fibonacci.h module
//

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "fibonacci.h"


char* lateralus[] = {
	"black",
	"then",
	"white are",
	"all I see",
	"in my in-fan-cy",
	"red and yel-low then came to be",
	"reach-ing out to me",
	"lets me see",

	"as be-low so a-bove and be-yond I i-mag-ine",
	"drawn be-yond the lines of rea-son",
	"push the en-ve-lope",
	"watch it bend",

	// over thinking, over analyzing separates the body from the mind
	// withering my intuition, missing opportunities and I must
	// feed my will to feel my moment drawing way outside the line

	"black",
	"then",
	"white are",
	"all I see",
	"in my in-fan-cy",
	"red and yel-low then came to be",
	"reach-ing out to me",
	"lets me see",

	"there is",
	"so",
	"much",
	"more and",
	"beck-ons me",
	"to look through to these",
	"in-fi-nite pos-si-bil-i-ties",

	"as be-low so a-bove and be-yond I i-mag-ine",
	"drawn be-yond the lines of rea-son",
	"push the en-ve-lope",
	"watch it bend"
};

// Επιστρέφει τον αριθμό των συλλαβών στο s (σε κάθε λέξη πρέπει να χωρίζονται με '-')
int syllables_no(char* s) {
	int num = 1;
	for (; *s != '\0'; s++)
		if (*s == ' ' || *s == '-')
			num++;

	return num;
}

void test_fibonacci(void) {
	// genius
	TEST_ASSERT(syllables_no(lateralus[ 0]) == fibonacci(1));
	TEST_ASSERT(syllables_no(lateralus[ 1]) == fibonacci(2));
	TEST_ASSERT(syllables_no(lateralus[ 2]) == fibonacci(3));
	TEST_ASSERT(syllables_no(lateralus[ 3]) == fibonacci(4));
	TEST_ASSERT(syllables_no(lateralus[ 4]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[ 5]) == fibonacci(6));
	TEST_ASSERT(syllables_no(lateralus[ 6]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[ 7]) == fibonacci(4));

	TEST_ASSERT(syllables_no(lateralus[ 8]) == fibonacci(7));
	TEST_ASSERT(syllables_no(lateralus[ 9]) == fibonacci(6));
	TEST_ASSERT(syllables_no(lateralus[10]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[11]) == fibonacci(4));

	TEST_ASSERT(syllables_no(lateralus[12]) == fibonacci(1));
	TEST_ASSERT(syllables_no(lateralus[13]) == fibonacci(2));
	TEST_ASSERT(syllables_no(lateralus[14]) == fibonacci(3));
	TEST_ASSERT(syllables_no(lateralus[15]) == fibonacci(4));
	TEST_ASSERT(syllables_no(lateralus[16]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[17]) == fibonacci(6));
	TEST_ASSERT(syllables_no(lateralus[18]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[19]) == fibonacci(4));
	TEST_ASSERT(syllables_no(lateralus[20]) == fibonacci(3));
	TEST_ASSERT(syllables_no(lateralus[21]) == fibonacci(2));

	TEST_ASSERT(syllables_no(lateralus[22]) == fibonacci(2));
	TEST_ASSERT(syllables_no(lateralus[23]) == fibonacci(3));
	TEST_ASSERT(syllables_no(lateralus[24]) == fibonacci(4));
	TEST_ASSERT(syllables_no(lateralus[25]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[26]) == fibonacci(6));

	TEST_ASSERT(syllables_no(lateralus[27]) == fibonacci(7));
	TEST_ASSERT(syllables_no(lateralus[28]) == fibonacci(6));
	TEST_ASSERT(syllables_no(lateralus[29]) == fibonacci(5));
	TEST_ASSERT(syllables_no(lateralus[30]) == fibonacci(4));

	// Και κάποια μεγάλα
	TEST_ASSERT(fibonacci(20) == 6765);
	TEST_ASSERT(fibonacci(30) == 832040);
	TEST_ASSERT(fibonacci(45) == 1134903170);

	fibonacci_destroy();
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "fibonacci", test_fibonacci },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};