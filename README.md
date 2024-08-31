# Projet de programmation nécessaire - KILOGRAM



## Notre dépôt
Le dépôt du projet (**version symétrique**) est disponible via ce lien [GitLab](https://gitlab.univ-lorraine.fr/fagioli7u/pne_projet). Une **version asymétrique** est également disponible dans la branche [asym](https://gitlab.univ-lorraine.fr/fagioli7u/pne_projet/tree/asym) du dépôt.


## Documentation
La documentation complète du projet (**version asymétrique**) est disponible via ce [lien](https://siix.fr/pne_projet/asym/) (documentation générée avec Doxygen) à l'aide des identifiants suivants :  

    Username : ufr
    Password : SX5837ufr


## Description du projet
Le projet a pour objectif de simuler le fonctionnement d'un réseau social. Il est composé de différentes fonctions permettant d'ajouter, de supprimer et d'afficher des personnes et des amitiés. 
Dans cette version du projet, on s'intéresse à la simulation d'amitiés **asymétriques**. On considère donc qu'une amitié allant de A vers B **ne vas pas** de de B vers A.  

**Attention**, au cours de l'exécution du programme, il sera demandé à l'utilisateur de saisir des données (chemin vers un fichier, date de naissance, etc...). Ces données doivent être saisies en respectant les conventions affichées à l'écran.
A noter que lorsque le programme demande de saisir un chemin vers un fichier, on parle évidemment du chemin relatif vers le fichier (Exemple : **data/person.dta** pour la liste des personnes).

Le projet peut être lancé à l'aide de la commande **'make'** dans un environnement **Linux**. Le makefile s'occupe automatiquement du chargement des fichiers 'person.dta' et 'friendship.dta' ainsi que de les convertir en une structure 'graph_t'. À noter que le programme peut également être lancé manuellement à partir de l'exécutable. Le premier argument du programme correspond à un fichier contenant une liste de personnes, et le deuxième correspond à un fichier contenant une liste d'amitiés.


## Différences entre la version asymétrique et symétrique

Cette section est dédiée à l'explication des **différences** entre la version **symétrique** et **asymétrique**. La majorité du projet est la même que la version symétrique, seules 7 fonctions ont été modifiées. Toutes les modifications apportées ont été commentées dans les fichiers.

Voici une rapide explication des différentes modifications apportées :  

- **fscanf_graph :** La fonction de lecture d'un graphe n'ajoute plus l'amitié symétrique dans la matrice d'adjacence.
- **list_2_graph :** La fonction de conversion d'une liste en un graphe n'ajoute plus l'amitié symétrique dans la matrice d'adjacence.
- **cmp_friendship :** La fonction de comparaison d'amitié considère maintenant que l'amitié A->B est différente de l'amitié B->A.
- **free_friendship :** La fonction de suppression d'amitiés ne supprime plus l'amitié symétrique. De plus, si le booléen 'del_person' est vrai, elle ne supprime que la personne A.
- **free_person :** La fonction de suppression de personnes cherche maintenant l'amitié symétrique dans la liste d'amitiés pour la supprimer, puisque `free_friendship` ne supprime plus l'amitié symétrique (cf `free_friendship`).
- **stream_2_friendship :** La fonction de lecture d'amitiés depuis un fichier n'ajoute plus l'amitié symétrique dans les listes d'amis.


## Menu du projet
Lors du lancement du projet, un menu apparaîtra dans le terminal. Ce dernier ne fait pas partie intégrante du projet, il permet simplement d'utiliser les outils mis en place dans le projet.<br/><br/>
Voici la liste des différentes fonctionnalités du menu ainsi que les principales fonctions qu'elles impliquent :<br/>

### 1. Gérer les personnes
- **Ajouter une personne manuellement :** Ajoute une personne manuellement (`scan_person`).
- **Ajouter une personne depuis un fichier :** Ajoute une/des personne(s) depuis un fichier (`stream_2_persons`).
- **Enregistrer une personne dans un fichier :** Enregistre une personne dans un fichier (`person_2_stream`).
- **Enregistrer toutes les personnes dans un fichier :** Enregistre les personnes du réseau dans un fichier (`list_2_stream`).
- **Supprimer une personne :** Supprime une personne du réseau (`free_person`).
- **Afficher la liste des personnes :** Affiche la liste des personnes du réseau (`view_list`).

### 2. Gérer les amitiés
- **Ajouter une amitié manuellement :** Ajoute une amitié manuellement(`scan_friendship`).
- **Ajouter une amitié depuis un fichier :** Ajoute une/des amitié(s) depuis un fichier (`stream_2_friendships`).
- **Enregistrer une amitié dans un fichier :** Enregistre une amitié dans un fichier (`friendship_2_stream`).
- **Enregistrer toutes les amitiés dans un fichier :** Enregistre toutes les amitiés du réseau dans un fichier (`list_2_stream`).
- **Supprimer une amitié :** Supprime une amitié du réseau ainsi que les personnes si besoin (`free_friendship`).
- **Afficher la liste des amitiés :** Affiche la liste des amitiés du réseau (`views_list`).

### 3. Gérer les matrices
- **Convertir la liste des personnes en une matrice :** Convertit la liste des personnes en une structure de type 'graph_t' (`list_2_graph`).
- **Lire une matrice d'amitié depuis un fichier :** Lit une matrice d'amitié depuis un fichier (`fscanf_graph`).
- **Enregistrer la matrice d'adjacence dans un fichier :** Enregistre la matrice d'adjacence du graphe dans un fichier (`graph_2_stream`).
- **Calculer les cliques maximales :** Calcule les cliques maximales dans une matrice d'adjacence (`BronKerbosch`).
- **Afficher la matrice :** Affiche le graphe représentant les amitiés du réseau et la matrice distance si besoin (`printf_graph`).


## Première partie - Matrices et BronKerbosch
Cette section concerne les fichiers header **set.h** et **graph.h** ayant pour but de gérer la partie matricielle du projet.  

Vous trouverez ci-dessous la description des principales fonctions associées à ces fichiers :<br/><br/>

- **Fonction `fscanf_graph`**

Cette fonction permet de créer et de lire un graphe à l'aide de la fonction `new_graph`. Elle se charge de remplir la matrice d'adjacence avec les amitiés entre les personnes, ainsi que de calculer les distances entre les personnes. **Attention**, les fichiers de matrice doivent respecter certaines conventions : la première ligne correspond à la taille de la matrice (nb_vertices), les autres lignes représentent les relations d'amitié. La fonction lit chaque ligne jusqu'à rencontrer la valeur **'BUD_END'** (-1).

- **Fonction `graph_2_stream`**

Cette fonction permet d'enregistrer la matrice d'adjacence (les relations d'amitiés du réseau) dans un fichier donné en argument. La matrice est enregistrée en suivant les conventions du projet, afin de pouvoir la réutiliser lors d'une autre exécution. Si le fichier n'existe pas, il sera créé, sinon, le contenu du fichier existant sera écrasé.

- **Fonction `symetric_graph`**

Cette fonction permet de déterminer si un graphe est symétrique ou non. Pour ce faire, elle compare chaque élément situé en dessous de la diagonale avec l'élément correspondant de l'autre côté de la diagonale, et elle retourne vrai si le graphe est symétrique.

- **Fonction `printf_graph`**

Cette fonction permet d'afficher le contenu d'un graphe. Elle affiche la matrice d'adjacence du graphe ainsi que que la matrice des distances si le le booléen 'and_Distances' est vrai.

- **Fonction `distance_calculus`**

Cette fonction permet de calculer les distances séparant les personnes du réseau. Pour ce faire, nous avons décidé d'utiliser l'algorithme de **Floyd-Warshall**.  

La fonction commence par initialiser la matrice des distances de cette manière :

1. Si la cellule est sur la diagonale de la matrice (i=j), alors on met la distance à 0 car il s'agit du cas où la première personne est égale à la deuxième personne.
2. Si la cellule est à 1, alors il existe une connexion directe entre les deux personnes, nous la laissons donc à 1.
3. Sinon, s'il n'existe aucun lien direct, nous mettons une grande valeur (**'INT_MAX'** de **'limit.h'**) dans la cellule pour représenter l'infini (pour signifier que pour le moment, les personnes sont très éloignées).

La fonction applique ensuite l'algorithme de **Floyd-Warshall** pour rechercher les distances entre les personnes du réseau.  

Voici une version de l'algorithme en pseudo-code :  

```
PROCEDURE FloydWarshall(Graph: graph_t)
// Algorithme de FloydWarshall
DÉBUT
    // n est le nombre de sommets dans le graphe
    n ← Graph.num_vertices
    // Distance réprésente la matrice des distances les plus courtes entre chaque paire de sommets
    Distance ← Graph.distances

    POUR k DE 0 à n-1 FAIRE
        POUR i DE 0 à n-1 FAIRE
            POUR j DE 0 à n-1 FAIRE
                // Mise à jour de la distance la plus courte de i à j si un chemin plus
                // court est trouvé via k
                SI Distance[i][k] + Distance[k][j] < Distance[i][j] ALORS
                    Distance[i][j] ← Distance[i][k] + Distance[k][j]
                FIN SI
            FIN FAIRE
        FIN FAIRE
    FIN FAIRE
FIN
```
Pour en apprendre davantage sur le fonctionnement de cet algortihme, voici sa page [Wikipédia](https://fr.wikipedia.org/wiki/Algorithme_de_Floyd-Warshall).  

Après l'exécution de l'algorithme, la matrice contiendra les distances séparants les personnes, allant de 0 (lorsque i=j) à l'infini (INT_MAX en l'absence de connexion).

- **Fonction `BronKerbosch`**

Cette fonction permet de rechercher toutes les cliques maximales dans un graphe. Elle prend en argument un graphe ainsi que différents ensembles pour réaliser la recherche et renvoie la clique maximale trouvée. Pour réaliser cette recherche, elle utilise comme son nom l'indique, l'algorithme de **Coenraad Bron** et **Joep Kerbosch**. À noter que le menu propose à l'utilisateur de choisir le sommet à partir duquel on doit rechercher la clique maximale (-1 signifie qu'on utilise le fonctionnement par défaut).

Voici la liste de ses paramètres :

1. **const graph_t * G :** Un pointeur vers la structure `graph_t` représentant le graphe.
2. **set_t * R :** Un pointeur vers un ensemble représentant la clique actuellement en construction.
3. **set_t * P :** Un pointeur vers un ensemble représentant les sommets candidats à rejoindre la clique.
4. **set_t * X :** Un pointeur vers un ensemble représentant les sommets exclus de la clique.
5. **set_t C :** Un pointeur vers un ensemble représentant la plus grande clique maximale trouvée.

Voici une version de l'algorithme en pseudo-code :  

```
PROCEDURE BronKerbosch(G: Graph_t, R: set_t, P: set_t, X: set_t, C: set_t)
// Algorithme de BronKerbosch
DEBUT
    SI P ET X sont tous les deux vides ALORS
        AJOUTER R à C
        RETOURNE
    FIN SI

    POUR CHAQUE sommet v DANS P FAIRE
        APPELER BronKerbosch(G, R ∪ {v}, P ∩ voisins(v), X ∩ voisins(v), C)
        RETIRER v de P
        AJOUTER v à X
    FIN FAIRE
FIN
```
Pour en apprendre davantage sur le fonctionnement de cet algortihme, voici sa page [Wikipédia](https://fr.wikipedia.org/wiki/Algorithme_de_Bron-Kerbosch).

## Deuxième partie - Gestion des listes
Cette section concerne les fichiers header **elmlist.h**, **list.h**, **person.h**, **friendship.h** et **io.h**, qui ont pour but de gérer les listes de personnes et d'amitiés.  

Vous trouverez ci-dessous la description des principales fonctions associées à ces fichiers :<br/><br/>

- **Fonctions `scan_person` et `scan_friendship`**

Ces deux fonctions permettent de saisir des personnes ou des amitiés sur le réseau. Elles utilisent chacune la primitive respective de leur fichier en-tête (`new_person` ou `new_friendship`) pour créer une structure de type **person_t** ou **friendship_t**. La saisie prend en charge la gestion des doublons.
En effet, si un élément est déjà présent dans la liste, il ne sera pas ajouté. **Attention :** la saisie des dates de naissance des personnes doit suivre la convention **JJ/MM/AAAA**, sinon le programme s'arrêtera.

- **Fonctions `cmp_person` et `cmp_friendship`**

Ces deux fonctions permettent de comparer deux personnes/amitiés et renvoient 0 si les personnes/amitiés en question sont égales (même nom/prénom). Ces fonctions sont utilisées un peu partout dans le code, notamment pour déterminer s'il faut insérer un élément dans une liste ou s'il est déjà présent dans celle-ci. 
Dans ce cas, on utilise les fonctions de comparaison associées à la fonction de recherche dans une liste `find`.

- **Fonctions `print_person` et `print_friendship`**

Ces fonctions permettent d'afficher une personne/amitié. Elles sont principalement utilisées par la fonction `view_list` qui prend en argument la liste en question et la fonction permettant d'afficher la donnée.

 - **Fonctions `stream_2_persons` et `stream_2_friendships`**

Ces deux fonctions permettent de lire toutes les personnes ou amitiés présentes dans un fichier (texte ou binaire) donné à l'aide de leurs primitives respectives `stream_2_person` et `stream_2_friendship`. Ces deux primitives s'occupent de lire depuis un fichier uniquement une personne/amitié. En outre, les fonctions principales ont été conçues pour ne pas ajouter à la liste un élément déjà présent dans celle-ci. Il n'est donc pas grave de retrouver plusieurs fois la même personne dans un fichier, car elle ne sera ajoutée qu'une seule fois (de même pour les amitiés). **Attention**, il est important de noter que pour pouvoir lire correctement un fichier texte, ces derniers doivent respecter certaines conventions.

- **Fonctions `person_2_stream` et `friendship_2_stream`** 

Ces deux fonctions permettent d'enregistrer une personne ou une amitié dans un fichier donné. Le menu s'occupe d'ouvrir le fichier selon le mode sélectionné et ajoute les données à la fin du fichier. À noter que si le fichier n'existe pas, alors il sera créé.

- **Fonctions `free_person` et `free_friendship`**

Ces deux fonctions permettent de libérer la mémoire allouée aux structures représentant les personnes et les amitiés. Afin de pouvoir supprimer correctement les personnes et les amitiés du réseau, nous avons fait le choix d'ajouter deux paramètres supplémentaires dans chaque fonction.  

Les paramètres **list_t * Lpers** et **list_t * Lfriends** ont donc été ajoutés pour permettre la mise à jour de la liste des personnes du réseau ainsi que la liste des amitiés du réseau. Cet ajout est très utile pour éviter de se retrouver avec des listes corrompues si celles-ci contenaient des personnes ayant été supprimées du réseau. Ces paramètres sont facultatifs et peuvent donc être NULL si jamais on ne stocke pas les personnes/amitiés dans des listes.

Les deux fonctions font appel à `remove_elm_from_list`, qui permet de supprimer un élément de liste en mettant à jour les successeurs et prédécesseurs des voisins de l'élément à supprimer.  

Pour finir, on notera que `free_person` s'occupe dans tous les cas de supprimer toutes les relations d'amitiés dans lesquelles la personne en question était impliquée. La fonction `free_friendship`, quant à elle, a deux possibilités de fonctionnement. Si le booléen 'del_person' est vrai, alors la fonction supprime les personnes du réseau à l'aide de `free_person`, en plus de supprimer la relation de base. Sinon, elle gère uniquement la suppression de la personne A de la liste d'amis de la personne B.

- **Fonction `list_2_graph`**

Cette fonction permet de convertir une liste de personnes **(list_t * Lpers)** en une matrice d'adjacence représentant un graphe **(graph_t *)**. Cette conversion permet de représenter les relations entre personnes (amis) sous forme de graphe, où chaque personne est un sommet et chaque relation d'amitié est une arête.

Fonctionnement : 

1. **Initialisation du graphe :** Un nouveau graphe est créé avec un nombre de sommets égal au nombre de personnes dans la liste. Chaque sommet correspond à une personne dans la liste.
2. **Parcours de la liste des personnes :** La fonction itère sur chaque élément de la liste de personnes **(Lpers)**. Pour chaque personne, elle parcourt sa liste d'amis.
3. **Création des arêtes :** Pour chaque ami d'une personne, la fonction trouve l'indice correspondant de cet ami dans la liste principale **(Lpers)** en utilisant `find_person_index`. Si l'ami est trouvé dans la liste, une arête est ajoutée dans la matrice d'adjacence du graphe, indiquant une relation entre la personne courante et son ami. Cette opération est effectuée de manière symétrique pour s'assurer que le graphe est non orienté (chaque relation est bidirectionnelle).
4. **Calcul des distances :** Après la construction de la matrice d'adjacence, la fonction distance_calculus est appelée pour calculer les distances minimales entre tous les sommets du graphe, en utilisant l'algorithme de Floyd-Warshall. Ces distances sont stockées dans la matrice distances du graphe.