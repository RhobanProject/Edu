
.. _capteurs:

Capteurs
========

La couche capteurs vous permet de monitorer et d'accéder aux valeurs des capteurs
de votre robot en temps réel.

Lancer le monitoring 
~~~~~~~~~~~~~~~~~~~~

.. cpp:function:: void Sensors::start(int frequency)

.. py:function:: Sensors.start(frequency)

Lance un thread de monitoring des capteurs qui maintiendra à jours les valeurs des capteurs

La fréquence, exprimée en hertz, correspond au nombre de mise à jour par seconde des capteurs

Accéder aux valeurs
~~~~~~~~~~~~~~~~~~~

.. cpp:function:: void Sensors::operator[](string sensorName)

.. py:function:: Sensors.__getitem__(sensorName)

L'opérateur ``[]`` vous permet d'accéder à un capteur à partir de son nom

Exemple
~~~~~~~

C++ :

.. code-block:: cpp

    // Connexion & initialisation
    ...

    // Lancement du monitoring des capteurs
    robot.sensors.start(10)

    // Accès à une valeur
    value = robot.sensors["Accelerometer"].getValue()

Python :

.. code-block:: python
    
    # Connexion & initialisation
    ...

    # Lancement du monitoring des capteurs
    robot.sensors.start(10)

    # Accès à une valeur
    value = robot.sensors['Accelerometer'].getValue()
