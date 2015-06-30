#ifndef POINT_H
#define POINT_H

#include <math.h>

/**
  * Class Point, permettant de representer un point dans un espace 3D (x/y/z).
  * Utilise un template pour plus de liberté.
  **/
template <class T>
class Point
{

public:
    Point(T x=0,T y=0,T z=0) : _x(x), _y(y), _z(z)
    {}
    ~Point(void){};

    T getX()
    {
        return _x;
    }
    T getY()
    {
        return _y;
    }
    T getZ()
    {
        return _z;
    }

    void setX(T x)
    {
        _x = x;
    }
    void setY(T y)
    {
        _y = y;
    }
    void setZ(T z)
    {
        _z = z;
    }

    Point<T> operator*(const T& right)
      {
	Point<T> result;
        result.setX(getX()*right);
        result.setY(getY()*right);
        result.setZ(getZ()*right);
	return result;
      }

    Point<T> operator+(const Point<T>& right)
      {
        Point<T> result;
        result.setX( getX() + right.getX());
        result.setY( getY() + right.getY());
        result.setZ( getZ() + right.getZ());
        return result;
      }

    Point<T> multFunc(T right)
    {
        Point<T> result;
        result.setX( getX()*right);
        result.setY( getY()*right);
        result.setZ( getZ()*right);
        return result;
    }

    Point<T> addFunc(Point<T> p2)
    {
        Point<T> result;
        result.setX( getX() + p2.getX() );
        result.setY( getY() + p2.getY() );
        result.setZ( getZ() + p2.getZ() );
        return result;
    }

    /*
     *Renvoie un Point de coordonnée la soustraction des valeurs de la classe moins celles du paramêtre
     */
    Point<T> subFunc(Point<T> p)
    {
        Point<T> result;
        result.setX( getX() - p.getX() );
        result.setY( getY() - p.getY() );
        result.setZ( getZ() - p.getZ() );
        return result;
    }

    T norm()
    {
        T result;
        T x=getX();
        T y=getY();
        T z=getZ();

        result=sqrt(x*x + y*y + z*z);

        return result;
    }



private:
    T _x;
    T _y;
    T _z;



};

#endif // POINT_H
