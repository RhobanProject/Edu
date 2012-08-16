
.. _motion_tracking:

Motion-Tracking
===============

La gestion du motion tracking
-----------------------------

Le logiciel Tracking Tools embarque un système de streaming des
données via VRPN. La classe Tracker_Client a pour but de récupérer
et traiter les données streamée.

L'initialisation
----------------

*Tracker_Client(string name,string ip, int port, int cap)*

Pour chaque "rigid body" tracké par le logiciel il faut instancier un
objet. . Le constructeur prend l'adresse de la machine sur laquelle
tourne le logiciel et un port sur lequel écouter, la connexion est
donc gérée à ce moment.  Le dernier argument
