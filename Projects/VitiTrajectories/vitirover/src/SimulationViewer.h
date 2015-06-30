#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

/**
  * Surcouche Graphique a la class Simulation.
  * SimulationViewer est une class fille de QGLWidget, et permet
  * de faire la representation 3D de la simulation dans un widget Qt
  * Aucune opération de simulation n'est effectuer ici. Seul Simulation modifie/joue avec
  * la simulation.
  **/

#include <iostream>

#include <QtOpenGL>
#include <QGLWidget>
#include <GL/glut.h>

#include "Point.h"
#include "Simulation.h"
#include "Camera.h"

using namespace std;

class SimulationViewer : public QGLWidget
{
    Q_OBJECT
public:

    explicit SimulationViewer(QWidget *parent = 0);
    SimulationViewer(Simulation* simulation, int width, int height, QWidget *parent = 0);

    static int speedSimulation; // permet de faire allez la simulation plus vite que la réalité

    void addSimulation( Simulation* simulation );
    void initializeGL();
    void paintGL();
    void initialization();
    void resizeGL(int w, int h);

    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    Simulation* getSimulation();

    Camera* getCamera();
    void setCamera(Camera* camera);

    /**
      * Timer permettant de definir l'interval entre chaque "pas" de repaint().
      **/
    QTimer* _timerRepaint;

    static const int cote = 5; // 5 cm = 1 unité
    static int precision;
    
public slots:
    virtual void timeOutSlot();
    void setDrawStakes(bool b);
    void setDrawVines(bool b);
    void setDrawMap(bool b);
    void setDrawGrass(bool b);
    void setTrackRobot(bool b);
    
 private:
    /**
      * Précision entre la Carte et notre representation 3D.
      * _précision est le pas a effectuer pour recuperer les données de la carte.
      * Si precision = 1, alors la representation représentera toutes les cases de notre carte.
      * Si precision = map.size() /2, seulement 4 cases seront représentées.
      **/

    unsigned int _precision;

    Simulation* _simulation;
    bool _drawStakes;
    bool _drawVines;
    bool _drawMap;
    bool _drawGrass;
    bool _trackRobot;

    Camera* _camera;

    /**
      * Dessine la Map, par le biais d'elements OpenGL (triangles/lignes/...)
      **/
    void drawMap();
    void drawAxis();
    void drawRobot( Robot* robot);
    void drawStakes();
    void drawStake(Point<int> p);

    void drawVines();
    void drawVine(Point<int> p);

    void drawGrasses();
    void drawGrasse(int i);

    /**
      * Fonction <utils> permettant d'envoyer le sommet passé en parametre au serveur OpenGL.
      **/
    void send(Point<GLfloat>& p);

};


#endif // SIMULATIONVIEWER_H
