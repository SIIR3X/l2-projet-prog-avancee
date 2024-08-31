#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "set.h"



/**
 * @brief Crée un nouvel ensemble avec une capacité maximale spécifiée.
 *
 * Cette fonction crée un nouvel ensemble avec la capacité maximale spécifiée. L'ensemble est initialisé
 * avec zéro élément. Si la capacité maximale est inférieure ou égale à zéro, la fonction renvoie NULL.
 *
 * @param max_numelm La capacité maximale de l'ensemble.
 * @return Un pointeur vers le nouvel ensemble créé, ou NULL en cas d'erreur.
 */
set_t * new_set( int max_numelm ){
	assert(max_numelm >= 0);

	set_t * S = calloc(1, sizeof(set_t));
	assert(S);

	// Initialisation du nombre d'éléments à zéro
	S->numelm = 0;

	// Initialisation de la capacité maximale de l'ensemble
	S->max_numelm = max_numelm;

	// Allocation de la mémoire pour le tableau des données et initialisation à zéro (avec calloc)
	S->data = calloc(max_numelm, sizeof(int));
	if (S->data == NULL){
		free(S);
		return NULL;
	}

	return S;
}


/**
 * @brief Duplique les éléments d'un ensemble source vers un ensemble de destination.
 *
 * Cette fonction copie les éléments de l'ensemble source `S1` vers l'ensemble de destination `S2`.
 * Les deux ensembles doivent déjà avoir été créés et alloués avec une capacité suffisante dans `S2`
 * pour accueillir les éléments de `S1`. Le nombre d'éléments de `S2` sera mis à jour pour refléter
 * le nombre d'éléments dupliqués.
 *
 * @param S1 L'ensemble source à dupliquer.
 * @param S2 L'ensemble de destination où les éléments seront copiés.
 */
void dup_set( const set_t * S1, set_t * S2 ){
	// On vérifie si l'ensemble S2 peut acceuillir les éléments de l'ensemble S1
	assert(S1 && S2 && S2->max_numelm >= S1->numelm);

	// Copie du nombre d'éléments de S1 dans S2
	S2->numelm = S1->numelm;

	// Duplication des éléments de S1 dans S2
	for (int i = 0; i < S1->numelm; ++i) {
		S2->data[i] = S1->data[i];
	}

	// Initialisation du reste des éléments de S2 à zéro ou à une valeur appropriée si nécessaire
	for (int i = S1->numelm; i < S2->max_numelm; ++i) {
		S2->data[i] = 0; // ou une autre valeur par défaut
	}
}


/**
 * @brief Crée un ensemble (set) contenant un unique élément.
 *
 * Cette fonction crée un ensemble (set) contenant un unique élément spécifié `elm`. L'ensemble a une capacité maximale de 1,
 * car il s'agit d'un singleton. L'élément `elm` est ajouté à cet ensemble et devient le seul élément de celui-ci.
 *
 * @param elm L'élément unique à ajouter à l'ensemble.
 * @return Un pointeur vers le nouvel ensemble contenant l'élément unique, ou NULL en cas d'erreur.
 */
set_t * singleton_set( const int elm ){
	assert(elm);

	// On crée un set avec un max_numelm de 1 puisqu'il s'agit d'un singleton
	set_t * S = new_set(1);

	// Initialisation des données du set
	S->numelm = 1;
	S->data[0] = elm;

	return S;
}


/**
 * @brief Libère la mémoire allouée pour un ensemble (set) et ses données.
 *
 * Cette fonction libère la mémoire allouée pour un ensemble (set) spécifié, y compris les données stockées dans cet ensemble.
 * Après l'appel à cette fonction, l'ensemble spécifié n'est plus valide et son pointeur est défini sur NULL.
 *
 * @param ptrSet Un pointeur vers un pointeur d'ensemble (set) à libérer.
 */
void del_set( set_t ** ptrSet ){
	assert(ptrSet && *ptrSet);

	// Libération des données si elles existent
	if ((*ptrSet)->data != NULL){
		free((*ptrSet)->data);
	}

	// Libération de la mémoire allouée pour la structure `set_t`
	free(*ptrSet);
	*ptrSet = NULL;
}


/**
 * @brief Affiche les éléments d'un ensemble (set) avec un en-tête facultatif.
 *
 * Cette fonction affiche les éléments d'un ensemble (set) spécifié. Un en-tête facultatif peut être fourni pour
 * préciser le contexte de l'affichage. Les éléments de l'ensemble sont affichés entre des accolades et séparés par
 * des espaces.
 *
 * @param S Un pointeur vers l'ensemble (set) à afficher.
 * @param entete Une chaîne de caractères représentant l'en-tête à afficher (peut être NULL pour aucune en-tête).
 */
void print_set( const set_t * S, char * entete ){
	assert(S);

	// Affichage de l'en-tête si fourni
	if (entete != NULL){
		printf("%s\n\n", entete);
	}

	// Affichage des éléments de l'ensemble
	printf("{ ");
	for (int i = 0; i < S->numelm; i++){
		printf("%d ", S->data[i]);
	}
	printf("}\n");
}


/**
 * @brief Récupère le nombre d'éléments dans un ensemble (set).
 *
 * Cette fonction renvoie le nombre d'éléments présents dans l'ensemble (set) spécifié.
 *
 * @param S Un pointeur vers l'ensemble (set) dont on souhaite obtenir le nombre d'éléments.
 * @return Le nombre d'éléments présents dans l'ensemble.
 */
int numelm_set( const set_t * S ){
	assert(S);
	return S->numelm;
}


/**
 * @brief Vérifie si un élément donné existe dans un ensemble (set).
 *
 * Cette fonction vérifie si l'élément spécifié existe dans l'ensemble (set) donné. Si l'élément est présent dans l'ensemble,
 * la fonction renvoie 1, sinon elle renvoie 0.
 *
 * @param e L'élément à rechercher dans l'ensemble.
 * @param S Un pointeur vers l'ensemble (set) dans lequel effectuer la recherche.
 * @return 1 si l'élément existe dans l'ensemble, 0 sinon.
 */
int in_set( const int e, const set_t * S ){
	assert(S);

	// Parcours de l'ensemble S
	for (int i = 0; i < S->numelm; i++){
		if (S->data[i] == e){
			return 1;
		}
	}

	return 0;
}


/**
 * @brief Crée un nouvel ensemble contenant l'union de deux ensembles donnés.
 *
 * Cette fonction crée un nouvel ensemble (set) qui contient l'union des éléments de deux ensembles donnés, S1 et S2.
 * L'ensemble résultant contiendra tous les éléments de S1 et tous les éléments de S2, sans doublons.
 *
 * @param S1 Un pointeur vers le premier ensemble.
 * @param S2 Un pointeur vers le deuxième ensemble.
 * @return Un pointeur vers le nouvel ensemble contenant l'union des éléments de S1 et S2.
 */
set_t * union_set( const set_t * S1, const set_t * S2 ){
	assert(S1 && S2);

	// Création d'un ensemble de taille max S1->max_numelm + S2->max_numelm
	set_t * S_union = new_set(S1->max_numelm + S2->max_numelm);
	assert(S_union);

	// Ajout des éléments de S1 à l'ensemble union
	for (int i = 0; i < S1->numelm; i++){
		add_set(S_union, S1->data[i]);
	}

	// Ajout des éléments de S2 à l'ensemble union
	for (int i = 0; i < S2->numelm; i++){
		add_set(S_union, S2->data[i]);
	}

	return S_union;
}


/**
 * @brief Crée un nouvel ensemble contenant l'intersection de deux ensembles donnés.
 *
 * Cette fonction crée un nouvel ensemble (set) qui contient l'intersection des éléments de deux ensembles donnés, S1 et S2.
 * L'ensemble résultant contiendra uniquement les éléments communs à S1 et S2.
 *
 * @param S1 Un pointeur vers le premier ensemble.
 * @param S2 Un pointeur vers le deuxième ensemble.
 * @return Un pointeur vers le nouvel ensemble contenant l'intersection des éléments de S1 et S2.
 */
set_t * inter_set( const set_t * S1, const set_t * S2 ){
	assert(S1 && S2);

	// Compare le nombre d'éléments dans S1 et S2 et garde le plus petit des deux
	int max_capacity = S1->numelm < S2->numelm ? S1->numelm : S2->numelm;

	// Création d'un ensemble de taille max_capacity
	set_t * S_inter = new_set(max_capacity);
	assert(S_inter);

	// Parcours de l'ensemble S1
	for (int i = 0; i < S1->numelm; i++){
		// Si l'élément i est également dans l'ensemble S2 alors on l'ajoute à S_inter
		if (in_set(S1->data[i], S2) == 1){
			add_set(S_inter, S1->data[i]);
		}
	}

	return S_inter;
}


/**
 * @brief Supprime un élément spécifié d'un ensemble donné.
 *
 * Cette fonction supprime l'élément spécifié de l'ensemble S. Si l'élément n'est pas présent dans l'ensemble,
 * la fonction ne fait rien.
 *
 * @param S Un pointeur vers l'ensemble à modifier.
 * @param e L'élément à supprimer de l'ensemble.
 */
void substract_set( set_t * S, const int e ){
	assert(S);

	// Parcours de l'ensemble S
	for (int i = 0; i < S->numelm; i++){
		// Si l'élément e est présent dans l'ensemble alors on le supprime
		if (S->data[i] == e){
			// Décalage des valeurs de l'ensemble
			for (int j = i; j < S->numelm - 1; j++){
				S->data[j] = S->data[j + 1];
			}

			// Mise à jour de l'ensemble
			S->numelm--;
			S->data[S->numelm] = 0;

			break;
		}
	}
}


/**
 * @brief Ajoute un élément spécifié à un ensemble donné, s'il n'est pas déjà présent.
 *
 * Cette fonction ajoute l'élément spécifié à l'ensemble S, à condition que cet élément ne soit pas déjà présent
 * dans l'ensemble. Si l'ensemble est plein ou si l'élément est déjà présent, la fonction ne fait rien.
 *
 * @param S Un pointeur vers l'ensemble où ajouter l'élément.
 * @param e L'élément à ajouter à l'ensemble.
 */
void add_set( set_t * S, const int e ){
	assert(S);

	// Si l'élement e n'est pas déjà présent dans l'ensemble S
	if (in_set(e, S) == 0){
		// S'il reste de la place dans l'ensemble S, alors on ajoute l'élément à la fin de l'ensemble
		if (S->numelm < S->max_numelm){
			S->data[S->numelm] = e;
			S->numelm++;
		}
	}
}