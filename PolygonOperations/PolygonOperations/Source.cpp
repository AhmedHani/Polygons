#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#define MAX_POINTS 100
#define MAX_POLYGONS 100
#define MAX_OPERATIONS 100

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

///////////////// STRUCTURES ////////////////////
struct Point {
	float x;
	float y;
};

class Polygon {
public:
	Point* p_points;
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


Polygon* polygons;
pair<string, int>* operations;

void parse_file(string file_name) {
	std::ifstream ifs(file_name);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	vector<string> file_tokens = split(content, ';');

	polygons = new Polygon[file_tokens.size()];
	
	for (int i = 0; i < file_tokens.size() - 1; i++) {
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
			point.x = atof(remove_delim_x.c_str());
			point.y = atof(remove_delim_y.c_str());

			polygon_points[index++] = point;
		}

		Polygon polygon;
		polygon.p_points = polygon_points;

		polygons[i] = polygon;
	}

	string operations_line = file_tokens[file_tokens.size() - 1];
	vector<string> operations_list = split(operations_line, ' ');
	operations = new pair<string, int>[operations_list.size()];

	for (int i = 0; i < operations_list.size(); i++) {
		string current_operation = operations_list[i];
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

		operations[i] = make_pair(existed_operation, operations_level);
	}
}

void operations_processing() {
	cout << "\n\n OPERATIONS PROCESSING \n" << endl;

	for (int i = 0; i < (sizeof(operations)/sizeof(*operations)); i++) {

	}
}


int main(int argc, char** argv) {
	parse_file("input.txt");



	return 0;

}