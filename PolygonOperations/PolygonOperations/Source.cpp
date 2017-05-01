#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#define MAX_POINTS 100
#define MAX_POLYGONS 100

using namespace std;

///////////////// AUX FUNCTIONS ////////////////
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
///////////////// END OF FUNCTIONS //////////////

struct Point {
	int x;
	int y;
};

struct Polygon {
	Point* p_points;
};

Polygon* polygons;

void read_data(string file_name) {
	std::ifstream ifs(file_name);
	std::string content((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));

	vector<string> file_tokens = split(content, ';');

	polygons = new Polygon[file_tokens.size()];
	
	for (int i = 0; i < file_tokens.size(); i++) {
		string current_polygon_data = file_tokens[i];
		vector<string> polygon_points_data = split(current_polygon_data, '),');
		Point* polygon_points = new Point[polygon_points_data.size() / 2];
		int index = 0;
		
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

			polygon_points[index++] = point;
		}

		Polygon polygon;
		polygon.p_points = polygon_points;

		polygons[i] = polygon;
	}
}



int main(int argc, char** argv) {
	read_data("input.txt");



	return 0;

}