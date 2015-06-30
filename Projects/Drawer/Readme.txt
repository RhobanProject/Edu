Le projet est décomposé en 2 binaires.
- Le binaire qui gère l'enregistrement et l'execution de mouvements du bras
- Le binaire qui applique l'algo GMM-GMR


1. Communication avec le robot

1.a) Installation
Compiler le fichier drawerBin.cpp avec le SDK (durant le projet il était placé dans Edu/sdk/cpp/example).
Adapter le CMakeLists.txt pour y intégrer ce fichier source si besoin.
Après compilation (cmake puis make), lancer le programme généré.

1.b) Execution
Deux modes d'execution sont possible. 
  ./drawerBin GET <fileName> <nbrExperiment> <nbLoop>
Pour effectuer un enregistrement de mouvement.
fileName est votre nom de fichier, nbreExperiment le nbre d'experience, 
et nbLoop la durée de l'experience en nombre de boucles (=multiples de 30ms)
  ./drawerBin USE <fileName>
Pour effectuer un mouvement à partir d'un fichier de mouvement.


2. Execution de l'algorithme

2.a) Installation
Accéder au dossier gmm-gmr et compiler le programme avec le makefile fourni.
Placer les fichiers des enregistrements d'entrée dans le dossier input. 

2.b) Execution
Lancer le programme avec pour argument la base du nom du fichier. La programme ajoute 01.txt, 02.txt, ... à la base. Le résultat se trouve dans output/data_final.txt.
Vous n'avez plus qu'à le réexécuter avec drawerBin.

