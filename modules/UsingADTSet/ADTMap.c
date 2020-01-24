///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω του ADT Set (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include "stdlib.h"
#include "assert.h"

#include "ADTSet.h"
#include "ADTMap.h"


struct map {
	Set set;
	CompareFunc compare;
};

typedef struct map_node* MapNode;
struct map_node {
	Pointer key;
	Pointer value;
	Map owner;
};

int compare_map_nodes(MapNode a, MapNode b) {
	return a->owner->compare(a->key, b->key);
}

Map map_create(CompareFunc compare) {
	Map map = malloc(sizeof(*map));
	map->set = set_create((CompareFunc)compare_map_nodes);
	map->compare = compare;
	return map;
}

int map_size(Map map) {
	return set_size(map->set);
}

Pointer map_find(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	MapNode node = set_find(map->set, &search_node);
	return node == NULL ? NULL : node->value;
}

bool map_insert(Map map, Pointer key, Pointer value) {
	struct map_node search_node = { .key = key, .owner = map };

	MapNode node = set_find(map->set, &search_node);
	if(node != NULL) {
		node->value = value;
		return false;
	}

	node = malloc(sizeof(*node));
	node->key = key;
	node->value = value;
	node->owner = map;

	set_insert(map->set, node);

	return true;
}

bool map_remove(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	MapNode node = set_find(map->set, &search_node);
	if(node == NULL)
		return false;

	set_remove(map->set, node);		// πριν το free!
	free(node);

	return true;
}

// Επιστρέφει array με όλα τα keys
Pointer* get_all_keys(Map map) {
	Pointer* keys = malloc(set_size(map->set) * sizeof(*keys));
	int i = 0;
	for(MapNode node = map_first(map); node != MAP_EOF; node = map_next(map, node))
		keys[i++] = node->key;

	return keys;
}

void map_destroy(Map map, bool free_keys, bool free_values) {
	// Δεν μπορούμε να κάνουμε free τα keys όσο είναι μέσα στο set, οπότε
	// αν free_keys == true, τα αποθηκεύουμε για να τα κάνουμε free μετά.
	Pointer* keys = free_keys ? get_all_keys(map) : NULL;

	int size = map_size(map);		// αποθήκευση για να το έχουμε μετά το destroy!

	// Από την άλλη τα values μπορούμε να τα κάνουμε free αμέσως
	if(free_values)
		for(MapNode node = map_first(map); node != MAP_EOF; node = map_next(map, node))
			free(node->value);

	// destroy το set, μαζί με τα values του (τα map nodes δηλαδή)
	set_destroy(map->set, true);

	// τώρα μπορούμε να διαγράψουμε τα keys
	if(free_keys) {
		for(int i = 0; i < size; i++)
			free(keys[i]);
		free(keys);
	}

	// Τέλος free το ίδιο το map
	free(map);
}

MapNode map_find_node(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	return set_find(map->set, &search_node);
}

MapNode map_first(Map map) {
	SetNode node = set_first(map->set);
	return node == SET_EOF ? MAP_EOF : set_node_value(map->set, node);
}

MapNode map_next(Map map, MapNode node) {
	SetNode set_node = set_find_node(map->set, node);
	SetNode next_node = set_next(map->set, set_node);
	return next_node == SET_EOF ? MAP_EOF : set_node_value(map->set, next_node);
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}