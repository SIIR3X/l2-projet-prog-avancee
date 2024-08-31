#pragma once

#include "list.h"
#include "types.h"
#include "person.h"

typedef struct {
	person_t * A;
	person_t * B;
} friendship_t;

friendship_t * new_friendship ();
friendship_t * scan_friendship (list_t * Lpers);
void free_friendship ( friendship_t ** ptrFriend, bool del_persons, list_t * Lpers, list_t * Lfriends );

int cmp_friendship ( friendship_t * F1, friendship_t * F2 );
void print_friendship ( friendship_t * F );

friendship_t * stream_2_friendship ( FILE * stream, stream_mode_t mode, list_t * Lpers, list_t * Lfriends );
void friendship_2_stream (friendship_t * F, FILE * fd, stream_mode_t mode);

void free_friendship_wrapper(void *ptr);