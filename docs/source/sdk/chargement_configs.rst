
.. _configurations:

Chargement des configurations
=============================

Les configurations sont des fichiers au format *XML* permettant de décrire le système
qui compose le robot utilisé.

.. note::

    Charger les configurations sur le robot est une étape indispensable afin de commencer
    à l'utiliser. En effet, sans configuration de bas niveau (`LowLevelConfig`) et sans
    configuration des moteurs (`MoveSchedulerConfig`), il sera impossible de déplacer
    les moteurs ou de piloter des mouvements.

.. _load_env:

Charger un environement
~~~~~~~~~~~~~~~~~~~~~~~

.. note::

    Il est préférable de charger les environements à l'aide du :ref:`fichier de configuration
    Yaml <configuration>`.

Charger un environement est sans doute le moyen le plus rapide de procéder au chargement
de l'ensemble des configurations. Un environement est un dossier utilisé par l':ref:`IHM <ihm>`
pour stocker ses fichiers de configurations dans une arborescence donnée.

.. cpp:function:: void Robot::loadEnvironment(string directory)

.. py:function:: Robot.loadEnvironment(directory)

Lors du chargement d'un environement, les fichiers de configurations de bas niveau et de
mouvement seront tous les deux chargés automatiquement.

Exemples :

.. code-block:: cpp

    // Charge un environement
    robot.loadEnvironment("/home/rhoban/RobotBoard/Spider/");

.. code-block:: python

    # Charge un environment
    robot.loadEnvironment('/home/rhoban/RobotBoard/Spider')

Configuration bas niveau
~~~~~~~~~~~~~~~~~~~~~~~~

La configuration bas niveau, ou `LowLevelConfig` permet de décrire comment est constitué
le robot physiquement, c'est à dire quel sont ses capteurs, moteurs etc.

Elle peut être chargée à l'aide de la méthode ``loadLowLevelConfig()`` sur le robot :

.. cpp:function:: void Robot::loadLowLevelConfig(string filename, bool force = false)

.. py:function:: Robot.loadLowLevelConfig(filename, force = False)

Envoie la configuration bas niveau (`LowLevelConfig`) sur le serveur. Si une configuration
bas niveau a déjà été chargée, elle ne sera écrasée que si le flag ``force`` est à ``true``.

Exemples :

.. code-block:: cpp

    // Force le chargement de la configuration bas niveau (C++)
    robot.loadLowLevelConfig("LowLevelConfig.xml", true);

.. code-block:: python

    # Force le chargement de la configuration bas niveau (Python)
    robot.loadLowLevelConfig('LowLevelConfig.xml', True)

Configuration des mouvements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La configuration des mouvements (`MoveSchedulerConfig`) fait correspondre les identifiants
des moteurs utilisés à des paramètres (angle initial, angle max, type etc.) et surtout à un
nom sous forme de chaîne de caractère facilement exploitable et mémorisable.

En réalité, cette configuration est utile pour faire fonctionner l'ordonanceur de 
:ref:`mouvements <moves>`.

.. cpp:function:: void Robot::loadMoveSchedulerConfig(string filename, bool force = false)

.. py:function:: Robot.loadMoveSchedulerConfig(filename, force = False)

Envoie la configuration mouvements (`MoveSchedulerConfig`) sur le serveur. Si une configuration
mouvements a déjà été chargée, elle ne sera écrasée que si le flag ``force`` est à ``true``.

.. note::

    La configuration des mouvements est également utilisée pour faire marcher la couche 
    :ref:`moteurs <motors>` du SDK. En effet, c'est ce fichier de configuration qui servira
    à faire correspondre les noms des servomoteurs aux identifiants associés.

Exemples :

.. code-block:: cpp

    // Force le chargement de la configuration mouvements (C++)
    robot.loadMoveSchedulerConfig("MoveSchedulerConfig.xml", true);

.. code-block:: python

    # Force le chargement de la configuration mouvements (Python)
    robot.loadMoveSchedulerConfig('MoveSchedulerConfig.xml', True)
