
.. _commands:

Commandes
=========

Les commandes sont présentes nativement dans le SDK vous permettent d'effectuer
des actions et intéragir avec le serveur sans écrire de code.

.. note::

    Les commandes lieront le :ref:`fichier de configuration <configuration>` ``config.yml`` pour charger les robots.

L'appel à une commande peut se faire de la manière suivante :

.. code-block:: bash

    ./sdk <commande> [robot robot2 ...]

Par exemple:

.. code-block:: bash

    ./sdk compliant spider

Pour relâcher les moteurs du robot définit par l'entée ``spider`` du :ref:`fichier de configuration <configuration>`.

Si aucun robot n'est spécifié, tous les robots seront touchés par la commande.

``compliant``: Relâcher les servomoteurs
----------------------------------------

*A venir*

``init``: Prendre la position initiale
--------------------------------------

*A venir*

``monitor``: Monitorer les servomoteurs
---------------------------------------

*A venir*

``set``: Définir la position et le torque d'un moteur
-----------------------------------------------------

*A venir*

``snapshot``: Capturer la position des moteurs
----------------------------------------------

*A venir*

``status``: Obtenir des informations sur le serveur
---------------------------------------------------

La commande status vous retournera des informations sur le serveur :

* La version du serveur
* Les :ref:`configurations <configurations>` chargées
* Eventuellement le nombre de moteurs actuellement reconnus


``zero``: Prendre le zéro géométrique
-------------------------------------

*A venir*
