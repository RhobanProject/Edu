
.. _motors:

Moteurs
=======

Le SDK met à votre disposition une API facile pour manipuler l'ensemble des moteurs
de votre robot.

Avant de faire cela, vous devrez être :ref:`connecté <communication>` et avoir 
chargé la :ref:`configuration <configurations>` de vos servos.

Voici un exemple de code utilisant la couche moteur recopiant la valeur d'un servomoteur
("Left") dans un autre ("Right")

C++
---

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

Python
------

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
