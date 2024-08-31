#pragma once

#include "list.h"
#include "types.h"

/*
	Modélisation statique des chaînes de caractères
	afin d'utiliser au mieux les fichiers binaires
*/
#define LG 40

typedef struct {
	char name[LG];
	char forename[LG];
	struct {
		int day;
		int month;
		int year;
	} birth_date;
	list_t * friends;
} person_t;

person_t * new_person ();
person_t * scan_person ();
void free_person (person_t ** ptrP, list_t * Lpers, list_t * Lfriends );

int cmp_person (person_t * P1, person_t * P2 );
void print_person ( person_t * P );

person_t * stream_2_person ( FILE * stream, stream_mode_t mode );
void person_2_stream ( person_t * P, FILE * fd, stream_mode_t mode );

void free_person_wrapper(void *ptr);