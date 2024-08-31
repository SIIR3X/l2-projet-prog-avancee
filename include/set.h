#pragma once

typedef struct {
	int numelm;
	int max_numelm;
	int * data;
} set_t;

set_t * new_set( int max_numelm );

void dup_set( const set_t * S1, set_t * S2 );

set_t * singleton_set( const int elm );

void del_set( set_t ** ptrSet );

void print_set( const set_t * S, char * entete );

int numelm_set( const set_t * S );

int in_set( const int e, const set_t * S );

set_t * union_set( const set_t * S1, const set_t * S2 );

set_t * inter_set( const set_t * S1, const set_t * S2 );

void substract_set( set_t * S, const int e );

void add_set( set_t * S, const int e );