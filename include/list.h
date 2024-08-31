#pragma once

#include <stdbool.h>

#include "graph.h"
#include "elmlist.h"

/**
Type abstrait pour liste double liée modélisée par
- 2 pointeurs pointant vers la tête et la queue de la liste
- le nombre d'éléments que contient la liste
*/
typedef struct {
	list_elm_t * head, * tail;
	int numelm;
} list_t;

/****************
Constructors & co
****************/
list_t * new_list();

void del_list ( list_t ** ptrL, void (*ptrf) () );

bool empty_list ( list_t * L );

/********************
Accessors & modifiers
********************/
list_elm_t * get_head ( list_t * L );

list_elm_t * get_tail ( list_t * L );

void set_head( list_t * L, list_elm_t * E);

void set_tail( list_t * L, list_elm_t * E);

int get_numelm( list_t * L);

void incrnumelm( list_t * L);

void decrnumelm( list_t * L);

void cons ( list_t * L, void * datum );

void queue ( list_t * L, void * datum );

void insert_ordered ( list_t * L, void * datum, int (*cmp_ptrf) () );

void view_list( list_t * L, void (*ptrf) (), char * entete );

void * find ( list_t * L, void * datum, int (*ptrFct) () );

void remove_elm_from_list(list_t * L, void * target, int (*cmp_ptrf)(), void (*del_ptrf)(void *));

graph_t * list_2_graph(list_t * Lpers);