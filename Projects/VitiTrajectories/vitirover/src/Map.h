/**
  * Class Map, genere et represente uen carte 3D en utilisant
  * l'algorithme de Perlin. La Map se compose de plusieurs Layer.
  *  Map peut enregistrer le résultat sous forme de heigh_map, format PPM par exemple)
  *
  * @since Decembre 2012
  **/

#ifndef __MAP__
#define __MAP__

#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Layer.h"
#include "Field.h"
#include "Point.h"

using namespace std;

class Map
{
public:
    /**
      * @param size , Taille de la carte (carré)
      * @param octave, nombre de calque a utiliser
      * @param weighting, poid initial (<1.0)
      **/
    Map( int size, int octave, float weighting, float min_height, float max_height );
    ~Map();

    /**
      * Fusionne les different calques en prenant en compte leur poid respectif.
      **/
    void mergeLayer( );
    /**
      * Initialise les different calque selon la frequence definit.
      **/
    void processLayer( int freq );
    /**
      * Envoie sur la sortie standard la Map au format PPM.
      **/
    void printAsPPM( );
    /**
      * Adoucit la Map.
      **/
    void smooth( int value );
    /**
      * Permet d'enregistrer la Map dans un fichier filename, au format PPM, sous forme de heigh_map.
      **/
    void saveAsFile( std::string filename);
    /**
      * Retourne la taille de la carte (carre)
      **/
    int getSize();
    /**
      * Retourne la valeur en Y du pixel en x/z données (Y = hauteur)
      **/
    int getPixel(int x, int z);
    /**
      * Retourne la valeur en Y du pixel en x+(size*z) données (Y = hauteur)
      **/
    int getPixel(int xz);

    Field* getField(int i);
    Field* getField(int x, int z);

    float getMinWeight() const;
    float getMaxWeight() const;
    int getOctave() const;

    vector< Point<int> >* getVines();
    vector< Point<int> >* getStakes();

    void processToFieldMap();
    // Lan_epsace and co doivent etre en Unité (comprendre en "pixel"), et non cm or m
    void generateLane( float lane_space, float stake_spacing, float vine_spacing);

    float getAverageGrassHigh();

    static const float grass_high = 5.0;
    static float lane_space;


protected:


private:
    /**
      * Taille de la carte (carré uniquement pour l'instant)
      **/
    int _size;

    /**
      * Nombre de calque a utiliser (plus de calque = plus de bruit/variation)
      **/
    int _octave;

    /**
      * Hauteur la plus petite de la carte
      **/
    float _min_height;

    /**
      * Hauteur maximal de la carte
      **/
    float _max_height;

    /**
      * Calque de donnée
      **/
    Layer* _datas;

    vector<Field*>* _fields;
    vector<Point<int> >* _stakes;
    vector<Point<int> >* _vines;

    /**
      * Layer intermediaire. Permet de generer et stocker des nombres aléatoire utile pour la creation de la carte.
      **/
    Layer* _random_layer;

    /**
      * Layer intermediaires de creation de la carte. A terme, sotn fusionnée et lissé dans _datas.
      **/
    std::vector<Layer*>* _layers;


    int interpolate( int y1, int y2, int n, int delta );
    int getInterpolateValue(  int i, int j, int frequence );



};
#endif //__MAP__
