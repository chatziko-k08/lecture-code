/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Hash Table με open addressing (linear probing)
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMap.h"


// Οι κόμβοι του map στην υλοποίηση με hash table, μπορούν να είναι σε 3 διαφορετικές καταστάσεις,
// ώστε αν διαγράψουμε κάποιον κόμβο, αυτός να μην είναι empty, ώστε να μην επηρεάζεται η αναζήτηση
// αλλά ούτε occupied, ώστε η εισαγωγή να μπορεί να το κάνει overwrite.
typedef enum {
	EMPTY, OCCUPIED, DELETED
} State;

// Το μέγεθος του Hash Table ιδανικά θέλουμε να είναι πρώτος αριθμός σύμφωνα με την θεωρία.
// Η παρακάτω λίστα περιέχει πρώτους οι οποίοι έχουν αποδεδιγμένα καλή συμπεριφορά ως μεγέθη.
// Κάθε re-hash θα γίνεται βάσει αυτής της λίστας. Αν χρειάζονται παραπάνω απο 1610612741 στοχεία, τότε σε καθε rehash διπλασιάζουμε το μέγεθος.
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.5

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)
struct map_node{
	Pointer key;		// Το κλειδί που χρησιμοποιείται για να hash-αρουμε
	Pointer value;  	// Η τιμή που αντισοιχίζεται στο παραπάνω κλειδί
	State state;		// Μεταβλητή για να μαρκάρουμε την κατάσταση των κόμβων (βλέπε διαγραφή)
};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	MapNode* array;				// Ο πίνακας που θα χρησιμοποιήσουμε για το map (remember, φτιάχνουμε ένα hash table)
	int capacity;				// Πόσο χώρο έχουμε δεσμεύσει. Στην πραγματικότητα index στην παραπάνω λίστα πρώτων, απο όπου παίρνουμε το size του array.
	int size;					// Πόσα στοιχεία έχουμε προσθέσει
	CompareFunc compare;		// Συνάρτηση για σύγκρηση δεικτών, που πρέπει να δίνεται απο τον χρήστη
	HashFunc hash_function;		// Συνάρτηση για να παίρνουμε το hash code του κάθε αντικειμένου.
	DestroyFunc destroy_key;	// Συναρτήσεις που καλούνται όταν διαγράφουμε έναν κόμβο απο το map.
	DestroyFunc destroy_value;
};

// Συνάρτηση που καταστρέφει ένα map node
static void destroy_map_node(Map map, MapNode node) {
	if (map->destroy_key && node->state != EMPTY)
		map->destroy_key(node->key);
	if (map->destroy_value && node->state != EMPTY)
		map->destroy_value(node->value);
}

// Βοηθητική συνάρτηση για να βρίσκουμε την νέα χωρητικότητα του πίνακα κατακερματισμού
int find_next_capacity(int cap) {
	// Διασχίζουμε την λίστα των πρώτων ώστε να βρούμε τον επόμενο πρώτο. Αν την έχουμε εξαντλήσει, απλά διπλασιάζουμε.
	int prime_no = sizeof(prime_sizes) / sizeof(int);

	for (int i = 0; i < prime_no; i++) {
		if (prime_sizes[i] > cap)
			return prime_sizes[i]; 
	}
	return 2 * cap; //LCOV_EXCL_LINE, καθώς στα test που θα κάνουμε δεν θα χρησιμοποιήσουμε τόσο μεγάλα hash table.
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Δημιουργία ενός Hash Table με όρισμα το μέγεθός του, που μας χρησιμεύει στην δέσμευση του πίνακα.
Map sized_hash_create(int capacity, CompareFunc compare){
	// Δεσμεύουμε κατάλληλα τον χώρο που χρειαζόμαστε για το hash table
	Map map = malloc(sizeof(*map));
	map->capacity = capacity;
	map->array = malloc(capacity * sizeof(MapNode));

	// Αρχικοποιούμε τους κόμβους που έχουμε σαν διαθέσιμους.
	for (int i = 0; i < capacity; i++) {
		map->array[i] = malloc(sizeof(struct map_node));
		map->array[i]->state = EMPTY;
	}
	map->size = 0;
	map->compare = compare;
	return map;
}

// Όταν ο χρήστης θέλει να δημιουργήσει ένα map, απλά το δημιουργούμε τον πίνακα κατακερματισμού
// με την παραπάνω συνάρτηση και όρισμα το αρχικό μέγεθος που θέλουμε.
Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Αρχικά, δημιουργόυμε έναν μικρό πίνακα κατακερματισμού
	Map map = sized_hash_create(prime_sizes[0], compare);
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;
	return map;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

// Συνάρτηση για την επέκταση του Hash Table σε περίπτωση που ο load factor μεγαλώσει πολύ.
Map rehash(Map map) {
	// Δημιουργούμε ένα μεγαλύτερο hash table, παίρνοντας σαν χωρητικότητα το επόμενο στοιχείο της λίστας πρώτων,
	// ή διπλασιάζοντας την ήδη υπάρχουσα.
	// Δεν χρειάζεται να φτιάξουμε εκ νέου ένα map, το μόνο που χρειάζεται είναι να φτιάξουμε έναν νέο πίνακα.
	int old_capacity = map->capacity;
	map->capacity = find_next_capacity(map->capacity);
	MapNode* new_array = malloc(map->capacity * sizeof(MapNode));
	// Κρατάμε τον παλίο πίνακα σε μία άλλη μεταβλητή
	MapNode* old_array = map->array;
	for (int i = 0; i < map->capacity; i++) {
		new_array[i] = malloc(sizeof(struct map_node));
		new_array[i]->state = EMPTY;
	}
	// Και μετά την δέσμευση του νέου πίνακα, μπορούμε πλέον να τον ορίσουμε ως πίνακα του map μας.
	map->array = new_array;
	// Επανα-αρχικοποιούμε τα στοιχεία του hash σε 0, καθώς θα τα ξαναβάλουμε απο την αρχή
	// ΠΡΟΣΟΧΗ: Αυτό είναι απαραίτητο, καθώς λόγω του καινούργιου μεγέθους, είναι προφανές οτι τα ήδη υπάρχοντα
	// στοιχεία δεν θα hash-άρουν στην ίδια θέση με πριν.
	map->size = 0;
	// Τοποθετούμε ΜΟΝΟ τα entries που όντως περιέχουν ένα στοιχείο.
	// Το rehash είναι και μία ευκαιρία να ξεφορτωθούμε τα deleted nodes μας, καθώς θα κάνουμε insert μόνο 
	// αυτών που είναι occupied, χωρίς να μας νοίαζουν τα κενά, αφού θα γίνει εκ νεου instert.
	for (int i = 0; i < old_capacity; i++) {
		if (old_array[i]->state == OCCUPIED) {
			map_insert(map, old_array[i]->key, old_array[i]->value);
		}
		else if (old_array[i]->state == DELETED) {
			destroy_map_node(map, old_array[i]);
		}
		// Αρχίζουμε να αποδεσμεύουμε τον παλίο πίνακα, σβήνοντας τα περιεχόμενά του
		free(old_array[i]);
	}
	//Αποδεσμεύουμε τον παλιό πίνακα ώστε να μήν έχουμε leaks
	free(old_array);
	return map;
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

void map_insert(Map map, Pointer key, Pointer item) {
	// Hash-αρουμε για να δούμε απο πού θα τοποθετήσουμε την νέα εγγραφή
	unsigned int position = map->hash_function(key) % map->capacity;

	// Σκανάρουμε το Hash Table μέχρι να βρούμε διαθέσιμη θέση για να τοποθετήσουμε το ζευγάρι,
	// ή μέχρι να βρούμε το κλειδί ώστε να το αντικαταστήσουμε.
	bool already_in_map = false;
	while (map->array[position]->state == OCCUPIED) {
		if (map->compare(map->array[position]->key, key) == 0) {
			already_in_map = true;
			break;
		}

		// Έχουμε linear probing, άρα κάθε φορά πηγαίνουμε στο επόμενο στοιχείο του πίνακα
		position = (position + 1) % map->capacity;
	}

	// Σε αυτό το σημείο, το position θα έχει πάρει την τιμή της θέσης που πρέπει να γίνει εισαγωγή.
	if (!already_in_map) {
		// Δημιουργούμε το καινούριο ζευγάρι που θα βάλουμε στο map από τα ορίσματα.
		// ΠΡΟΣΟΧΗ: Δεν δεσμεύουμε χώρο, για την δέσμευση χώρου είναι υπεύθυνος ο χρήστης
		// Εμείς απλά αποθηλεύουμε pointers, τους οποίους "βάζουμε" στο map με απλή ανάθεση.
		map->array[position]->key = key;
		map->array[position]->value = item;
		map->array[position]->state = OCCUPIED;

		// Άν βάζουμε νέο στοιχείο, αυξάνουμε τα συνολικά στοιχεία του map,
		map->size++;

	} else {
		// Αν ο χρήστης επιθυμεί, ελευθερώνουμε την δεσμευμένη μνήμη της τιμής του value που αντικαθίσταται.
		if (map->destroy_value)
			map->destroy_value(map->array[position]->value);
		// Αλλάζουμε το value του ήδη υπάρχοντος κόμβου.
		map->array[position]->value = item;
		// Αν περνάμε ένα ισοδύναμο κλειδί, δηλ ιδια τιμή σε άλλη θέση μνήμης, τότε το προηγούμενο, ίσως να πρέπει να διαγραφεί.
		if (map->array[position]->key != key && map->destroy_key)
			map->destroy_key(map->array[position]->key);
		// Αλλάζουμε το key του ήδη υπάρχοντος κόμβου.
		map->array[position]->key = key;
	}

	// Αν με την νέα εισαγωγή ξεπερνάμε το μέγιστο load factor, πρέπει να κάνουμε rehash
	float load_factor = (float)map->size / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR) {
		map = rehash(map);
	}
}

// Βοηθητική συνάρτηση, η οποία παίρνει σαν όρισμα την θέση στην οποία hashαρει το κλειδί
// και επιστρέφει τον κόμβο, και την θέση που βρέθηκε το αντικείμενο

MapNode search(Map map, Pointer key, int* position) {
	//Κρατάμε την τιμή σε μία νέα μεταβλητή, ώστε να προσπελάσουμε το map
	int it = *position;
	bool found = false;

	// Σκανάρουμε το map μέχρις ότου να βρούμε κενή θέση, όποτε και τερματίζουμε την αναζήτησή μας.
	while (map->array[it]->state != EMPTY) {
		// Αν είμαστε σε occupied κόμβο, ελέγχουμε μήπως βρήκαμε αυτό που ψάχνουμε
		// Αν είμαστε σε deleted, απλά το αγνοούμε. ΠΡΟΣΟΧΗ! Αν το ελέγχαμε και στα deleted, 
		// θα πέρναμε ένα υπέροχο seg fault
		if (map->array[it]->state == OCCUPIED) {
			if (map->compare(map->array[it]->key, key) == 0) {
				found = true;
				break;
			}

		}
		// Έχουμε linear probing, άρα κάθε φορά πηγαίνουμε στο επόμενο στοιχείο του πίνακα
		// Αν ξαναφτάσουμε στο σημείο από όπου ξεκινήσαμε, αυτό σημαίνει οτι το στοιχείο δεν υπάρχει οπότε επιστρέφουμε MAPEOF
		it = (it + 1) % map->capacity;
		if (it == *position) {
			return MAP_EOF;
		}
	}

	if (found) {
		// Ανανέωση του ορίσματος για επιστροφή
		*position = it;
		return map->array[it];
	} else {
		return MAP_EOF;
	}
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

Pointer map_find(Map map, Pointer key) {
	// Καλούμε την βοηθητική συνάρτηση, για να βρούμε τον κόμβο που αντιστοιχεί σε αυτό το κλειδί.
	int position = map->hash_function(key) % map->capacity;

	MapNode node = search(map, key, &position);
	if (node != MAP_EOF)
		return node->value;
	else
		return NULL;
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
Pointer map_remove(Map map, Pointer key) {
	int position = map->hash_function(key) % map->capacity;
	MapNode node = search(map, key, &position);
	if(node == MAP_EOF)
		return NULL;

	// Δεν διαγράφουμε τον κόμβο, αλλά τον θέτουμε ως "deleted", ώστε να μην διακόπτεται η αναζήτηση μας,
	// αλλά ταυτόχρονα να γίνεται ομαλά η εισαγωγή μας.
	node->state = DELETED;
	map->size--;

	return node->value;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {
	for (int i = 0; i < map->capacity; i++) {
		destroy_map_node(map, map->array[i]);
		free(map->array[i]);
	}
	free(map->array);
	free(map);
}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

// Εύρεση του πρώτου στοιχείου του map
MapNode map_first(Map map) {
	//Ξεκινάμε την επανάληψή μας απο το 1ο στοιχείο, μέχρι να βρούμε κάτι όντως τοποθετημένο
	int pos = 0;
	while (map->array[pos]->state == EMPTY) {
		pos ++;
		// Αν φτάσουμε στο τέλος, το hash μας είναι άδειο, οπότε επιστρέφουμε  null τιμη.
		if (pos == map->capacity)
			return MAP_EOF;
	}
	//Θέλουμε να επιστρέψουμε map_node*, ενώ έχουμε στοχεία τύπου map_node αποθηκευμένα στο
	// πίνακά μας. Άρα, χρησιμοποιούμε &
	return map->array[pos];
}

MapNode map_next(Map map, MapNode node) {
	int pos = map->hash_function(node->key) % map->capacity;
	// Βρες την θέση του αντικειμένου στο map. Δεν μας ενδιαφέρει η επιστροφή της συνάρτησης
	// παρά μόνο η θέση που βρίσκεται το αντικέιμενο, οπότε αγνοούμε αυτό που επιστρέφει.
	search(map, node->key, &pos);
	// Η μεταβλητή pos περιέχει την πραγματική θέση του αντικειμένου στο map
	// Θέλουμε να βρούμε το επόμενο!
	pos++;
	// Προσπέλασέ το μέχρι να βρείς κλειδί που υπάρχει στο map
	while(map->array[pos]->state == EMPTY) {
		pos++;
		// Αν φτάσουμε στο τέλος του map, δεν υπάρχει επόμενο στοιχείο, οπότε επισρέφουμε null τιμη.
		if (pos == map->capacity) {
			return MAP_EOF;
		}
	}
	if (map->array[pos]->state == OCCUPIED)
		return map->array[pos];
	else
		return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}

// Διαφοροποίηση με map_find: Άν ο κόμβος δεν υπάρχει, επιτρέφουμε την σταθερά MAP_EOF
MapNode map_find_node(Map map, Pointer key) {
	int pos = map->hash_function(key) % map->capacity;
	return search(map, key, &pos);
}

int hash_int(const Pointer value) {
	return *(int*)value;
}