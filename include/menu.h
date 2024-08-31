#pragma once

#include "list.h"
#include "types.h"

int scan_choice(int min, int max);
void scan_file(char * filename, stream_mode_t * mode);

void person_menu(list_t * Lpers, list_t * Lfriends);
void friendship_menu(list_t * Lpers, list_t * Lfriends);
void matrix_menu(list_t * Lpers, list_t * Lfriends, graph_t ** Graph);