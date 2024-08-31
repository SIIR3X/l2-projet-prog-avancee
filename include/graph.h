#pragma once

#include <stdbool.h>

#include "types.h"
#include "set.h"

#define BUD_END -1
#define BUDDY 1

/** Un graphe est une matrice d'adjacence complétée par une matrice de distance d'amitié */
typedef struct {
	int num_vertices;
	int * adjacencies;
	int * distances;
	int * gaps;			// Non utilisé
	int * connections;	// Non utilisé
	int far_from;		// Non utilisé
} graph_t;

graph_t * new_graph ( const int nb_vertices );

void del_graph ( graph_t ** ptrG );

graph_t * fscanf_graph ( const char * filename, const stream_mode_t mode );

bool symetric_graph( const graph_t * G );

void printf_graph ( const graph_t * G, const char * entete, const bool and_Distances );

void distance_calculus ( graph_t * G );

set_t * friends( const graph_t * G, const int p );

void BronKerbosch(const graph_t * G, set_t * R, set_t * P, set_t * X, set_t * C);

void graph_2_stream(FILE * stream, graph_t * G, const stream_mode_t mode);