/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef TRACKER_CLIENT_H_
#define TRACKER_CLIENT_H_

#include <list>
#include <string>
#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <math.h>

#include "vrpn_Connection.h"
#include "vrpn_Tracker.h"

using namespace std;

typedef struct Tracking_Info Tracking_Info;

typedef struct {
    double timestamp;
    double pos[3];
    double rot_mat[3][3];
} Tracking_Data ;

enum mode {
    LASTDATA,
    CURSDATA
};

class TrackerClient {
    private :
        list<Tracking_Info *> track_list;
        pthread_t tracker_thread;
        pthread_mutex_t lock;
        int store_cap;
        list<Tracking_Info *>::iterator cur_info;
        double discount;

        friend void VRPN_CALLBACK tracker_handler (void *, const vrpn_TRACKERCB t);

    public :
        /* the first string is name of the rigid body to track, the second
           one is the ip address of the broadcasting server and the first
           int a port number and the last one the number of value to store
           -1 if all*/

        Tracker_Client(string name,string ip, int port, int cap);
        ~Tracker_Client();

        /*return NULL if empty wether you want to get the last value or
          the one pointed by the cursor*/
        Tracking_Data * getInfo(enum mode mode);

        void smooth(double gamma);

        // mettre toute la liste dans un ficher texte
        void Log(string file_name);
};


#endif
