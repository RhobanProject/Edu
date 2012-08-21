
.. _commands:

Commandes
=========

Fonctionnement
~~~~~~~~~~~~~~

Les commandes sont présentes nativement dans le SDK vous permettent d'effectuer
des actions et intéragir avec le serveur sans écrire de code.

.. note::

    Les commandes liront le :ref:`fichier de configuration <configuration>` ``config.yml`` pour charger les robots.

L'appel à une commande peut se faire de la manière suivante :

.. code-block:: bash

    ./sdk <commande> arguments

Pour obtenir la liste des commandes, vous pouvez saisir :

.. code-block:: bash

    ./sdk help

Les commandes sont regroupées en familles. 

Certaines commandes s'appliquent à plusieurs robots (comme ``emergency`` ou ``status``) et d'autres
à un seul (comme ``loadmove`` ou ``set``).

Celles qui s'appliquent à plusieurs robots seront effectuées par défaut sur tous les robots sauf
si une liste de robot est passée en argument (ex: ``./sdk emergency spider humanoid``).

Les commandes ne s'appliquant qu'à un seul robot prennent comme premier paramètre le nom du robot
concerné.

Commandes basiques
~~~~~~~~~~~~~~~~~~

``emergency``: Arrêt d'urgence
------------------------------

.. code-block:: bash

    ./sdk emergency [robot1 [robot2 ...]]

Cette commande est importante à connaître car elle permettra d'arrêter les mouvements d'un
ou plusieurs robots et de relâcher les moteurs.

Tous les mouvements seront déchargés et les moteurs passés en mode de compliance (torque 0).

``status``: Obtenir des informations sur le serveur
---------------------------------------------------

.. code-block:: bash

    ./sdk status [-v] [robot1 [robot2 ...]]

La commande status vous retournera des informations sur le serveur :

* La version du serveur
* Les :ref:`configurations <configurations>` chargées
* Le nombre de moteurs reconnus et présents dans le fichier de configuration
* Le nombre de mouvements chargés

Si l'option verbeuse (``-v``) est activée, des informations plus approfondies
seront affichées :

* Le nom, id, angle, charge et vitesse des moteurs
* Le nom des mouvements actuellement chargés

Commandes moteur
~~~~~~~~~~~~~~~~

``compliant``: Relâcher les servomoteurs
----------------------------------------

.. code-block:: bash

    ./sdk compliant [robot1 [robot2 ...]]

Relâche les torque des servomoteurs

``hard``: Durcir les servomoteurs
---------------------------------

.. code-block:: bash

    ./sdk hard [robot1 [robot2 ...]]

Active le torque des robots en conservant les angles actuels ("durcit" les moteurs)

``init``: Prendre la position initiale
--------------------------------------

.. code-block:: bash

    ./sdk init [robot1 [robot2 ...]]

Prend la position d'initialisation. Cette posision est présente dans le fichier de
configuration des servomoteurs (:ref:`MoveSchedulerConfig <configurations>`)

Le torque sera monté progressivement pendant plusieurs secondes pour éviter les 
mouvements brusques.

``zero``: Prendre le zéro géométrique
-------------------------------------

.. code-block:: bash

    ./sdk zero [robot1 [robot2 ...]]

De même que ``init``, le torque sera monté progressivement pendant plusieurs
secondes pour éviter les mouvements brusques.

``monitor``: Monitorer les servomoteurs
---------------------------------------

.. code-block:: bash

    ./sdk monitor [-f frequency] [-i] <robotName>

Lance le monitoring des servomoteurs d'un robot. Un tableau texte sera écrit dans 
le terminal représentant les moteurs, leur angle, charge et vitesse.

Si l'option ``-i`` est activée, les moteurs seront triés par ``id`` au lieu d'être
trié par nom.

L'option ``-f frequency`` permet de définir une fréquence de rafraîchissement en hz 
du tableau (1 par défaut).

``scan``: Lancer un scan des servomoteurs
-----------------------------------------

.. code-block:: bash

    ./sdk scan [robot1 [robot2 ...]]

Lance le scan des moteurs d'un robot.

``snapshot``: Capturer la position des moteurs
----------------------------------------------

.. code-block:: bash

    ./sdk snapshot [-p] [-r] [robot1 [robot2 ...]]

Capture la valeur des angles moteur et l'affiche dans le terminal.

Si l'option ``-p`` est activé, la sortie sera affichée sous forme de dictionnaire Python.

Si l'option ``-r`` est activé, les angles seront relatifs à la position initiale et au
zéro géométrique.

``set``: Définir la position et le torque d'un moteur
-----------------------------------------------------

.. code-block:: bash

    ./sdk set [-r] <robot> <motorName> <angle> [load = 1 [speed = 1]]

``set`` permet de régler les valeurs d'un moteur (angle, torque et speed).

Si l'option ``-r`` est activée, l'angle sera définit de manière relative à la position
initiale et au zéro géométrique.

Les paramètres ``load`` et ``speed`` servent à régler respectivement la
charge (c'est à dire la puissance) et la vitesse du moteur. Ils sont exprimés sous
forme de nombres flottants en 0 et 1.

Commandes mouvement
~~~~~~~~~~~~~~~~~~~

``loadmove``: Charger un mouvement
----------------------------------

.. code-block:: bash

    ./sdk loadmove <robotName> <moveName>

Envoie un mouvement sur un robot.

.. note::

    N'oubliez pas que si votre :ref:`fichier de configuration <configuration>` ``config.yml`` 
    contient une directive ``loadMoves``, il n'est pas nécéssaire de charger les mouvements à 
    l'aide de cette commande, car ils seront chargés automatiquement lors de la lecture de la 
    configuration.

``startmove``: Lancer un mouvement
----------------------------------

.. code-block:: bash

    ./sdk startmove <robotName> <moveName> [duration = 0 [smooth = 500]]

Lance un mouvement sur le robot distant.

Les paramètres ``duration`` et ``smooth`` servent respectivement à régler la durée et 
le lissage du début de mouvement et sont exprimés en millisecondes.

``stopmove``: Arrêter un mouvement
----------------------------------

.. code-block:: bash

    ./sdk stopmove <robotName> <moveName> [smooth = 500]

Arrête un mouvement.

Le paramètre ``smooth`` sert à régler le lissage de l'arrêt de mouvement, il est
exprimé en millisecondes.

``killmove``: Tuer/décharger un mouvement
-----------------------------------------

.. code-block:: bash

    ./sdk killmove <robotName> <moveName>

Arrête et décharge un mouvement du serveur.

Il est recommandé de ne pas utiliser ``killmove`` lorsqu'un mouvement est en train d'être exécuté.

``updateconstant``: Mettre à jour une constante
-----------------------------------------------

.. code-block:: bash

    ./sdk updateconstant <robotName> <moveName> <constantName> <value>

Met à jour une constante de mouvement.
