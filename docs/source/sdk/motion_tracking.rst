
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
objet. **name** représente le nom du rigid body défini dans le
logiciel. Le constructeur prend l'adresse de la machine, **ip**, sur
laquelle tourne Tracking Tools et un **port**, la connexion est
initialisée à ce moment.  Le dernier argument **cap** définit le nombre
maximum de données sauvegardées. Une fois initialisé l'instance stockera
les données réçues.

Obtenir les informations
------------------------

Deux méthodes permettent d'extraire les informations :
*void Log(string* **file_name** *)*
qui écrit l'intégralité des données stockées dans le fichier dont le nom
est passé en paramètre.
*Tracking_Data* * *getInfo(enum mode* **mode** *)*
deux modes sont possibles *LASTDATA* qui renvoie la dernière données
reçue et *CURSDATA* qui renvoie la donnée la plus vieille n'ayant
pas était lue.

Changer la valeur du discount
-----------------------------

La méthode *void setDiscount(int* **discount** *)* permet de changer
la valeur du discount utilisé pour lisser les données.
