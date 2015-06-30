#ifndef SIMULATION_H
#define SIMULATION_H


/**
  * Class representant la simulation dans sa globalité
  * Contient la carte, le(s) robot(s) entre autre.
  **/

#include <vector>

#include "Robot.h"
#include "Map.h"

class Simulation
{

 public:
  const static float timeSlice = 1; // en s
  Simulation();
  Simulation( Robot* robot );

  /**
    * Initialise la carte contenu dans la class Simulation. Voir la documentation de Map pour plus de details
    **/
  void initializeMap( int size, int octave, float weighting, float min_height, float max_height, int smooth, int freq_layer );
  /**
    * Affecte Map* map, à la class Simulation
    **/
  void setMap( Map* map );
  /**
    * Retourne un pointeur vers la Map utilisée pour la simulation
    **/
  Map* getMap();

  /**
    * Ajoute un/des robots a la simulation
    **/
  void addRobot( Robot* robot);
  /**
    * Deplacer tout les robots de notre simulation. Gerer les collisions, le ralentissement etc...
    **/
  void moveAllRobots();

  int moveUntilCollision(Robot* robot,Point<float> center, Point<float> theoreticalNextPosition);

  vector<Robot*>* getRobots();

  int isOccupied(int i,int j);
  float getZ(int i, int j);

 private:
  vector<Robot*> _robots;
  void moveRobot( Robot* robot );

  Map* _map;

};

#endif // SIMULATION_H
