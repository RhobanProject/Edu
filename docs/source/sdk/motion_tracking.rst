
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

.. cpp:function:: TrackerClient::Tracker_Client(string name,string ip, int port, int cap)

Pour chaque ``rigid body`` tracké par le logiciel il faut instancier un
objet. ``name`` représente le nom du rigid body défini dans le
logiciel. Le constructeur prend l'adresse de la machine, ``ip``, sur
laquelle tourne Tracking Tools et un ``port``, la connexion est
initialisée à ce moment.  Le dernier argument ``cap`` définit le nombre
maximum de données sauvegardées. Une fois initialisé l'instance stockera
les données réçues.

Obtenir les informations
------------------------

.. cpp:function:: Tracking_Data * TrackerClient::getInfo()

.. cpp:function:: Tracking_Data * TrackerClient::getLastInfo()

.. cpp:function:: void TrackerClient::Log(string file_name)

Trois méthodes permettent d'extraire les informations, ``Log(string
file_name)`` écrit l'intégralité des données stockées dans le fichier
dont le nom est passé en paramètre. ``getInfo()`` la donnée la plus
vieille n'ayant pas été lue et ``getLastInfo()`` renverra la dernière
donnée reçue.

Changer la valeur du discount
-----------------------------

.. cpp:function:: void TrackerClient::setDiscount(double discount)

La méthode ``setDiscount(double discount)``
permet de changer la valeur du discount utilisé pour lisser les données.

Exemples
~~~~~~~~

C++ :

.. code-block:: cpp

    // l'objet est instancié
    TrackerClient tc("tracker", "192.168.16.100", 3883, -1);

    //on change la valeur du discount pour le lissage de la capture
    tc.setDiscount(0.2);

    int i;

    //on affiche les données reçues dans l'ordre de leur arrivée
    //pendant les 600 premier tour de boucle
    for ( i = 0; i < 600; i++) {
        Tracking_Data * temp = tc.getInfo();

        if (temp != NULL) {
            cout << "Timestamp : " << temp->timestamp << endl;

            cout << "Position : (" << temp->pos[0] << "," << temp->pos[1] << ",";
            cout << temp->pos[2] << ")" << endl;

            cout << "Rotation Matrix :" << endl;
            cout << temp->rot_mat[0][0] << " " << temp->rot_mat[0][1] << " ";
            cout << temp->rot_mat[0][0] << endl;
            cout << temp->rot_mat[1][0] << " " << temp->rot_mat[1][1] << " ";
            cout << temp->rot_mat[1][0] << endl;
            cout << temp->rot_mat[2][0] << " " << temp->rot_mat[2][1] << " ";
            cout << temp->rot_mat[2][0] << endl << endl;

            usleep(10);
        }

        delete temp;
    }

    //on affiche la dernière donnée reçue
    Tracking_Data * temp = tc.getLastInfo();

    if (temp != NULL) {
        cout << "Timestamp : " << temp->timestamp << endl;

        cout << "Position : (" << temp->pos[0] << "," << temp->pos[1] << ",";
        cout << temp->pos[2] << ")" << endl;

        cout << "Rotation Matrix :" << endl;
	cout << temp->rot_mat[0][0] << " " << temp->rot_mat[0][1] << " ";
        cout << temp->rot_mat[0][0] << endl;
        cout << temp->rot_mat[1][0] << " " << temp->rot_mat[1][1] << " ";
        cout << temp->rot_mat[1][0] << endl;
        cout << temp->rot_mat[2][0] << " " << temp->rot_mat[2][1] << " ";
        cout << temp->rot_mat[2][0] << endl << endl;
    }

    //on enregistre tout les données reçue dans tracking.log
    tc.Log("tracking.log);
