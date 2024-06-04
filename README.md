# Projet Pluridisciplinaire d'Informatique Intégrative n°2 (PP2I-2)

Bienvenue sur le repository de PP2I-2 - Jeu vidéo en C.

## Sujet du PP2I 

Créer un jeu en language C à l'aide de la librairie SDL2.


## Membres du groupe 

Johan KASZCZYK ; 
Timéo ALAUX ; 
Julien MARLAND ; 
Alexis CHAVY


## Nom du Jeu

Ultimate Car Racing 80 : retour vers le passé


## Présentation du jeu

Dans Ultimate Car racing 80 retour vers le passé,

vous contrôlez une voiture et tentez de faire le plus rapidement possible le tour du circuit.
Le design retro, la 3d type "Mode 7", la musique 8 bits et le concept simple du jeu participent à un vrai retour du joueur dans le passé, 
dans les jeux vidéo arcade des années 80.

Vous avez accès à 2 circuits au style et à la structure complètement différents. Le véhicule qui vous est proposé est
une DeLorean DMC-12 inspirée de celle des films Retour vers le Futur.


## Installation

Suivez ces étapes pour configurer l'environnement de développement et installer l'application.


### Prérequis

Assurez-vous d'avoir installé sdl2-dev, sdl2_mixer-dev et sdl2_ttf-dev sur votre système.


## Utilisation

Toute la compilation et l'exécution est gérée par le Makefile. Il suffit d'exécuter les commandes suivantes en ligne de commande dans le dossier source :

"make main.out" pour compiler puis 
"./main.out" pour éxécuter OU
"make run" pour compiler et exécuter automatiquement ;
"make clean" pour nettoyer et supprimer les fichiers de compilation

/!\ POUR JOUER SOUS MACOS MERCI D'EXECUTER "make main_mac" AU LIEU DE "make main.out" PUIS "./main.out" /!\


## Choix d'implémentation

utilisation des librairies SDL2, SDL2_mixer et SDL2_ttf.


### Moteur 3D

Le rendu 3D se fait en plusieurs étapes :
- tout d'abord la gestion du roulis de la caméra (rotation autour de l'axe central) se fait simplement en rotatant l'écran autour de son centre, ce qui nécessite de calculer au tout début du rendu la zone de l'écran qui sera rendue, la rendre normalement, puis appliquer à la toute fin la rotation (avec SDL_RenderCopyEx) et afficher le résultat sur l'écran.
- ensuite, la skybox est affichée ; elle consiste en une image (texture) qui boucle sur elle-même, ce qui lui permet de représenter l'horizon sur 360° autour du joueur (mais son élévation sur l'horizon est donc limitée : avec une telle skybox, impossible de lever/baisser la tête trop haut/bas sans changer radicalement de procédé). Le rendu consiste à déterminer la portion de ce bandeau qu'il faut afficher en fond sur l'écran, puis le faire (via SDL_RenderCopy).
- vient ensuite la partie principale du rendu : le moteur est capable d'afficher une succession de plans (textures) à l'horizontale, chacun avec sa propre rotation, position, et échelle. Il lui faut en entrée une liste doublement chaînée de ces plans, TRIEE PAR HAUTEUR (coordonnée z croissante). Il prend aussi, pour chaque espacement interplan, une liste chaînée de tableaux de sprites, chacun avec une position dans l'espace 3D (la liste chaînée est stockée dans le plan directement sous l'espacement, autrement dit chaque plan garde une liste chaînée de tableaux de sprites directement au-dessus de lui). Il est capable ensuite de générer une vue de ces plans et sprites depuis n'importe quelle position et avec n'importe quelle orientation du regard (paramètres qu'il faut lui spécifier sous la forme d'une structure CAMERA). L'algorithme de rendu parcourt les plans par hauteur décroissante, depuis le plus haut jusqu'à la hauteur de la caméra, puis par hauteur croissante depuis le plus bas jusqu'à la hauteur de la caméra, et à chaque étape rend le plan et les sprites dans le bon espacement interplan. Tout ceci est fait afin d'afficher les plans dans le bon ordre (le plus éloigné sera rendu avant le plus proche de la caméra). Le rendu d'un ensemble de sprites consiste à calculer la position de chaque sprite dans l'espace caméra (avec quaternions et matrices de rotation), projeter chaque position (dans l'espace caméra) sur le plan écran de la caméra, vérifier que le sprite correspondant est bien visible pour la caméra, si oui l'ajouter dans un tableau à taille dynamique (mais déjà alloué avec une taille suffisante), puis trier ce tableau suivant la distance à la caméra (pas exactement mais c'est l'idée), et afficher les sprites du plus éloigné au plus proche, centré sur le projeté de leur position et avec une échelle dépendant de l'inverse de leur distance à la caméra (cette fois c'est exactement ce qu'il se passe). Le rendu d'un plan est du type Mode 7, il en suit l'idée générale : on délimite un trapèze (sur la texture du plan) correspondant à la partie visible par la caméra, on parcourt ce trapèze dans le sens de la hauteur et on copie chaque bande sur la ligne de l'écran correspondante. La détermination du trapèze ne tient compte que partiellement de l'orientation de la caméra (élévation sur l'horizon, appelée latitude dans le code), car la longitude est prise en compte avec une étape préalable consistant à rotater et placer la texture du plan dans l'espace caméra-sans-latitude-ni-roulis pour ainsi dire; le tout permet d'afficher un plan de n'importe quelle position, sous n'importe quel angle (avec le roulis on a bien les trois angles d'euler).

### Implémentation de la voiture

Le modèle physique de la voiture est basé sur une physique du point (la voiture est assimilée à un point massique). 
Mais plutôt que d'appliquer des forces on modifie directement sa vitesse frame après frame en fonction des touches 
pressées par le joueur, ce qui permet des contrôles dits "arcades" réactifs pour une meilleure jouabilité. 
La décélération du joueur (lorsqu'il cesse d'activer le moteur) suit tout de même une loi exponentielle inverse, 
donc des frottements classiques.

Gestion des collisions : elle est elle aussi complètement "arcade" et passe par des modifications directes 
de la vitesse de la voiture.

### Détection des collisions

A cette partie "gestionnaire de voiture" il faut donc rajouter le système de détection des collisions lui-même, qui 
consiste tout simplement en une grille (un tableau à deux dimensions) de tableaux indiquant la présence d'une voiture 
dans la case en question :
    Les voitures sont stockées dans un tableau à part, hors de la grille, et identifiées uniquement par leur indice dans ce tableau. 
    Chaque case de la grille contient une liste des identifiants des voitures présentes dans cette même case, cette liste des identifiants 
    étant un tableau dynamique. On a donc un tableau dynamique par case de la grille, mais comme on connaît au départ le nombre de voitures, 
    on connaît aussi la taille maximale de ces tableaux donc on peut allouer dès le départ la quantité de mémoire maximale dont ils auront 
    besoin (ce qu'on fait), ce qui fait finalement de notre grille un tableau à trois dimensions : nb_lignes * nb_colonnes * nb_voitures

On stocke aussi en mémoire, à part (hors de la grille), un tableau des segments constituants les hitbox du relief du terrain.
Pour éviter de parcourir l'entièreté de ce tableau à chaque test de collision entre une voiture et le terrain, ce tableau est trié de sorte 
à ce que tous les segments faisant partie d'une même case de la grille soient côtes à côtes en mémoire (quitte à mettre des doublons dans le cas 
d'un segment appartenant à plusieurs cases de la grille). Une fois ceci fait, il ne reste plus qu'à indiquer dans chaque case de la grille 
(en plus du tableau dynamique de voitures sujet du précédent paragraphe) l'indice correspondant dans le tableau des segments, et ainsi si on 
connaît les cases dans lesquelles une voiture se trouve, on peut aller lire dans ces cases les régions du tableau de segments correspondantes 
(et se contenter de parcourir ces régions, qui font en pratique (au total sur qquelques cases) de l'ordre de la dizaine de segments, à comparer 
aux milliers de segments composant l'entièreté du relief terrain...)
Il s'agit d'un compromis mémoire/vitesse d'exécution complètement en faveur de la vitesse d'exécution


### Circuit

Concernant la concrétisation du circuit nous avons créé un ensemble de checkpoints qui doivent être passés dans l'ordre et sans en manquer 
(pour éviter de pouvoir finir le circuit plus rapidement en utilisant des bugs de collisions) avant de passer la ligne d'arrivée ce qui affiche le 
temps final et le meilleur temps (sauvegardé d'une session à l'autre).

(Non implémenté) nous avons démarré la réalisation d'adversaires à affronter, ceux-ci auraient eu des mouvements dirigés par du pathfinding entre 
les différents checkpoints créés tout au long du parcours.

### Musique

La musique est entièrement gérée avec les fonctions de la librairie SDL_mixer, en utilisant une msuique 8 bits libre de droits.

