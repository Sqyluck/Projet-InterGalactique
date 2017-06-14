# Brain

pour récupérer le package :

- creer un projet visual studio console constellation appeler brain

- dans le dossier de visual studio supprimer le fichier 'programe.cs'

- copier coller les quatre fichiers 'User.cs', 'Notif.cs', 'programme.cs' et 'UsersSave.txt'au même endroit

- dans visual studio -> click droit sur brain 
			-> ajouter 
			-> element existants 
			-> selectionner les quatre fichiers ajouter précedement 

ATTENTION CHEMIN COMPLET 
dans le code il faut changer dans les fonctions loadUsers et saveUsers le chemin du fichier 'UsersSave.txt' 
et mettre votre propre chemin, pour l'instant le chemin relatif ne fonctionne pas.

pour trouver le chemin, click droit sur 'UsersSave.txt' -> propriété -> chemin complet -> copier/coller et ca part

Si vous faites pas ca ca ne fonctionnera pas !! on vient pas se plaindre les lapins, vous etes prevenus <3

sur visual studio :
- tester le package en mode debug en appuyant sur la fleche noir dans un cercle vert
- publier le package sur constellation avec la fleche bleu vers de haut 
	-> choisir de publier sur le server constellation local
	-> choisir localhost:8088