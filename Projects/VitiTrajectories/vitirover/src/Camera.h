#ifndef CAMERA_H
#define CAMERA_H
/**
  * Class representant la Camera pour OpenGL.
  * Elle contient les informations necessaire aux choix de la position de la camera et son angle de vue.
  * Necessite Glu
  **/
#include <GL/glu.h>

#include "Point.h"
class Camera
{
public:
    Camera();
    /**
      * Constructeur.
      * @param p, Position de la camera (x/y/z)
      * @param to, point regardé par la camera (objectif)
      * @param up, Orientation de la camera. Plus précisement, position du "sommet" de la camera.
      */
    Camera( Point<float> p, Point<float> to, Point<float> up);

    /**
      * fonction d'initialisation
      * @param p, Position de la camera (x/y/z)
      * @param to, point regardé par la camera (objectif)
      * @param up, Orientation de la camera. Plus précisement, position du "sommet" de la camera.
      */
    void initialization(Point<float> p, Point<float> to, Point<float> up);
    void initialization(int px, int py, int pz, int tox, int toy, int toz, int upx, int upy, int upz);

    /**
      * Point représentant la camera en x, y, z.
      **/
    Point<float> _position;

    /**
      * Point représentant l'endroit observé par la camera.
      **/
    Point<float> _to;

    /**
      * Point representant le sommet de la camera (permet de definir l'orientation de la pyramide de vision dans l'espace 3D.
      **/
    Point<float> _up;

    /**
      * @obsolete Decalage du point observé.
      **/
    Point<float> _delta_to;
    /**
      * angle de rotation de la scene en X
      **/
    float _x_angle;

    /**
      * angle de rotation de la scene en Y
      **/
    float _y_angle;

    float _z_angle;

    /**
      * Definit la camera, le point observé, l'orientation et la roation de la scene pour OpenGL/Glu
      * ??? Necessite d'etre appelé a chaque modification de la scene ???
      **/
    void lookAt();
};

#endif // CAMERA_H
