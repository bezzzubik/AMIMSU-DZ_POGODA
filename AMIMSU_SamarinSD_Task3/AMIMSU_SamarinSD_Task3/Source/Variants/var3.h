#pragma once
#include "base_header.h"


void var3_create_intervals_gauss(vector<float>* temps, vector<vector<float>>* temp_intervals)
{
    temp_intervals->resize(10);
    vector<vector<float>> temps_sorted;
    temps_sorted.resize(10);
	
    int cur_decade = 0;
    for (int i = 0; i < temps->size()/10; i++)
    {
        for (int j = 0; j < 10; j++)
            temps_sorted[j].push_back(temps->at(i*10 + j));
    }
    for (int i = 0; i < 10; i++)
        sort(temps_sorted[i].begin(), temps_sorted[i].end());
    for (int i = 0; i < 10; i++)
    {
        string file_name = "../Output/General/var3_sorted_temps_decade_" + to_string(i+1);
        ofstream var3_out_sorted_temp(file_name);
        for (int j = 0; j < static_cast<int>(temps_sorted[i].size()); j++)
            var3_out_sorted_temp << temps_sorted[i][j] << "\n";
        var3_out_sorted_temp.close();
        cout << "\n\nDecade " << i+1 << " :\n";
        get_intervals(&temps_sorted[i],&temp_intervals->at(i));
    }
}

void var3_create_decade_transits(vector<float>* temps, vector<vector<float>>* temp_intervals, vector<vector<vector<float>>>* decade_transits)
{
    //Create Transit Chances Matrixes
    for (int i = 0; i < 9; i++)
        decade_transits->push_back(create_transit_matrix(temps, temp_intervals, i, i+1));
	
    //Transpose Matrixes
    for (int i = 0; i < 9; i++)
    {
        decade_transits->at(i) = transpose_matrix(&decade_transits->at(i));
        out_file_vector_matrix("var3_transposed_transit_matrix_"+to_string(i+1)+"_"+to_string(i+2), &decade_transits->at(i), decade_transits->at(i).size(), decade_transits->at(i)[0].size(), false);
    }


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
    for (int i = 0; i < 9; i++)
        pi1 = mult_matrix_vector(&decade_transits->at(i), &pi1);


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