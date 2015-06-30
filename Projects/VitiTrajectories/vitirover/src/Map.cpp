#include "Map.h"


float Map::lane_space = 30 ; // en unité
Map::Map( int size, int octave, float weighting, float min_height, float max_height )
{
    // Creation du calque de nombre random.
    _random_layer = new Layer( size, 1, max_height );
    _size = size;
    _octave = octave;
    _min_height = min_height;
    _max_height = max_height;
    _layers = new std::vector<Layer*>();

    _vines = new std::vector<Point<int> >();
    _stakes = new std::vector<Point<int> >();

    float c_weighting = weighting;

    // Creation du tableau de calques utiles
    for ( int i = 0 ; i < octave ; ++i )
    {
        _layers->push_back( new Layer( size, c_weighting , 0) );
        c_weighting *= weighting;
    }
    _datas = new Layer( size, 1, 0 );
}

Map::~Map()
{
/*
    for( int i= _layers->size() ; i>=0 ; --i  )
        delete _layers->at( i );
*/
}
void Map::generateLane(float lane_space, float stake_spacing, float vine_spacing)
{
    for ( int i = 0 ; i<_size ; i+=lane_space )
    {
        //les piquet
        for ( int j = 0; j<_size ; j+=stake_spacing )
        {
            _fields->at(i*_size+j)->setIsStake(true);
            _stakes->push_back(Point<int>(j,i,_datas->getDatasAt(i*_size+j)));
        }
        for ( int j = 0; j<_size ; j+=vine_spacing)
        {
            _fields->at(i*_size+j)->setIsVine(true);
            _vines->push_back(Point<int>(j,i,_datas->getDatasAt(i*_size+j)));
        }
    }
}

float Map::getAverageGrassHigh()
{
    float sum = 0.0;
    for ( int i = _size*_size-1 ; i>=0 ; --i )
    {
        sum += _fields->at(i)->getGrassHigh();
    }
    return sum/(_size*_size);
}

vector<Point<int> >* Map::getStakes()
{
    return _stakes;
}
vector<Point<int> >* Map::getVines()
{
    return _vines;
}

float Map::getMinWeight() const
{
    return  _min_height;
}

float Map::getMaxWeight() const
{
    return  _max_height;
}
int Map::getOctave() const
{
    return _octave;
}

void Map::processLayer( int freq )
{
    // remplissage de calque
    for (int n=0; n<_octave ; ++n)
    {
        for( int i=_size*_size-1 ; i>=0 ; --i )
            _layers->at( n )->setDatasAt(i, getInterpolateValue( (int)(i/_size), i%_size, freq) );
        freq *= freq;
    }

    std::cerr<<"Map::processLayer : DONE"<<std::endl;
}

int Map::interpolate(int y1, int y2, int n, int delta)
{
    // interpolation non linéaire
    if (n==0)
        return y1;
    if (n==1)
        return y2;

    float a = (float)delta/n;

    float fac1 = 3*pow(1-a, 2) - 2*pow(1-a,3);
    float fac2 = 3*pow(a, 2) - 2*pow(a, 3);

    return y1*fac1 + y2*fac2;
}

int Map::getInterpolateValue(int i, int j, int frequence)
{
    // valeurs des bornes
    int borne1x, borne1y, borne2x, borne2y, q;
    float pas;
    if ( frequence == 0)
    {
        std::cerr<<"[Error] {13.68} div by 0 "<<std::endl;
        exit( 1 );
    }
    pas = (float)_size/frequence;

    q = (float)i/pas;
    borne1x = q*pas;
    borne2x = (q+1)*pas;

    if( borne2x >= _size )
        borne2x = _size-1;

    q = (float)j/pas;
    borne1y = q*pas;
    borne2y = (q+1)*pas;

    if(borne2y >= _size)
        borne2y = _size-1;

    int b00,b01,b10,b11;
    b00 = _random_layer->getDatasAt( borne1x+borne1y*_size );
    b01 = _random_layer->getDatasAt( borne1x+borne2y*_size );
    b10 = _random_layer->getDatasAt( borne2x+borne1y*_size );
    b11 = _random_layer->getDatasAt( borne2x+borne2y*_size );

    int v1 = interpolate(b00, b01, borne2y-borne1y, j-borne1y);
    int v2 = interpolate(b10, b11, borne2y-borne1y, j-borne1y);
    int fin = interpolate(v1, v2, borne2x-borne1x , i-borne1x);

    return fin;
}

void Map::mergeLayer( )
{
    // calcul de la somme de toutes les persistances,
    //pour ramener les valeurs dans un intervalle acceptable
    float sum_persistances = 0.0;
    for( int i= _layers->size()-1 ; i>=0 ; --i  )
    {
        sum_persistances += _layers->at(i)->getWeighting();
    }
    // ajout des calques successifs
    for( int i=_size*_size-1 ; i>0 ; --i  )
    {
        for( int n = _layers->size()-1 ; n >=0 ; --n  )
        {
            _datas->setDatasAt( i, _datas->getDatasAt(i)+ _layers->at(n)->getDatasAt( i ) * _layers->at(n)->getWeighting() );
        }
        // normalisation
        _datas->setDatasAt( i,  _datas->getDatasAt( i ) / sum_persistances );
    }
    std::cerr<<"Map::mergeLayer : DONE"<<std::endl;

}
void Map::printAsPPM()
{
    Layer *l = _datas;
    std::cout<<"P2"<<std::endl<<_size<<" "<<_size<<std::endl<<_max_height<<std::endl;
    for( int i= 0 ; i<_size ; ++i  )
    {
        for( int j= 0 ; j<_size ; ++j  )
            std::cout<<l->getDatasAt(i*_size+j)<<" ";
        std::cout<<std::endl;
    }
}

void Map::saveAsFile(std::string filename)
{
    std::ofstream file( filename.c_str(), ios::out | ios::trunc);

    if( file )
    {
        Layer *l = _datas;
        file<<"P2"<<std::endl<<_size<<" "<<_size<<std::endl<<_max_height<<std::endl;
        for( int i= 0 ; i<_size ; ++i  )
        {
            for( int j= 0 ; j<_size ; ++j  )
                file<<l->getDatasAt(i*_size+j)<<" ";
            file<<std::endl;
        }
        file.close();
    }
    else
        std::cerr << "[Error] when saving the PPM file -> *ABORDED*" << std::endl;
}

void Map::smooth( int value )
{
    Layer *s = new Layer(_size,1,0);
    int k = 0;
    int n,x,y = 0;
    int l,a = 0;
    int k_max;
    int l_max;
    //on fait toute les cases du tableau
    for ( int i = _size*_size-1 ; i>=0 ; --i )
    {
        n = 0;
        a = 0;
        x = i % _size;
        y = (int)i / _size;
        k_max = x+value;
        l_max = y+value;
        // on calcule le smooth a l'aide d'un kernel de size VALUE
        for (k=x-value; k<=k_max; k++)
        {
             for (l=y-value; l<=l_max; l++)
                 if ((k>=0) && (k<_size) && (l>=0) && (l<_size))
                 {
                     n++;
                     a+=_datas->getDatasAt( k + l*_size);
                 }
         }
         s->setDatasAt( i ,(float)a/n );

    }
    _datas = s;
    std::cerr<<"Map::smooth : DONE"<<std::endl;
}

void Map::processToFieldMap()
{
    _fields = new vector<Field*>();

    for ( int i = _size*_size-1 ; i>=0 ; --i )
    {
        _fields->push_back( new Field(Point<float>( i%_size,_datas->getDatasAt(i),(int)(i/_size) ),
                                      1,0,Map::grass_high));
    }
}


int Map::getSize()
{
    return _size;
}
Field* Map::getField(int i)
{
    return _fields->at(i);
}
Field* Map::getField(int x, int z)
{
    return _fields->at( x+(z*_size) );
}

int Map::getPixel(int x, int z)
{
    return _datas->getDatasAt( x+(z*_size) );
}
int Map::getPixel(int xz)
{
    return _datas->getDatasAt( xz );
}
