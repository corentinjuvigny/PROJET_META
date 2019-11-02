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

NOTE: Nous vous invitons à lire notre rapport qui vous renseignera sur la démarche et les choix engagés dans ce projet, ainsi que la répartition des tâches.


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

CUnit est une bibliothèque de tests unitaires pour C. Il permet de programmer des tests, de les exécuter, et d’afficher un résumé des tests réussis ou échoués. Un test unitaire ne teste qu’une partie atomique des spécifications sous des conditions précises qui, bien généralement, ne couvrent pas tous les cas.

**Entrez dans votre console** : 
> sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev

*********************************************************************************************

Valgrind
--------

Valgrind est un outil qui permet de repérer les fuites mémoires (et autres erreurs)

**Entrez dans votre console** : 
> sudo apt-get install valgrind

*********************************************************************************************



3. Conseils d'utilisation
==================================================================================

Voici différentes commandes qui vont permettrons d'observer des résultats et informations sur le travail effectué :


Lancer le programme via une console
------------------------

Pour lancer l'algorithme glouton suivit du recuit simulé :

**Entrez dans votre console** : 
> make && ./bin/main

**ou sinon :** : 
> make run


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

Pour contrôler la présence de fuites mémoires : 

**Sur le jeu console (attention, cela ralenti considérablement le jeu)** : 

> make && valgrind ./bin/main 


*Comment lire les résultats de valgrind*

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

Vous pouvez obtenir plus de détails sur les différentes fuites et erreurs via cette commande : 

**Entrez dans votre console** : 

> make && valgrind --track-origins=yes ./bin/main  


*********************************************************************************************
Tests unitaires CUnit
---------------------

Pour obtenir l'ensemble des résultats de nos tests unitaires : 

**Entrez dans votre console** : 
> make test && ./bin/test

**ou sinon :** : 
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
