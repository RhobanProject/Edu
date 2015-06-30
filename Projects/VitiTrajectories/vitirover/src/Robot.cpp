#include "Robot.h"

using namespace std;

Robot::Robot()
{
    _length = 1;
    _width = 1;
    _height = 1;
    _weight = 1;
    _maxSpeed = 1.0;
    _maxAcceleration = 0.1 ;
    _xdirection = 1.0;
    _ydirection = 1.0;
    _speed = Point<float>(0.0,0.0,0.0);
    _acceleration = Point<float>(0.0,0.0,0.0);
    _strategy = 0;
    _coeff = 0.8;
    _coeffMoitie = 0.5;
}

Robot::Robot(float length, float width, float height, float weight)
{
    _length = length;
    _width = width;
    _height = height;
    _weight = weight;
    _strategy = 0;

    //todo: à passer en argument et donner des valeurs judicieuses!

    //norme max des vitesses et accélérations
    _maxSpeed = 1.0;//unité par seconde: 1 unité= 5cm
    _maxAcceleration = 0.2 ;//va de 0à2 unité par seconde en 10s->0.2unite/s/s
    _speed = Point<float>(0.0,0.0,0.0);
    _acceleration = Point<float>(0.0,0.0,0.0);
    _coeff = 0.8;
    _coeffMoitie = 0.5;
}


void Robot::setSpeed (Point<float> speed)
{
  _speed = speed ;
}

void Robot::setAcceleration (Point<float> acceleration)
{
  _acceleration = acceleration ;
}

void Robot::setSpeedNorm (float speedNorm)
{
    float n = (_speed).norm();
    //si le robot a un vecteur vitesse null
    if( n==0.0)
    {
        Point<float> d =getDirection();
        float m = d.norm();
        if(m==0.0)
        {
            cout<<"Probleme dans setSpeedNorm, vecteur direction null"<<endl;
        }

        setSpeed((d.subFunc(getCenter()))*(speedNorm/m));//Par défaut, avance dans la direction
    }
    else
    {

        setSpeed(getSpeed()*(speedNorm/n));
    }

}

void Robot::setAccelerationNorm (float accelerationNorm)
{
  float n = (_acceleration).norm();
  //si le robot a un vecteur acceleration nul
  if(n==0.0)
  {
      Point<float> d = getDirection();
      float m = d.norm();
      if(m==0.0)
      {
          cout<<"Probleme dans setAccelerationNorm, vecteur direction null "<<endl;
      }
      setAcceleration((d.subFunc(getCenter()))*(accelerationNorm/m));//Par défaut, on accélère dans la direction
  }
  else
  {

      setAcceleration(getAcceleration()*(accelerationNorm/n));
  }

  cout<<"Acceleration: x y z "<<_acceleration.getX()<<" "<<_acceleration.getY()<<" "<<_acceleration.getZ()<<endl;
}


Point<float> Robot::getAcceleration()
{
  return _acceleration;
}

Point<float> Robot::getSpeed()
{
  return _speed;
}

Point<float> Robot::getDirection()
{
    Point<float>r(_xdirection,_ydirection,0.0);
    return r;
}


void Robot::turnToObjective(float* x, float* y)
{
    //si changement de direction
    if( (_xdirection != *x) || (_ydirection != *y) )
    {
        //Réduit la vitesse d'1/4
        setSpeed(getSpeed()*(0.75));
        setAcceleration(getAcceleration()*0.75);
        cout<<"changement de direction détecté, réduction de vitesse effectué"<<endl;
    }

    //Tourne le robot en 1 coup: TODO à modifier
    float a=getAcceleration().norm();
    float s=getSpeed().norm();

    Point<float> direct(*x,*y,(float)0);//0 par défaut, car on travaille sur le plan 2D
    cout<<direct.getX()<<" "<<direct.getY()<<" "<<direct.getZ()<<endl;

    Point<float> vector=direct.subFunc(getCenter());//vecteur du point position vers le point direction
    float v=vector.norm();
    vector=vector*(1/v);//vecteur est normalisé

    setSpeed(vector*s);
    setAcceleration(vector*a);

    _xdirection = *x;
    _ydirection = *y;
}

float Robot::getLength() const
{
    return _length;
}

float Robot::getWidth() const
{
    return _width;
}
float Robot::getWeight() const
{
    return _weight;
}
float Robot::getHeight() const
{
    return _height;
}

float Robot::getMaxAccelerationNorm() const
{
  return _maxAcceleration;
}

float Robot::getMaxSpeedNorm() const
{
  return _maxSpeed;
}

void Robot::computeObjective(float* x, float* y)
{
  //TODO
  /*d'après l'algo du vitirover, calcule depuis historique des coupes et découpe de la parcelle...*/
  
    //version 1: FAKE: on veut bouger de 1 selon x et y
  //*x=_xdirection+1;
  //*y=_ydirection+1;
  //cout << "robot veut aller en " <<*x << "et en " << *y <<endl;

    //version 2: allez-retour piquet, 0
    Point<float> c=getCenter();
    float xx=c.getX();
    float yy=c.getY();

    //Aller au centre
    if(_strategy==0)
    {
        *x=200;
        *y=200;

        //cout<<xx<<" "<<yy<<endl;
        if((xx<201.0 && xx>199.0)&&(yy<201.0||yy>199.0))
        {
            cout<<"changement de stratégie"<<endl;
            _strategy=1;
        }
    }
    else
    {
       *x = 0;
       *y = 0;

       if((xx<4 && xx>0)&&(yy<4||yy>0))
       {
           cout<<"changement de stratégie"<<endl;
           _strategy=0;
       }

    }


}

void Robot::setPosition(Point<float> nextPosition)
{
  _center.setX(nextPosition.getX()); 
  _center.setY(nextPosition.getY()); 
  _center.setZ(nextPosition.getZ()); 

}

void Robot::setPosition(int x,int y,int z)
{
  _center.setX(x); 
  _center.setY(y); 
  _center.setZ(z); 
}


float Robot::getCoeff()
{
  return _coeff;
}

float Robot::getCoeff_galere()
{
  return _coeffMoitie;
}

Point<float> Robot::getCenter()
{
  return _center;
}
