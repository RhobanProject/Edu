

Prise en main
=============

Prérequis
---------

Avant de commencer, assurez vous que vous disposez de :

* Python
* GCC/G++
* Makefile
* Cmake (>= 2.8)

Si vous êtes sous Ubuntu/Debian, vous pouvez obtenir les paquets nécéssaire à l'aide de `apt-get`:

.. code-block:: bash

    apt-get install gcc g++ make python cmake

Si vous êtes sous Windows, vous devrez installer MinGW et MSYS, et éxécuter les commandes dans le "Shell MinGW"

Obtenir les sources
-------------------

Il y a deux façons d'obtenir les sources, vous pouvez cloner le :ref:`dépôt Github <https://github.com/RhobanProject/Edu>`_ :

.. code-block:: bash
    
    git clone --recursive http://github.com/RhobanProject/Edu.git

Ou vous pouvez également vous rendre dans la rubrique :ref:`telechargements` pour obtenir une archive du SDK.

Compilation des sources C++
---------------------------

Afin de compiler le SDK, vous aurez besoin de l'outil ``cmake``, qui génerera des ``Makefile``.

Pour compiler, créez tout d'abord un dossier, par exemple ``build/``, dans lequel les fichiers servant à la construction seront créés :

.. code-block:: bash

    mkdir build
    cd build

Créez ensuite les Makefile à l'aide de ``cmake`` :

.. code-block:: bash

    cmake chemin/vers/sdk/cpp

Si vous êtes sous Windows, vous devrez forcer ``cmake`` à créer des `Makefiles MSYS` :

.. code-block:: bash

    # Si vous êtes sous Windows
    cmake -G "MSYS Makefiles" chemin/vers/sdk/cpp

Lancez alors la compilation à l'aide de la commande ``make``. La compilation produira trois fichiers :

* ``libRhobanSDK.a`` : La bibliothèque statique contenant tous les objets permettant d'utiliser le SDK
* ``libRhobanSDKMain.a`` : Idem, en incluant un ``main()`` (cf :ref:`commands`) qui vous permet de définir vos propres
  commandes tout en utilisant celles proposées par le SDK
* ``sdk`` ou ``sdk.exe`` pour Windows : Un executable "couteau-suisse" qui vous permet d'intéragir avec le
  serveur en ligne de commande

Travailler avec le SDK
----------------------

Le SDK vous propose un certain nombre de :ref:`commandes <commands>` natives que vous pouvez éxécuter. Ces
:ref:`commandes <commands>` sont en fait des mots clés permettant d'embarquer plusieurs actions dans le 
même executable.

C++
~~~

Dans le code C++, vous trouverez un dossier ``example/`` contenant un fichier ``CMakeLists.txt`` ainsi qu'une
commande d'exemple.

Une commande peut être déclarée à l'aide de la macro ``COMMAND_DEFINE`` :

.. code-block:: cpp

    #include <main/Command.h>

    COMMAND_DEFINE(name, "Description de la commande")
    {
        cout << "Hello world!" << endl;
    }

Le paramètre ``name`` est une chaine de caractères, sans espaces, qui définit l'argument à utiliser pour appeler l'exécution de cette commande.

La description sera affichée dans l'aide du programme.

Python
~~~~~~

*A venir*
