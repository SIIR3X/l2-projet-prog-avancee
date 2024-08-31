#pragma once

/**
Type abstrait modélisant un élément de liste contenant
- une donnée
- 2 pointeurs vers son prédécesseur et son successeur
*/
typedef struct  list_elm {
	void * datum;
	struct  list_elm * suc;
	struct  list_elm * pred;
} list_elm_t;

list_elm_t * new_list_elm ( void * datum );

void del_list_elm ( list_elm_t ** ptrE, void (*ptrf) () );

list_elm_t * get_suc ( list_elm_t * E );

list_elm_t * get_pred ( list_elm_t * E );

void * get_data ( list_elm_t * E );

void set_suc ( list_elm_t * E, list_elm_t * S );

void set_pred ( list_elm_t * E, list_elm_t * P );

void set_data ( list_elm_t * E, void * datum );

void view_list_elm ( list_elm_t * E, void (*ptrf)() );