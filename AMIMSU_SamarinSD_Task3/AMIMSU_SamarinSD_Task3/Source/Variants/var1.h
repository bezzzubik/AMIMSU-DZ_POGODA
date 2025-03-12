#pragma once
#include "base_header.h"

void var1_create_intervals_gauss(vector<float>* temps, vector<vector<float>>* temp_intervals)
{
    temp_intervals->resize(10);
    vector<float> feb1_sorted_temps;
    vector<float> may1_sorted_temps;
    int cur_decade = 0;
    for (int i = 0; i < temps->size()/10; i++)
    {
        feb1_sorted_temps.push_back(temps->at(i*10));
        may1_sorted_temps.push_back(temps->at(i*10+9));
    }
    sort(feb1_sorted_temps.begin(), feb1_sorted_temps.end());
    sort(may1_sorted_temps.begin(), may1_sorted_temps.end());
    ofstream out_sorted_temp_feb1_file("../Output/General/var1_sorted_temps_feb1.txt");
    for (int i = 0; i < static_cast<int>(feb1_sorted_temps.size()); i++)
        out_sorted_temp_feb1_file << feb1_sorted_temps.at(i) << "\n";
    out_sorted_temp_feb1_file.close();
    ofstream out_sorted_temp_may1_file("../Output/General/var1_sorted_temps_may1.txt");
    for (int i = 0; i < static_cast<int>(may1_sorted_temps.size()); i++)
        out_sorted_temp_may1_file << may1_sorted_temps.at(i) << "\n";
    out_sorted_temp_may1_file.close();
    cout << "FEB1 : \n";
    get_intervals(&feb1_sorted_temps, &temp_intervals->at(0));
    cout << "\n\nMAY1 : \n";
    get_intervals(&may1_sorted_temps, &temp_intervals->at(9));

}

void var1_create_decade_transits(vector<float>* temps, vector<vector<float>>* temp_intervals, vector<vector<vector<float>>>* decade_transits)
{
    //Create Transit Chances Matrixes
    decade_transits->push_back(create_transit_matrix(temps, temp_intervals, 0, 9));
	out_file_vector_matrix("var1_transit_matrix_1_10", &decade_transits->at(0), decade_transits->at(0).size(), decade_transits->at(0)[0].size(), false);
    //Transpose Matrixes
    decade_transits->at(0) = transpose_matrix(&decade_transits->at(0));
    out_file_vector_matrix("var1_transposed_transit_matrix_1_10", &decade_transits->at(0), decade_transits->at(0).size(), decade_transits->at(0)[0].size(), false);


    vector<float> pi1;
    pi1.resize(temp_intervals->at(0).size()+1);
    for (int i = 0; i < static_cast<int>(temp_intervals->at(0).size()); i++)
        if (p1 < temp_intervals->at(0)[i])
        {
            pi1[i] = 1;
            break;
        }
        else if (i == static_cast<int>(temp_intervals->at(0).size())-1)
            pi1[i+1] = 1;
    
    pi1 = mult_matrix_vector(&decade_transits->at(0), &pi1);


    //Outputs
    cout << "\n\nResult vector :\n";
    for (int i = 0; i < static_cast<int>(pi1.size()); i++)
        cout << pi1[i] << "\n";
	
    cout << "\n\nIntervals\t\t:\tChance to end up\n";
    vector<float> result_interval = temp_intervals->at(9);
    result_interval.insert(result_interval.begin(), result_interval[0]-sigma);
    result_interval.insert(result_interval.end(), result_interval[result_interval.size()-1]+sigma);
	
    for (int i = 0; i < static_cast<int>(result_interval.size())-1; i++)
        cout << "[" << result_interval[i] << " ; " << result_interval[i+1] << "]\t\t:\t" << pi1[i]*100 << "%" << "\n";
	
    float mu = 0.0f;
    for (int i = 0; i < static_cast<int>(result_interval.size())-1; i++)
        mu+= pi1[i] * (result_interval[i] + result_interval[i+1])/2;
    cout << "Result mu is : " << mu << "\n";
}