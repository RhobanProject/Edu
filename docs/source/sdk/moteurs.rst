
.. _motors:

Moteurs
=======

Le SDK met à votre disposition une API simple pour manipuler l'ensemble des moteurs
de votre robot.

Avant de faire cela, vous devrez être :ref:`connecté <communication>` et avoir 
chargé la :ref:`configuration <configurations>` de vos servomoteurs.

Voici un exemple de code utilisant la couche moteur recopiant la valeur d'un servomoteur
("Left") dans un autre ("Right").

Le nom des moteurs est indiqué dans `MoveSchedulerConfig` (cf :ref:`moves`).

Lancement du dispatcher
-----------------------

.. cpp:function:: void Motors.start(int frequency)

.. py:function:: Motors.start(frequency)

La fonction ``start`` lance un processus léger (thread) qui synchronisera à la fréquence
``frequency`` Hz la valeur des moteurs (lecture+écriture).

La configuration :ref:`MoveSchedulerConfig <configurations>` doit être chargée au moment de
l'appel à cette méthode, en effet, ce fichier de configuration permet de faire le lien entre
le nom des moteurs et leurs identifiants.

Accéder à un moteur
-------------------

.. cpp:function:: void Motors.operator[](string name)

.. py:function:: Motors.__getitem__(name)

L'opérateur ``[]`` sur l'objet motors permet d'obtenir l'instance d'un moteur et d'intéragir
avec ses valeurs (angle, charge, vitesse).

Lire les valeurs
~~~~~~~~~~~~~~~~

.. cpp:function:: double Motor.getAngle()
.. cpp:function:: double Motor.getLoad()
.. cpp:function:: double Motor.getSpeed()

.. py:function:: Motor.getAngle()
.. py:function:: Motor.getLoad()
.. py:function:: Motor.getSpeed()

Les accesseurs ``getAngle()``, ``getLoad()`` et ``getSpeed()`` permettent d'obtenir respectivement
l'angle, la charge et la vitesse d'un moteur.

L'angle est exprimé en degré, la charge et la vitesse sont des nombre flottants allant de 0 à 1.

Ces valeurs sont lues depuis le moteur à la fréquence indiquée lors de l'appel à ``start()``.

Ecrire les valeurs
~~~~~~~~~~~~~~~~~~

.. cpp:function:: void Motor.setAngle(double angle)
.. cpp:function:: void Motor.setLoad(double load)
.. cpp:function:: void Motor.setSpeed(double speed)

.. py:function:: Motor.setAngle(angle)
.. py:function:: Motor.setLoad(load)
.. py:function:: Motor.setSpeed(speed)

De même, les accesseurs ``setAngle()``, ``setLoad()`` et ``setSpeed()`` permettent de définir
la valeur pour l'angle, la charge et la vitesse du moteur.

Ces valeurs sont écrites sur les moteurs à la fréquence indiquée lors de l'appel à ``start()``.

Exemples
--------

C++ :

.. code-block:: cpp

    // Connexion & chargement des configurations
    ...

    robot.motors.start(30); // Lance le dispatcher moteur à 30Hz
    robot.allCompliant(); // Relache les servos

    robot.motors["Right"].setLoad(1023); // Met la charge de "Right" au maximum

    while (1) {
        // Recopie l'angle du servo "Left" dans "Right"
        robot.motors["Right"].setAngle(robot.motors["Left"].getAngle())
        // Dort un peu
        usleep(50000);
    }

Python :

.. code-block:: python

    # Exemple d'accès à la couche moteurs (Python)

    # Connexion & chargement des configurations
    ...

    robot.motors.start(30) # Lance le dispatcheuir moteurs à 30Hz
    robot.allCompliant() # Relâche les servos

    robot.motors['Right'].setLoad(1023) # Met la charge de "Right" au maximum

    while True:
        # Recopie l'angle du servo "Left" dans "Right"
        robot.motors['Right'].setAngle(robot.motors['Left'].getAngle())
        # Dort un peu
        time.sleep(0.05)
