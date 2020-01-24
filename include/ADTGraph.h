////////////////////////////////////////////////////////////////////////
//
// ADT Graph
//
// Abstract μη κατευθυνόμενος γράφος με βάρη.
//
////////////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"
#include "ADTList.h"			// Ορισμένες συναρτήσεις επιστρέφουν λίστες


// Ενα γράφος αναπαριστάται από τον τύπο Graph

typedef struct graph* Graph;

// Δείκτης σε συνάρτηση που επισκέπτεται τα στoιχεία του γράφου
typedef void (*GraphVisitFunc)(Graph graph, Pointer value);


// Δημιουργεί και επιστρέφει ένα γράφο, στο οποίο τα στοιχεία συγκρίνονται με βάση
// τη συνάρτηση compare.
// Αν destroy_vertex != NULL, τότε καλείται destroy_vertex(vertex) κάθε φορά που αφαιρείται μια κορυφή.

Graph graph_create(CompareFunc compare, DestroyFunc destroy_vertex);

// Επιστρέφει τον αριθμό στοιχείων (κορυφών) που περιέχει ο γράφος graph.

int graph_size(Graph graph);

// Προσθέτει μια κορυφή στο γράφο

void graph_insert_vertex(Graph graph, Pointer vertex);

// Επιστρέφει λίστα με όλες τις κορυφές του γράφου

List graph_get_vertices(Graph graph);

// Διαγράφει μια κορυφή από τον γράφο

void graph_remove_vertex(Graph graph, Pointer vertex);

// Προσθέτει μια ακμή με βάρος weight στο γράφο

void graph_insert_edge(Graph graph, Pointer vertex1, Pointer vertex2, int weight);

// Αφαιρεί μια ακμή από το γράφο

void graph_remove_edge(Graph graph, Pointer vertex1, Pointer vertex2);

// Επιστρέφει το βάρος της ακμής ανάμεσα στις δύο κορυφές, ή INT_MAX αν δεν είναι γειτονικές.

int graph_get_weight(Graph graph, Pointer vertex1, Pointer vertex2);

// Επιστρέφει λίστα με τους γείτονες μιας κορυφής

List graph_get_adjacent(Graph graph, Pointer vertex);

// Επιστρέφει (σε λίστα) το συντομότερο μονοπάτι ανάμεσα στις κορυφές source
// και target, ή κενή λίστα αν δεν υπάρχει κανένα μονοπάτι.

List graph_shortest_path(Graph graph, Pointer source, Pointer target);

// Καλεί τη visit(graph, vertex) για κάθε στοιχείο του γράφου, ξεκινώντας από
// την κορυφή vertex, και διασχίζοντας τον γράφο πρώτα κατά πλάτος (BFS)

void graph_bfs(Graph graph, Pointer vertex, GraphVisitFunc visit);

// Καλεί τη visit(graph, vertex) για κάθε στοιχείο του γράφου, ξεκινώντας από
// την κορυφή vertex, και διασχίζοντας τον γράφο πρώτα κατά βάθος (DFS)

void graph_dfs(Graph graph, Pointer vertex, GraphVisitFunc visit);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το γράφος.
// Οποιαδήποτε λειτουργία πάνω στο γράφο μετά το destroy είναι μη ορισμένη.

void graph_destroy(Graph graph);
