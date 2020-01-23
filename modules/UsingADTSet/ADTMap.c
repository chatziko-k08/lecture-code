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

bool map_exists(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	return set_exists(map->set, &search_node);
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

void map_destroy(Map map) {
	// Κάνουμε free τα στοιχεία του set (που είναι set nodes)
	// Αλλά προσοχή!!! Δεν μπορούμε να κάνουμε free ένα στοιχείο που είναι ακόμα μέσα στο set
	// γιατί οι συναρτήσεις του set καλούν την compare πάνω σε αυτό το στοιχείο!
	//
	SetNode node = set_first(map->set);
	while(node != SET_END) {							// while, γιατί πρέπει να πάρουμε το next
		SetNode next = set_next(map->set, node);		// πριν κάνουμε remove!

		// αφαίρεση του στοιχείου από το set
		MapNode map_node = set_node_value(map->set, node);
		set_remove(map->set, map_node);

		// πλέον μπορούμε να κάνουμε free!
		free(map_node);

		// συνεχίζουμε με το next, που το είχαμε βρει πριν κάνουμε remove
		node = next;
	}

	set_destroy(map->set);
	free(map);
}

MapNode map_find_node(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	return set_find(map->set, &search_node);
}

MapNode map_first(Map map) {
	SetNode node = set_first(map->set);
	return node == SET_END ? MAP_END : set_node_value(map->set, node);
}

MapNode map_next(Map map, MapNode node) {
	SetNode set_node = set_find_node(map->set, node);
	SetNode next_node = set_next(map->set, set_node);
	return next_node == SET_END ? MAP_END : set_node_value(map->set, next_node);
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}