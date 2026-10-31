# Gestion des étudiants et des absences

Application de bureau (C++ / Qt) pour gérer une liste d'étudiants : fiches détaillées, absences, import et export en **JSON** et en **XML**.

Projet réalisé en 1re année de BUT Informatique, **à deux** avec Maxence Mine. Stack : C++17, Qt 6 (Widgets, XML).

## Fonctionnalités

- Liste des étudiants avec **filtre** de recherche.
- Fiche détaillée : nom, prénom, date de naissance, numéro étudiant, INE, téléphone, emails, adresses, section, photo.
- Ajout, modification et suppression d'un étudiant.
- Tableau des **absences** de chaque étudiant (date, justifiée ou non, motif) : ajout, modification, suppression.
- Ouverture et enregistrement en **JSON** ou en **XML**, enregistrement sous un autre nom, et alerte en cas de modifications non enregistrées à la fermeture.

## Répartition du travail

| | Qui |
|---|---|
| Interface Qt (`mainwindow.cpp/.h`), liste, fiche, tableau des absences, branchement des boutons et des menus | **Emmanuel Bochu** (543 lignes sur 557 du fichier, d'après `git blame`) |
| Classes de données et logique : `Student`, `Section`, `Absence`, `StudentManager` (chargement et sauvegarde JSON/XML, filtre) | **Maxence Mine** (écrit pour l'essentiel) |

La répartition vient de l'historique git du dépôt (24 commits : 16 de Emmanuel, 8 de Maxence). Le diagramme UML du projet est dans [`docs/diagramme-uml.png`](docs/diagramme-uml.png).

## Compiler et lancer

Prérequis : Qt 6 (modules Widgets et XML) et un compilateur C++17.

```bash
qmake6 S201/S201.pro
make
./S201.app/Contents/MacOS/S201     # macOS
```

Ou ouvrir `S201/S201.pro` dans Qt Creator. L'application démarre **vide** : utiliser *Ouvrir* et choisir `exemples/etudiants_exemple.json`.

Les données d'exemple sont entièrement **fictives** (noms, adresses et numéros inventés).

## Limites connues

- L'interface est écrite en anglais dans le code avec un fichier de traduction prévu pour le français, qui n'est pas fourni.
- Le chargement ne fait pas de contrôle poussé sur le contenu des fichiers : un fichier mal formé peut être refusé sans message précis.
