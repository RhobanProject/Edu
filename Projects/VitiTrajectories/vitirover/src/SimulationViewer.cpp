#include "SimulationViewer.h"


int SimulationViewer::speedSimulation=10; // permet de faire allez la simulation plus vite que la réalité
int SimulationViewer::precision = 10;

SimulationViewer::SimulationViewer(QWidget *parent)
    : QGLWidget( parent ), _simulation(NULL)
{
  }

SimulationViewer::SimulationViewer(Simulation* simulation, int width, int height, QWidget *parent )
    : QGLWidget( parent ){
    this->resize( width, height );
    _simulation  = simulation;
    initialization();
}

void SimulationViewer::addSimulation(Simulation *simulation)
{
    _simulation  = simulation;
}

void SimulationViewer::initialization()
{
    _camera = new Camera();

    _drawStakes = true;
    _drawVines = true;
    _drawMap = false;
    _drawGrass = true;
    _trackRobot = false;

    SimulationViewer::precision = 20;
    SimulationViewer::speedSimulation = 10;
    _timerRepaint = new QTimer(this);
    connect(_timerRepaint, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    _timerRepaint->start( Simulation::timeSlice*1000/ SimulationViewer::speedSimulation );
    setMouseTracking(true);
    _camera->initialization(500,500,500,   width()/2,height()/2,height()/2,  0,1,0);

}
Simulation* SimulationViewer::getSimulation()
{
    return _simulation;
}

void SimulationViewer::initializeGL()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

}
void SimulationViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,w/h,1,10000);
    //glOrtho(0, w, 0, h, -1000, 1000); // set origin to bottom left corner


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SimulationViewer::mouseMoveEvent(QMouseEvent *event) {

    /*_camera._delta_to.setX( event->x() );
    _camera._delta_to.setY( event->y() );*/
//    _camera->_x_angle = (event->y()-width()/2) *90/width();
//    _camera->_y_angle = (event->x()-height()/2 )*90/height();
}

void SimulationViewer::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0)
    {
    //    _camera._position.setX( _camera._position.getX( )+20 );
    //    _camera._position.setY( _camera._position.getY( )+20 );
        //_camera->_position.setZ( _camera._position.getZ( )+20 );
    }
    else
    {
   //     _camera._position.setX( _camera._position.getX( )-20 );
    //    _camera._position.setY( _camera._position.getY( )-20 );
        //_camera->_position.setZ( _camera._position.getZ( )-20 );
    }

}
Camera* SimulationViewer::getCamera()
{
    return _camera;
}

void SimulationViewer::setCamera(Camera *camera)
{
    _camera = camera;
}

void SimulationViewer::paintGL()
{
    // On vide les buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // On remet la matrice identité
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    
    if ( _trackRobot )
    {
        _camera->_to.setX( _simulation->getRobots()->at(0)->getCenter().getX() );
        _camera->_to.setZ( _simulation->getRobots()->at(0)->getCenter().getZ() );
        _camera->_to.setY( _simulation->getRobots()->at(0)->getCenter().getY() );

    }

    _camera->lookAt();

 //   drawAxis();
    if ( _drawMap )
        drawMap();
    if ( _drawGrass )
        drawGrasses();

   vector<Robot*>* robots =  _simulation->getRobots();
    for( int i = robots->size()-1; i>=0; --i )
    {
        drawRobot( robots->at(i) );
    }
    if ( _drawStakes )
        drawStakes();
    if ( _drawVines )
        drawVines();

    glFlush();
}
void SimulationViewer::setDrawStakes(bool b)
{
    _drawStakes = b;
}
void SimulationViewer::setDrawVines(bool b)
{
    _drawVines = b;
}
void SimulationViewer::setDrawMap(bool b)
{
    _drawMap = b;
}
void SimulationViewer::setDrawGrass(bool b)
{
    _drawGrass = b;
}
void SimulationViewer::setTrackRobot(bool b)
{
    _trackRobot = b;
}




void SimulationViewer::drawAxis()
{
    glBegin(GL_LINES);
    // X
    glColor3ub(255, 0, 0);
    glVertex3i(0,0, 0);
    glVertex3i(1000,0, 0);
    //Y
    glColor3ub(0, 255, 0);
    glVertex3i(0, 0,  0);
    glVertex3i(0, 1000, 0);
    //Z
    glColor3ub(0, 0, 255);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 1000);

    glEnd();
}
void SimulationViewer::drawRobot(Robot *robot)
{
    glPushMatrix();//on memorise l'etat de la matrice
    glTranslated(robot->getCenter().getX(),robot->getCenter().getZ(),robot->getCenter().getY());
    glColor3ub(200, 40, 10);
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluSphere(params,(robot->getLength()*0.5)/SimulationViewer::cote,40,40);
    gluDeleteQuadric(params);
    glPopMatrix();//on remet la matrice mémorisé en debut de fonction

}

void SimulationViewer::drawGrasses()
{

    for ( int i = 0; i < _simulation->getMap()->getSize()* _simulation->getMap()->getSize() ; i+=7 )
    {
        drawGrasse( i );
    }

}
void SimulationViewer::drawGrasse(int i)
{
    Field* f = _simulation->getMap()->getField(i);
    
    glPushMatrix();
    glTranslated( f->getPosition().getX(), f->getPosition().getY(),f->getPosition().getZ() );
    glRotated(-90,1,0,0);
    
    glColor3ub( 0, 150, 0 );
    glBegin( GL_LINES );
    {
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, 0, f->getGrassHigh()/SimulationViewer::cote );

    }
    glEnd();
    glPopMatrix();
}

void SimulationViewer::drawStake(Point<int> p)
{
    glPushMatrix();
    glTranslated( p.getX(), p.getZ(),p.getY() );
    glRotated(-90,1,0,0);

    glColor3ub( 60, 40, 175 );
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);

    gluCylinder( params,5/SimulationViewer::cote,5/SimulationViewer::cote,150/SimulationViewer::cote,10,1);

    gluDeleteQuadric(params);
    glPopMatrix();
}


void SimulationViewer::drawStakes()
{
    vector<Point<int> >* stakes = _simulation->getMap()->getStakes();
    for ( int i = 0; i < stakes->size() ; i++ )
    {
        drawStake( stakes->at(i) );
    }
}
void SimulationViewer::drawVines()
{
    vector<Point<int> >* vines = _simulation->getMap()->getVines();
    for ( int i = 0; i < vines->size() ; i++ )
    {
        drawVine( vines->at(i) );

    }
}
void SimulationViewer::drawVine(Point<int>p)
{
    glPushMatrix();
    glTranslated( p.getX(), p.getZ(),p.getY() );
    glRotated(-90,1,0,0);
    glRotated(90,0,0,1);

    //tronc
    glColor3ub( 110, 40, 40 );
    GLUquadric* params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluCylinder( params,5/SimulationViewer::cote,5/SimulationViewer::cote,50/SimulationViewer::cote,5,1);
  //  gluDeleteQuadric(params);

    //droit
    glTranslated( 0, 0 ,10 );
    glRotated(-55,1,0,0);
    params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluCylinder( params,5/SimulationViewer::cote,5/SimulationViewer::cote,45/SimulationViewer::cote,5,1);

    glTranslated( 0, 0 ,0 );
    glRotated(110,1,0,0);
    params = gluNewQuadric();
    gluQuadricDrawStyle(params,GLU_LINE);
    gluCylinder( params,5/SimulationViewer::cote,5/SimulationViewer::cote,45/SimulationViewer::cote,5,1);


    gluDeleteQuadric(params);

    glPopMatrix();


    return;
}

void SimulationViewer::timeOutSlot()
{
  _simulation->moveAllRobots();
    updateGL(); //on redessine OpenGL
}



void SimulationViewer::drawMap()
{
    /**
      * Pas encore comprit l'histoire de compilation, glLists etc...
      **/
    // Génération d'un identifiant pour notre display lsit
   // _list = glGenLists( 1 );
    // On demande à OpenGL de compiler ce qui va suivre dans
    // une display list identifiée par _list, que l'on vient de
    // générer.
    //glNewList( _list, GL_COMPILE );
    {
        // Nous informons OpenGL que ce qui va suivre est une
        // énumération de triangles.
        //glBegin( GL_TRIANGLES );
        
        glBegin( GL_LINE_STRIP );
        {
            glColor3ub( 0, 150, 0 );
            //  Pour chaque ligne, avec un pas dépendant de la précision souhaitée
            for ( unsigned int x=0; x<(_simulation->getMap()->getSize()-SimulationViewer::precision); x+=SimulationViewer::precision)
            {
                // Pour chaque colonne, avec un pas dépendant de la précision souhaitée
                for ( unsigned int z=0; z<(_simulation->getMap()->getSize()-SimulationViewer::precision); z+=SimulationViewer::precision)
                {
                    // Définition des coordonnées des points
                    Point<GLfloat> vertex1( x,
                                            _simulation->getMap()->getPixel( x, z ),
                                            z);

                    Point<GLfloat> vertex2( SimulationViewer::precision+x,
                                            _simulation->getMap()->getPixel( SimulationViewer::precision+x, z ),
                                            z);

                    Point<GLfloat> vertex3( SimulationViewer::precision+x,
                                            _simulation->getMap()->getPixel( SimulationViewer::precision+x, SimulationViewer::precision+z ),
                                            SimulationViewer::precision+z);

                    Point<GLfloat> vertex4( x,
                                            _simulation->getMap()->getPixel( x, SimulationViewer::precision+z ),
                                            SimulationViewer::precision+z );

                    // Premier triangle
                    send( vertex3 );
                    send( vertex2 );
                    send( vertex1 );

                    // Deuxième triangle
                    send( vertex4 );
                    send( vertex3 );
                    send( vertex1 );
                }
            }
        }
        glEnd();
    }
 //   glEndList();

}

void SimulationViewer::send(Point<GLfloat>& p)
{
    glVertex3f( static_cast<float>(p.getX()),
                static_cast<float>(p.getY()),
                static_cast<float>(p.getZ()) );
}


