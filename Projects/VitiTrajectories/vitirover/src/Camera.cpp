#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Point<float>p, Point<float>to, Point<float>up): _position(p),  _to(to), _up(up)
{


}

void Camera::initialization(Point<float>p, Point<float>to, Point<float>up)
{
    _position = p;
    _to = to;
    _up = up;
}
void Camera::initialization(int px, int py, int pz, int tox, int toy, int toz, int upx, int upy, int upz)
{
    _position.setX( px );
    _position.setY( py );
    _position.setZ( pz );

    _to.setX( tox );
    _to.setY( toy );
    _to.setZ( toz );

    _up.setX( upx );
    _up.setY( upy );
    _up.setZ( upz );
    _x_angle = 0;
    _y_angle = 40;
    _z_angle = 0;

}

void Camera::lookAt()
{
    gluLookAt( _position.getX()+_delta_to.getX(), _position.getY()+_delta_to.getY(), _position.getZ()+_delta_to.getZ(),
               _to.getX() , _to.getY(), _to.getZ(),
               _up.getX(), _up.getY(), _up.getZ());
    glRotated( _x_angle, 1.0, 0.0, 0.0);
    glRotated( _y_angle, 0.0, 1.0, 0.0);
    glRotated( _z_angle, 0.0, 0.0, 1.0);
}

