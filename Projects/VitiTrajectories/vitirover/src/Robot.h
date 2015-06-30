
#ifndef ROBOT_H
#define ROBOT_H

#include "Point.h"
#include "Triple.h"
#include <ctime>
#include <iostream>

#include <cstdlib>

class Robot
{

public:
    explicit Robot();

    Robot( float length, float width, float height, float weight );

    void setSpeed(Point<float>  speed );
    void setAcceleration(Point<float> acceleration );

    Point<float> getAcceleration();
    Point<float> getSpeed();
    Point<float> getCenter();

    time_t getTime();
    void reinitTime();

    /*
     *Choisi un point x,y de la carte ou aller
     *modifie la stratégie
     */
    void computeObjective(float* x, float *y);

    /*
     *Tourne le robot pour qu'il aille vers le point de la carte
     */
    void turnToObjective(float* x, float *y);

    float getCoeff();
    float getLength() const;
    float getWidth() const;
    float getWeight() const;
    float getHeight() const;

    /*
     *retourne la norme maximale autorisée physiquement pour le vecteur accélération
     */
    float getMaxAccelerationNorm() const;

    /*
     *retourne la norme maximale autorisée physiquement pour le vecteur vitesse
     */
    float getMaxSpeedNorm() const;

    /*
     *impose la norme du vecteur accélération
     *si le vecteur accélération est nulle, prend la direction comme vecteur accélération
     */
    void setAccelerationNorm(float);

    /*
     *impose la norme du vecteur vitesse
     *si le vecteur vitesse est nulle, prend la direction comme vecteur accélération
     */
    void setSpeedNorm(float);

    float getCoeff_galere();
    void setPosition(Point<float> nextPosition);
    void setPosition(int x,int y,int z);
    void updateTime();

    //inélégeant
    Point<float> getDirection();

private:
    //Set constant values:
    float _length;
    float _width;
    float _height;
    float _weight;

    //Variating values:
    Point<float> _center;//center of gravity position
    Point<float> _speed;
    Point<float> _acceleration;

    /*
     *coordonnées x et y du point d'importance ou le robot doit aller
     */
    float _xdirection;
    float _ydirection;

    /*
     *norme maximale physiquement possible du vecteur vitesse
     */
    float _maxSpeed;

    /*
     *norme maximale physiquement possible du vecteur accélération
     */
    float _maxAcceleration;

    /*
     *coefficient de viscosité au delà duquel le robot ne bougera quasiment plus
     */
    float _coeff;

    /*
     *coefficient de viscosité au délà duquel le robot bougera moitié moins vite
     */
    float _coeffMoitie;

    /*
     *stratégie en cours, influe sur l'objectif à atteindre
     */
    int _strategy;
};

#endif // ROBOT_H
