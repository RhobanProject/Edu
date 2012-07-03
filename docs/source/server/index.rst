
.. _server:

Documentation: Serveur
======================

Les binaires de serveur Rhoban peut être téléchargé dans la rubrique :ref:`telechargements`.

.. code-block:: bash

    Usage: ./rhoban_server [-h|--help] [-p <port>] [-m <module|fake|serial> [-s <baudrate> -d <device>]]

Lors de l'exécution du serveur, il est possible de lui fournir des arguments permettant de définir
quel adaptateur bas niveau doit être utilisé et quelques éléments de configuration :

=============== ====================================================================
Option          Description
=============== ====================================================================
``-h``          Affiche l'aide d'utilisation du serveur

``-p <port>``   Fournit un port TCP sur lequel le serveur écoutera (défaut: ``12345``)

``-m <mode>``   Définit le mode à utiliser, parmi :

                - ``fake``: fais "semblanc", permet de faire des tests sans avoir de
                  robot. Les moteurs capteurs seront automatiquement attachés et les
                  valeurs seront générées aléatoirement pour les capteurs, et suivant
                  les ordres pour les moteurs.
                - ``serial``: utilise la communication série, voir les option ``-d`` et
                  ``-s`` pour le parametrage du device.
                - ``roboard``: fonctionne exactement comme ``-serial`` mais fournit 
                  en plus l'échantillonage ADC disponible sur la Roboard.
                - ``module``: utilise le module bas niveau ``/dev/bot``, ce module est
                  actuellement distribué pour le linux embarqué sur la MMnet.

``-d <device>`` Dans le cas du mode ``serial`` ou ``roboard``, l'option ``-d`` permettra
                de définir quel périphérique série devra être utilisé. Sous linux, il
                pourra par exemple s'agir de ``/dev/ttyUSB1`` ou de ``/dev/ttyS2``. Sous
                windows, le nom ressemble à ``COM1``, ``COM2``...

``-s <speed>``  Définit la rapidité (baudrate) à utiliser pour la communication série.

=============== ====================================================================
