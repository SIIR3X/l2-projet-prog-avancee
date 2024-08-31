#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "graph.h"
#include "set.h"



/**
 * @brief Crée un nouveau graphe avec un nombre spécifié de sommets.
 *
 * Cette fonction crée un nouveau graphe avec le nombre de sommets spécifié. Elle alloue la mémoire nécessaire
 * pour stocker les matrices d'adjacence, de distances, les tableaux de lacunes et de connexions. Si une allocation
 * échoue, la fonction libère la mémoire déjà allouée et renvoie NULL. Le paramètre `nb_vertices` doit être supérieur
 * à zéro.
 *
 * @param nb_vertices Le nombre de sommets du graphe.
 * @return Un pointeur vers le nouveau graphe créé, ou NULL en cas d'erreur.
 */
graph_t * new_graph ( const int nb_vertices ){
	assert(nb_vertices >= 0);

	graph_t * G = malloc(sizeof(graph_t));
	assert(G);

	G->num_vertices = nb_vertices;

	// Allocation des matrices et des tableaux
	int size = nb_vertices * nb_vertices;
	G->adjacencies = calloc(size, sizeof(int));
	G->distances = calloc(size, sizeof(int));
	G->gaps = calloc(nb_vertices, sizeof(int));
	G->connections = calloc(size, sizeof(int));

	// Vérifie si toutes les allocations ont réussi
	if (!G->adjacencies || !G->distances || !G->gaps || !G->connections){
		del_graph(&G);
	}

	G->far_from = 0;

	return G;
}


/**
 * @brief Libère la mémoire allouée pour un graphe.
 *
 * Cette fonction libère la mémoire allouée pour un graphe, y compris les matrices d'adjacence et de distances,
 * ainsi que les tableaux de lacunes et de connexions. Elle prend un pointeur vers un pointeur de graphe (`ptrG`)
 * en tant que paramètre et s'assure que le pointeur de graphe est valide avant de libérer la mémoire.
 *
 * @param ptrG Un pointeur vers un pointeur de graphe.
 */
void del_graph(graph_t ** ptrG) {
	assert(ptrG && *ptrG);

	free((*ptrG)->adjacencies);
	free((*ptrG)->distances);
	free((*ptrG)->gaps);
	free((*ptrG)->connections);

	free(*ptrG);
	*ptrG = NULL;
}


/**
 * @brief Lit les données d'un graphe depuis un fichier en utilisant fscanf avec symétrie.
 *
 * Cette fonction lit les données d'un graphe depuis un fichier spécifié en utilisant la fonction fscanf.
 * Le graphe est créé avec le nombre de sommets (nb_vertices) récupéré depuis le fichier. Ensuite, les
 * liaisons entre les sommets sont lues et enregistrées dans la matrice d'adjacence du graphe. Cela inclut
 * l'ajout de la symétrie pour refléter les connexions bidirectionnelles entre les sommets. La fonction
 * retourne un pointeur vers la structure de graphe `graph_t` créée ou NULL en cas d'erreur de lecture ou
 * d'allocation mémoire.
 *
 * @param filename Le nom du fichier à partir duquel lire les données du graphe.
 * @param mode Le mode du fichier (TEXT pour texte, BIN pour binaire).
 * @return Un pointeur vers la structure `graph_t` créée, ou NULL en cas d'erreur.
 */
graph_t * fscanf_graph(const char * filename, const stream_mode_t mode) {
	assert(filename);

	// Ouverture du fichier en fonction du mode sélectionné
	FILE *file = fopen(filename, mode == TEXT ? "r" : "rb");
	assert(file);

	int nb_vertices;

	// Récupération de nb_vertices
	if (mode == TEXT) {
		// Lecture en mode texte
		if (fscanf(file, "%d", &nb_vertices) != 1) {
			fclose(file);
			return NULL;
		}
	} 
	else {
		// Lecture en mode binaire
		if (fread(&nb_vertices, sizeof(int), 1, file) != 1) {
			fclose(file);
			return NULL;
		}
	}

	// Création de la structure graph_t avec le nb_vertices
	graph_t * G = new_graph(nb_vertices);
	if (G == NULL) {
		fclose(file);
		return NULL;
	}

	// Récupération des lignes du graph
	for (int i = 0; i < nb_vertices; i++) {
		int j;
		while (1) {
			if (mode == TEXT) {
				// Lecture en mode texte
				if (fscanf(file, "%d", &j) != 1 || j == BUD_END) {
					break;
				}
			} 
			else {
				// Lecture en mode binaire
				if (fread(&j, sizeof(int), 1, file) != 1 || j == BUD_END) {
					break;
				}
			}

			if (i != j) {
				G->adjacencies[i * nb_vertices + j] = BUDDY;
				G->adjacencies[j * nb_vertices + i] = BUDDY; // Ajout de la symétrie
			}
		}
	}

	// Calcul des distances 
	distance_calculus(G);

	fclose(file);
	return G;
}


/**
 * @brief Vérifie si un graphe est symétrique.
 *
 * Cette fonction vérifie si un graphe donné est symétrique, c'est-à-dire que la matrice d'adjacence
 * du graphe est symétrique par rapport à sa diagonale. Elle parcourt la matrice d'adjacence pour
 * chaque paire de sommets (i, j), en vérifiant que l'élément (i, j) est égal à l'élément (j, i).
 * Si une asymétrie est trouvée à n'importe quel endroit, la fonction retourne false, indiquant que
 * le graphe n'est pas symétrique. Sinon, elle retourne true, indiquant que le graphe est symétrique.
 *
 * @param G Un pointeur vers la structure `graph_t` représentant le graphe à vérifier.
 * @return true si le graphe est symétrique, false sinon.
 */
bool symetric_graph( const graph_t * G ){
	assert(G);

	for (int i = 0; i < G->num_vertices; ++i) {
		for (int j = 0; j < i; ++j) { // Pas besoin de vérifier la diagonale ou au-delà
			if (G->adjacencies[i * G->num_vertices + j] != 
				G->adjacencies[j * G->num_vertices + i]) {
				return false; // Asymétrie trouvée
			}
		}
	}

	return true; // Aucune asymétrie n'a été trouvée, le graphe est symétrique
}


/**
 * @brief Affiche la matrice d'adjacence et la matrice des distances d'un graphe.
 *
 * Cette fonction affiche la matrice d'adjacence d'un graphe, ainsi que la matrice des distances
 * si spécifié, dans la sortie standard. Elle peut également afficher un en-tête en haut du graphe.
 *
 * @param G Un pointeur vers la structure `graph_t` représentant le graphe à afficher.
 * @param entete Un pointeur vers une chaîne de caractères contenant l'en-tête à afficher (peut être NULL).
 * @param and_Distances Un booléen indiquant si la matrice des distances doit être affichée (true) ou non (false).
 */
void printf_graph ( const graph_t * G, const char * entete, const bool and_Distances ){
	assert(G);

	printf("\n");
	// Affiche l'en-tête si présent
	if (entete != NULL) {
		printf("\t%s\n\n", entete);
	}

	// Affiche la matrice d'adjacence
	printf("Matrice d'adjacence :\n");
	for (int i = 0; i < G->num_vertices; ++i) {
		for (int j = 0; j < G->num_vertices; ++j) {
			printf("%d ", G->adjacencies[i * G->num_vertices + j]);
		}
		printf("\n");
	}

	// Affiche la matrice des distances si demandé
	if (and_Distances && G->distances != NULL) {
		printf("\nMatrice des distances:\n");
		for (int i = 0; i < G->num_vertices; ++i) {
			for (int j = 0; j < G->num_vertices; ++j) {
				printf("%d ", G->distances[i * G->num_vertices + j]);
			}
			printf("\n");
		}
	}
	printf("\n");
}


/**
 * @brief Calcule les distances minimales entre tous les sommets d'un graphe en utilisant l'algorithme de Floyd-Warshall.
 *
 * Cette fonction calcule les distances minimales entre tous les sommets d'un graphe en utilisant l'algorithme de Floyd-Warshall.
 * Les distances sont stockées dans la matrice `distances` du graphe.
 *
 * @param G Un pointeur vers la structure `graph_t` représentant le graphe dont les distances doivent être calculées.
 */
void distance_calculus(graph_t * G) {
	// Initialisation des distances
	for (int i = 0; i < G->num_vertices; ++i) {
		for (int j = 0; j < G->num_vertices; ++j) {
			// Cas 1 : On est sur la diagonale, la distance d'un sommet à lui-même est toujours 0
			if (i == j) {
				G->distances[i * G->num_vertices + j] = 0;
			}
			// Cas 2 : Si une connexion directe (BUDDY) existe entre les sommets, la distance est 1
			else if (G->adjacencies[i * G->num_vertices + j] == BUDDY) {
				G->distances[i * G->num_vertices + j] = 1;
			}
			// Cas 3 : Si aucun lien direct, la distance initiale est définie comme infinie (INT_MAX)
			else {
				G->distances[i * G->num_vertices + j] = INT_MAX;
			}
		}
	}

	// Algorithme de Floyd-Warshall
	for (int k = 0; k < G->num_vertices; ++k) {
		for (int i = 0; i < G->num_vertices; ++i) {
			for (int j = 0; j < G->num_vertices; ++j) {
				// Vérifie si un chemin plus court est trouvé via le sommet k
				// On s'assure d'abord que les chemins i -> k et k -> j existent (ne sont pas infinis)
				if (G->distances[i * G->num_vertices + k] != INT_MAX && 
					G->distances[k * G->num_vertices + j] != INT_MAX &&
					// On vérifie ensuite si le passage par k offre un chemin plus court de i à j
					G->distances[i * G->num_vertices + k] + G->distances[k * G->num_vertices + j] < G->distances[i * G->num_vertices + j]) {
					// Pour finir, on met à jour la distance si un chemin plus court est trouvé
					G->distances[i * G->num_vertices + j] = G->distances[i * G->num_vertices + k] + G->distances[k * G->num_vertices + j];
				}
			}
		}
	}
}


/**
 * @brief Récupère l'ensemble des amis d'un sommet donné dans un graphe.
 *
 * Cette fonction prend en entrée un graphe `G` et un sommet `p`, puis elle renvoie un ensemble `friends_set` contenant les amis du sommet `p` dans le graphe.
 *
 * @param G Un pointeur vers la structure `graph_t` représentant le graphe.
 * @param p L'indice du sommet pour lequel on veut trouver les amis.
 * @return Un pointeur vers une structure `set_t` contenant les amis du sommet `p`.
 */
set_t * friends(const graph_t * G, const int p) {
	assert(G && p >= 0 && p < G->num_vertices);

	// Création d'un nouvel ensemble pour les amis du sommet p
	set_t * friends_set = new_set(G->num_vertices);
	assert(friends_set);

	// Parcours de la ligne de la matrice d'adjacence correspondant au sommet p
	for (int i = 0; i < G->num_vertices; ++i) {
		if (G->adjacencies[p * G->num_vertices + i] == BUDDY) {
			// i est un ami de p, ajouter i à l'ensemble
			add_set(friends_set, i);
		}
	}

	return friends_set;
}


/**
 * @brief Algorithme de recherche de toutes les cliques maximales dans un graphe.
 *
 * Cette fonction permet de trouver toutes les cliques maximales dans un graphe à l'aide de l'algorithme de Bron-Kerbosch.
 * 
 * @param G Un pointeur vers la structure `graph_t` représentant le graphe.
 * @param R Un pointeur vers un ensemble représentant la clique actuellement en construction.
 * @param P Un pointeur vers un ensemble représentant les sommets candidats à rejoindre la clique.
 * @param X Un pointeur vers un ensemble représentant les sommets exclus de la clique.
 * @param C Un pointeur vers un ensemble représentant la plus grande clique maximale trouvée.
 */
void BronKerbosch(const graph_t * G, set_t * R, set_t * P, set_t * X, set_t * C) {
	if (numelm_set(P) == 0 && numelm_set(X) == 0) {
		// Si P et X sont vides, alors R est une clique maximale
		if (numelm_set(R) > numelm_set(C)) {
			// Si R est plus grande que la clique maximale actuelle alors on met à jour C
			dup_set(R, C);
		}
		return;
	}

	for (int i = 0; i < P->numelm; ++i) {
		int v = P->data[i];

		// Ajoute v à l'ensemble R
		add_set(R, v);

		// Trouve tous les voisins de v
		set_t * neighbors = friends(G, v);

		// Calcule de P ∩ N(v)
		set_t * PinterN = inter_set(P, neighbors);
		
		// Calcule de X ∩ N(v)
		set_t * XinterN = inter_set(X, neighbors);

		// Appel récursif
		BronKerbosch(G, R, PinterN, XinterN, C);

		// Retire v de l'ensemble R
		substract_set(R, v);

		// Ajoute v à X
		add_set(X, v);

		// Nettoyage de la mémoire
		del_set(&PinterN);
		del_set(&XinterN);
		del_set(&neighbors);
	}
}


/**
 * @brief Écrit la représentation du graphe dans un flux (fichier) au format spécifié (TEXT ou BIN).
 *
 * Cette fonction prend en charge l'écriture de la représentation d'un graphe dans un flux (fichier) donné, en utilisant le mode
 * spécifié (TEXT ou BIN). Le graphe à écrire est passé en tant que pointeur vers la structure `graph_t` `G`, et le flux est
 * passé en tant que pointeur vers un fichier `stream`. Le mode détermine le format de sortie (texte ou binaire).
 *
 * @param stream Un pointeur vers un fichier dans lequel écrire la représentation du graphe.
 * @param G Un pointeur vers le graphe `graph_t` à écrire dans le fichier.
 * @param mode Le mode d'écriture du fichier (TEXT ou BIN).
 */
void graph_2_stream(FILE * stream, graph_t * G, const stream_mode_t mode){
	assert(stream && G);

	int num_vertices = G->num_vertices;

	// Écriture du nombre de sommets
	if (mode == TEXT) {
		fprintf(stream, "%d\n", num_vertices);
	} 
	else if (mode == BIN) {
		fwrite(&num_vertices, sizeof(int), 1, stream);
	}

	// Écriture des adjacences
	for (int i = 0; i < num_vertices; ++i) {
		int written = 0; // Flag pour vérifier si au moins un élément est écrit

		for (int j = 0; j < num_vertices; ++j) {
			if (G->adjacencies[i * num_vertices + j] == BUDDY) {
				if (mode == TEXT) {
					fprintf(stream, "%d ", j);
				} 
				else {
					fwrite(&j, sizeof(int), 1, stream);
				}
				written = 1;
			}
		}

		int end = BUD_END;
		if (mode == TEXT) {
			fprintf(stream, "%d\n", end); // Marqueur de fin de ligne

			if (i < num_vertices-1){
				fprintf(stream, "\n");
			}
		} 
		else if (written) { // Écriture en binaire seulement si quelque chose a été écrit
			fwrite(&end, sizeof(int), 1, stream);
		}
	}
}