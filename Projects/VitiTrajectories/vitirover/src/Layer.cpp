#include "Layer.h"
#include <time.h>


Layer::Layer( int size, float weighting, int height_max ) {
    setWeighting( weighting );
    _size = size;
    initData( height_max );
}


Layer::~Layer(){
}

void Layer::initData(int value_max) {
    int i = _size*_size;
    _datas = new int[ i ];
    for( (i=i-1) ; i >=0 ; --i )
    {
        if (value_max == 0)
            _datas[ i ] = 0;
        else
        {
            _datas[ i ] = (float)rand() / RAND_MAX * value_max;
        }
    }
}

//----- Accesseurs--------//
int* Layer::getDatas() {
    return _datas;
}
int Layer::getDatasAt(int i) {
    return _datas[ i ];
}
void Layer::setDatasAt(int i, int x) {
    _datas[i] = x;
}

float Layer::getWeighting() {
    return _weighting;

}

void Layer::setWeighting(float weighting) {
    if ( weighting > 1.0 || weighting < 0.0)
        _weighting = 0.5;
    else
        _weighting = weighting;
}
void Layer::print() {
    for( int i= 0 ; i<_size ; ++i  )
    {
        for( int j= 0 ; j<_size ; ++j  )
            std::cout<<_datas[i*_size+j]<<" ";
        std::cout<<std::endl;
    }
}


