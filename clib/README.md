# PROJET METAHEURISTIQUE

Auteurs: 

*  Adrien BLASSIAU (adrien.blassiau@ensiie.fr)
*  Corentin JUVIGNY (corentin.juvigny@ensiie.fr)


      
IMPORTANT: LIRE CE FICHIER AVANT DE LANCER LE PROGRAMME



  Sommaire
==================================================================================

1. Introduction
2. Instructions d'installation des différents outils
3. Conseils d'utilisation



1. Introduction                 
==================================================================================

Ceci est le README du projet de méta-heuristique **Couverture connexe minimum dans les réseaux de capteurs**. Vous trouverez dans ce document toutes les informations nécessaires pour faire fonctionner le code.

NOTE1: Nous vous invitons à lire notre rapport qui vous renseignera sur la démarche et les choix engagés dans ce projet, ainsi que la répartition des tâches.

NOTE2: Le fichier Makefile.options est important. Il permet de modifier 
certains paramètres du projet.


2. Instructions d'installation des différents outils
==================================================================================


**Utilisateur Linux**


*********************************************************************************************

Doxygen
--------

Doxygen est un outil qui permet de gérer la documentation de son code. Il permet à terme de fournir une page web ou un pdf contenant la documentation du code que le développeur souhaite publier.

**Entrez dans votre console** : 
> sudo apt-get install doxygen

*********************************************************************************************

CUnit
--------

CUnit est une bibliothèque de tests unitaires pour C. Elle permet de programmer des tests, de les exécuter, et d’afficher un résumé des tests réussis ou échoués. Un test unitaire ne teste qu’une partie atomique des spécifications sous des conditions précises qui, bien généralement, ne couvrent pas tous les cas.

**Entrez dans votre console** : 
> sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev

*********************************************************************************************

Valgrind
--------

Valgrind est un outil qui permet de repérer les fuites mémoires (et autres erreurs).

**Entrez dans votre console** : 
> sudo apt-get install valgrind

*********************************************************************************************

Python
--------

Python est utilisé pour l'affichage graphique à l'intérieur même du code C. Pour cela, il vous faut tout d'abord installer python, ici python 3 par
exemple :

**Entrez dans votre console** : 
> sudo apt-get install python3-dev

**Attention** : dans Makefile.options, pensez à spécifier la version de python
que vous possédez sur votre machine (trouvable au niveau de /usr/include/).

*********************************************************************************************


3. Conseils d'utilisation
==================================================================================

Voici différentes commandes qui vont permettrons d'observer des résultats et informations sur le travail effectué :


Lancer le programme via une console
------------------------

Pour lancer l'algorithme glouton suivit du recuit simulé, vous pouvez lancer l’exécutable déjà fournit (commande 2). Il est aussi possible de tout recompiler (commande 1).
Vous pouvez modifier tout un ensemble de paramètres via le fichier
Makefile.options. Si vous changez PYTHON_VERSION ou DEBUG, vous devez recompiler pour que le changement opère (commande 1). Sinon, le changement des autres paramètres ne nécessite pas de recompiler.
Vous pouvez compiler et lancer simultanément avec la commande 3.

**1. Entrez dans votre console (compiler)** : 
> make meta

**2. Entrez dans votre console (lancer)** : 
> make exec_meta

**3. Entrez dans votre console (compiler et lancer)** : 
> make run_meta



*********************************************************************************************

Doxygen
-------

Pour obtenir la documentation générée par Doxygen, un document sous format web est déjà présent dans le dossier fournit, pour l'ouvrir :

*Via navigateur fichier*

Il est présent dans doc/html/index.html Ouvrez le sur votre navigateur web favori.

*Via console*

**Entrez dans votre console** : 
> firefox doc/html/index.html &

NOTE: Remplacez firefox par votre navigateur web favori.

*Informations*

Vous pouvez aussi régénérer la documentation puis l' ouvrir : 

**Entrez dans votre console** :
> make doxygen && firefox doc/html/index.html &



*********************************************************************************************
Valgrind
--------

Pour contrôler la présence de fuites mémoires (attention, cela ralenti 
considérablement l’exécution du programme) : 

> make exec_free

**Attention** : ne SURTOUT pas tester cette commande avec le mode graphique activé (GRAPHIC à 1 dans Makefile.options). L'exécution de python créée énormément de fuites mémoires ...

*Comment lire les résultats de valgrind ?*

Une fois lancé, rendez vous à la section HEAP SUMMARY : 
    
      ==22390== 
      ==22390== HEAP SUMMARY:
      ==22390==     in use at exit: 0 bytes in 0 blocks
      ==22390==   total heap usage: 6,189,434 allocs, 6,189,434 frees, 269,465,331 bytes allocated
      ==22390== 
      ==22390== All heap blocks were freed -- no leaks are possible
      ==22390== 
      ==22390== For counts of detected and suppressed errors, rerun with: -v
      ==22390== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


Vous obtenez la mémoire allouée (allocs) et la mémoire libérée (frees) ainsi que certaines autres erreurs liées à une mauvaise gestion de la mémoire (ERROR SUMMARY).
 
*********************************************************************************************
Tests unitaires CUnit
---------------------

Pour obtenir l'ensemble des résultats de nos tests unitaires : 

**1. Entrez dans votre console (compiler)** : 
> make test

**2. Entrez dans votre console (lancer)** : 
> make exec_test

**3. Entrez dans votre console (compiler et lancer)** : 
> make run_test


*Comment lire le résultat d'un test CUnit ?*

      CUnit - A unit testing framework for C - Version 2.1-3
      http://cunit.sourceforge.net/

    Run Summary:     Type  total    Ran Passed Failed  Inactive
                   suites      1      1    n/a      0         0
                    tests      5      5      5      0         0
                  asserts    118    118    118      0       n/a

    Elapsed time =    0.001 seconds


La ligne tests correspond au nombre de tests unitaires effectués, la ligne asserts correspond au nombres d'assertions vérifiées. Pour chaque tests, il peux y avoir plusieurs assertions. 

                                



*********************************************************************************************


*Merci pour votre lecture !*
