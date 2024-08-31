#include <stdio.h>

#include "io.h"
#include "menu.h"
#include "graph.h"
#include "person.h"
#include "friendship.h"



int main(int argc, char * argv[]){
	int choice = 0;

	// Initialisation des graphes
	list_t * Lpers;		// Liste des personnes
	list_t * Lfriends;	// Liste des amitiés
	graph_t * Graph;	// Graphe du réseau

	// Si deux listes sont passées en argument (Lpers et Lfriends), alors on les charges
	if (argc == 3){
		Lpers = stream_2_persons(argv[1], TEXT);
		Lfriends = stream_2_friendships(argv[2], TEXT, Lpers);
		Graph = list_2_graph(Lpers);
	}
	// Sinon, on initialise simplement les éléments
	else{
		Lpers = new_list();
		Lfriends = new_list();
		Graph = new_graph(0);
	}

	do{
		printf("=========================================");
		printf("\n\n\t\tKILOGRAM\n\n");
		printf("\t1. Gérer les personnes\n");
		printf("\t2. Gérer les amitiés\n");
		printf("\t3. Gérer les matrices\n");
		printf("\t4. Quitter\n\n");

		choice = scan_choice(1, 4);

		switch(choice){
			// 1. Gérer les personnes
			case 1:
				person_menu(Lpers, Lfriends);
				break;

			// 2. Gérer les amitiés
			case 2:
				friendship_menu(Lpers, Lfriends);
				break;

			// 3. Gérer les matrices
			case 3:
				matrix_menu(Lpers, Lfriends, &Graph);
				break;

			// 4. Quitter
			default:
				printf(" Fermeture de l'application.\n\n");
		}
	} while (choice != 4);

	// Libération de la mémoire
	del_list(&Lfriends, free_friendship_wrapper);
	del_list(&Lpers, free_person_wrapper);
	del_graph(&Graph);

	return 0;
}