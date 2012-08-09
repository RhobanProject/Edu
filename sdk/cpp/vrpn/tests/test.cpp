#include <iostream>

#include <main/Command.h>

#include "TrackerClient.h"

COMMAND_DEFINE(tracker_test)
{
    TrackerClient tc("tracker", "192.168.16.100", 3883, -1);

    sleep(10);
    int i;
    for ( i = 0; i < 600; i++) {
        Tracking_Data * temp = tc.getInfo(CURSDATA);

        if (temp != NULL) {
            cout << "Timestamp : " << temp->timestamp << endl;

            cout << "Position : (" << temp->pos[0] << "," << temp->pos[1] << ",";
            cout << temp->pos[2] << ")" << endl;

            cout << "Rotation Matrix :" << endl;
            cout << temp->rot_mat[0][0] << " " << temp->rot_mat[0][1] << " ";
            cout << temp->rot_mat[0][0] << endl;
            cout << temp->rot_mat[1][0] << " " << temp->rot_mat[1][1] << " ";
            cout << temp->rot_mat[1][0] << endl;
            cout << temp->rot_mat[2][0] << " " << temp->rot_mat[2][1] << " ";
            cout << temp->rot_mat[2][0] << endl << endl;

            usleep(10);
        }

        delete temp;
    }
    tc.Log("avant.txt");
    tc.smooth(0.2);
    tc.Log("après.txt");

    return 0;
}
