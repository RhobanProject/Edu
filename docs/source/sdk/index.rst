
.. _sdk:

Documentation: SDK
==================

Les codes sources des SDK (Software Development Kit) Python et C++ sont récupérables dans la rubrique :ref:`telechargements`.

Utilisation du SDK
~~~~~~~~~~~~~~~~~~

Compilation des sources C++
---------------------------

La compilation s'effectue via l'utilisation de `make`, et de plusieurs fichiers `Makefile`.
D'une part, à la racine du SDK, se trouve le fichier `Makefile` principal, et d'autre part, dans chaque sous répertoire, un fichier `Makefile.sources`.

La première ligne du fichier `Makefile` est la ligne `MODULES := [...]`. On y place la liste de tous les sous répertoires dans lesquels se trouvent les fichiers à compiler.

Dans chacun de ces répertoires doit se trouver un fichier `Makefile.sources`, de la forme `SOURCES := [...]`. On y place la liste de tous les fichiers `.cpp` du répertoire courant qui seront à compiler.

Ainsi, lors de l'ajout de code, il faudra penser à rajouter, d'une part le chemin du répertoire concerné dans le `Makefile` principal, et d'autre part un `Makefile.sources` dans le répertoire concerné, avec les noms des fichiers `.cpp` à compiler.

Définition de commande
----------------------

Afin de pouvoir exécuter un code, il faut définir une commande, qui pour être utilisée par le `main`, de la façon suivante :

.. code-block:: cpp

    COMMAND_DEFINE(command_name, "command_description")
    {
      Code C++;
      Code C++;
      Code C++;
    }

Le paramètre `command_name` est une chaine de caractères, sans espaces, qui définit l'argument à utiliser pour appeler l'exécution de cette commande. Le paramètre `command_description` est une chaîne de caractère, qui peut comporter des espaces, qui est la description de la commande concernée, qui sera affichée par l'aide du programme.

Exécution du programme
----------------------

Bla

Architecture du SDK
~~~~~~~~~~~~~~~~~~~

Diagramme de classes
--------------------

Bla

Description des modules
-----------------------

Bla
