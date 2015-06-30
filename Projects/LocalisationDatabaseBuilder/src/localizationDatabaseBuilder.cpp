#include <string>
#include <iostream>
#include <opencv/highgui.h>
#include <rhoban/robot/Robots.h>
#include <rhoban/robot/Robot.h>
#include <communication/Message.h>
#include <Localisation/LocalizationDatabase.h>

#include <timing/TickTimer.h>

#define ROBOT_X_STEPS 4
#define ROBOT_Y_STEPS 4
#define ROBOT_ROT_STEPS 8

vector<float> getServosAngles(Connection* connection)
{
	vector<float> angles;
	Message msg = connection->GetRelativeServosAngles_response(1000);
	msg.read_string_array();
	
	return msg.read_float_array();
}

void build_points(float robotX, float robotY, float ballX, float ballY, float orientation, Connection* connection, LocalizationDatabase* db)
{
	float minPan = -30;
	float maxPan = 30;
	float minTilt = -30;
	float maxTilt = 30;
	float steps = 1;
	vector<string> str;
	str.push_back("TeteAlt");
	str.push_back("TeteLat");
	vector<float> angles;
	
	angles.resize(2);;

	for (float i = minPan; i < maxPan; i += (maxPan - minPan) / steps)
	{
		angles[0] = i;
		connection->SetRelativeServosAngles(str, angles);
		for (float j = minTilt; j < maxTilt; j += (maxTilt - minTilt) / steps)
		{
			angles[1] = j;
			connection->SetRelativeServosAngles(str, angles);
			vector<float> remote_angles = getServosAngles(connection);

			LocalizationSample sample(robotX, robotY, ballX, ballY, orientation, remote_angles);
			
			Vision vision = Vision(connection);
			cv::Mat frame = vision.grabJpegFrame("bgr", LOC_DB_IMG_WIDTH, LOC_DB_IMG_HEIGHT);

			stringstream stra;
			stra << db->size();
			string filename = stra.str() + string(".png");
			cv::imwrite(filename, frame);
			vector<float> goal_values = connection->VisionGetFullGoalInfo_response(10000).read_float_array();
			sample.goal.from_vector(goal_values);
			db->addPoint(sample);
			cout << sample.goal.toString() << endl;
		}
	}
}

void next_guided_point(int index, float* robotX, float* robotY, float* ballX, float* ballY, float* dir)
{
	if (index > ROBOT_X_STEPS*ROBOT_Y_STEPS*ROBOT_ROT_STEPS)
	{
		cout << "All the required points are added" << endl;
		return;
	}

	for (int i = 0; i < ROBOT_X_STEPS; i++)
	{
		for (int j = 0; j < ROBOT_Y_STEPS; j++)
		{
			for (int k = 0; k < ROBOT_ROT_STEPS; k++)
			{
				if (i*ROBOT_Y_STEPS + j*ROBOT_ROT_STEPS + k == index)
				{
					*robotX = (2.0f / (float) i) - 1;
					*robotY = (2.0f / (float) j) - 1;
					*ballX = 0.0f;
					*ballY = 0.0f;
					*dir = 360.0f / (float) k;
					
					cout << "Next point:" << endl;
					cout << "robot: " << *robotX << " " << *robotY << endl;
					cout << "ball: " << *ballX << " " << *ballY << endl;
					cout << "direction: " << *dir << endl;
					return;
				}
			}
		}
	}

	cout << "Last point reached" << endl;
}



int main(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			cout << "Usage: ./localizationDatabaseBuilder IP" << endl;
			return 1;
		}
		
		Connection connection;
		connection.connectTo(argv[1], 7777);
		
		LocalizationDatabase db;
		float robotX = 0;
		float robotY = 0;
		float ballX = 0;
		float ballY = 0;
		float orientation = 0;
		bool run = true;
		bool guided = false;
		int guided_index = 0;
		string command;
		cv::Mat frame;
		
		while (run)
		{
			cout << ">> " << flush;
			getline(cin, command);
		
			if (command == "quit" || command == "exit" || command == "stop")
			{
				run = false;
			}
			else if (command == "help")
			{
				cout << "Available commands:" << endl;
				cout << "quit: exits the program" << endl;
				cout << "new: prompts the coordinates of the robot and the ball, and create a new sample" << endl;
				cout << "add: use the current values of the coordinates, and create a new sample" << endl;
				cout << "print: display the xml of the database" << endl;
				cout << "save: saves the database in a file" << endl;
				cout << "load: loads the database from a file" << endl;
				cout << "robotX [value]: set robotX to value" << endl;
				cout << "robotY [value]: set robotY to value" << endl;
				cout << "ballX [value]: set ballX to value" << endl;
				cout << "ballY [value]: set ballX to value" << endl;				
			}
			else if (command.find("robotX ") == 0 && command.size() > 7)
			{
				robotX = atof((command.substr(7).c_str()));
				cout << "Setting robotX to : " << robotX << endl;
			}
			else if (command.find("robotY ") == 0 && command.size() > 7)
			{
				robotY = atof((command.substr(7).c_str()));
			}
			else if (command.find("ballX ") == 0 && command.size() > 6)
			{
				ballX = atof((command.substr(6).c_str()));
			}
			else if (command.find("ballY ") == 0 && command.size() > 6)
			{
				ballY = atof((command.substr(6).c_str()));
			}
			else if (command == "values")
			{
				cout << "robotX: " << robotX << endl;
				cout << "robotY: " << robotY << endl;
				cout << "ballX: " << ballX << endl;
				cout << "ballY: " << ballY << endl;	
			}
			else if(command == "test")
			{
				build_points(0, 0, 0, 1, 0, &connection, &db);
			}
			else if (command == "add")
			{					
				build_points(robotX, robotY, ballX, ballY, orientation, &connection, &db);
				
				if (guided)
				{
					guided_index++;
					next_guided_point(guided_index, &robotX, &robotY, &ballX, &ballY, &orientation);
				}
			}			
			else if (command == "new")
			{
				cout << "Robot X = ";
				cin >> robotX;
				cout << "Robot Y = ";
				cin >> robotY;
				cout << "Ball X = ";
				cin >> ballX;
				cout << "Ball Y = ";
				cin >> ballY;
				cout << "orientation = ";
				cin >> orientation;
				
				build_points(robotX, robotY, ballX, ballY, orientation, &connection, &db);
				
				getline(cin, command);
			}
			else if (command == "manual")
			{
				cout << "Mode Manual enabled" << endl;
				guided = false;
			}
			else if (command == "guided")
			{
				cout << "Mode Guided enabled" << endl;
				guided = true;
				guided_index = 0;
				next_guided_point(guided_index, &robotX, &robotY, &ballX, &ballY, &orientation);
			}
			else if (command == "print")
			{
				cout << db.size() << endl;
				db.pretty_print();
			}
			else if (command == "save")
			{
				db.save_file("loc_db.xml");
			}
			else if (command == "load")
			{
				db.load_file("loc_db.xml");
			}
		}
	}
	catch (const string& exception)
	{
		cout << exception << endl;
	}
	
	return 0;
}
