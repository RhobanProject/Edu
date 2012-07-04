
.. _protocol:

Protocole
=========

Fonctionnement
~~~~~~~~~~~~~~

Le protocole de Rhoban est un protocole *binaire*, ce qui veut dire que les 
données sont transmises de manière "brute" sur le réseau.

Le client établit tout d'abord une connexion avec le serveur, puis il peut
envoyer des trames auxquelles le serveur répondra.

Forme des trames
~~~~~~~~~~~~~~~~

Les trames sont constituées d'une en-tête suivi de données, voici la forme
des en-têtes :

============= =============== ============================================
Taille (bits) Nom du champ    Description du champ
============= =============== ============================================
**32**        UID             L'UID (pour Unique ID) est un identifiant permettant
                              d'identifier un message de manière unique. Il peut
                              par exemple être incrémenté à chaque transmission de
                              trame. Cet identifiant permettra une traçabilité des
                              messages en mode d'opération asynchrone.

**32**        Destination     La destination est l'identifiant d'un composant du
                              serveur auquel on parle.

**32**        Commande        La commande est l'opération que l'on souhaite invoquer
                              sur le composant serveur cible.

**32**        Longueur        Représente la longueur des données en octets.
============= =============== ============================================

Ces nombres sont tous des entiers, octet de poid fort d'abord (MSB).

La trame est alors suivi d'un certain nombre d'octets de données, selon la taille
indiquée dans l'en-tête "Longueur".

Le serveur répondra par une trame de la même forme. La trame de réponse comportera 
la même destination et le même UID que le message envoyé. La commande et la longueur
en revanche peuvent varier.

Types
~~~~~

Chaque couple Destination/Commande correspond à une certaine opération qui est
caractérisée par une liste d'arguments passées lors de la requête et une liste
d'arguments retournés par le serveur au moment de la réponse.

L'ensemble des specifications des commandes peut se trouver dans le fichier
``commands.xml`` disponible dans le SDK.

Voici les types employés :

============== =============== =================================================
Taille (bit)   Nom du type     Specification
============== =============== =================================================
**32**         ``int``         Un entier signé, octet de poid fort d'abord (MSB)

**32**         ``ui32``        Un entier non signé, octet de poid fort d'abord (MSB)

**32**         ``float``       Un nombre flottant suivant la spécification IEEE 754,
                               codé sur 32 bits

**8**          ``byte``        Un octet
============== =============== =================================================

Il est également possible de représenter des tableaux à l'aide de la notation 
``type[]``, un tableau est représenté à l'aide d'une longueur (entier non signé de 32bits)
suivie de l'ensemble des éléments du tableau. Notez que cette définition est récursive,
le type ``float[][][]`` sera alors un tableau de tableau de tableau de flottants.

Le type spécial ``string`` est équivalent à ``byte[]``.

Destinations
~~~~~~~~~~~~

Dans le fichier ``commands.xml``, vous trouverez les destinations sous la forme
de chaînes de caractères, voici le tableau de correspondance avec les entiers :

======================== ===============================
Nom de la destination    Indice correspondant
======================== ===============================
``error``                0
``server``               1
``system``               2
``low_level``            3
``move_scheduler``       4
``vision``               5
``localisation``         6
======================== ===============================
