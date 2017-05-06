#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

#define MAX_POINTS 100
#define MAX_POLYGONS 100
#define MAX_OPERATIONS 100

using namespace std;

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
	"Polygon_Points",
	"Point_Polygons",
	"List_Polygons_Points",
	"List_Points_Polygons",
	"Polygon_Perimeter",
	"List_Triangles",
	"List_Rectangles",
	"List_Trapezoid"
};

const string level_3_supported_operations[] = {
	"Inside_Rectangle",
	"Inside_Circle",
	"Polygon_Area",
	"Polygons_Area_Range",
	"Polygons_Enclosing_Point",
	"Is_Intersecting",
	"Intersecting_Group",
	"Largest_Intersecting_Pair",
	"Largest_Rectangle_Inside",
	"Largest_Circle_Inside"
};

const string bonus_supported_operations[] = {
	"Is_Connected",
	"Is_Connected_Group",
	"Maximum_Intersecting_Group",
	"Maximum_Connected_Group"
};
////////////// END OF CONSTANTS ////////////////

///////////////// HELPER FUNCTIONS ////////////////
vector<string> split(string &text, char deli) {
	vector<string> tokens;
	int start = 0;
	int end = 0;

	while ((end = text.find(deli, start)) != string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}

	tokens.push_back(text.substr(start));

	return tokens;
}
///////////////// END OF FUNCTIONS //////////////

///////////////// STRUCTURES ////////////////////

struct Point {
	double x;
	double y;

	Point() {}

	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}

	bool operator< (const Point& pt) const {
		return (x < pt.x);
	}

	bool operator== (const Point& pt) const {
		return (x == pt.x && y == pt.y);
	}

	bool operator> (const Point& pt) const {
		return (x > pt.x);
	}

	Point operator+ (const Point& pt);
	Point operator- (const Point& pt);
};

Point Point::operator+(const Point& pt) {
	return Point(this->x + pt.x, this->y + pt.y);
}

Point Point::operator-(const Point& pt) {
	return Point(this->x - pt.x, this->y - pt.y);
}

struct Segment {
	Point start;
	Point end;

	Segment() {}

	Segment(Point start, Point end) {
		this->start = start;
		this->end = end;
	}
};

class Polygon {
private:
	Point* p_points;
	int all_num_points;
	vector<Point> redundant_points;
	int redundant_num_points;
	vector<Point> actual_points;
	int actual_num_points;

	double are_collinear(double x1, double y1, double x2, double y2, double x3, double y3) {
		return (y1 - y2) * (x1 - x3) == (y1 - y3) * (x1 - x2);
	}
	double euclidean_distance(double x1, double y1, double x2, double y2) {
		return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	}

public:
	void set_all_points(Point* p_points, int num_points) {
		this->p_points = p_points;
		this->all_num_points = num_points;

		this->remove_redundant_points();
	}
	Point* get_points() {
		return this->p_points;
	}
	int get_num_points() {
		return this->all_num_points;
	}
	Point get_point(int index) {
		return this->p_points[index];
	}
	bool in_polygon(Point point) {
		for (int i = 0; i < this->all_num_points; i++) {
			if (this->p_points[i] == point) {
				return true;
			}
		}

		return false;
	}
	vector<Point> get_redundant_points() {
		return this->redundant_points;
	}
	int get_num_redundant_points() {
		return this->redundant_points.size();
	}
	vector<Point> get_actual_points() {
		return this->actual_points;
	}
	int get_num_actual_points() {
		return this->actual_points.size();
	}
	int get_max_x() {
		int max = -1;
		Point max_point;

		for (int i = 0; i < this->all_num_points; i++) {
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

		for (int i = 0; i < this->all_num_points; i++) {
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

		for (int i = 0; i < this->all_num_points; i++) {
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

		for (int i = 0; i < this->all_num_points; i++) {
			int current_y = this->p_points[i].y;

			if (current_y < max) {
				max_point = this->p_points[i];
			}
		}

		return max_point.y;
	}
	vector<Point> remove_redundant_points() {
		vector<Point> all_points(this->p_points, this->p_points + this->all_num_points);
		vector<Point> unique_points(this->p_points, this->p_points + this->all_num_points);
		unique_points.erase(unique(unique_points.begin(), unique_points.end()), unique_points.end());

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
				this->redundant_points.push_back(all_points[i]);
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
			this->redundant_points.push_back(ignore[i]);
		}

		for (int i = 0; i < unique_points.size(); i++) {
			bool found = false;

			for (int j = 0; j < redundant_points.size(); j++) {
				if (this->redundant_points[j] == unique_points[i]) {
					found = true;

					break;
				}
			}

			if (!found) {
				this->actual_points.push_back(unique_points[i]);
			}
		}

		return redundant_points;
	}
	double get_perimeter() {
		double perimeter = 0.0;

		for (int i = 0; i < this->actual_points.size() - 1; i++) {
			Point current_point = this->actual_points[i];
			Point next_point = this->actual_points[i + 1];

			perimeter += this->euclidean_distance(current_point.x, current_point.y, next_point.x, next_point.y);
		}

		return perimeter;
	}
	bool is_triangle() {
		return this->actual_points.size() == 3;
	}
	bool is_rectangle() {
		if (this->actual_points.size() != 4) return false;

		map<double, int> distances_frequency;

		for (int i = 0; i < this->actual_points.size(); i++) {
			Point current_point = this->actual_points[i];

			for (int j = i + 1; j < this->actual_points.size(); j++) {
				Point next_point = this->actual_points[j];

				distances_frequency[this->euclidean_distance(current_point.x, current_point.x, next_point.x, next_point.y)]++;
			}
		}

		if (distances_frequency.size() != 2) return false;

		for (map<double, int>::iterator it = distances_frequency.begin(); it != distances_frequency.end(); it++) {
			if (it->second != 2) return false;
		}

		return true;
	}
	bool is_trapezoid() {
		if (this->actual_points.size() != 4) return false;
		if (this->is_rectangle()) return false;

		map<double, int> distances_frequency;

		for (int i = 0; i < this->actual_points.size(); i++) {
			Point current_point = this->actual_points[i];

			for (int j = i + 1; j < this->actual_points.size(); j++) {
				Point next_point = this->actual_points[j];

				distances_frequency[this->euclidean_distance(current_point.x, current_point.x, next_point.x, next_point.y)]++;
			}
		}

		if (distances_frequency.size() != 3) return false;

		int count_equal = 0;

		for (map<double, int>::iterator it = distances_frequency.begin(); it != distances_frequency.end(); it++) {
			if (it->second == 2) {
				count_equal++;
			}
		}

		if (count_equal != 2) return false;

		return true;
	}
	bool inside_rectangle(vector<Point> rectangle_points) {
		double min_x, min_y = INT_MAX;
		double max_x, max_y = INT_MIN;

		for (int i = 0; i < rectangle_points.size(); i++) {
			min_x = min(min_x, rectangle_points[i].x);
			min_y = min(min_y, rectangle_points[i].y);
			max_x = min(max_x, rectangle_points[i].x);
			max_y = min(max_y, rectangle_points[i].y);
		}

		return (min_x <= this->get_min_x() && max_x >= this->get_max_x() && min_y <= this->get_min_y() && max_y >= this->get_max_y());
	}
	bool inside_circle(Point center, double radius) {
		for (int i = 0; i < this->actual_points.size(); i++) {
			if (pow(this->actual_points[i].x - center.x, 2) 
				+ pow(this->actual_points[i].y - center.y, 2) >= pow(radius, 2)) {
					return false;
			}
		}

		return true;
	}
	double get_area() {
		//http://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/
		double area = 0.0;

		vector<Point> sorted_points(this->actual_points.begin(), this->actual_points.end());
		sort(sorted_points.begin(), sorted_points.end());

		int last = this->actual_points.size() - 1;

		for (int i = 0; i < this->actual_points.size(); i++) {
			area += (this->actual_points[last].x + this->actual_points[i].x) 
				* (this->actual_points[last].y + this->actual_points[i].y);

			last = i;
		}

		return fabs(area / 2.0);
	}
	bool inside_polygon(Point point) {
		//http://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
		if (point.x < this->get_min_x() || point.x > this->get_max_x() || point.y < this->get_min_y() || point.y > this->get_max_y())
			return false;

		bool inside = false;
		int last = this->actual_points.size() - 1;

		for (int i = 0; i < this->actual_points.size(); i++) {
			bool in_range = (this->actual_points[i].y > point.y) != (this->actual_points[last].y > point.y);

			if (in_range && point.x < (this->actual_points[last].x - this->actual_points[i].x) * (point.y - this->actual_points[i].y) / (this->actual_points[last].y - this->actual_points[i].y) + this->actual_points[i].y) {
				inside = !inside;
			}

			last = i;
		}

		return inside;
	}
	bool is_intersect(Polygon polygon) {
		vector<Point> other_polygon_points = polygon.get_actual_points();

	}
};
///////////////// END OF STRUCTURES ////////////////////

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
			point.x = atof(remove_delim_x.c_str());
			point.y = atof(remove_delim_y.c_str());

			polygon_points.push_back(point);
		}

		Polygon polygon = Polygon();
		Point* points_array = new Point[polygon_points.size()];
		copy(polygon_points.begin(), polygon_points.end(), points_array);

		polygon.set_all_points(points_array, polygon_points.size());
		polygons.push_back(polygon);
	}

	vector<string> operations_list(content_tokens.begin() + 1, content_tokens.end());

	for (int i = 0; i < operations_list.size(); i++) {
		string current_operation = split(operations_list[i], ' ')[0];

		if (current_operation == "Quit") break;

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

void operations_processing() {
	cout << "RESULTS:" << endl << endl;

	for (int i = 0; i < operations.size(); i++) {
		pair<string, int> current_operation = operations[i];
		vector<string> operation_tokens = split(current_operation.first, ' ');
		string operation_name = operation_tokens[0];

		if (operation_tokens.size() == 1) {
			if (operation_name == "Number_Polygons") {
				cout << "Number of input Polygons are: " << polygons.size() << endl;
			}

			if (operation_name == "Total_Number_Points") {
				for (int j = 0; j < polygons.size(); j++) {
					cout << "Polygon #" << i <<  endl << "Number of Points are: " << polygons[j].get_num_points() << endl;
				}
			}

			if (operation_name == "List_Triangles") {
				string output = "";

				for (int j = 0; j < polygons.size(); j++) {
					if (polygons[j].is_triangle()) {
						output += to_string(j + 1) + ",";
					}
				}

				cout << "Polygons that form triangle: " << output.substr(0, output.size() - 1) << endl;
			}

			if (operation_name == "List_Rectangles") {
				string output = "";

				for (int j = 0; j < polygons.size(); j++) {
					if (polygons[j].is_rectangle()) {
						output += to_string(j + 1) + ",";
					}
				}

				cout << "Polygons that form rectangle: " << output.substr(0, output.size() - 1) << endl;
			}

			if (operation_name == "List_Trapezoid") {
				string output = "";

				for (int j = 0; j < polygons.size(); j++) {
					if (polygons[j].is_trapezoid()) {
						output += to_string(j + 1) + ",";
					}
				}

				cout << "Polygons that form trapezoid: " << output.substr(0, output.size() - 1) << endl;
			}
		} else if (operation_tokens.size() == 2) {
			if (operation_name == "Point_Polygons") {
				string point_string = operation_tokens[1];
				vector<string> point_tokens = split(point_string, ',');
				Point point;
				point_tokens[0].erase(std::remove(point_tokens[0].begin(), point_tokens[0].end(), '('), point_tokens[0].end());
				point.x = atof(point_tokens[0].c_str());
				point_tokens[1].erase(std::remove(point_tokens[1].begin(), point_tokens[1].end(), ')'), point_tokens[1].end());
				point.y = atof(point_tokens[1].c_str());

				string output = "";

				for (int k = 0; k < polygons.size(); k++) {
					if (polygons[k].in_polygon(point)) {
						output += to_string(k + 1) + ",";
					}
				}

				cout << "Polygons that have point " << "(" << point.x << "," << point.y << "): " << output.substr(0, output.size() - 1) << endl;
			}

			else if (operation_name == "Polygon_Perimeter") {
				int n = atoi(operation_tokens[1].c_str());

				if (n <= polygons.size() - 1) {
					Polygon polygon = polygons[n - 1];

					cout << "Polygon #" << n << " perimeter: " << polygon.get_perimeter() << endl;
				}
			}

			else if (operation_name == "Polygon_Area") {
				int n = atoi(operation_tokens[1].c_str());

				if (n <= polygons.size() - 1) {
					Polygon polygon = polygons[n - 1];

					cout << "Polygon #" << n << " area: " << polygon.get_area() << endl;
				}
			}

			else if (operation_name == "Polygons_Area_Range") {
				string ranges_data = operation_tokens[1];
				vector<string> ranges_tokens = split(ranges_data, ',');
				double min_area = atof(ranges_tokens[0].c_str());
				double max_area = atof(ranges_tokens[1].c_str());

				string output = "";

				for (int k = 0; k < polygons.size(); k++) {
					double area = polygons[k].get_area();

					if (area <= min_area && area >= max_area) {
						output += to_string(k + 1) + ",";
					}
				}

				cout << "Polygons that have area <= " << min_area << " and area >= " << max_area << ": "
					<< output.substr(0, output.size() - 1) << endl;
			}

			else if (operation_name == "Inside_Rectangle") {
				string rectangle_points_string = operation_tokens[1];
				vector<string> rectangle_points_data = split(rectangle_points_string, '),');
				vector<Point> rectangle_points;

				for (int j = 0; j < rectangle_points_data.size(); j += 2) {
					string remove_delim_x = rectangle_points_data[j];
					remove_delim_x.erase(std::remove(remove_delim_x.begin(), remove_delim_x.end(), '('), remove_delim_x.end());
					remove_delim_x.erase(std::remove(remove_delim_x.begin(), remove_delim_x.end(), ')'), remove_delim_x.end());

					string remove_delim_y = rectangle_points_data[j + 1];
					remove_delim_y.erase(std::remove(remove_delim_y.begin(), remove_delim_y.end(), '('), remove_delim_y.end());
					remove_delim_y.erase(std::remove(remove_delim_y.begin(), remove_delim_y.end(), ')'), remove_delim_y.end());

					Point point;
					point.x = atof(remove_delim_x.c_str());
					point.y = atof(remove_delim_y.c_str());

					rectangle_points.push_back(point);
				}

				string output = "";

				for (int k = 0; k < polygons.size(); k++) {
					if (polygons[k].inside_rectangle(rectangle_points)) {
						output += to_string(k + 1) + ",";
					}
				}

				cout << "Polygons that are inside rectangle " << rectangle_points_string 
					<< ": " << output.substr(0, output.size() - 1) << endl;
			}

			else if (operation_name == "Inside_Circle") {
				string circle_data = operation_tokens[1];
				vector<string> circle_points_data = split(circle_data, '),');
				double radius = atof(circle_points_data[1].c_str());
				string unclean_point = circle_points_data[0];
				Point circle_point;

				unclean_point.erase(std::remove(unclean_point.begin(), unclean_point.end(), '('), unclean_point.end());
				vector<string> point_string = split(unclean_point, ',');

				circle_point.x = atof(point_string[0].c_str());
				circle_point.y = atof(point_string[1].c_str());

				string output = "";

				for (int k = 0; k < polygons.size(); k++) {
					if (polygons[k].inside_circle(circle_point, radius)) {
						output += to_string(k + 1) + ",";
					}
				}

				cout << "Polygons that are inside circle " << circle_data 
					<< ": " << output.substr(0, output.size() - 1) << endl;
			}

			else if (operation_name == "Polygons_Enclosing_Point") {
				string point_data = operation_tokens[1];
				Point point;
				vector<string> point_tokens = split(point_data, ',');

				point_tokens[0].erase(std::remove(point_tokens[0].begin(), point_tokens[0].end(), '('), point_tokens[0].end());
				point_tokens[1].erase(std::remove(point_tokens[1].begin(), point_tokens[1].end(), '('), point_tokens[1].end());

				point.x = atof(point_tokens[0].c_str());
				point.y = atof(point_tokens[1].c_str());

				string output = "";

				for (int k = 0; k < polygons.size(); k++) {
					if (polygons[k].inside_polygon(point)) {
						output += to_string(k + 1) + ",";
					}
				}

				cout << "Polygons that have point " << point_data << " inside: " << output.substr(0, output.size() - 1) << endl;
			}

		} else if (operation_tokens.size() == 3) {
			if (operation_name == "List_Polygons_Points") {
				string condition = operation_tokens[1];
				int n = atoi(operation_tokens[2].c_str());

				if (condition == "More") {
					string output = "";

					for (int k = 0; k < polygons.size(); k++) {
						if (polygons[k].get_actual_points().size() > n) {
							output += to_string(k + 1) + ",";
						}
					}

					cout << "Polygons that have more than " << n << ": " << output.substr(0, output.size() - 1) << endl;
				}

				else if (condition == "Less") {
					string output = "";

					for (int k = 0; k < polygons.size(); k++) {
						if (polygons[k].get_actual_points().size() < n) {
							output += to_string(k + 1) + ",";
						}
					}
					cout << "Polygons that have less than " << n << ": " << output.substr(0, output.size() - 1) << endl;
				} 

				else if (condition == "Equal") {
					string output = "";

					for (int k = 0; k < polygons.size(); k++) {
						if (polygons[k].get_actual_points().size() == n) {
							output += to_string(k + 1) + ",";
						}
					}

					cout << "Polygons that have equal " << n << ": " << output.substr(0, output.size() - 1) << endl;
				}
			}

			if (operation_name == "List_Points_Polygons") {
				string condition = operation_tokens[1];
				int n = atoi(operation_tokens[2].c_str());
				map<Point, int> points_frequency;

				for (int k = 0; k < polygons.size(); k++) {
					vector<Point> points = polygons[k].get_actual_points();

					for (int p = 0; p < points.size(); p++) {
						points_frequency[points[p]]++;
					}
				}

				string output = "";

				if (condition == "More") {
					for (map<Point, int>::iterator it = points_frequency.begin(); it != points_frequency.end(); it++) {
						if (it->second > n) {
							output += "(" + to_string(it->first.x) + "," + to_string(it->first.y) + "),";
						}
					}

					cout << "Points that are in more than " << n << " polygons: " << output << endl;
				}

				else if (condition == "Less") {
					for (map<Point, int>::iterator it = points_frequency.begin(); it != points_frequency.end(); it++) {
						if (it->second < n) {
							output += "(" + to_string(it->first.x) + "," + to_string(it->first.y) + "),";
						}
					}

					cout << "Points that are in less than " << n << " polygons: " << output << endl;
				}

				else if (condition == "Equal") {
					for (map<Point, int>::iterator it = points_frequency.begin(); it != points_frequency.end(); it++) {
						if (it->second == n) {
							output += "(" + to_string(it->first.x) + "," + to_string(it->first.y) + "),";
						}
					}

					cout << "Points that are in equal than " << n << " polygons: " << output << endl;
				}
			}
		}

		cout << endl << endl;

		for (int i = 0; i < polygons.size(); i++) {
			Polygon current_polygon = polygons[i];

			cout << "Polygon #" << i << endl;

			for (int j = 0; j < operations.size(); j++) {
				pair<string, int> current_operation = operations[j];
				vector<string> operation_tokens = split(current_operation.first, ' ');
				string operation_name = operation_tokens[0];

				if (operation_tokens.size() == 2) {
					if (operation_name == "Polygon_Points") {
						int n = atoi(operation_tokens[1].c_str());

						if (n <= polygons.size() - 1) { 
							Polygon polygon = polygons[n - 1];
							vector<Point> points = polygon.get_actual_points();

							string output = "";

							for (int k = 0; k < points.size(); k++) {
								output += "(" + to_string(points[k].x) + "," + to_string(points[k].y) + "),";
							}

							cout << "Points are: " << output.substr(0, output.size() - 1) << endl;
						}
					}
				} else {
					if (operation_name == "Minimum_X") {
						cout << "Minimum X: " << current_polygon.get_min_x() << endl;
					}

					if (operation_name == "Maximum_X") {
						cout << "Maximum X: " << current_polygon.get_max_x() << endl;
					}

					if (operation_name == "Minimum_Y") {
						cout << "Minimum Y: " << current_polygon.get_min_y() << endl;
					}

					if (operation_name == "Maximum_Y") {
						cout << "Maximum Y: " << current_polygon.get_max_y() << endl;
					}

					if (operation_name == "Total_Redundant_Points") {
						cout << "Total_Redundant_Points: " << current_polygon.get_redundant_points().size() << endl;
					}
				}
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