#include "Simulation.h"


Simulation::Simulation()
{

}
Simulation::Simulation(Robot* robot)
{
    addRobot( robot );
}

void Simulation::addRobot( Robot* robot)
{
    if ( robot != NULL )
        _robots.push_back( robot );
}

void Simulation::moveAllRobots( )
{
    for (int i= _robots.size()-1; i>=0; --i)
    {
        moveRobot( _robots.at(i) );
    }

}

vector<Robot*>* Simulation::getRobots()
{
    return &_robots;
}

/*TODO: question: est-ce qu'on fait la prévision de la direction ou le robot doit aller ici?*/
/*Move the robot, assuming the direction has been previously computed ?*/
void Simulation::moveRobot( Robot* robot )
{
    float* x = new float();
    float* y = new float();
    robot->computeObjective(x,y);//find a place to go to

    robot->turnToObjective(x,y);//turn the robot in the "good" direction, handle collision

    //cf rapport pour explication des 2 patinages possibles/ pas de patinage

    float mult=1.0;/*proportion du déplacement optimal qui sera effectué*/

    //areaViscosity=  _map->getviscosity(robot->getCenter());//Todo: add function and uncomment line
    float areaViscosity = 0.0;//cheat, considère qu'il n'y a pas de glissement pour l'instant

    if(areaViscosity > robot->getCoeff()){
        if(areaViscosity > robot->getCoeff_galere())
            mult=0.5;//the robot will move half the distance it could have moved in best conditions
        else
            mult=0.1;//the robot will move a very small distance
    }

    /*Position the robot should have access with the timeSlice he had left during simulation*/
    Point<float>* theoreticalNextPosition = new Point<float>();


    /*Accélération du robot*/
    float a= (robot->getAcceleration()).norm();
    float amax=robot->getMaxAccelerationNorm();

    //cout<<"a= "<<a<<"; amax= "<<amax<<endl;

    if(a<amax)
    {
        robot->setAccelerationNorm(a+amax/5);//sinon on peut faire (pas de "plein moteur dès le début" qui tienne)
    }
    //réajuste l'accélération (inutile d'après l'implémentation actuelle, sécurité sur l'avenir)
    if(a>amax)
    {
         robot->setAccelerationNorm(amax);
    }

    /*Vitesse*/
    float s=robot->getSpeed().norm();
    float smax=robot->getMaxSpeedNorm();
    if(s<smax)
    {
        robot->setSpeedNorm(s+timeSlice*a);
    }
    //réajuste la vitesse:
    if(s>smax)
    {
        robot->setSpeedNorm(smax);
    }

    /*Aide de lecture: Equation du Movement:
     *robot->getCenter() + robot->getSpeed()*delta + ((robot->getAcceleration()*delta*delta/2)*mult);
     */
    Point<float> prems=robot->getCenter();
    //cout<<"prems "<<prems.getX()<<" "<<prems.getY()<<" "<<prems.getZ()<<endl;
    Point<float> deuz=(robot->getSpeed()).multFunc(timeSlice);
    //cout<<"deuz "<<deuz.getX()<<" "<<deuz.getY()<<" "<<deuz.getZ()<<endl;
    Point<float> troiz=(robot->getAcceleration().multFunc(timeSlice*timeSlice/2)).multFunc(mult);
    //cout<<"prems "<<troiz.getX()<<" "<<troiz.getY()<<" "<<troiz.getZ()<<endl;
    *theoreticalNextPosition = prems.addFunc(deuz.addFunc(troiz));

    //cout<<"theoreticalNextPosition x,y,z: "<<theoreticalNextPosition->getX()<<" "<<theoreticalNextPosition->getY()<<" "<<theoreticalNextPosition->getZ()<<endl;

    /*move the robot step by step until a collision or arriving to the theoreticalNextPosition*/
    //DANGER: fonction pas si terrible que ça
    //moveUntilCollision(robot,robot->getCenter(), *theoreticalNextPosition);


    float xx=theoreticalNextPosition->getX();
    float yy=theoreticalNextPosition->getY();
    theoreticalNextPosition->setZ(getZ(xx,yy));
    robot->setPosition(*theoreticalNextPosition);

    //float zz=theoreticalNextPosition->getZ();
    //float map = getZ(xx,yy);
    //cout<<"x,y,z,map "<<xx<<" "<<yy<<" "<<zz<<" "<<map<<endl;

}

int Simulation::moveUntilCollision(Robot* robot,Point<float> center, Point<float> theoreticalNextPosition)
{
    //le pas est à déterminer (taille minimale d'un élément obstruant?)
    float step = 1.0;
    int i,j,k, i_min, i_max, j_min,j_max,k_min,k_max;

    i_min=(int)center.getX();
    i_max=(int)theoreticalNextPosition.getX();
    j_min=(int)center.getY();
    j_max=(int)theoreticalNextPosition.getY();

    for(i=i_min;i<i_max;i+=step)
    {
        for(j=j_min;j<j_max;j+=step)
	{
            //si sur la map il y a un objet génant à cet endroit
            if(isOccupied(i,j))
	    {
                robot->setPosition(i,j,getZ(i,j));
                //updateCaptors();//TODO:create this function and uncomment line
                return 1;
	    }
	}
    }
    //updateCaptors();//TODO:create this function and uncomment line

    //theoreticalNextPosition.setZ(getZ(theoreticalNextPosition.getX(),theoreticalNextPosition.getY()));


    robot->setPosition(theoreticalNextPosition);
    return 0;
}

int Simulation::isOccupied(int i,int j)
{
    //TODO
    //if there is an object on this position return 1
    //else return 0

    return 0;
}

float Simulation::getZ(int i, int j)
{
    return _map->getPixel(i,j);
}

void Simulation::initializeMap( int size, int octave, float weighting, float min_height, float max_height, int smooth, int freq_layer )
{
    _map = new Map( size, octave, weighting, min_height, max_height );
    _map->processLayer( freq_layer );
    _map->mergeLayer();
    _map->smooth( smooth );
    _map->saveAsFile("terrain_new.ppm");
}

void Simulation::setMap( Map* map )
{
    if ( _map )
        delete _map;
    _map = map;
}

Map* Simulation::getMap()
{
    return _map;
}

