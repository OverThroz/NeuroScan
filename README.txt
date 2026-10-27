================================================================================
PROJET NEUROSCAN - OCR WORD SEARCH SOLVER
================================================================================
Réalisé dans le cadre de la SAÉ S3 (EPITA).

Description :
Application en langage C prenant en entrée l'image d'une grille de mots cachés,
la traitant via un réseau de neurones (OCR), et affichant la grille résolue 
avec les mots entourés.

Prérequis :
- Un compilateur C (gcc ou clang)
- L'outil make

Compilation :
Ouvrez un terminal à la racine du projet et tapez :
$ make
Cela générera l'exécutable principal 'solver' ainsi que la démo 'pipeline_demo'.

Utilisation :
- Lancer le solver en ligne de commande sur une grille texte :
  $ ./solver tests/sample_grids/simple_5x5.txt
  
- Lancer la démo de bout en bout (simule l'OCR, le Solver et le Rendu) :
  $ ./pipeline_demo

Tests et Nettoyage :
- Lancer les tests unitaires :
  $ make test
  
- Vérifier les fuites mémoires (si AddressSanitizer est disponible) :
  $ make clean && make ASAN=1 && make test
  
- Nettoyer les fichiers générés (binaires, objets .o) :
  $ make clean
================================================================================