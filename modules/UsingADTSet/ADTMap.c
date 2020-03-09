///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω του ADT Set (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTSet.h"
#include "ADTMap.h"


struct map {
	Set set;
	CompareFunc compare;
	DestroyFunc destroy_key, destroy_value;
};

typedef struct map_node* MapNode;
struct map_node {
	Pointer key;
	Pointer value;
	Map owner;
};

static int compare_map_nodes(MapNode a, MapNode b) {
	return a->owner->compare(a->key, b->key);
}

// Συνάρτηση που καταστρέφει ένα map node
static void destroy_map_node(MapNode node) {
	if (node->owner->destroy_key != NULL)
		node->owner->destroy_key(node->key);

	if (node->owner->destroy_value != NULL)
		node->owner->destroy_value(node->value);

	free(node);
}

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	Map map = malloc(sizeof(*map));
	map->set = set_create((CompareFunc)compare_map_nodes, (DestroyFunc)destroy_map_node);
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;
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

void map_insert(Map map, Pointer key, Pointer value) {
	struct map_node search_node = { .key = key, .owner = map };

	MapNode node = set_find(map->set, &search_node);
	if (node != NULL) {
		if (key != node->key && map->destroy_key != NULL)
			map->destroy_key(node->key);

		if (value != node->value && map->destroy_value != NULL)
			map->destroy_value(node->value);

		node->key = key;
		node->value = value;
		return;
	}

	node = malloc(sizeof(*node));
	node->key = key;
	node->value = value;
	node->owner = map;

	set_insert(map->set, node);
}

bool map_remove(Map map, Pointer key) {
	struct map_node search_node = { .key = key, .owner = map };

	MapNode node = set_find(map->set, &search_node);
	if (node == NULL)
		return false;

	set_remove(map->set, node);

	return true;
}

DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = map->destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

void map_destroy(Map map) {
	// destroy το set, τα περιεχόμενα θα τα κάνει free η destroy_map_node
	set_destroy(map->set);

	// free το ίδιο το map
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



//// Dummy υλοποίηση των συναρτήσεων που αφορούν hashing, αφού η συγκεκριμένη υλοποίηση του ADTMap δε χρησιμοποιεί hashing

// LCOV_EXCL_START (δεν πρόκειται να κληθούν)
uint hash_string(Pointer value) {
	return 0;
}
uint hash_int(Pointer value) {
	return 0;
}
uint hash_pointer(Pointer value) {
	return 0;
}
// LCOV_EXCL_STOP

void map_set_hash_function(Map map, HashFunc hash_func) {
}