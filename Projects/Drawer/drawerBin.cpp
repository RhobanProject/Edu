/*************************************************
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <main/Command.h>

#include <ticks.h>

#include <rhoban/robot/Robots.h>

using namespace std;
using namespace Rhoban;

void getDatas(Robot * robot, Motors * motors, const string & fileName, const int nbLoop);
void useDatas(Robot * robot, Motors * motors, const string & fileName);

void ssFirstPos(Motors * motors, const string &motorName, const double initPos, const double goalPos, const int pourcentage);
void goToFirstPos(Motors * motors, double posEpauleLong, double posEpauleLat, double posEpauleRot, 
		  double coude, double coudeRot, double poignetLong, double poignetLat, 
		  const double duration);

void inttostring (int num, std::string *str);

int main(int argc, char **argv)
{
  try
    {
      cout << endl << "*** ROBOT DRAWER ***" << endl << endl;
      
      if (argc != 5 && argc != 3)
	{
	  cout << "Utilisation du programme : " << endl
	       << "   ./drawerBin GET <fileName> <nbrExperiment> <nbLoop>" << endl
	       << "OU ./drawerBin USE <fileName>" << endl
	       << "Avec <fileName> nom de fichier avec num de l'experience ajoute av dernier ." << endl
	       << "     <nbLoop> la duree d'une experience en nbre de boucles (=multiples de 30ms)" << endl;
	  return 0; //nbLoop = 250 <=> 7.5 sec d'enregistrement
	}

      string fileName = "fichierSansNom.txt";
      Robots robots;
      cout << "Starting. Opening configuration.yml " << endl;
      // Charge la configuration et connecte les robots
      robots.loadYaml("config.yml");
      cout << "Loading robot drawer" << endl;
      Robot * robot = robots["drawer"];
      Motors * motors = robot->getMotors();
      Moves * moves = robot->getMoves();
      cout << "Starting motors" << endl;
      motors->start(30);
      syst_wait_ms(1000);
      cout << "Putting all motors compliant " << endl;
      robot->allCompliant();
      syst_wait_ms(1000);
      cout << "Initialization OK" << endl;
    
      if (strcmp(argv[1],"GET") == 0)
	{ //Demande d'acquisition
	  if (argc != 5)
	    {
	      cout << "Erreur : nombre d'arguments de GET invalide." << endl;
	      return 1;
	    }
	  fileName = argv[2];
	  if (fileName == "")
	    {
	      cout << "Error reading filename argument (arg2)" << endl;
	      return 1;
	    }
	  int posPt = fileName.find_last_of('.'); //La pos du dernier pt du fichier
	  int nbChiffres = 3;
	  int nbreExp = atoi(argv[3]);
	  string strNumExp = "000";
	  while (strNumExp.length()<nbChiffres)
	    strNumExp = "0"+strNumExp;
	  //fichier.txt => fichier000.txt si nbexp = 200
	  string fileNameExp = "";
	  int nbLoop = atoi(argv[4]);
	  for (int numExp=1; numExp<=nbreExp; numExp++)
	    {
	      //Generation du nom de fichier
	      strNumExp = "";
	      inttostring(numExp, &strNumExp);
	      if (posPt >= fileName.npos)
		fileNameExp = fileName + "0" + strNumExp;
	      else
		fileNameExp = fileName.substr(0,posPt) + "0" + strNumExp + fileName.substr(posPt);
	      getDatas(robot, motors, fileNameExp, nbLoop);
	    }
	}
      else if (strcmp(argv[1],"USE") == 0)
	{ //Demande d'execution
	  fileName = argv[2];
	  if (fileName == "")
	    {
	      cout << "Error reading filename argument (arg2)" << endl;
	      return 1;
	    }
	  useDatas(robot, motors, fileName);
	}
      else
	{
	  cout << "Erreur avec l'argument 1 : doit etre GET ou USE. " << endl;
	  return 1;
	}

      cout << "Program end. Putting all motors compliant. Final waiting of 5 sec." << endl;
      robot->allCompliant();
      syst_wait_ms(5000);
      cout << "*** END ***" << endl;
      robots.stop();
    }
  catch(string exc)
    {
      cout << "Received exception " << exc << endl;
      exit(0);
    }
  return 0;
}

void getDatas(Robot * robot, Motors * motors, const string & fileName, const int nbLoop)
{      
      cout << "Results will be written in " << fileName << " during nbLoop*30 ms" << endl;
      
      bool timeOut = false; int loop = 0;
      ofstream fichier(fileName.c_str(), ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
      
      if(!fichier)
	{
	    cout << "Opening file " << fileName << " failure. Stopping !";
	    robot->allCompliant();
	    return;
	}
      double valeur = 0;
      string varBidon = "";
      cout << "Ecrivez quelque chose et appuyez sur Entree pour demarrer. "; 
      cin >> varBidon;
      cout << "Attention... " << endl; syst_wait_ms(1000);
      cout << "Top !" << endl;
      while (!timeOut)
	  {
	    cout << loop << " / " << nbLoop << endl; 
	    fichier << loop << " ";
	    valeur = (*motors)["GEpauleLong"]->getCurrentAngle(); //Epaule longitudinale (1er moteur)
	    fichier << valeur << " ";
	    valeur = (*motors)["GEpauleLat"]->getCurrentAngle(); //Epaule laterale
	    fichier << valeur << " ";
	    valeur = (*motors)["GEpauleRot"]->getCurrentAngle();
	    fichier << valeur << " ";
	    valeur = (*motors)["GCoude"]->getCurrentAngle();
	    fichier << valeur << " ";
	    valeur = (*motors)["GCoudeRot"]->getCurrentAngle();
	    fichier << valeur << " ";
	    valeur = (*motors)["GPoignetLong"]->getCurrentAngle();
	    fichier << valeur << " ";
	    valeur = (*motors)["GPoignetLat"]->getCurrentAngle();
	    fichier << valeur << endl;
	    syst_wait_ms(30);
	    loop++;
	    if (loop > nbLoop) //nbLoop should be 250
	      timeOut = true;
	  }
      cout << "Experiment finished" << endl;
	fichier.close();
}

void useDatas(Robot * robot, Motors * motors, const string & fileName)
{
  cout << "Now motors are going to their initial position " << endl;
  motors->goToInit();
  //robot->motors["GEpauleLong"].setLoad(1.0);
  
  cout << "Applying results from " << fileName << " during 7.5 sec" << endl;
  
  bool timeOut = false; int loop = 0;
  ifstream fichier(fileName.c_str(), ios::in);  // ouverture en lecture
  
  if(!fichier)
    {
      cout << "Opening file " << fileName << " failure. Stopping !";
      robot->allCompliant();
      return;
    }
  //On lit les premieres valeurs pour initialiser le moteur
  cout << "Now motors are going to position 1 of file" << endl;
  fichier >> loop;
  double posEpauleLong, posEpauleLat, posEpauleRot, posCoude, posCoudeRot, posPoignetLong, posPoignetLat;
  fichier >> posEpauleLong >> posEpauleLat >> posEpauleRot >> posCoude >> posCoudeRot >> posPoignetLong >> posPoignetLat;
  syst_wait_ms(3000);
  goToFirstPos(motors, posEpauleLong, posEpauleLat, posEpauleRot, posCoude, posCoudeRot, posPoignetLong, posPoignetLat,5);
  
  cout << "Reading sequence" << endl;
  timeOut = false;
  double valeur = 0;
  while (!timeOut)
    {
      if (fichier.eof())
	{
	  timeOut = true;
	  cout << "EOF" << endl;
	}
      fichier >> loop;
      cout << loop << endl;
      if (fichier.eof())
	{
	  timeOut = true;
	  cout << "EOF2" << endl;
	}
      else
	{
	  fichier >> valeur; 
	  //cout << "GEpauleLong : " << valeur << endl;
	  (*motors)["GEpauleLong"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  (*motors)["GEpauleLat"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  (*motors)["GEpauleRot"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  //cout << "GCoude : " << valeur << endl; 
	  (*motors)["GCoude"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  (*motors)["GCoudeRot"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  (*motors)["GPoignetLong"]->setGoalAngle(valeur);
	  fichier >> valeur;
	  (*motors)["GPoignetLat"]->setGoalAngle(valeur); 
	  syst_wait_ms(30);
	  loop++;
	}
    }
  fichier.close();
  
  cout << "End of movement. Putting all motors compliant " << endl;
  robot->allCompliant();
  syst_wait_ms(1000);
  //robot->emergency();
}

void goToFirstPos(Motors * motors, double posEpauleLong, double posEpauleLat, double posEpauleRot, 
		  double posCoude, double posCoudeRot, double posPoignetLong, double posPoignetLat, 
		  const double duration)
{
  double initEpauleLong = (*motors)["GEpauleLong"]->getCurrentAngle(),
    initEpauleLat = (*motors)["GEpauleLat"]->getCurrentAngle(),
    initEpauleRot = (*motors)["GEpauleRot"]->getCurrentAngle(),
    initCoude = (*motors)["GCoude"]->getCurrentAngle(), 
    initCoudeRot = (*motors)["GCoudeRot"]->getCurrentAngle(),
    initPoignetLong = (*motors)["GPoignetLong"]->getCurrentAngle(),
    initPoignetLat = (*motors)["GPoignetLat"]->getCurrentAngle();
  for (int i=1; i<=100; i++)
    {
      //EpauleLong
      ssFirstPos(motors, "GEpauleLong", initEpauleLong, posEpauleLong, i);
      //EpauleLat
      ssFirstPos(motors, "GEpauleLat", initEpauleLat, posEpauleLat, i);
      //EpauleRot
      ssFirstPos(motors, "GEpauleRot", initEpauleRot, posEpauleRot, i);
      //Coude
      ssFirstPos(motors, "GCoude", initCoude, posCoude, i);
      //CoudeRot
      ssFirstPos(motors, "GCoudeRot", initCoudeRot, posCoudeRot, i);
      //PoignetLong
      ssFirstPos(motors, "GPoignetLong", initPoignetLong, posPoignetLong, i);
      //PoignetLat
      ssFirstPos(motors, "GPoignetLat", initPoignetLat, posPoignetLat, i);
      syst_wait_ms(30);
    }
}

void ssFirstPos(Motors * motors, const string &motorName, const double initPos, const double goalPos, const int pourcentage)
{
  /*bool posOk = true;
  double valeur = (*motors)[motorName.c_str()]->getCurrentAngle();
  cout << motorName.c_str() << " : currentAngle = " << valeur << " goal = " << posMotor << " ";
  if (valeur-posMotor>1)
    {
      posOk = false;
      (*motors)[motorName.c_str()]->setGoalAngle(valeur-1);
      cout << motorName.c_str() << " : pos--" << endl;
    }
  else if (valeur-posMotor<-1)
    {
      posOk = false;
      (*motors)[motorName.c_str()]->setGoalAngle(valeur+1);
      cout << motorName.c_str() << " : pos++" << endl;
    }
  else
    (*motors)[motorName.c_str()]->setGoalAngle(posMotor);
    return posOk;*/
  double currentPos = (initPos*(100-pourcentage) + goalPos*pourcentage) / 100;
  //cout << motorName.c_str() << " : currentPos = " << currentPos << " goal = " << goalPos << " ";
  (*motors)[motorName.c_str()]->setGoalAngle(currentPos);
}

void inttostring (int num, std::string *str)
{
    /*Insere le int dans la chaine str, prend en compte les nombres<0
    La chaine ne sera pas vidée, mais complétée à droite*/
    char chiffre=0;
    string abs=""; //le nombre en valeur absolue
    if (num == 0)
    {
        *str += "0";
    }
    else
    {
        if (num<0)
        {
            *str += "-";
            num *= -1;
        }
        while (num>0)
        {
            chiffre = num%10+48;
            num = num/10;
            abs = chiffre + abs;
        }
        *str += abs;
    }
}
