#include <iostream>
#include <Localisation/PositionEstimator.cpp>
#include <rhoban/robot/Robots.h>
#include <rhoban/robot/Robot.h>
#include <communication/Message.h>
#include <unistd.h>

#define ROBOT_DIR 0
#define ROBOT_X 0
#define ROBOT_Y 0
#define YAW 0
#define PITCH 0

vector<float> getServosAngles(Robot* robot)
{
	Message msg = robot->getConnection()->GetRelativeServosAngles_response(10000);
	vector<string> names = msg.read_string_array();
	vector<float> angles = msg.read_float_array();
	vector<float> res(2);
	for (int i = 0; i < names.size(); i++)
	{
		if (names[i] == "Pan")
		{
			res[0] = angles[i];
		}
		if (names[i] == "Tilt")
		{
			res[1] = angles[i];
		}
	}	
	
	return res;
}

int main(void)
{
	try{

	Robots robots;
	robots.loadYaml("config_vision.yml");
	Robot* robot = robots["server"];
	double robot_dir = ROBOT_DIR;

	Matrix robot_2D_coords(1, 2);
	robot_2D_coords[0] = ROBOT_X;
	robot_2D_coords[1] = ROBOT_Y;

	Matrix object_2D_img_coords(1, 2);
	double cam_pan = 0;
	double cam_tilt = 0;
	while(true)
	{
		Message msg = robot->getConnection()->VisionGetBallInfo_response(true, 10000);
		vector<float> angles = getServosAngles(robot);		
		cam_pan = angles[0];
		cam_tilt = angles[1];
		usleep(1000 * 200);
		if (msg.read_bool())
		{
			cout << string(50, '\n');
			
			object_2D_img_coords[0] = msg.read_float();
			object_2D_img_coords[1] = msg.read_float();
			Matrix pos = RobocupParticleFilter::theorical_object_postion(robot_dir, robot_2D_coords, object_2D_img_coords, cam_tilt, cam_pan, true);
			cout << pos.transpose() << endl;
		}
		else
		{
			cout << "no ball detected" << endl;
		}
	}
	}
	catch (const string& exception)
	{
		cout << exception << endl;
	}	
}


