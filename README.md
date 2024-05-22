# PP2I-2 - Jeu vidéo en C

Bienvenue sur le repository de PP2I-2 - Jeu vidéo en C.

## Nom du Jeu

UltimateCarRacing80 : retour vers le passé

## Installation

Suivez ces étapes pour configurer l'environnement de développement et installer l'application.

### Prérequis

- Assurez-vous d'avoir installé [sdl2_la_bonne_version] sur votre système.
- Assurez-vous d'avoir [C_bonne_version] d'installé sur votre système.


### Installation des dépendances

Pour installer les dépendances, exécutez la commande suivante :

```bash
pip install -r requirements.txt
```

## Utilisation

Pour lancer l'application, exécutez la commande suivante :

```bash
[COMMANDE A RENSEIGNER]
```

## Contribution

Pour créer votre branch, exécutez la commande suivante :

```bash
git branch <nom-de-votre-branch>
```
Utilisez un nom de branch qui décrit ce que vous faites précédé du type de branch (feature, fix, etc.).
Exemple : feature/collisions

Au moment de faire le premier push de votre branch, utilisez la commande suivante :

```bash
git push --set-upstream origin <nom-de-votre-branch>
```


Une fois que vous voulez merge votre branch avec la branche main exécutez les commandes suivantes :

```bash
git checkout main
git pull origin main
git checkout <nom-de-votre-branch>
git merge main
```

Une fois que vous avez vérifié que tout fonctionne, vous pouvez push votre branch sur le repo avec la commande suivante :

```bash
git push origin <nom-de-votre-branch>
```

Ensuite, vous pouvez créer une pull request sur GitHub pour demander à ce que votre branch soit mergée avec la branche main.
