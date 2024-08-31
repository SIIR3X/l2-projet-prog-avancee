#include <stdio.h>
#include <assert.h>

#include "io.h"
#include "list.h"
#include "person.h"
#include "friendship.h"



/**
 * @brief Lit les données des personnes depuis un fichier (texte ou binaire) et les convertit en une liste de personnes.
 *
 * Cette fonction lit les données des personnes depuis un fichier spécifié, qu'il soit au format texte ou binaire.
 * Les informations de chaque personne sont lues séquentiellement à partir du fichier et converties en structures `person_t`. 
 * Ces structures sont ensuite ajoutées à une liste de personnes (`list_t`). 
 * La fonction continue de lire et d'ajouter des personnes tant qu'il y a des personnes valides dans le fichier.
 * Si une personne est présente plusieurs fois dans le fichier, elle ne sera ajoutée qu'une seule fois.
 *
 * @param filename Le nom du fichier à lire les données des personnes.
 * @param mode Le mode de lecture du fichier, soit TEXT (texte) soit BIN (binaire).
 * @return Un pointeur vers la liste de personnes résultante. En cas d'erreur lors de la lecture ou si le fichier est vide, la fonction renvoie NULL.
 */
list_t * stream_2_persons(char * filename, stream_mode_t mode){
	assert(filename);

	// Ouverture du fichier en fonction du mode sélectionné
	FILE * file = fopen(filename, mode == TEXT ? "r" : "rb");
	assert(file);

	// Création d'une nouvelle liste pour stocker les personnes
	list_t * Lpers = new_list();
	if (!Lpers) {
		fclose(file);
		return NULL;
	}

	// Tant que la fin du fichier n'est pas atteinte, on continue la lecture des personnes
	while (!feof(file)) {
		person_t * P = stream_2_person(file, mode);  // Lecture de la personne à l'aide de `stream_2_person`

		// Si aucun personne n'a été trouvée
		if (P == NULL) {
			// Vérifie si la fin du fichier a été atteinte ou s'il s'agit d'une erreur
			if (!feof(file)) {
				// Si ce n'est pas la fin du fichier, alors c'est une erreur
				del_list(&Lpers, free_person_wrapper); 
				fclose(file);
				return NULL;
			}
			break; // Fin du fichier atteinte sans erreurs
		}

		// Vérifie si la personne n'existe pas déjà dans la liste. Si elle n'existe pas, alors on l'ajoute (utile si on a deux fois la même personne dans un fichier)
		if (find(Lpers, P, &cmp_person) == NULL){
			cons(Lpers, P); 
		}
	}

	fclose(file);
	return Lpers;
}


/**
 * @brief Lit les données des amitiés depuis un fichier (texte ou binaire) et les convertit en une liste d'amitiés.
 *
 * Cette fonction lit les données des amitiés depuis un fichier spécifié, qu'il soit au format texte ou binaire.
 * Chaque ligne du fichier représente une amitié entre deux personnes. Les informations de chaque amitié sont lues
 * séquentiellement à partir du fichier et converties en structures `friendship_t`. Ces structures sont ensuite ajoutées
 * à une liste d'amitiés (`list_t`). La fonction continue de lire et d'ajouter des amitiés tant qu'il y a des amitiés valides dans le fichier.
 *
 * @param filename Le nom du fichier à lire les données des amitiés.
 * @param mode Le mode de lecture du fichier, soit TEXT (texte) soit BIN (binaire).
 * @param Lpers Une liste de personnes (`list_t`) contenant les informations sur les personnes impliquées dans les amitiés.
 * @return Un pointeur vers la liste d'amitiés résultante. En cas d'erreur lors de la lecture ou si le fichier est vide, la fonction renvoie NULL.
 */
list_t * stream_2_friendships(char * filename, stream_mode_t mode, list_t * Lpers){
	assert(filename && Lpers);

	// Ouverture du fichier en fonction du mode sélectionné
	FILE * file = fopen(filename, mode == TEXT ? "r" : "rb");
	assert(file);

	// Création d'une nouvelle liste pour stocker les personnes
	list_t * Lfriends = new_list();
	if (!Lfriends) {
		fclose(file);
		return NULL;
	}

	// Tant que la fin du fichier n'est pas atteinte, on continue la lecture des amitiés
	while (!feof(file)) {
		friendship_t * F = stream_2_friendship(file, mode, Lpers, Lfriends);

		// Si aucun personne n'a été trouvée
		if (F == NULL) {
			// Vérifie si la fin du fichier a été atteinte ou s'il s'agit d'une erreur
			if (!feof(file)) {
				// Si ce n'est pas la fin du fichier, alors c'est une erreur
				del_list(&Lfriends, free_friendship_wrapper);
				fclose(file);
				return NULL;
			}
			break; // Fin du fichier atteinte sans erreurs
		}
	}

	fclose(file);
	return Lfriends;
}


/**
 * @brief Écrit les éléments d'une liste dans un fichier en utilisant une fonction spécifiée.
 *
 * Cette fonction parcourt une liste et utilise une fonction fournie pour écrire les informations
 * de chaque élément de la liste dans un fichier spécifié. Si le fichier n'existe pas, il sera créé.
 * En mode texte (TEXT), les données sont ajoutées à la fin du fichier existant (append mode),
 * tandis qu'en mode binaire (BIN), le fichier existant est écrasé (write mode).
 *
 * @param filename Le nom du fichier dans lequel écrire les éléments de la liste.
 * @param mode Le mode d'écriture du fichier (TEXT pour texte, BIN pour binaire).
 * @param L La liste à écrire dans le fichier.
 * @param write_func La fonction à utiliser pour écrire chaque élément de la liste dans le fichier.
 */
void list_2_stream(char * filename, stream_mode_t mode, list_t * L, void (*write_func)(void *, FILE *, stream_mode_t)){
	assert(filename && L && write_func);

	FILE * file = fopen(filename, mode == TEXT ? "a" : "wb");
	assert(file);

	list_elm_t * currentElm = get_head(L);
	while (currentElm != NULL) {
		write_func(get_data(currentElm), file, mode);
		currentElm = get_suc(currentElm);
	}

	fclose(file);
}