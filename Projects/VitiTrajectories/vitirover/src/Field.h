#ifndef FIELD_H
#define FIELD_H

#include "Point.h"
#include "Triple.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

class Field
{
public:
    /*
     *Construit la classe Field sans arguments, donc avec des valeurs par défaut
     *cette classe sert à représenter une partie discrétisée du terrain
     */
    Field();

    /*
     *Construit la classe Field en utilisant des arguments
     */
    Field(Point<float> position, float viscosity, bool obstacle, float grass_high);

    /*
     *Retourne la position de la partie du terrain
     */
    Point<float> getPosition();

    /*
     *Fixe la position de la partie du terrain
     */
    void setPosition(Point<float> position);

    /*
     *Retourne la viscosité du terrain
     */
    float getViscosity();

    /*
     *Fixe la viscosité du terrain
     */
    void setViscosity(float);

    /*
     *Indique par un bouléen si le terrain est occupé par un obstacle (exemple: caillou/pied de vigne/piquet)
     *est mis à vrai si on impose un pied de vigne ou piquet
     *n'est pas remis à faux, car il peut y avoir un obstacle autre que piquet/pied de vigne (comme le caillou)
     */
    bool isObstacle();

    /*
     *Impose un obstacle sur le terrain
     */
    void setIsObstacle(bool obstacle);

    /*
     *Indique par un bouléen si le terrain est occupé par un piquet
     *si il n'y a pas d'obstacle, se charge d'imposer l'obstacle
     *peut-être à la fois piquet et pied de vigne
     */
    bool isStake();

    /*
     *Impose un piquet sur le terrain
     */
    void setIsStake(bool stake);


    /*
     *Indique par un bouléen si le terrain est occupé par un pied de vigne
     *si le terrain n'est pas un obstacle, se charge de mettre un obstacle
     *peut-être à la fois piquet et pied de vigne
     */
    bool isVine();

    /*
     *Impose un pied de vigne sur le terrain
     */
    void setIsVine(bool vine);

    /*
     *Donne la hauteur de l'herbe
     */
    float getGrassHigh();

    /*
     *Modifie la hauteur de l'herbe
     */
    void setGrassHigh(float high);

    /*
     *Augmente la hauteur de l'herbe du parametre high
     */
    void growGrass(float high);

private:
    /*
     *Position du terrain
     */
    Point<float> _position;

    /*
     *Viscosité du terrain, entre 0 et 1
     *une herbe haute, de la boue donnent une viscosité élevée
     *un terrain sec et stable donne une viscosité faible
     */
    float _viscosity;

    /*
     *Booleen indiquant si oui ou non il y a un obstacle sur le terrain
     */
    bool _obstacle;

    /*
     *Indique si il y a un piquet sur le terrain
     */
    bool _stake;

    /*
     *Indique si il y a un pied de vigne sur le terrain
     */
    bool _vine;

    /*
     *Hauteur de l'herbe sur le terrain, en mm
     */
    float _grass_high;
};

#endif // FIELD_H
