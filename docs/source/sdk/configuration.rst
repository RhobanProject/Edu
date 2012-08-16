
.. _configuration:

Configuration
=============

Lorsque vous gérerez un ou plusieurs robot, vous aurez sans doute besoin de pouvoir
changer les paramètres (ip, port, emplacement des :ref:`configurations <configurations>` etc.)
sans modifier le code (ni le recompiler dans le cas de C++)

Pour cela, vous pouvez utiliser un fichier de configuration au format Yaml.

.. cpp:function: void Robots.loadYaml(string filename)

.. py:function: Robots.loadYaml(string filename)

La fonction ``loadYaml()`` de la classe ``Robots`` vous permet de charger les robots présents
dans un fichier Yaml et de pouvoir intéragir avec eux.

Exemple de fichier Yaml
-----------------------

Voici un exemple de fichier Yaml :

.. code-block:: yaml

    # config.yml
    spider:
        ip: 192.168.0.5
        lowLevelConfig: /configs/spider/LowLevelConfig.xml
        moveSchedulerConfig: /configs/spider/MoveSchedulerConfig.xml
    
    humanoid:
        ip: 192.168.0.12
        port: 89898
        lowLevelConfig: /configs/humanoid/LowLevelConfig.xml
        moveSchedulerConfig: /configs/humanoid/MoveSchedulerConfig.xml

Exemple d'utilisation
---------------------

Voici comment ce fichier pourrait être utilisé dans le code, en C++ :

.. code-block:: cpp

    Robots robots;
    // Charge la configuration et connecte les robots
    robots.loadYaml("config.yml");
    // Relâche les moteurs de l'arraignée
    robots["spider"].allCompliant();
    // Ferme toutes les connexions
    robots.stop();

Puis en Python :

.. code-block:: python

    robots = Robots()
    # Charge la configuration et connecte les robots
    robots.loadYaml('config.yml')
    # Relâche les moteurs de l'arraignée
    robots['spider'].allCompliant()
    # Ferme toutes les connexions
    robots.stop()
