#ifndef TRIPLE_H
#define TRIPLE_H

/**
  * @obsolete , va degager rapidement au profit de "Point"
  *
  **/
class Triple
{

private:
    float _x;
    float _y;
    float _z;

    public:
    Triple();

    Triple(float x,float y,float z);

    float getX();
    float getY();
    float getZ();

    void setX( float x );
    void setY( float y );
    void setZ( float z );

};

#endif // TRIPLE_H
