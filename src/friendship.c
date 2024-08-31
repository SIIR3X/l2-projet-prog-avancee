#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list.h"
#include "person.h"
#include "friendship.h"



#define LG 40

/**
	@note une zone tampon utile aux fichiers binaires
*/
typedef struct {
	char nameA[LG];
	char forenameA[LG];
	char nameB[LG];
	char forenameB[LG];
} buffer_t;


/**
 * @brief Crée une nouvelle relation d'amitié entre deux personnes.
 *
 * Cette fonction alloue dynamiquement une nouvelle structure `friendship_t` pour représenter une relation d'amitié entre deux
 * personnes. Elle initialise la relation et retourne un pointeur vers la structure créée.
 *
 * @return Un pointeur vers la nouvelle structure `friendship_t` représentant la relation d'amitié créée.
 */
friendship_t * new_friendship() {
	friendship_t * F = calloc(1, sizeof(friendship_t));
	assert(F);
	return F;
}


/**
 * @brief Saisit les détails d'une relation d'amitié entre deux personnes et les ajoute au réseau social de manière symétrique.
 *
 * Cette fonction permet à l'utilisateur de saisir des données de relation d'amitié entre deux personnes
 * à partir de l'entrée standard (clavier). Elle prend en compte la liste de personnes (Lpers) pour trouver
 * les personnes correspondantes et crée une nouvelle relation d'amitié entre elles si elle n'existe pas déjà.
 * Dans tous les cas, la fonction finit par renvoyer la relation d'amitié. L'amitié est ajoutée symétriquement,
 * ce qui signifie que si A est ami avec B, alors B est aussi ami avec A.
 *
 * @param Lpers Un pointeur vers la liste de personnes existantes.
 * @return Un pointeur vers la structure friendship_t représentant la relation d'amitié, ou NULL en cas d'erreur.
 */
friendship_t * scan_friendship(list_t * Lpers){
	friendship_t * F = new_friendship();
	assert(F);

	printf("\n Personne 1 (la personne amis avec P2) :\n\n");
	person_t * tempP = scan_person();
	person_t * P1 = find(Lpers, tempP, &cmp_person);
	assert(P1);
	free_person(&tempP, NULL, NULL);

	printf("\n Personne 2 (L'amis de P1):\n\n");
	tempP = scan_person();
	person_t * P2 = find(Lpers, tempP, &cmp_person);
	assert(P2);
	free_person(&tempP, NULL, NULL);

	// Permet de s'assurer que P est différent de P2
	assert(cmp_person(P1, P2) != 0);

	F->A = P1;
	F->B = P2;

	// Si jamais les personnes ne sont pas déjà amis, alors on ajoute l'amitié
	if (find(P1->friends, P2, &cmp_person) == NULL){
		cons(P1->friends, P2);
	}

	/*
	Contrairement à la version symétrique, on n'ajoute pas la personne 1 à la liste d'amis de la personne 2
	étant donné qu'on considère qu'une amitié peut ne pas être réciproque.

	if (find(P2->friends, P1, &cmp_person) == NULL){
		cons(P2->friends, P1);
	}
	*/

	return F;
}


/**
 * @brief Libère la mémoire associée à une structure `friendship_t` et, si spécifié, supprime également la personne impliquée dans l'amitié.
 *
 * Cette fonction gère la suppression d'une amitié et, en option, la suppression de la personne impliquée dans cette amitié. 
 * Elle retire d'abord la relation d'amitié entre les personnes A et B de la liste d'amis de A, puis, si demandé, 
 * libère la personne elle-même. La fonction gère aussi la suppression de l'amitié de la liste globale d'amitiés `Lfriends`.
 * Cela permet de s'assurer que toutes les références à l'amitié et à la personne impliquée sont correctement supprimées et que la mémoire est libérée.
 *
 * @note Si `del_person` est vrai, la personne impliquée est également libérée, et ses références sont retirées de `Lpers` et `Lfriends`.
 *
 * @param ptrFriend Un pointeur vers un pointeur vers la structure `friendship_t` à libérer.
 * @param del_person Un booléen indiquant si la personne impliquée doit être supprimée (`true`) ou si seule l'amitié doit être supprimée (`false`).
 * @param Lpers Un pointeur vers la liste de personnes, peut être `NULL` si la liste n'est pas fournie.
 * @param Lfriends Un pointeur vers la liste d'amitiés, peut être `NULL` si la liste n'est pas fournie.
 */
void free_friendship(friendship_t ** ptrFriend, bool del_person, list_t * Lpers, list_t * Lfriends){
	assert(ptrFriend && *ptrFriend);

	// Suppression de la personne B de la liste d'amis de la personne A
	remove_elm_from_list((*ptrFriend)->A->friends, (*ptrFriend)->B, &cmp_person, NULL);


	/*
	Contrairement à la version symétrique, on ne supprime pas la personne A de la liste d'amis de la personne B
	étant donné qu'une amitié se lit A->B et non B->A (-> pour 'est amis avec').

	Suppression de la personne A de la liste d'amis de la personne B
	remove_elm_from_list((*ptrFriend)->B->friends, (*ptrFriend)->A, &cmp_person, NULL);
	*/

	// Si une liste d'amitiés est passée en argument, alors on retire l'amitié de la liste
	if (Lfriends != NULL){
		// On supprime l'amitié de la liste
		remove_elm_from_list(Lfriends, (*ptrFriend), &cmp_friendship, NULL);
	}

	// Si le booleen del_person est vrai, alors on supprime les deux personnes
	if (del_person == true){
		// Suppression de la personne A
		free_person(&((*ptrFriend)->A), Lpers, Lfriends);

		/*
		Contrairement à la version symétrique, si le booleen del_person (et non del_persons) est vrai, on, supprime uniquement la personne A du réseau
		étant donné qu'on s'intéresse à l'amitié de A vers B uniquement. Supprimer les deux personnes n'est donc pas correct

		// Suppression de la personne B
		free_person(&((*ptrFriend)->B), Lpers, Lfriends);
		*/
	}

	// Libération de la mémoire allouée pour l'amitié elle-même
	free(*ptrFriend);
	*ptrFriend = NULL;
}


/**
 * @brief Compare deux relations d'amitié pour déterminer s'ils représentent la même relation de manière asymétrique.
 *
 * Cette fonction compare deux relations d'amitié spécifiées pour déterminer si elles représentent la même relation de manière
 * asymétrique. Elle vérifie si les personnes associées dans les deux relations sont les mêmes, mais considère que A->B est différent
 * de B->A. Si les deux relations représentent la même relation d'amitié de manière asymétrique, la fonction renvoie 0 ; sinon, elle
 * renvoie -1.
 *
 * @param F1 Un pointeur vers la première structure `friendship_t` à comparer.
 * @param F2 Un pointeur vers la seconde structure `friendship_t` à comparer.
 * @return 0 si les deux relations d'amitié sont identiques de manière asymétrique, 1 sinon.
 */
int cmp_friendship(friendship_t *F1, friendship_t *F2) {
	assert(F1 && F2);

    // Comparaison asymétrique
	// Ici on retire la comparaison symétrique : cmp_person(F1->A, F2->B) == 0 && cmp_person(F1->B, F2->A)
	if ((cmp_person(F1->A, F2->A) == 0 && cmp_person(F1->B, F2->B) == 0)) {
		return 0; // Les amitiés sont identiques
	}

	return -1; // Les amitiés ne sont pas identiques
}


/**
 * @brief Affiche les détails d'une relation d'amitié entre deux personnes.
 *
 * Cette fonction affiche les détails d'une relation d'amitié spécifiée entre deux personnes. Elle affiche les informations
 * sur les deux personnes impliquées dans la relation, en indiquant qu'elles sont amies.
 *
 * @param F Un pointeur vers la structure `friendship_t` représentant la relation d'amitié à afficher.
 */
void print_friendship(friendship_t * F){
	printf("___________Friendship___________\n");
	print_person(F->A);
	printf("\tEST AMIS AVEC\n");
	print_person(F->B);
	printf("________Friendship ended________\n");
}


/**
 * @brief Lit les données d'une amitié depuis un flux (texte ou binaire) et crée une structure `friendship_t`.
 *
 * Cette fonction lit les données d'une amitié depuis un flux spécifié, qu'il soit au format texte ou binaire,
 * et crée une nouvelle structure `friendship_t` pour stocker ces informations. Les informations de chaque amitié
 * sont lues séquentiellement à partir du flux et converties en structure `friendship_t`. Ces structures sont ensuite ajoutées
 * à une liste d'amitiés (`list_t`).
 *
 * Si l'amitié n'existe pas déjà dans la liste d'amitiés, elle est également ajoutée à la liste d'amitiés (`Lfriends`).
 * Cela permet de maintenir la cohérence entre les listes d'amis et les amitiés. Contrairement à la version symétrique,
 * l'amitié n'est pas ajoutée de manière réciproque. Cela signifie que si A est ami avec B, cela ne signifie pas nécessairement
 * que B est ami avec A.
 *
 * @param stream Un pointeur vers le flux depuis lequel lire les informations de l'amitié.
 * @param mode Le mode du flux (TEXT pour texte, BIN pour binaire).
 * @param Lpers La liste de toutes les personnes, nécessaire pour rechercher les personnes associées à l'amitié.
 * @param Lfriends La liste de toutes les amitiés existantes, nécessaire pour vérifier si l'amitié existe déjà.
 * @return Un pointeur vers la nouvelle structure `friendship_t` contenant les informations lues, ou NULL en cas d'erreur.
 */
friendship_t * stream_2_friendship(FILE * stream, stream_mode_t mode, list_t * Lpers, list_t * Lfriends){
	assert(stream && Lpers && Lfriends);

	friendship_t * F = new_friendship();
	assert(F);

	person_t * P1, * P2;
	buffer_t buffer;

	// Lecture en mode texte
	if (mode == TEXT){
		// S'il y'a une erreur lors de la lecture en mode texte
		if (fscanf(stream, "%s %s %s %s", buffer.nameA, buffer.forenameA, buffer.nameB, buffer.forenameB) != 4) {
			free(F);
			return NULL;
		}
	}
	// Lecture en mode binaire
	else if (mode == BIN){
		// S'il y'a une erreur lors de la lecture en mode binaire
		if (fread(&buffer, sizeof(buffer_t), 1, stream) != 1) {
			free(F);
			return NULL;
		}
	}

	person_t * tempP = new_person();

	// On vérifie que la personne 1 est bien présente sur le réseau social
	strcpy(tempP->name, buffer.nameA);
	strcpy(tempP->forename, buffer.forenameA);
	P1 = find(Lpers, tempP, &cmp_person);
	assert(P1);

	// On vérifie que la personne 2 est bien présente sur le réseau social
	strcpy(tempP->name, buffer.nameB);
	strcpy(tempP->forename, buffer.forenameB);
	P2 = find(Lpers, tempP, &cmp_person);
	assert(P2);

	free_person(&tempP, NULL, NULL);

	// Si les deux personnes sont présentes sur le réseau, alors on crée l'amitié
	F->A = P1;
	F->B = P2;

	// On recherche l'amitié dans la liste des amitiés
	friendship_t * existingFriendship = find(Lfriends, F, &cmp_friendship);

	// Si l'amitié n'existe pas déjà dans la liste d'amis, alors on crée l'amitié entre les deux personnes et on ajoute l'amitié à la liste des amitiés
	if (find(Lfriends, F, &cmp_friendship) == NULL){
		// Si a personne A n'est pas déjà dans la liste d'amis de la personne B et inverssement
		if (find(P1->friends, P2, &cmp_person) == NULL){
			cons(P1->friends, P2);
		}

		/*
		Contrairement à la version symétrique, on n'ajoute pas la personne 1 à la liste d'amis de la personne 2
		étant donné qu'on considère qu'une amitié peut ne pas être réciproque.

		if (find(P2->friends, P1, &cmp_person) == NULL){
			cons(P2->friends, P1);
		}
		*/

		cons(Lfriends, F);
	}
	// Si elle existe déjà, alors on supprime l'amitié que l'on vient de créer et on renvoie simplement l'amitié déjà existante (pratique pour éviter des pertes de mémoire).
	else{
		free(F);
		F = existingFriendship;
	}

	return F;
}


/**
 * @brief Écrit les informations d'une amitié dans un flux (texte ou binaire).
 *
 * Cette fonction écrit les informations d'une amitié spécifiée dans un flux spécifié, qu'il soit au format texte ou binaire.
 *
 * @param F Un pointeur vers la structure `friendship_t` contenant les informations de l'amitié à écrire.
 * @param fd Un pointeur vers le flux dans lequel écrire les informations de l'amitié.
 * @param mode Le mode du flux (TEXT pour texte, BIN pour binaire).
 */
void friendship_2_stream (friendship_t * F, FILE * fd, stream_mode_t mode){
	assert(F && fd);

	// Écriture au format texte
	if (mode == TEXT){
		// Vérifie si le fichier est vide (pour un enregistrement dans le fichier plus propre si jamais on l'ouvre en mode `a`)
		long filePos = ftell(fd);  // La fonction ftell renvoie un `long int`, on utilise donc une variable `long`
		if (filePos > 0) { // Si le fichier n'est pas vide, ajoute un saut de ligne
			fprintf(fd, "\n");
		}

		fprintf(fd, "%s %s %s %s", F->A->name, F->A->forename, F->B->name, F->B->forename);
	}
	// Écriture au format binaire
	else if (mode == BIN){
		buffer_t buffer;
		memset(&buffer, 0, sizeof(buffer_t));  // Initialise toute la structure à zéro

		strcpy(buffer.nameA, F->A->name);
		strcpy(buffer.forenameA, F->A->forename);
		strcpy(buffer.nameB, F->B->name);
		strcpy(buffer.forenameB, F->B->forename);

		fwrite(&buffer, sizeof(buffer_t), 1, fd);
	}
}


/**
 * @brief Wrapper pour libérer la mémoire d'une amitié (`friendship_t`) dans le contexte de la gestion d'une liste.
 *
 * Cette fonction est utilisée comme une fonction de suppression lors de la suppression d'une amitié d'une liste.
 * Elle prend un pointeur vers une amitié et libère la mémoire associée à cette amitié en utilisant la fonction `free_friendship`.
 *
 * @param ptr Un pointeur vers l'amitié (`friendship_t`) à libérer.
 */
void free_friendship_wrapper(void * ptr) {
	friendship_t * friendship = (friendship_t *)ptr;
	free_friendship(&friendship, false, NULL, NULL);
}