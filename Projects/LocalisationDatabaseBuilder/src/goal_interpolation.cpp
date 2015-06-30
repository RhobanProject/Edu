#include <Localisation/LocalizationDatabase.h>
#include <ObjectDetection/goal_detector.hpp>
#include <rhoban/robot/Robots.h>
#include <rhoban/vision/Vision.hpp>
#include <opencv2/opencv.hpp>

#define CLOSEST_COUNT 3
#define HUGE_FLOAT 1000.0
#define DIR_DIST_GAIN 20
#define FIELD_X 1.75
#define FIELD_Y 3.0

vector<LocalizationSample> find_closest(LocalizationDatabase &db, float x, float y, float dir);
float distance(LocalizationSample a, float x, float y, float dir);
GoalLines interpolate(vector<LocalizationSample> closest, float x, float y, float dir);
cv::Point interpolate_point(vector<cv::Point> points, vector<float> weights);

float distance(LocalizationSample a, float x, float y, float dir)
{
	float dx = (a.robotX - x) * FIELD_X;
	float dy = (a.robotY - y) * FIELD_Y;
	float d_dir = (a.orientation - dir) / DIR_DIST_GAIN;
	
	return sqrt(dx*dx + dy*dy + d_dir*d_dir);
}

GoalLines pos_to_GoalLines(LocalizationDatabase db, float x, float y, float dir)
{
	vector<LocalizationSample> closest_samples = find_closest(db, x, y, dir);

	cout << "Found " << closest_samples.size() << " close samples" << endl;

	GoalLines goal = interpolate(closest_samples, x, y, dir);
	
	return goal;
}

vector<LocalizationSample> find_closest(LocalizationDatabase &db, float x, float y, float dir)
{
	vector<LocalizationSample> closest(CLOSEST_COUNT);
	vector<float> distances(CLOSEST_COUNT, HUGE_FLOAT);
	
	for (int i = 0; i < db.size(); i++)
	{
		float dist = distance(db.getSample(i), x, y, dir);
		for (int j = 0; j < CLOSEST_COUNT; j++)
		{
			if (dist < distances[j])
			{
				for (int k = CLOSEST_COUNT - 1; k > j; k--)
				{
					closest[k] = closest[k-1];
				}
				
				closest[j] = db.getSample(i);
				distances[j] = dist;
				
				break;
			}
		}
	}
	
	return closest;
}

GoalLines interpolate(vector<LocalizationSample> closest, float x, float y, float dir)
{
	vector<cv::Point> closest_bot_left;
	vector<cv::Point> closest_bot_right;
	vector<cv::Point> closest_top_left;
	vector<cv::Point> closest_top_right;
	vector<float> weights;
	
	for (int i = 0; i < CLOSEST_COUNT; i++)
	{
		cout << "found closest at: " << closest[i].robotX << " " << closest[i].robotY << " " << closest[i].orientation << endl;
		cout << "distance: " << distance(closest[i], x, y, dir) << endl;

		closest_bot_left.push_back(closest[i].goal.left_bot);
		closest_bot_right.push_back(closest[i].goal.right_bot);
		closest_top_left.push_back(closest[i].goal.left_top);
		closest_top_right.push_back(closest[i].goal.right_top);
		float dist = distance(closest[i], x, y, dir);
		float weight = 1 / (dist == 0 ? (1 / HUGE_FLOAT) : dist);
		
		cout << "weight: " << weight << endl;
		weights.push_back(weight);
	}
	
	cout << "Weights: ";
	for (int i = 0; i < weights.size(); i++)
		cout << " " << weights[i];
	cout << endl;
	
	cv::Point bot_left = interpolate_point(closest_bot_left, weights);
	cv::Point bot_right = interpolate_point(closest_bot_right, weights);
	cv::Point top_left = interpolate_point(closest_top_left, weights);
	cv::Point top_right = interpolate_point(closest_top_right, weights);		
	
	cout << "bot left: " << bot_left.x << " " << bot_left.y << endl;
	cout << "top left: " << top_left.x << " " << top_left.y << endl;
	cout << "top right: " << top_right.x << " " << top_right.y << endl;
	cout << "bot right: " << bot_right.x << " " << bot_right.y << endl;
	
	GoalLines goal(bot_left, top_left, top_right, bot_right);
	
	return goal;
}

cv::Point interpolate_point(vector<cv::Point> points, vector<float> weights)
{
	float x = 0;
	float y = 0;
	float weight_sum = 0;
	
	for (int i = 0; i < CLOSEST_COUNT; i++)
	{	
		weight_sum += weights[i];
		x += points[i].x * weights[i];
		y += points[i].y * weights[i];
	}
	
	x /= weight_sum;
	y /= weight_sum;
	
	return cv::Point(x, y); 
}

int main(void)
{
	LocalizationDatabase db;
	db.load_file("loc_db.xml");
	float x = 0;
	float y = 0.2;
	float direction = 0;
	GoalLines goal = pos_to_GoalLines(db, x, y, direction);
	cout << goal.toString() << endl;
	
	try
	{
		Connection connection;
		connection.connectTo("localhost", 7777);

		Rhoban::Vision vision(&connection);

		while(true)
		{
			cv::Mat png_frame = vision.grabPngFrame("bgr", 320, 240);
			goal.tag(png_frame, cv::Scalar(0,130,255), false, 4); 
			cv::imshow("png", png_frame);
			cv::waitKey(33);
		}
	}
	catch(string exc)
	{
		cout << "Received exception " << exc << endl;
		exit(0);
	}
}
