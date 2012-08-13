

Communication
=============

Si vous souhaitez obtenir des informations sur le fonctionnement des paquets, consultez
la documentation du :ref:`protocol`

Etablir une connexion
---------------------

Afin d'établir une connexion avec un robot, instanciez tout d'abord la classe ``Robot`` puis
utilisez la méthode ``connect`` :

.. cpp:function:: void Robot::connect(string address, int port)

.. code-block:: cpp

    // Exemple de connexion à un robot (C++)
    #include <robots/Robot.h>

    Robot robot;
    robot.connect("127.0.0.1", 12345);

.. py:function:: Robot.connect(hostname = 'localhost', port = 12344)

.. code-block:: python
    
    # Exemple de connexion à un robot (Python)
    from rhoban.robot import Robot

    robot = Robot()
    robot.connect('127.0.0.1', 12345)


Envoyer un message
------------------

Les messages qui peuvent être envoyés sont tous présents dans le magasin de commandes (``commands.xml``). Pour
plus d'informations, réferez vous à la documentation du :ref:`protocol`.

Sans réponse
~~~~~~~~~~~~

Les messages sans réponses peuvent être directement appelés sur la connexion :

.. code-block:: cpp

    // Exemple d'envoi de message sans réponse (C++)
    robot.connection.StartMove("Something", 0, 0);

.. code-block:: python

    # Exemple d'envoi de message sans réponse (Python)
    robot.connection.StartMove('Something', 0, 0);

Réponse synchrone
~~~~~~~~~~~~~~~~~

Si vous souhaitez patienter durant l'envoi d'une commande et ne reprendre la main que lorsque la réponse
est arrivée, utilisez le nom de la commande concaténé à ``_response`` :

.. code-block:: cpp

    // Exemple d'envoi de message synchrone (C++)
    response = robot.connection.ServerGetVersion_response()

    cout << "La version du serveur est : " << response.read_uint() << endl;

.. code-block:: python

    # Exemple d'envoi de message synchrone (python)
    response = robot.connection.ServerGetVersion_response()

    print 'La version du serveur est : ' + response[0]

Réponse asynchrone
~~~~~~~~~~~~~~~~~~

Enfin, si vous souhaitez traiter la réponse de manière asynchrone, vous pouvez utiliser le nom de la 
commande concaténé à ``_callback`` :

.. code-block:: cpp

    // Exemple d'envoi de message avec réponse asynchrone (C++)
    void processAnswer(Message *message) {
       cout << "La version du serveur est : " << message->read_uint() << endl;
    }

    robot.connection.ServerGetVersion_callback(processAnswer);

.. code-block:: python

    # Exemple d'envoi de message avec réponse asynchrone (python)
    def processAnswer(response):
        print 'La version du serveur est : ' + response[0]

    robot.connection.ServerGetVersion_callback(processAnswer)



