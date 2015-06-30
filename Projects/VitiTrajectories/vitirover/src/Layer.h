
/**
  * Class Layer ,  Permet de decrire un des calques definissant notre Map
  * Un calques est definit par sa taille, ses données, et sont poid (coefficient d'importance)
  * @param weighting : muste be between 0.0 and 1.0, but > 0.5 is not so good.
  * @since Decembre 2012
  *
  **/

#ifndef __LAYER__
#define __LAYER__

#include <time.h>
#include <cstdlib>
#include <iostream>

class Layer
{
public:
    Layer( int size,
           float weighting,
           int height_max /** Hauteur maximal d'initialisation (uniquement) pour notre Layer. Les données vont etre
initialisées aléatoirement entre 0 et height_max**/           );
    ~Layer();

    /**
      * Definit le poid du calque (coefficient)
      **/
    void setWeighting( float weighting );

    /**
      * Retourne un pointeur vers le tableau de donnée
      **/
    int* getDatas();
    /**
      * Retourne la valeur à l'indice i.
      **/
    int getDatasAt( int i );
    /**
      * Affecte la valeur x à l'indice i.
      **/
    void setDatasAt(int i /** i indice **/, int x /** x valeur**/ );
    /**
      * Retourne le poid du calque.
      **/
    float getWeighting();
    /**
      * Sortie sur STDOUT du calques et ses données
      **/
    void print();

protected:

private:
    /**
      * Données du Layer. Tableau d'int
      **/
    int* _datas;

    /**
      * Taille du calque (carré uniquement pour l'instant)
      **/
    int _size;

    /**
      * Point (coefficient) du calque courant ( 0 <X< 1  )
      **/
    float _weighting;
    /**
      * Initialisation des données du calques a ${value_max}
      **/
    void initData( int value_max );

};

#endif //__LAYER__
