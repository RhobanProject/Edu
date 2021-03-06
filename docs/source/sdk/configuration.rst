
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

Entrées
~~~~~~~

Voici les entrées du fichier de configuration:

* ``commands``: emplacement du :ref:`magasin de commandes <commandsstore>`
* ``robots``: robots à instancier
    * ``host``: adresse à laquelle établir la connexion (ou se trouve le serveur)
    * ``port``: numéro de port à utiliser
    * ``environment``: :ref:`environement à charger <load_env>` automatiquement
    * ``loadMoves``: :ref:`mouvements <moves>` à charger automatiquement

Exemple de fichier Yaml
~~~~~~~~~~~~~~~~~~~~~~~

Voici un exemple de fichier Yaml :

.. code-block:: yaml

    # config.yml

    # Seulement utile pour python
    commands: /configs/commands.xml

    robots:
        spider:
            host: 192.168.0.5 # Adresse de connexion
            port: 89898 # Port (Si différent de 12345)
            environment: /configs/spider/ # Emplacement de l'environement Spider
        
        humanoid:
            host: 192.168.0.12
            environment: /configs/humanoid/
            loadMoves: ['Walk', 'Standup'] # Mouvements à charger automatiquement

Exemple d'utilisation
~~~~~~~~~~~~~~~~~~~~~

Voici comment ce fichier pourrait être utilisé dans le code, en C++ :

.. code-block:: cpp

    Robots robots;
    // Charge la configuration et connecte les robots
    robots.loadYaml("config.yml");
    // Relâche les moteurs de l'arraignée
    robots["spider"]->allCompliant();
    // Lance le mouvement de marche
    robots["humanoid"]->startMove("Walk", 0, 1000)
    // Ferme toutes les connexions
    robots.stop();

Puis en Python :

.. code-block:: python

    robots = Robots()
    # Charge la configuration et connecte les robots
    robots.loadYaml('config.yml')
    # Relâche les moteurs de l'arraignée
    robots['spider'].allCompliant()
    # Lance le mouvement de marche
    robots['humanoid'].startMove("Walk', 0, 1000)
    # Ferme toutes les connexions
    robots.stop()
