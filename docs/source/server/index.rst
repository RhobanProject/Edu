
.. _server:

Documentation: Serveur
======================

Les binaires permettant d'exécuter le serveur Rhoban sont récupérables dans la rubrique :ref:`telechargements`.

.. code-block:: bash

    Utilisation :
    $> ./RhobanServer [-h|--help] [-p <port>] [-m <module|fake|serial> [-s <baudrate> -d <device>]]

Lors de l'exécution du serveur, il est possible de lui fournir des arguments permettant de définir
quel adaptateur bas niveau doit être utilisé ainsi que quelques éléments de configuration :

=============== ====================================================================
Option          Description
=============== ====================================================================
``-h``          Affiche l'aide d'utilisation du serveur

``-p <port>``   Définit le port découte TCP du serveur (valeur par défaut : ``12345``)

``-m <mode>``   Définit le mode à utiliser, parmi les suivants :

                - ``fake`` : Simule une connexion. Cela permet de faire des tests 
		  sans utiliser de robot. Les moteurs capteurs seront automatiquement 
		  attachés et les valeurs seront générées aléatoirement pour les 
		  capteurs, et suivant les ordres pour les moteurs.
                - ``serial``: utilise la communication série, voir les option ``-d`` et
                  ``-s`` pour le paramétrage du dispositif.
                - ``roboard``: fonctionne exactement comme ``-serial`` mais fournit 
                  en plus l'échantillonnage ADC disponible sur la Roboard.
                - ``module``: utilise le module bas niveau ``/dev/bot``, ce module est
                  actuellement distribué pour le linux embarqué sur la MMnet.

``-d <device>`` Dans le cas des modes ``serial`` ou ``roboard``, l'option ``-d`` 
                permettra de définir quel périphérique série sera utilisé. Sous 
		linux, il pourra par exemple s'agir de ``/dev/ttyUSB1`` ou de 
		``/dev/ttyS2``. Sous windows, les noms sont de la forme ``COM1``, 
		``COM2``...
		
``-s <speed>``  Définit la rapidité (baudrate) à utiliser pour la communication série.

=============== ====================================================================
