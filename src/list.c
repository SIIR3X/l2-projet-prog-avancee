#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "list.h"
#include "graph.h"
#include "person.h"
#include "elmlist.h"



/*********************************
DÉCLARATIONS DES FONCTIONS PRIVÉES
*********************************/
void insert_after(list_t * L, void * datum, list_elm_t * place);
int find_person_index(list_t * Lpers, person_t * person);


/**********************************
DÉFINITIONS DES FONCTIONS PUBLIQUES
**********************************/

/**
 * @brief Crée une nouvelle liste et la retourne.
 *
 * Cette fonction alloue dynamiquement une nouvelle liste, l'initialise et retourne un pointeur vers la liste créée.
 *
 * @return Un pointeur vers la nouvelle liste allouée.
 */
list_t * new_list () {
	list_t * L = calloc ( 1, sizeof(list_t) );
	assert (L);
	return L;
}


/**
 * @brief Supprime une liste et libère la mémoire associée, y compris les éléments de la liste.
 *
 * Cette fonction supprime la liste spécifiée et libère la mémoire associée, y compris tous les éléments de la liste,
 * en utilisant une fonction de libération spécifiée pour les données stockées dans les éléments de la liste.
 *
 * @param ptrL Un pointeur vers un pointeur de liste à supprimer et à mettre à NULL après la suppression.
 * @param ptrf Un pointeur vers une fonction de libération des données stockées dans les éléments de la liste (peut être NULL).
 */
void del_list ( list_t ** ptrL, void (*ptrf) () ) {
	assert (ptrL && *ptrL);
	for(list_elm_t * iterator = get_head((*ptrL)); iterator; ) {
		// note that there is no iteration step
		list_elm_t * E = iterator;

		// The iteration step needs to be defined here
		iterator = get_suc(iterator);
		del_list_elm (&E, ptrf);
	}
	free (*ptrL);
	*ptrL = NULL;
}


/**
 * @brief Vérifie si une liste est vide.
 *
 * Cette fonction vérifie si la liste spécifiée est vide en examinant le nombre d'éléments de la liste.
 *
 * @param L Un pointeur vers la liste à vérifier.
 * @return true si la liste est vide, sinon false.
 */
bool empty_list ( list_t * L ) {
	assert (L);
	return L->numelm == 0;
}


/**
 * @brief Obtient la tête (premier élément) d'une liste.
 *
 * Cette fonction retourne un pointeur vers la tête (premier élément) de la liste spécifiée.
 *
 * @param L Un pointeur vers la liste dont on souhaite obtenir la tête.
 * @return Un pointeur vers la tête (premier élément) de la liste spécifiée, ou NULL si la liste est vide.
 */
list_elm_t * get_head ( list_t * L ) {
	assert (L);
	return L->head;
}


/**
 * @brief Obtient la queue (dernier élément) d'une liste.
 *
 * Cette fonction retourne un pointeur vers la queue (dernier élément) de la liste spécifiée.
 *
 * @param L Un pointeur vers la liste dont on souhaite obtenir la queue.
 * @return Un pointeur vers la queue (dernier élément) de la liste spécifiée, ou NULL si la liste est vide.
 */
list_elm_t * get_tail ( list_t * L ) {
	assert (L);
	return L->tail;
}


/**
 * @brief Définit la tête (premier élément) d'une liste.
 *
 * Cette fonction définit la tête (premier élément) de la liste spécifiée avec l'élément donné.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite définir la tête.
 * @param E Un pointeur vers l'élément que l'on souhaite définir comme tête de la liste.
 */
void set_head( list_t * L, list_elm_t * E){
	assert(L);
	L->head = E;
}


/**
 * @brief Définit la queue (dernier élément) d'une liste.
 *
 * Cette fonction définit la queue (dernier élément) de la liste spécifiée avec l'élément donné.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite définir la queue.
 * @param E Un pointeur vers l'élément que l'on souhaite définir comme queue de la liste.
 */
void set_tail( list_t * L, list_elm_t * E){
	assert(L);
	L->tail = E;
}


/**
 * @brief Obtient le nombre d'éléments dans une liste.
 *
 * Cette fonction retourne le nombre d'éléments actuellement présents dans la liste spécifiée.
 *
 * @param L Un pointeur vers la liste dont on souhaite obtenir le nombre d'éléments.
 * @return Le nombre d'éléments dans la liste spécifiée.
 */
int get_numelm( list_t * L){
	assert(L);
	return L->numelm;
}


/**
 * @brief Incrémente le nombre d'éléments dans une liste.
 *
 * Cette fonction incrémente le nombre d'éléments présents dans la liste spécifiée.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite incrémenter le nombre d'éléments.
 */
void incrnumelm( list_t * L){
	assert(L);
	L->numelm++;
}


/**
 * @brief Décrémente le nombre d'éléments dans une liste.
 *
 * Cette fonction décrémente le nombre d'éléments présents dans la liste spécifiée.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite décrémenter le nombre d'éléments.
 */
void decrnumelm( list_t * L){
	assert(L);
	L->numelm--;
}


/**
 * @brief Ajoute un nouvel élément en tête de la liste.
 *
 * Cette fonction ajoute un nouvel élément contenant les données spécifiées en tête de la liste spécifiée.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite ajouter un nouvel élément en tête.
 * @param datum Un pointeur vers les données à stocker dans le nouvel élément.
 */
void cons ( list_t * L, void * datum ) {
	assert (L);

	list_elm_t * E = new_list_elm ( datum );
	set_suc(E, get_head(L));

	if( empty_list ( L ) ) {
		set_tail(L, E);
	} 
	else {
		set_pred(get_head(L), E);
	}

	set_head(L, E);
	incrnumelm(L);
}


/**
 * @brief Ajoute un nouvel élément en queue de la liste.
 *
 * Cette fonction ajoute un nouvel élément contenant les données spécifiées en queue de la liste spécifiée.
 *
 * @param L Un pointeur vers la liste à laquelle on souhaite ajouter un nouvel élément en queue.
 * @param datum Un pointeur vers les données à stocker dans le nouvel élément.
 */
void queue ( list_t * L, void * datum ) {
	assert (L);

	list_elm_t * E = new_list_elm ( datum );
	set_pred(E, get_tail(L));

	if ( empty_list ( L ) ) {
		set_head(L, E);
	} 
	else {
		set_suc(get_tail(L), E);
	}

	set_tail(L, E);
	incrnumelm(L);
}


/**
 * @brief Insère des données de manière ordonnée dans la liste en utilisant une fonction de comparaison.
 *
 * Cette fonction insère les données spécifiées dans la liste de manière ordonnée en utilisant une fonction
 * de comparaison spécifiée pour déterminer l'ordre.
 *
 * @param L Un pointeur vers la liste dans laquelle on souhaite insérer les données.
 * @param datum Un pointeur vers les données à insérer.
 * @param cmp_ptrf Un pointeur vers une fonction de comparaison pour déterminer l'ordre (doit être définie).
 */
void insert_ordered ( list_t * L, void * datum, int (*cmp_ptrf) () ) {
	assert ( cmp_ptrf );

	if ( empty_list ( L ) || (*cmp_ptrf) ( datum, get_data(get_head(L)) ) <= 0 ) {
		cons ( L, datum );
	} 
	else if ( (*cmp_ptrf) ( get_data(get_tail(L)) , datum ) <= 0 ) {
		queue ( L, datum );
	} 
	else {
		list_elm_t * iterator = get_suc(get_head(L));
		while ( (*cmp_ptrf) ( get_data(iterator) , datum ) < 0 ) {
		iterator = get_suc(iterator);
		}
		insert_after ( L, datum, get_pred(iterator) );
	}
}


/**
 * @brief Recherche des données dans la liste en utilisant une fonction de comparaison.
 *
 * Cette fonction recherche les données spécifiées dans la liste en utilisant une fonction de comparaison
 * spécifiée pour déterminer si les données correspondent à un élément de la liste.
 *
 * @param L Un pointeur vers la liste dans laquelle on souhaite effectuer la recherche.
 * @param datum Un pointeur vers les données à rechercher.
 * @param ptrFct Un pointeur vers une fonction de comparaison pour vérifier si les données correspondent (doit être définie).
 * @return Un pointeur vers les données correspondantes dans la liste, ou NULL si aucune correspondance n'est trouvée.
 */
void * find ( list_t * L, void * datum, int (*ptrFct) () ) {
	assert (L && ptrFct);

	for ( list_elm_t * E =  get_head(L) ; E; E = get_suc(E) ) {
		if ( (*ptrFct) ( datum, get_data(E) ) == 0 ) return get_data(E);
	}

	return NULL;
}


/**
 * @brief Affiche une ligne de caractères égaux pour la mise en forme.
 *
 * Cette fonction affiche une ligne de caractères égaux pour la mise en forme dans la sortie standard.
 *
 * @param length La longueur de la ligne à afficher (nombre de caractères égaux).
 */
void view_ligne ( int length ) {
	printf ( "\t\t" );
	for(int c=0; c < length; c += 1 ) printf ( "=");
	printf ( "\n" );
}


/**
 * @brief Affiche les éléments d'une liste en utilisant une fonction de visualisation.
 *
 * Cette fonction affiche les éléments de la liste spécifiée en utilisant une fonction de visualisation
 * et une entête spécifiée pour la mise en forme.
 *
 * @param L Un pointeur vers la liste dont on souhaite afficher les éléments.
 * @param view_fct_ptr Un pointeur vers une fonction de visualisation des éléments de la liste (doit être définie).
 * @param entete Une chaîne de caractères représentant l'entête pour la mise en forme.
 */
void view_list ( list_t * L, void (*view_fct_ptr) (), char * entete) {
	int lg = strlen ( entete ) + 5;

	view_ligne ( lg );
	printf ( "\t\t|| %s ||\n", entete );
	view_ligne ( lg );

	if ( empty_list ( L ) ) {
		printf("[ ] //empty list\n");
	}
	else {
		list_elm_t * iterator = get_head(L);

		while ( iterator ) {
		(*view_fct_ptr) ( get_data(iterator) );
				printf ( "\n" );
		iterator = get_suc(iterator);
		}
	}
	view_ligne ( lg );
}


/**
 * @brief Supprime un élément de la liste s'il existe, en utilisant une fonction de comparaison.
 *
 * Cette fonction recherche un élément cible dans la liste spécifiée en utilisant une fonction de comparaison
 * fournie (`cmp_ptrf`). Si l'élément est trouvé, il est supprimé de la liste. La variable `prevElm` est utilisée
 * pour garder une trace de l'élément précédant l'élément actuel dans la liste pendant la recherche. Cela permet
 * de relier correctement l'élément précédent à l'élément suivant lors de la suppression, pour maintenir l'intégrité
 * de la liste chaînée. Après la suppression, le nombre d'éléments dans la liste est mis à jour. Si une fonction de 
 * suppression (`del_ptrf`) est fournie, elle est utilisée pour libérer la donnée (`datum`) contenue dans l'élément de 
 * liste en plus de libérer l'élément de liste lui-même. Si `del_ptrf` est `NULL`, seule la mémoire de l'élément de liste
 * est libérée.
 *
 * @param L Un pointeur vers la liste à partir de laquelle supprimer l'élément.
 * @param target Un pointeur vers l'élément cible à rechercher et supprimer.
 * @param cmp_ptrf Une fonction de comparaison pour déterminer si un élément est égal à la cible.
 * @param del_ptrf Une fonction optionnelle pour libérer la donnée de l'élément cible (peut être NULL).
 */
void remove_elm_from_list(list_t * L, void * target, int (*cmp_ptrf)(), void (*del_ptrf)(void *)){
	assert(L && target && cmp_ptrf);

	// Recherche de la cible dans la liste
	list_elm_t * currentElm = get_head(L);
	list_elm_t * prevElm = NULL;
	while (currentElm != NULL && (*cmp_ptrf)(get_data(currentElm), target) != 0) {
		prevElm = currentElm;
		currentElm = get_suc(currentElm);
	}

	// Si la cible a été trouvée dans la liste, alors on la supprime de la liste
	if (currentElm != NULL){
		// On détache l'élément de la liste
		if (prevElm) {
			set_suc(prevElm, get_suc(currentElm));
		} 
		else {
			set_head(L, get_suc(currentElm));
		}

		// Mise à jour des pointeurs
		if (get_suc(currentElm)) {
			set_pred(get_suc(currentElm), prevElm);
		} 
		else {
			set_tail(L, prevElm);
		}
		
		decrnumelm(L);

		// Si une fonction de suppression est fournie, on supprime la donnée de l'élément
		if (del_ptrf != NULL) {
			(*del_ptrf)(get_data(currentElm));  // Suppression de la donnée
		}

		// Libération de l'élément de la liste (list_elm_t)
		free(currentElm);
	}
}


/**
 * @brief Convertit une liste de personnes en une matrice d'adjacence asymétrique.
 *
 * Cette fonction prend une liste de personnes `Lpers` et la convertit en une matrice d'adjacence `G`. Chaque personne
 * dans la liste correspond à un sommet dans la matrice, et les amitiés entre les personnes sont représentées par
 * des valeurs non nulles dans la matrice. La fonction parcourt la liste des personnes et leurs amis pour créer les
 * valeurs non nulles dans la matrice d'adjacence. Contrairement à la version symétrique, cette version n'ajoute pas la
 * symétrie pour refléter les amitiés mutuelles dans la matrice.
 * Ensuite, elle calcule les distances entre les sommets de la matrice.
 *
 * @param Lpers Un pointeur vers la liste de personnes.
 * @return Un pointeur vers la matrice d'adjacence résultante.
 */
graph_t * list_2_graph(list_t * Lpers) {
	assert(Lpers);

	// Création du graphe avec un sommet pour chaque personne
	graph_t * G = new_graph(get_numelm(Lpers));
	assert(G);

	int i = 0;

	for (list_elm_t * currentPersElm = get_head(Lpers); currentPersElm != NULL; currentPersElm = get_suc(currentPersElm), i++) {
		person_t * currentPers = get_data(currentPersElm);

		for (list_elm_t * currentFriendElm = get_head(currentPers->friends); currentFriendElm != NULL; currentFriendElm = get_suc(currentFriendElm)) {

			int friend_index = find_person_index(Lpers, get_data(currentFriendElm));

			if (friend_index != -1) {
				// Ajoute une valeur non nulle dans la matrice d'adjacence
				G->adjacencies[i * G->num_vertices + friend_index] = 1;
				
				// Contrairement à la version symétrique, on n'ajoute pas la symétrie dans la matrice
				// G->adjacencies[friend_index * G->num_vertices + i] = 1; Ajout de la symétrie
			}
		}
	}

	// Calcule les distances entre les sommets de la matrice
	distance_calculus(G);

	return G;
}


/*******************************************
Définitions des fonctions privées du TA list
*******************************************/

/**
 * @brief Insère des données après un élément spécifié dans la liste.
 *
 * Cette fonction insère les données spécifiées après un élément donné dans la liste spécifiée.
 *
 * @param L Un pointeur vers la liste dans laquelle on souhaite effectuer l'insertion.
 * @param datum Un pointeur vers les données à insérer.
 * @param place Un pointeur vers l'élément après lequel les données doivent être insérées (peut être NULL pour insérer en tête).
 */
void insert_after ( list_t * L, void * datum, list_elm_t * place) {
	assert (L);

	if ( empty_list ( L ) || !place ) {
		cons ( L, datum );
	} 
	else if ( place == get_tail(L) ) {
		queue ( L, datum );
	} 
	else {
		list_elm_t * E = new_list_elm( datum );
		set_pred(E, place);
		set_suc(E, get_suc(place));
		set_pred(get_suc(place), E);
		set_suc(place, E);
		incrnumelm(L);
	}
}


/**
 * @brief Recherche l'index d'une personne dans la liste des personnes.
 *
 * Cette fonction recherche la personne donnée dans la liste des personnes `Lpers` en utilisant la fonction
 * de comparaison `cmp_person`. Si la personne est trouvée, la fonction renvoie son index dans la liste. Si
 * la personne n'est pas trouvée, la fonction renvoie -1 pour indiquer que la personne n'est pas dans la liste.
 *
 * @param Lpers Un pointeur vers la liste des personnes.
 * @param person Un pointeur vers la personne à rechercher.
 * @return L'index de la personne si trouvée, sinon -1.
 */
int find_person_index(list_t * Lpers, person_t * person) {
	int index = 0;
	list_elm_t * currentPersElm = get_head(Lpers);

	while(currentPersElm != NULL){
		if (cmp_person(get_data(currentPersElm), person) == 0){
			return index;
		}
		currentPersElm = get_suc(currentPersElm);
		index++;
	}

	return -1; // Retourne -1 si la personne n'est pas trouvée
}