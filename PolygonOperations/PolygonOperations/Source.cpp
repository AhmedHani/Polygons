#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#define MAX_POINTS 100
#define MAX_POLYGONS 100
#define MAX_OPERATIONS 100

using namespace std;

///////////////// HELPER FUNCTIONS ////////////////
vector<std::string> split(string &text, char sep) {
  vector<std::string> tokens;
  int start = 0;
  int end = 0;

  while ((end = text.find(sep, start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }

  tokens.push_back(text.substr(start));

  return tokens;
}

double are_collinear(double x1, double y1, double x2, double y2, double x3, double y3) {
	return (y1 - y2) * (x1 - x3) == (y1 - y3) * (x1 - x2);
}
///////////////// END OF FUNCTIONS //////////////

///////////////// STRUCTURES ////////////////////

struct Point {
	double x;
	double y;

	bool operator< (const Point& pt) const {
        return (x < pt.x);
	}

	bool operator== (const Point& pt) const {
		return (x == pt.x && y == pt.y);
	}

	bool operator> (const Point& pt) const {
		return (x > pt.x);
	}
};


class Polygon {
private:
	Point* p_points;
	Point* redundant_points;
	int num_points;

	double are_collinear(double x1, double y1, double x2, double y2, double x3, double y3) {
		return (y1 - y2) * (x1 - x3) == (y1 - y3) * (x1 - x2);
	}

public:
	void set_points(Point* p_points) {
		this->p_points = p_points;
	}
	void set_num_points(int num_of_points) {
		this->num_points = num_of_points;
	}
	int get_num_points() {
		return this->num_points;
	}
	Point* get_points() {
		return this->p_points;
	}
	Point get_point(int index) {
		return this->p_points[index];
	}
	int get_max_x() {
		int max = -1;
		Point max_point;

		for (int i = 0; i < this->num_points; i++) {
			int current_x = this->p_points[i].x;

			if (current_x > max) {
				max_point = this->p_points[i];
			}
		}

		return max_point.x;
	}
	int get_min_x() {
		int max = INT_MAX;
		Point max_point;

		for (int i = 0; i < this->num_points; i++) {
			int current_x = this->p_points[i].x;

			if (current_x < max) {
				max_point = this->p_points[i];
			}
		}

		return max_point.x;
	}
	int get_max_y() {
		int max = -1;
		Point max_point;

		for (int i = 0; i < this->num_points; i++) {
			int current_y = this->p_points[i].y;

			if (current_y > max) {
				max_point = this->p_points[i];
			}
		}

		return max_point.y;
	}
	int get_min_y() {
		int max = INT_MAX;
		Point max_point;

		for (int i = 0; i < this->num_points; i++) {
			int current_y = this->p_points[i].y;

			if (current_y < max) {
				max_point = this->p_points[i];
			}
		}

		return max_point.y;
	}
	vector<Point> get_redundant_points() {
		vector<Point> all_points(this->p_points, this->p_points + this->num_points);
		vector<Point> unique_points(this->p_points, this->p_points + this->num_points);
		unique_points.erase(unique(unique_points.begin(), unique_points.end()), unique_points.end());

		vector<Point> redundant_points;
		set<Point> visited_points;

		for (int i = 0; i < all_points.size(); i++) {
			bool found = false;

			for (set<Point>::iterator it = visited_points.begin(); it != visited_points.end(); it++) {
				if (it->x == all_points[i].x && it->y == all_points[i].y) {
					found = true;

					break;
				}
			}

			if (found) {
				redundant_points.push_back(all_points[i]);
			} else {
				visited_points.insert(all_points[i]);
			}
		}

		vector<Point> collinear_points;

		for (vector<Point>::iterator itf = unique_points.begin(); itf != unique_points.end(); itf++) {
			for (vector<Point>::iterator its = unique_points.begin(); its != unique_points.end(); its++) {
				for (vector<Point>::iterator itt = unique_points.begin(); itt != unique_points.end(); itt++) {
					if (itf == its && itf == itt && itt == its)
						continue;
					if (itf == its || its == itt || itt == itf)
						continue;

					if (this->are_collinear(itf->x, itf->y, its->x, its->y, itt->x, itt->y)) {
						collinear_points.push_back(*itt);
					}
				}
			}
		}

		vector<Point> ignore;

		for (int k = 0; k < collinear_points.size(); k += 3) {
			Point p1 = collinear_points[k];
			Point p2 = collinear_points[k + 1];
			Point p3 = collinear_points[k + 2];

			ignore.push_back(p2);
		}

		sort(ignore.begin(), ignore.end());
		ignore.erase(unique(ignore.begin(), ignore.end()), ignore.end());

		for (int i = 0; i < ignore.size(); i++) {
			redundant_points.push_back(ignore[i]);
		}

		return redundant_points;
	}
};
///////////////// END OF STRUCTURES ////////////////////

///////////////// CONSTANTS ///////////////////////////
const string level_1_supported_operations[] = {
	"Number_Polygons",
	"Total_Number_Points",
	"Minimum_X", "Maximum_X",
	"Minimum_Y", "Maximum_Y",
	"Enclosing_Rectangle",
	"Total_Redundant_Points",
	"Quit"
};

const string level_2_supported_operations[] = {
	""
};

const string level_3_supported_operations[] = {
	""
};

////////////// END OF CONSTANTS ////////////////


vector<Polygon> polygons;
vector<pair<string, int>> operations;

void parse_file(string file_name) {
	std::ifstream ifs(file_name);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	vector<string> content_tokens = split(content, '\n');

	vector<string> polygons_data = split(content_tokens[0], ';');

	polygons.clear();
	
	for (int i = 0; i < polygons_data.size(); i++) {
		string current_polygon_data = polygons_data[i];
		vector<string> polygon_points_data = split(current_polygon_data, '),');
		vector<Point> polygon_points;
		
		for (int j = 0; j < polygon_points_data.size(); j += 2) {
			string remove_delim_x = polygon_points_data[j];
			remove_delim_x.erase(std::remove(remove_delim_x.begin(), remove_delim_x.end(), '('), remove_delim_x.end());
			remove_delim_x.erase(std::remove(remove_delim_x.begin(), remove_delim_x.end(), ')'), remove_delim_x.end());

			string remove_delim_y = polygon_points_data[j + 1];
			remove_delim_y.erase(std::remove(remove_delim_y.begin(), remove_delim_y.end(), '('), remove_delim_y.end());
			remove_delim_y.erase(std::remove(remove_delim_y.begin(), remove_delim_y.end(), ')'), remove_delim_y.end());

			Point point;
			point.x = atoi(remove_delim_x.c_str());
			point.y = atoi(remove_delim_y.c_str());

			polygon_points.push_back(point);
		}

		Polygon polygon = Polygon();
		Point* points_array = new Point[polygon_points.size()];
		copy(polygon_points.begin(), polygon_points.end(), points_array);

		polygon.set_points(points_array);
		polygon.set_num_points(polygon_points.size());

		polygons.push_back(polygon);
	}

	vector<string> operations_list(content_tokens.begin() + 1, content_tokens.end());

	for (int i = 0; i < operations_list.size(); i++) {
		string current_operation = operations_list[i];

		if (current_operation == "Quit")
			break;

		int operations_level = -1;
		string existed_operation = "";

		for (int j = 0; j < (sizeof(level_1_supported_operations)/sizeof(*level_1_supported_operations)); j++) {
			if (current_operation == level_1_supported_operations[j]) {
				operations_level = 1;
				existed_operation = current_operation;

				break;
			}
		}

		if (operations_level == -1) {
			for (int j = 0; j < (sizeof(level_2_supported_operations)/sizeof(*level_2_supported_operations)); j++) {
				if (current_operation == level_2_supported_operations[j]) {
					operations_level = 2;
					existed_operation = current_operation;

					break;
				}
			}
		}

		if (operations_level == -1) {
			for (int j = 0; j < (sizeof(level_3_supported_operations)/sizeof(*level_3_supported_operations)); j++) {
				if (current_operation == level_3_supported_operations[j]) {
					operations_level = 3;
					existed_operation = current_operation;

					break;
				}
			}
		}

		operations.push_back(make_pair(existed_operation, operations_level));
	}
}

void display_data() {
	cout << "\n\nPolygons Data:" << endl << endl;

	for (int i = 0; i < polygons.size(); i++) {
		cout << "Polygon #" << i << endl;

		for (int j = 0; j < polygons[i].get_num_points(); j++) {
			cout << polygons[i].get_point(j).x << " " <<  polygons[i].get_point(j).y << endl;
		}
	}

	cout << "\n\nTarget Operations" << endl << endl;

	for (int i = 0; i < operations.size(); i++) {
		cout << operations[i].first << " Level #" << operations[i].second << endl;
	}

	cout << endl << endl;
}

void remove_redundant_points() {
	for (int i = 0; i < polygons.size(); i++) {
		cout << endl;
		vector<Point> current_redundant = polygons[i].get_redundant_points();

		for (int j = 0; j < current_redundant.size(); j++) {
			cout << current_redundant[j].x << " " << current_redundant[j].y << endl;
		}
	}
}

void operations_processing() {
	cout << "RESULTS:" << endl << endl;

	for (int i = 0; i < operations.size(); i++) {
		if (operations[i].first == "Number_Polygons") {
			cout << "Number of input Polygons are: " << polygons.size() << endl;
		}

		if (operations[i].first == "Total_Number_Points") {
			for (int j = 0; j < polygons.size(); j++) {
				cout << "Polygon #" << i <<  endl << "Number of Points are: " << polygons[i].get_num_points() << endl;
			}
		}
	}

	cout << endl << endl;

	for (int i = 0; i < polygons.size(); i++) {
		Polygon current_polygon = polygons[i];

		cout << "Polygon #" << i << endl;

		for (int j = 0; j < operations.size(); j++) {
			pair<string, int> current_operation = operations[j];

			if (current_operation.first == "Minimum_X") {
				cout << "Minimum X: " << current_polygon.get_min_x() << endl;
			}

			if (current_operation.first == "Maximum_X") {
				cout << "Maximum X: " << current_polygon.get_max_x() << endl;
			}

			if (current_operation.first == "Minimum_Y") {
				cout << "Minimum Y: " << current_polygon.get_min_y() << endl;
			}

			if (current_operation.first == "Maximum_Y") {
				cout << "Maximum Y: " << current_polygon.get_max_y() << endl;
			}

			if (current_operation.first == "Total_Redundant_Points") {
				cout << "Total_Redundant_Points: " << current_polygon.get_redundant_points().size() << endl;
			}
		}
	}
}


int main(int argc, char** argv) {
	parse_file("input.txt");
	display_data();
	operations_processing();

	return 0;

}