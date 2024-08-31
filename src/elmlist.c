#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "elmlist.h"



/**
 * @brief Crée un nouvel élément de liste et le retourne.
 *
 * Cette fonction alloue dynamiquement un nouvel élément de liste, l'initialise avec les données spécifiées
 * et retourne un pointeur vers cet élément de liste.
 *
 * @param datum Un pointeur vers les données à stocker dans le nouvel élément de liste.
 * @return Un pointeur vers le nouvel élément de liste alloué.
 */
list_elm_t * new_list_elm ( void * datum ) {
	list_elm_t * E = calloc ( 1, sizeof(list_elm_t) );
	assert (E);
	set_data(E, datum);
	return E;
}


/**
 * @brief Supprime un élément de liste et libère la mémoire associée.
 *
 * Cette fonction supprime l'élément de liste spécifié et libère la mémoire associée.
 *
 * @param ptrE Un pointeur vers un pointeur d'élément de liste à supprimer et à mettre à NULL après la suppression.
 * @param ptrf Un pointeur vers une fonction de libération des données stockées dans l'élément (peut être NULL).
 */
void del_list_elm ( list_elm_t ** ptrE, void (*ptrf) () ) {
	assert (ptrE && *ptrE);
	if ( ptrf ) (*ptrf) ( get_data((*ptrE)) );
	free ( *ptrE );
	*ptrE = NULL;
}


/**
 * @brief Obtient l'élément successeur de l'élément donné.
 *
 * Cette fonction retourne l'élément successeur de l'élément spécifié.
 *
 * @param E Un pointeur vers l'élément dont on souhaite obtenir le successeur.
 * @return Un pointeur vers l'élément successeur de l'élément spécifié, ou NULL s'il n'y en a pas.
 */
list_elm_t * get_suc ( list_elm_t * E ) {
	assert (E);
	return E->suc;
}


/**
 * @brief Obtient l'élément prédécesseur de l'élément donné.
 *
 * Cette fonction retourne l'élément prédécesseur de l'élément spécifié.
 *
 * @param E Un pointeur vers l'élément dont on souhaite obtenir le prédécesseur.
 * @return Un pointeur vers l'élément prédécesseur de l'élément spécifié, ou NULL s'il n'y en a pas.
 */
list_elm_t * get_pred ( list_elm_t * E ) {
	assert (E);
	return E->pred;
}


/**
 * @brief Obtient les données stockées dans l'élément de liste donné.
 *
 * Cette fonction retourne un pointeur vers les données stockées dans l'élément de liste spécifié.
 *
 * @param E Un pointeur vers l'élément de liste dont on souhaite obtenir les données.
 * @return Un pointeur vers les données stockées dans l'élément de liste spécifié.
 */
void * get_data ( list_elm_t * E ) {
	assert (E);
	return E->datum;
}


/**
 * @brief Définit l'élément successeur d'un élément de liste.
 *
 * Cette fonction définit l'élément successeur de l'élément spécifié.
 *
 * @param E Un pointeur vers l'élément de liste auquel on souhaite définir un successeur.
 * @param S Un pointeur vers l'élément successeur à définir pour l'élément spécifié.
 */
void set_suc ( list_elm_t * E, list_elm_t * S ) {
	assert (E);
	E->suc = S;
}


/**
 * @brief Définit l'élément prédécesseur d'un élément de liste.
 *
 * Cette fonction définit l'élément prédécesseur de l'élément spécifié.
 *
 * @param E Un pointeur vers l'élément de liste auquel on souhaite définir un prédécesseur.
 * @param P Un pointeur vers l'élément prédécesseur à définir pour l'élément spécifié.
 */
void set_pred ( list_elm_t * E, list_elm_t * P ) {
	assert (E);
	E->pred = P;
}


/**
 * @brief Définit les données stockées dans un élément de liste.
 *
 * Cette fonction définit les données stockées dans l'élément de liste spécifié.
 *
 * @param E Un pointeur vers l'élément de liste auquel on souhaite définir les données.
 * @param data Un pointeur vers les données à stocker dans l'élément de liste spécifié.
 */
void set_data ( list_elm_t * E, void * data ) {
	assert (E);
	E->datum = data;
}


/**
 * @brief Affiche les données stockées dans un élément de liste à l'aide d'une fonction spécifiée.
 *
 * Cette fonction affiche les données stockées dans l'élément de liste spécifié en utilisant une fonction de
 * visualisation spécifiée.
 *
 * @param E Un pointeur vers l'élément de liste dont on souhaite afficher les données.
 * @param ptrf Un pointeur vers une fonction de visualisation des données (doit être définie).
 */
void view_list_elm ( list_elm_t * E, void (*ptrf)() ){
	assert(ptrf);
	(*ptrf)(get_data(E));
}