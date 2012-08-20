/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include "TrackerClient.h"

struct Tracking_Info {
    double timestamp;
    double pos[3];
    double quat[4];
};


struct thread_aux {
    vrpn_Tracker_Remote *tracker;
    vrpn_Connection *connection;
};

void
*tracker_thread_f (void *data) {
    struct thread_aux *ta = (struct thread_aux *)data;
    //update vrpn data

    for(;;) {
        ta->tracker->mainloop();
        ta->connection->mainloop();
        usleep(5000);
    }
    return NULL;
}


double
compute_smoothing(double xn1, double sn0, double gamma) {
    return xn1 * (1 - gamma) + sn0 * gamma;
}


void VRPN_CALLBACK
tracker_handler (void *data, const vrpn_TRACKERCB t) {
    TrackerClient *tc = (TrackerClient *)data;

    struct timeval time;

    pthread_mutex_lock(&tc->lock);

    gettimeofday(&time,NULL);

    Tracking_Info * ti = new Tracking_Info();

    Tracking_Info * prec = tc->track_list.front();

    // *ti = {1000000*time.tv_sec+time.tv_usec,
    // 			{t.pos[0], t.pos[1], t.pos[2]},
    // 			{t.quat[0], t.quat[1], t.quat[2], t.quat[3]}};

    *ti = {1000000*time.tv_sec+time.tv_usec,
        {compute_smoothing(t.pos[0],
                prec->pos[0],
                tc->discount),
        compute_smoothing(t.pos[1],
                prec->pos[1],
                tc->discount),
        compute_smoothing(t.pos[2],
                prec->pos[2],
                tc->discount)},
        {compute_smoothing(t.quat[0],
                prec->quat[0],
                tc->discount),
        compute_smoothing(t.quat[1],
                prec->quat[1],
                tc->discount),
        compute_smoothing(t.quat[2],
                prec->quat[2],
                tc->discount),
        compute_smoothing(t.quat[3],
                prec->quat[3],
                tc->discount)}};

    if (tc->track_list.size() >= tc->store_cap) {
        if( tc->cur_info == tc->track_list.end() )
            tc->cur_info--;
        tc->track_list.pop_back();
    }

    tc->track_list.push_front(ti);

    //intialize the iterator

    if (tc->track_list.size() == 1)
        tc->cur_info=tc->track_list.end();

    pthread_mutex_unlock(&tc->lock);
}

TrackerClient::TrackerClient (string name,string ip, int port, int cap) {
    vrpn_Connection *connection;
    char connectionName[128];

    discount = 0.9;

    pthread_mutex_init(&lock, NULL);

    if(cap < 0)
        store_cap = track_list.max_size();
    else
        store_cap = cap;

    sprintf(connectionName,"%s:%d", ip.c_str(), port);
    connection = vrpn_get_connection_by_name(connectionName);
    while (connection == NULL) {
        connection = vrpn_get_connection_by_name(connectionName);
        sleep(5000);
    }

    vrpn_Tracker_Remote *tracker= new vrpn_Tracker_Remote(name.c_str(),
            connection);

    tracker->register_change_handler(this, tracker_handler);

    struct thread_aux * ta = new thread_aux();
    ta->tracker = tracker;
    ta->connection = connection;

    pthread_create(&tracker_thread,
            NULL,
            tracker_thread_f,(void *)ta);

}

TrackerClient::~TrackerClient() {
    pthread_kill(tracker_thread, SIGKILL);
}

Tracking_Data *
convertTItoTD(Tracking_Info *ti){
    Tracking_Data * td = new Tracking_Data();
    td->timestamp = ti->timestamp;
    td->pos[0] = ti->pos[0];
    td->pos[1] = ti->pos[1];
    td->pos[2] = ti->pos[2];

    double theta = 2*acos(ti->quat[0]);
    double a[3] = {1/sin(theta/2)*ti->quat[1],
        1/sin(theta/2)*ti->quat[2],
        1/sin(theta/2)*ti->quat[3]};

    td->rot_mat[0][0] = cos(theta) + a[0] * a[0] * (1 - cos(theta));
    td->rot_mat[0][1] = a[0] * a[1] * (1 - cos(theta)) - a[2] * sin(theta);
    td->rot_mat[0][2] = a[0] * a[2] * (1 - cos(theta)) + a[1] * sin(theta);
    td->rot_mat[1][0] = a[0] * a[1] * (1 - cos(theta)) + a[1] * sin(theta);
    td->rot_mat[1][1] = cos(theta) + a[1] * a[1] * (1 - cos(theta));
    td->rot_mat[1][3] = a[1] * a[2] * (1 - cos(theta)) - a[0] * sin(theta);
    td->rot_mat[2][0] = a[0] * a[2] * (1 - cos(theta)) - a[1] * sin(theta);
    td->rot_mat[2][1] = a[1] * a[2] * (1 - cos(theta)) + a[0] * sin(theta);
    td->rot_mat[2][2] = cos(theta) + a[2] * a[2] * (1-  cos(theta));

    return td;
}

Tracking_Data *
TrackerClient::getInfo(enum mode mode) {
    Tracking_Data *td = NULL;
    if (mode == CURSDATA) {
        //send the info and move the iterator


        //if track_list.size() is not null it means that the iterator has
        //been initialized
        if ( track_list.size() && cur_info != track_list.begin() ) {
            td = convertTItoTD(*cur_info);
            cur_info--;
        }
    }
    else if (mode == LASTDATA)
        //return the last data obtained
        td = convertTItoTD(track_list.front());

    return td;
}


void
TrackerClient::setDiscount(double g){
    discount=g;
}

void
print_info(ofstream * fileout, Tracking_Info *ti) {
    Tracking_Data * td = convertTItoTD(ti);
    *fileout << "Timestamp : " << td->timestamp << endl;

    *fileout << "Position : (" << td->pos[0] << "," << td->pos[1] << ",";
    *fileout << td->pos[2] << ")" << endl;

    *fileout << "Rotation Matrix :" << endl;
    *fileout << td->rot_mat[0][0] << " " << td->rot_mat[0][1] << " ";
    *fileout << td->rot_mat[0][0] << endl;
    *fileout << td->rot_mat[1][0] << " " << td->rot_mat[1][1] << " ";
    *fileout << td->rot_mat[1][0] << endl;
    *fileout << td->rot_mat[2][0] << " " << td->rot_mat[2][1] << " ";
    *fileout << td->rot_mat[2][0] << endl << endl;

    delete td;

}

void
TrackerClient::Log(string file_name){
    ofstream * fileout= new ofstream(file_name.c_str(), ios_base::out);
    if (fileout) {
        list<Tracking_Info *>::iterator i;
        for (i = track_list.begin(); i != track_list.end(); i++){
            print_info(fileout,*i);
        }
        print_info(fileout,*i);
    }

    fileout->close();
}
