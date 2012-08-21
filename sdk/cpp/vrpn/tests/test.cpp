/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <iostream>

#include <main/Command.h>

#include "TrackerClient.h"

COMMAND_DEFINE(tracker_test)
{
    TrackerClient tc("tracker", "192.168.16.100", 3883, -1);
    tc.setDiscount(0.2);

    int i;
    for ( i = 0; i < 600; i++) {
        Tracking_Data * temp = tc.getInfo();

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
    tc.Log("Tracker.log");

    return 0;
}
