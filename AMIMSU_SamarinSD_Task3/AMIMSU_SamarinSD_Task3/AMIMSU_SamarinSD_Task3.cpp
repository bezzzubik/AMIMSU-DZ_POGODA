#include <algorithm>
//#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <vector>
#include <bitset>
#include <iomanip>
#include <map>
#include <string>

#include "Source/Variants/var1.h"
#include "Source/Variants/var2.h"
#include "Source/Variants/var3.h"

using namespace std;

void fill_in_data(vector<float>* temps)
{
	ifstream in_file("Source/in_data.txt");
	if (!in_file.is_open())
	{
		cerr << "Error: Files for input or output could not be opened." << endl;
		exit(2);
	}
	while (!in_file.eof())
	{
		float inp;
		in_file >> inp;
		in_file >> inp;
		temps->push_back(inp);
	}
	in_file.close();	
}

int main(int argc, char* argv[])
{
	vector<float> temps;
	vector<vector<float>> temp_intervals;
	vector<vector<vector<float>>> decade_transits;
    fill_in_data(&temps);

	var3_create_intervals_gauss(&temps, &temp_intervals);
	var3_create_decade_transits(&temps, &temp_intervals, &decade_transits);

	//var1_create_intervals_gauss(&temps, &temp_intervals);
	//var1_create_decade_transits(&temps, &temp_intervals, &decade_transits);
	
    return 0;
}
