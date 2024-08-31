#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list.h"
#include "person.h"
#include "friendship.h"



/**
 * @brief Crée une nouvelle structure de données de type `person_t` et l'initialise.
 *
 * Cette fonction alloue dynamiquement une nouvelle structure de données de type `person_t`, l'initialise avec des valeurs
 * par défaut pour la date de naissance et crée une liste vide pour les amis. Elle retourne un pointeur vers la structure
 * `person_t` nouvellement créée.
 *
 * @return Un pointeur vers la nouvelle structure `person_t` allouée et initialisée.
 */
person_t * new_person() {
	person_t * P = calloc(1, sizeof(person_t));
	assert(P);

	// Initialisation de la date de naissance à des valeurs par défaut
	P->birth_date.day = 1;
	P->birth_date.month = 1;
	P->birth_date.year = 2000;

	// Initialisation de la liste d'amis
	P->friends = new_list();

	return P;
}


/**
 * @brief Saisit les informations d'une personne et crée une structure de données de type `person_t`.
 *
 * Cette fonction permet à l'utilisateur de saisir les informations d'une personne, y compris le nom, le prénom
 * et la date de naissance au format "jj/mm/aaaa". Elle crée ensuite une nouvelle structure de données de type
 * `person_t` pour stocker ces informations et retourne un pointeur vers cette structure.
 *
 * @return Un pointeur vers la nouvelle structure `person_t` contenant les informations saisies.
 */
person_t * scan_person() {
	person_t * P = new_person();
	assert(P);

	// Saisie du nom
	printf("Nom : ");
	scanf("%s", P->name);

	// Saisie du prénom
	printf("Prénom : ");
	scanf("%s", P->forename);

	// Saisie de la date de naissance
	printf("Date de naissance (jj/mm/aaaa) : ");
	scanf("%d/%d/%d", &(P->birth_date.day), &(P->birth_date.month), &(P->birth_date.year));

	return P;
}


/**
 * @brief Libère la mémoire associée à une structure `person_t` et supprime les relations d'amitié de la personne de manière symétrique.
 *
 * Cette fonction libère la mémoire associée à une structure `person_t` pointée par `ptrP`. Elle gère également la suppression
 * des relations d'amitié de la personne, en parcourant et en supprimant les amitiés de la liste globale d'amitiés `Lfriends`.
 * Si la personne est présente dans la liste de personnes `Lpers`, elle est également retirée de cette liste. La fonction est conçue
 * pour gérer correctement les cas où `Lpers` et/ou `Lfriends` sont NULL, permettant ainsi une flexibilité dans son utilisation.
 * Lorsqu'une amitié est supprimée, la suppression est effectuée de manière symétrique, c'est-à-dire que si A est ami avec B, alors B est aussi
 * ami avec A, et les deux amitiés sont supprimées.
 *
 * @param ptrP Un pointeur vers un pointeur de la structure `person_t` à libérer.
 * @param Lpers Un pointeur vers la liste des personnes, (peut être NULL).
 * @param Lfriends Un pointeur vers la liste des amitiés, (peut être NULL).
 */
void free_person(person_t ** ptrP, list_t * Lpers, list_t * Lfriends) {
	assert(ptrP && *ptrP);

	// On commence par supprimer les relations dans lesquelles la personne se trouve, en parcourant sa liste d'amis
	// Il est préférable de parcourir la liste d'amis de la personne plutôt que la liste des amitiés, car le nombre d'éléments de Lfriends sera toujours 
	// supérieur ou égal au nombre d'éléments de la liste d'amis de la personne
	while (get_numelm((*ptrP)->friends) > 0){
		// On crée une amitié temporaire
		friendship_t * tempF = new_friendship();
		tempF->A = (*ptrP);
		tempF->B = get_data(get_head((*ptrP)->friends));

		// On crée également l'amitié symétrique étant donné que 'free_friendship' ne libère plus que l'amitié allant de A vers B
		// Or si on supprime une personne, il faut également supprimer l'amitié allant de B vers A si elle existe
		friendship_t * tempF_symetric = new_friendship();
		tempF_symetric->A = tempF->B;
		tempF_symetric->B = (*ptrP);

		// Si une liste d'amitiés est passé en argument
		if (Lfriends != NULL){
			// On recherche l'amitié dans la liste des amitiés 
			friendship_t * existingFriendship = find(Lfriends, tempF, &cmp_friendship);

			// Si jamais l'amitié existe, alors on la supprime
			if (existingFriendship != NULL){
				free_friendship(&existingFriendship, false, Lpers, Lfriends);
			}

			// On recherche l'amitié symétrique dans la liste des amitiés 
			friendship_t * existing_Symetric_Friendship = find(Lfriends, tempF_symetric, &cmp_friendship);

			// Si jamais l'amitié symétrique existe, alors on la supprime
			if (existing_Symetric_Friendship != NULL){
				free_friendship(&existing_Symetric_Friendship, false, Lpers, Lfriends);
			}

			free(tempF);
			free(tempF_symetric);
		}
		else{
			free_friendship(&tempF, false, Lpers, NULL);
			free_friendship(&tempF_symetric, false, Lpers, NULL);
		}
	}

	// Si une liste de personnes est passée en argument
	if (Lpers != NULL){
		// On supprime la personne de la liste
		remove_elm_from_list(Lpers, (*ptrP), &cmp_person, NULL);
	}

	// Libération de la mémoire allouée pour la structure `person_t`
	free((*ptrP)->friends);
	free(*ptrP);
	*ptrP = NULL;
}


/**
 * @brief Compare deux structures de données de type `person_t` en fonction de leurs noms et prénoms.
 *
 * Cette fonction compare deux structures de données de type `person_t` en utilisant les champs "name" et "forename"
 * pour déterminer leur ordre. La comparaison est basée sur la comparaison lexicographique des noms, et si les noms
 * sont identiques, la comparaison est effectuée sur les prénoms.
 *
 * @param P1 Un pointeur vers la première structure `person_t` à comparer.
 * @param P2 Un pointeur vers la deuxième structure `person_t` à comparer.
 * @return Un entier négatif si P1 < P2, un entier positif si P1 > P2, et zéro si les structures sont considérées égales.
 */
int cmp_person(person_t * P1, person_t * P2) {
	// Comparaison des noms des deux personnes (cmpNames = 0 si les noms sont les mêmes)
	int cmpNames = strcmp(P1->name, P2->name);

	// Si les noms sont les mêmes, compare les prénoms ; sinon, utilise la comparaison des noms.
	return (cmpNames == 0) ? strcmp(P1->forename, P2->forename) : cmpNames;
}


/**
 * @brief Affiche les informations d'une personne.
 *
 * Cette fonction affiche les informations de la personne spécifiée, y compris son nom, prénom, date de naissance
 * et le nombre d'amitiés dans la liste d'amis.
 *
 * @param P Un pointeur vers la structure `person_t` contenant les informations de la personne à afficher.
 */
void print_person(person_t * P) {
	assert(P);

	printf("___________Person___________\n");
	printf("Nom : %s\n", P->name);
	printf("Prénom : %s\n", P->forename);
	printf("Date de naissance : %02d/%02d/%04d\n", P->birth_date.day, P->birth_date.month, P->birth_date.year);
	printf("Nombre d'amitié(s) : %d\n", get_numelm(P->friends));
	printf("________Person ended________\n");
}


/**
 * @brief Lit les informations d'une personne depuis un flux (texte ou binaire) et crée une structure `person_t`.
 *
 * Cette fonction lit les informations d'une personne depuis un flux spécifié, qu'il soit au format texte ou binaire,
 * et crée une nouvelle structure `person_t` pour stocker ces informations. Elle retourne un pointeur vers la structure
 * créée ou NULL en cas d'erreur de lecture.
 *
 * @param stream Un pointeur vers le flux depuis lequel lire les informations de la personne.
 * @param mode Le mode du flux (TEXT pour texte, BIN pour binaire).
 * @return Un pointeur vers la nouvelle structure `person_t` contenant les informations lues, ou NULL en cas d'erreur.
 */
person_t * stream_2_person(FILE * stream, stream_mode_t mode) {
	assert(stream);

	person_t * P = new_person();
	assert(P);

	// Lecture au format texte
	if (mode == TEXT) {
		// Lecture du nom, du prénom et de la date de naissance
		if (fscanf(stream, "%39s", P->name) != 1 ||
			fscanf(stream, "%39s", P->forename) != 1 ||
			fscanf(stream, "%d/ %d / %d", &P->birth_date.day, &P->birth_date.month, &P->birth_date.year) != 3) {
				
			// Gestion d'erreur
			free_person(&P, NULL, NULL);
			return NULL;
		}
	}
	// Lecture au format binaire
	else if (mode == BIN) {
		// On utilie trois fread au lieu de 'fread(P, sizeof(person_t), 1, stream)'
		// Pour comprendre pourquoi cf 'person_2_stream'
		if (fread(P->name, sizeof(P->name), 1, stream) != 1 || 
		fread(P->forename, sizeof(P->forename), 1, stream) != 1 || 
		fread(&P->birth_date, sizeof(P->birth_date), 1, stream) != 1) {
			// Gestion d'erreur si la lecture échoue
			free_person(&P, NULL, NULL);
			return NULL;
		}
	}

	return P;
}


/**
 * @brief Écrit les informations d'une personne dans un flux (texte ou binaire).
 *
 * Cette fonction écrit les informations d'une personne spécifiée dans un flux spécifié, qu'il soit au format texte ou binaire.
 *
 * @param P Un pointeur vers la structure `person_t` contenant les informations de la personne à écrire.
 * @param fd Un pointeur vers le flux dans lequel écrire les informations de la personne.
 * @param mode Le mode du flux (TEXT pour texte, BIN pour binaire).
 */
void person_2_stream(person_t * P, FILE * fd, stream_mode_t mode) {
	assert(P && fd);

	// Écriture au format texte
	if (mode == TEXT) {
		// Vérifie si le fichier est vide (pour un enregistrement dans le fichier plus propre si jamais on l'ouvre en mode `a`)
		long filePos = ftell(fd);  // La fonction ftell renvoie un `long int`, on utilise donc une variable `long`
		if (filePos > 0) { // Si le fichier n'est pas vide, ajoute deux sauts de ligne
			fprintf(fd, "\n\n");
		}

		fprintf(fd, "%s\n", P->name);
		fprintf(fd, "%s\n", P->forename);
		fprintf(fd, "%d/%d/%d", P->birth_date.day, P->birth_date.month, P->birth_date.year);
	}
	// Écriture au format binaire
	else if (mode == BIN) {
		// Ici on utilise trois fwrite à la place de 'fwrite(P, sizeof(person_t), 1, fd)'
		// C'est utile pour éviter d'enregistrer la liste d'amis 'list_t' qui est elle même une structure ce qui peut engendrer des problèmes
		fwrite(P->name, sizeof(P->name), 1, fd);
		fwrite(P->forename, sizeof(P->forename), 1, fd);
		fwrite(&P->birth_date, sizeof(P->birth_date), 1, fd);
	}
}


/**
 * @brief Wrapper pour libérer la mémoire d'une personne (`person_t`) dans le contexte de la gestion d'une liste.
 *
 * Cette fonction est utilisée comme une fonction de suppression lors de la suppression d'une personne d'une liste.
 * Elle prend un pointeur vers une personne et libère la mémoire associée à cette personne en utilisant la fonction `free_person`.
 *
 * @param ptr Un pointeur vers la personne (`person_t`) à libérer.
 */
void free_person_wrapper(void * ptr) {
	person_t * person = (person_t *)ptr;
	free_person(&person, NULL, NULL);
}