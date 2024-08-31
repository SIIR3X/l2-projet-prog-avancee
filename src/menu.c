#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "io.h"
#include "menu.h"
#include "list.h"
#include "graph.h"
#include "person.h"
#include "friendship.h"



/**
 * @brief Saisit un choix d'entier entre min et max depuis l'entrée standard.
 *
 * Cette fonction demande à l'utilisateur de saisir un entier entre les valeurs min et max (incluses).
 * Elle continue de demander un choix jusqu'à ce que l'utilisateur entre une valeur valide dans cette plage.
 *
 * @param min La valeur minimale valide.
 * @param max La valeur maximale valide.
 * @return Le choix de l'utilisateur.
 */
int scan_choice(int min, int max){
	int choice;
	
	do{
		printf(" Votre choix (%d - %d) : ", min, max);
		scanf("%d", &choice);

		if (choice < min || choice > max){
			printf(" Choix incorrect.\n");
		}
	} while (choice < min || choice > max);

	printf("\n");

	return choice;
}


/**
 * @brief Demande à l'utilisateur de saisir le chemin du fichier et le mode.
 *
 * Cette fonction demande à l'utilisateur de saisir le chemin d'un fichier et le mode (BIN ou TEXT) pour ce fichier.
 * Elle vérifie la validité du mode saisi et affecte la valeur correspondante à la variable `mode`.
 *
 * @param filename Pointeur vers une chaîne de caractères où le chemin du fichier sera stocké.
 * @param mode Pointeur vers la variable où le mode (BIN ou TEXT) sera stocké.
 */
void scan_file(char * filename, stream_mode_t * mode) {
	char modeInput[10];  // Pour stocker l'entrée de mode (BIN ou TEXT)

	// Demande à l'utilisateur le chemin du fichier
	printf(" Entrez le chemin du fichier : ");
	scanf("%255s", filename); // Limite à 255 caractères pour éviter le débordement de tampon

	// Demande à l'utilisateur le mode (BIN ou TEXT)
	printf(" Entrez le mode (BIN ou TEXT) : ");
	scanf("%9s", modeInput); // Limite à 9 caractères pour éviter le débordement de tampon

	// Vérifie et affecte le mode
	if (strcmp(modeInput, "BIN") == 0) {
		*mode = BIN;
	} 
	else if (strcmp(modeInput, "TEXT") == 0) {
		*mode = TEXT;
	} 
	else {
		printf(" Mode inconnu. Utilisation du mode TEXT par défaut.\n");
		*mode = TEXT;
	}
}


/**
 * @brief Menu de gestion des opérations sur les personnes.
 *
 * Cette fonction affiche un menu permettant à l'utilisateur de choisir parmi différentes opérations
 * liées aux personnes. Elle prend en compte les listes de personnes et d'amitiés pour effectuer les opérations.
 *
 * @param Lpers Pointeur vers la liste des personnes.
 * @param Lfriends Pointeur vers la liste des amitiés.
 */
void person_menu(list_t * Lpers, list_t * Lfriends){
	int person_choice;
	person_t * P, * tempP;
	char filename[256];
	stream_mode_t mode;

	do{
		printf("\t------------- PERSON MENU -------------");
		printf("\n\n\t1. Ajouter une personne manuellement\n");
		printf("\t2. Ajouter une personne depuis un fichier\n");
		printf("\t3. Enregistrer une personne dans un fichier\n");
		printf("\t4. Enregistrer toutes les personnes dans un fichier\n");
		printf("\t5. Supprimer une personne\n");
		printf("\t6. Afficher la liste des personnes\n");
		printf("\t7. Revenir au menu principal\n\n");

		person_choice = scan_choice(1, 7);

		switch(person_choice){

			// 1. Ajouter une personne manuellement
			case 1:
				P = scan_person();

				// Vérifie si la personne n'existe pas déjà
				if (find(Lpers, P, &cmp_person) == NULL){
					cons(Lpers, P);
					printf("\n Personne ajoutée.\n\n");
				}
				else{
					printf("\n La personne existe déjà.\n\n");
				}
				break;

			// 2. Ajouter une personne depuis un fichier
			case 2:
				// Saisie du fichier
				scan_file(filename, &mode);

				// Ajout des personnes du fichier dans une liste temporaire
				list_t * tempL = stream_2_persons(filename, mode);

				// Fusion de la liste des personnes du réseau avec la liste des nouvelles personnes
				list_elm_t * currentElm = get_head(tempL);
				while (currentElm != NULL && find(Lpers, get_data(currentElm), &cmp_person) == NULL){
					cons(Lpers, get_data(currentElm));
					currentElm = get_suc(currentElm);
				}

				printf("\n Personne(s) ajoutée(s).\n\n");
				del_list(&tempL, NULL);
				break;

			// 3. Enregistrer une personne dans un fichier
			case 3:
				// Saisie de la personne
				tempP = scan_person();
				P = find(Lpers, tempP, &cmp_person);
				free_person(&tempP, NULL, NULL);

				// Vérifie si la personne est présente dans la liste
				if (P == NULL){
					printf("\n Personne introuvable.\n");
				}
				else{
					printf("\n");
					// Saisie du fichier
					scan_file(filename, &mode);

					FILE * file = fopen(filename, mode == TEXT ? "a" : "wb");
					if (file == NULL){
						printf("\n Erreur lors de la tentative d'ouverture du fichier.\n\n");
					}
					else{
						// Enregistre la personne dans le fichier
						person_2_stream(P, file, mode);
						printf("\n Personne enregistrée dans le fichier.\n\n");
						fclose(file);
					}
				}
				break;

			// 4. Enregistrer toutes les personnes dans un fichier
			case 4:
				// Saisie du fichier
				scan_file(filename, &mode);

				// Enregistrement des personnes
				list_2_stream(filename, mode, Lpers, (void (*)(void *, FILE *, stream_mode_t))person_2_stream);
				break;

			// 5. Supprimer une personne
			case 5:
				// Saisie de la personne
				tempP = scan_person();
				P = find(Lpers, tempP, &cmp_person);
				free_person(&tempP, NULL, NULL);

				// Vérifie si la personne est présente dans la liste
				if (P == NULL){
					printf("\n Personne introuvable.\n\n");
				}
				else{
					free_person(&P, Lpers, Lfriends);
					printf("\n Personne supprimée.\n\n");
				}
				break;

			// 6. Afficher la liste des personnes
			case 6:
				view_list(Lpers, &print_person, "Liste des personnes");
				break;

			// 7. Revenir au menu principal
			default:
				printf(" Retour au menu principal.\n\n");
		}
	} while (person_choice != 7);
}


/**
 * @brief Menu de gestion des opérations sur les amitiés entre personnes.
 *
 * Cette fonction affiche un menu permettant à l'utilisateur de choisir parmi différentes opérations
 * liées aux amitiés entre personnes. Elle prend en compte les listes de personnes et d'amitiés pour effectuer les opérations.
 *
 * @param Lpers Pointeur vers la liste des personnes.
 * @param Lfriends Pointeur vers la liste des amitiés.
 */
void friendship_menu(list_t * Lpers, list_t * Lfriends){
	int friendship_choice;
	friendship_t * F, * tempF;
	char filename[256];
	stream_mode_t mode;

	do{
		printf("\t------------- FRIENDSHIP MENU -------------");
		printf("\n\n\t1. Ajouter une amitié manuellement\n");
		printf("\t2. Ajouter une amitié depuis un fichier\n");
		printf("\t3. Enregistrer une amitié dans un fichier\n");
		printf("\t4. Enregistrer toutes les amitiés dans un fichier\n");
		printf("\t5. Supprimer une amitié\n");
		printf("\t6. Afficher la liste des amitiés\n");
		printf("\t7. Revenir au menu principal\n\n");

		friendship_choice = scan_choice(1, 7);

		switch(friendship_choice){

			// 1. Ajouter une amitié manuellement
			case 1:
				// Saisie de l'amitié
				F = scan_friendship(Lpers);

				// Vérifie si l'amitié n'existe pas déjà
				if (find(Lfriends, F, &cmp_friendship) == NULL){
					cons(Lfriends, F);
					printf("\n Amitié ajoutée.\n\n");
				}
				else{
					free_friendship(&F, false, NULL, NULL);
					printf("\n L'amitié existe déjà.\n\n");
				}
				break;

			// 2. Ajouter une amitié depuis un fichier
			case 2:
				// Saisie du fichier
				scan_file(filename, &mode);

				// Ajout des amitiés du fichier dans une liste temporaire
				list_t * tempL = stream_2_friendships(filename, mode, Lpers);

				// Fusion de la liste des amitiés du réseau avec la liste des nouvelles amitiés
				list_elm_t * currentElm = get_head(tempL);
				while (currentElm != NULL && find(Lfriends, get_data(currentElm), &cmp_friendship) == NULL){
					cons(Lfriends, get_data(currentElm));
					currentElm = get_suc(currentElm);
				}

				printf("\n Amitié(s) ajoutée(s).\n\n");
				del_list(&tempL, NULL);
				break;
			
			// 3. Enregistrer une amitié dans un fichier
			case 3:
				// Saisie de l'amitié
				tempF = scan_friendship(Lpers);
				F = find(Lfriends, tempF, &cmp_friendship);
				free_friendship(&tempF, false, NULL, NULL);

				// Vérifie si l'amitié est présente dans la liste
				if (F == NULL){
					printf("\n Amitié introuvable.\n\n");
				}
				else{
					printf("\n");

					// Saisie du fichier
					scan_file(filename, &mode);

					FILE * file = fopen(filename, mode == TEXT ? "a" : "ab");
					if (file == NULL){
						printf("\n Erreur lors de la tentative d'ouverture du fichier.\n\n");
					}
					else{
						// Enregistre l'amitié dans le fichier
						friendship_2_stream(F, file, mode);
						printf("\n Amitié enregistrée dans le fichier.\n\n");
						fclose(file);
					}
				}
				break;

			// 4. Enregistrer toutes les amitiés dans un fichier
			case 4:
				// Saisie du fichier
				scan_file(filename, &mode);

				// Enregistrement des amitiés
				list_2_stream(filename, mode, Lfriends, (void (*)(void *, FILE *, stream_mode_t))friendship_2_stream);

				printf("\n Amitiés enregistrées dans le fichier.\n\n");
				break;

			// 5. Supprimer une amitié
			case 5:
				// Saisie de l'amitié
				tempF = scan_friendship(Lpers);
				F = find(Lfriends, tempF, &cmp_friendship);
				free_friendship(&tempF, false, NULL, NULL);

				// Vérifie si l'amitié existe
				if (F == NULL){
					printf("\n Amitié introuvable.\n\n");
				}
				else{
					// Demande à lutilisateur s'il faut supprimer ou non les personnes du réseau
					char delPerson;
					printf("\n Retirer les personnes du réseau ? (O/N) : ");
					scanf(" %c", &delPerson);
					delPerson = toupper(delPerson);

					if (delPerson == 'O'){
						free_friendship(&F, true, Lpers, Lfriends);
						printf("\n Amitié et personnes supprimées.\n\n");
					}
					else{
						free_friendship(&F, false, Lpers, Lfriends);
						printf("\n Amitié supprimée.\n\n");
					}
				}
				break;

			// 6. Afficher la liste des amitiés
			case 6:
				view_list(Lfriends, &print_friendship, "Liste des amitiés");
				break;

			// 7. Revenir au menu principal
			default:
				printf(" Retour au menu principal.\n\n");
		}
	} while(friendship_choice != 7);
}


/**
 * @brief Menu de gestion des opérations sur les matrices.
 *
 * Cette fonction affiche un menu permettant à l'utilisateur de choisir parmi différentes opérations
 * liées aux matrices. Elle prend en compte les listes de personnes, d'amitiés et le graphe pour effectuer les opérations.
 *
 * @param Lpers Pointeur vers la liste des personnes.
 * @param Lfriends Pointeur vers la liste des amitiés.
 * @param Graph Pointeur sur un pointeur de graphe.
 */
void matrix_menu(list_t * Lpers, list_t * Lfriends, graph_t ** Graph){
	int matrix_choice, vertice_id;
	char filename[256];
	stream_mode_t mode;

	do{
		printf("\t------------- MATRIX MENU -------------");
		printf("\n\n\t1. Convertir la liste des personnes en une matrice\n");
		printf("\t2. Lire une matrice depuis un fichier\n");
		printf("\t3. Enregistrer la matrice d'adjacence dans un fichier\n");
		printf("\t4. Calculer les cliques maximales\n");
		printf("\t5. Afficher la matrice\n");
		printf("\t6. Revenir au menu principal\n\n");

		matrix_choice = scan_choice(1, 6);

		switch (matrix_choice){

			// 1. Convertir la liste des personnes en une matrice
			case 1:
				del_graph(Graph);  // Libère l'ancien graph
				*Graph = list_2_graph(Lpers);

				printf(" La liste des personnes à été convertie en matrice. Les distances ont également été calculées\n\n");
				break;

			// 2. Lire une matrice depuis un fichier
			case 2:
				// Saisie du fichier
				scan_file(filename, &mode);

				del_graph(Graph);  // Libère l'ancien graph
				*Graph = fscanf_graph(filename, mode);

				printf("\n La matrice provenant du fichier '%s' à été ajoutée. Les distances ont également été calculées\n\n", filename);
				break;

			// 3. Enregistrer la matrice d'adjacence dans un fichier
			case 3:
				// Saisie du fichier
				scan_file(filename, &mode);

				FILE * file = fopen(filename, mode == TEXT ? "w" : "wb");
				if (file == NULL){
					printf("\n Erreur lors de la tentative d'ouverture du fichier.\n\n");
				}
				else{
					graph_2_stream(file, (*Graph), mode);
					printf("\n Matrice enregistrée dans le fichier.\n\n");
					fclose(file);
				}
				break;

			// 4. Calculer les cliques maximales
			case 4:
				// On vérifie si le graphe est symétrique pour appliquer l'algorithme de BronKerbosh (Possible dans cette version)
				if (symetric_graph(*Graph) == true){
					printf("\tNuméro de sommet\n\n");
					vertice_id = scan_choice(-1, (*Graph)->num_vertices-1);

					// Initialisation des ensembles
					set_t * R = new_set((*Graph)->num_vertices);
					set_t * P = new_set((*Graph)->num_vertices);
					set_t * X = new_set((*Graph)->num_vertices);
					set_t * C = new_set((*Graph)->num_vertices);

					// Si un sommet est entré
					if (vertice_id != -1){
						add_set(R, vertice_id); // Ajoute le sommet choisi à R

						set_t * neighbors = friends(*Graph, vertice_id);
						for (int i = 0; i < neighbors->numelm; ++i) {
							add_set(P, neighbors->data[i]);
						}
						del_set(&neighbors); // Nettoyage des voisins
					}
					// Si l'utilisateur n'a pas choisis de sommet (-1)
					else{
						for (int i = 0; i < (*Graph)->num_vertices; i++) {
							add_set(P, i);
						}
					}

					// Utilisation de l'algorithme de BronKerbosch
					BronKerbosch(*Graph, R, P, X, C);

					print_set(C, "Clique maximale trouvée");

					// Libération de la mémoire
					del_set(&R);
					del_set(&P);
					del_set(&X);
					del_set(&C);
				}
				else{
					printf(" Le graphe n'est pas symétrique. L'utilisation de l'algorithme de BronKerbosh est impossible.\n\n");
				}

				break;

			// 5. Afficher la matrice
			case 5:
				char printDistances;
				printf(" Afficher la matrice des distances ? (O/N) : ");
				scanf(" %c", &printDistances);
				printDistances = toupper(printDistances);

				if (printDistances == 'O') {
					printf_graph(*Graph, "Graphe des amitiés", true);
				} 
				else {
					printf_graph(*Graph, "Graphe des amitiés", false);
				}
				break;

			// 6. Revenir au menu principal
			default:
				printf(" Retour au menu principal.\n\n");
		}
	} while (matrix_choice != 6);
}