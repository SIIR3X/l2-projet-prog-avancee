#pragma once

#include <stdio.h>

#include "list.h"
#include "types.h"

list_t * stream_2_persons(char * filename, stream_mode_t mode);
list_t * stream_2_friendships(char * filename, stream_mode_t mode, list_t * Lpers);
void list_2_stream(char * filename, stream_mode_t mode, list_t * L, void (*write_func)(void *, FILE *, stream_mode_t));