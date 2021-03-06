
.. _moves:

Mouvements
==========

La couche de mouvements vous permet d'intéragir avec l'ordonnanceur de mouvement
du :ref:`Serveur <server>`.

Avant de pouvoir utiliser les mouvements, il vous faudra être :ref:`connecté <communication>`
et avoir :ref:`chargé un environement <load_env>` sur le robot.

Envoyer un mouvement
~~~~~~~~~~~~~~~~~~~~

.. note::

    Il est préférable de charger les mouvements à l'aide du :ref:`fichier de configuration
    Yaml <configuration>`.

Avant de pouvoir utiliser un mouvement, il faudra l'envoyer sur le serveur à l'aide de la
méthode ``loadMove()``.

.. cpp:function:: void Robot::loadMove(string name)

.. py:function:: Robot.loadMove(name)

.. note::

    Le fichier de mouvement se trouve sous le dossier de l'environement dans ``Moves/``.
    Pensez à bien définir l'environement avant de charger un mouvement.

Le contenu du fichier mouvement est alors envoyé sur le serveur qui le lit et le charge.

Lancer un mouvement
~~~~~~~~~~~~~~~~~~~

.. cpp:function:: void Robot::startMove(string name, int duration = 0, int smooth = 500)

.. py:function:: Robot.startMove(name, duration = 0, smooth = 500)

La méthode ``startMove`` permet de lancer un mouvement sur le serveur. Le mouvement sera
automatiquement arrêté après ``duration`` millisecondes. La valeur de ``smooth`` permet
de démarrer le mouvement en douceur, les valeurs envoyées au servomoteur seront amplifiées
progressivement pendant ``smooth`` millisecondes.

.. note::
    Si le mouvement n'est pas "brutal" ou si vous n'êtes pas sûrs de votre mouvement,
    il est fortement conseillé d'utiliser un ``smooth`` important.

Mettre à jour une constante de mouvement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les constantes sont des valeurs contenues dans le mouvement qui permettent en général de
paramètrer le comportement du mouvement.

Il est possible de les mettre à jour à l'aide des méthodes suivantes :

.. cpp:function:: void Robot::updateConstant(string moveName, string constantName, vector<float> values)

.. py:function:: Robot.updateConstant(moveName, constantName, values)

Mettre un mouvement en pause
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un mouvement lancé peut être mis en pause à l'aide de la méthode ``pauseMove()``

.. cpp:function:: void Robot::pauseMove(string moveName)

.. py:function:: Robot.pauseMove(moveName)

Stopper un mouvement
~~~~~~~~~~~~~~~~~~~~

.. cpp:function:: void Robot::stopMove(string name, int smooth = 500)

.. py:function:: Robot.stopMove(name, smooth = 500)

La méthode ``stopMove()`` permet d'arrêter un mouvement. Le mouvement sera stoppé progressivement
au cours de ``smooth`` millisecondes.

.. note::
    Lors de l'arrêt d'un mouvement, le robot reviendra dans sa position initiale.
    Dans tous les cas, il est fortement conseillé d'utiliser une forte valeur pour le 
    ``smooth``, pour éviter les chocs des moteurs.

Obtenir la liste des mouvements chargés
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La liste des mouvements chargés peut être obtenue via la méthode ``getLoadedMoves()``

.. cpp:function:: vector<string> Robot::getLoadedMoves()

.. py:function:: Robot.getLoadedMoves

Tuer/Décharger un mouvement
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez décharger un mouvement à l'aide de ``killMove()`` :

.. cpp:function:: void Robot::killMove(string name)

.. py:function:: Robot.killMove(name)

.. note::

    Si le mouvement est en train d'être exécuté, il sera alors arrêté. Tâchez de plutôt stopper
    vos mouvements avant de les tuer

Exemples
~~~~~~~~

C++ :

.. code-block:: cpp

    // Chargement du mouvement Walk
    robot.loadMove("Walk");
    // Lance le mouvement "Walk" pour une durée indéterminée, smoothée pendant 1s
    robot.startMove("Walk", 0, 1000);
    // Dort une minute
    sleep(60);
    // Stoppe le mouvement "Walk" avec un smooth de 500ms
    robot.stopMove("Walk", 500);

Python :

.. code-block:: python

    # Chargement du mouvement Walk
    robot.loadMove('Walk')
    # Lance le mouvement "Walk" pour une durée indéterminée, smoothée pendant 1s
    robot.startMove('Walk', 0, 1000)
    # Dort une minute
    sleep(60)
    # Stoppe le mouvement "Walk" avec un smooth de 500ms
    robot.stopMove(Walk, 500)
    
