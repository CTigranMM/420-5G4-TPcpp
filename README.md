# TP1 C++ TP Tech Emergeant
Repo de : Tigran Michael Matinyan

# Les fonctionnalités que vous avez choisi d’implémenter.

| Section | Fonctionnalités |
| :------- | :------: |
| Interface et Expérience Utilisateur | Détection des doublons plus intelligente (même titre, auteur, ISBN).
| Gestion des Donnée    | Tri des résultats par titre, auteur pour l’affichage (utilisation de la fonction de tri de la STL).

# Section des Questions
<h4> Question 1 :
<p> Au début, on n'avait pas vraiment vu la bibliothèque iostream en détail dans le cours. Une de mes premières galères a été de comprendre exactement ce que la méthode fromFileFormat devait faire. Après avoir cherché un peu sur le web et utilisé une IA pour m'aider à débloquer, j'ai fini par comprendre le principe, surtout qu'on avait déjà fait un truc un peu semblable dans un autre cours.

Ensuite, ce qui m'a pris le plus de temps (presque 2 jours de gossage), c'est de faire fonctionner les logs dans un fichier externe. Il a fallu que j'utilise des bibliothèques qu'on n'avait jamais vues :

   chrono : pour aller chercher l'heure exacte du système

   ctime : pour transformer ce temps-là en vraie date lisible

   iomanip : pour formater le texte et les dates proprement

Vu qu'on n'avait pas couvert ça en classe, j'ai dû fouiller pas mal dans la doc externe pour réussir à compiler le tout sans erreurs. </p>
<h4> Question 2 :
<p> J'aime vraiment l'idée d'utiliser un serveur pour stocker les données parce que c'est plus sécuritaire et les informations reste privée. Actuellement, il y a deux gros problèmes évidents : les données sont enregistrées en local et le logiciel peut seulement être utilisé sur un seul poste à la fois.

Pour rendre le système « futureproof » face à des millions de livres, je créerais un backend en C++ ou une alternative plus moderne comme Ruste (pour garder la rapidité du C++ tout en gérant la mémoire de façon plus efficace et sécuritaire), connecté à une vraie base de données comme MySQL pour bien gérer les tables SQL.

Au lieu de tout manipuler dans un terminal en ligne de commande, on pourrait faire une interface web très simple (user friendly) protégée par un mot de passe administrateur. De cette façon, si la Alexandrie veut moderniser son système plus tard avec une interface usager plus moderne, la transition sera déjà prête et se fera beaucoup plus facilement.

# Construire le projet
Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de donner chemin vers le repertoire data avec l'application `bibliotheque -d <chemin verrs data>`
