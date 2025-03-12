#pragma once
#include <iostream>
using namespace std;

constexpr float sigma_step = 1.0f;
constexpr float p1 = -8.33f;
//float may1_min;
//float may1_max;
float sigma;

void get_intervals(vector<float>* sorted_temps, vector<float>* temp_intervals)
{
    ios_base::fmtflags default_cout_flags( cout.flags() );
    int temps_size = static_cast<int>(sorted_temps->size());
    //mu = sum(X(i) * P(i)), P(i) = P(i+1) = ... P(n) = 1/n => mu = X(i)/n
    float mu = 0;
    for (auto i : *sorted_temps)
        mu+=i;
    mu /= static_cast<float>(temps_size);

    sigma = 0;
    for (auto i : *sorted_temps)
        sigma += static_cast<float>(pow(i - mu, 2));
    sigma = sqrt(sigma / (static_cast<float>(temps_size) - 1));
	sigma*=sigma_step;
    float delta = -3.0f * sigma;
    cout << "mu = " << mu
        << "\nsigma = " << sigma
        << "\nIntervals :"
        << "\n-inf ; ";
    int last_interval_index = 0;
    for (int i = 0; i < temps_size; i++)
        if (sorted_temps->at(i) > mu + delta)
        {
            if (i == 0)
            {
                i = -1;
                delta += sigma;
                continue;
            }
            temp_intervals->push_back(mu+delta);
            cout << fixed << setprecision(2) << mu+delta
            << "\nFrom : [";
            if (last_interval_index == 0)
                cout << "-inf";
            else
            {
                cout << "mu";
                if (fabs((delta-sigma)/sigma) > 1e-5)
                    cout << "+(" << fixed << setprecision(0) << (delta-sigma)/sigma << ")s" << std::setprecision(2);
            }
            cout << " ; mu";
            if (fabs(delta/sigma) > 1e-5)
                cout << "+(" << fixed << setprecision(0) << delta/sigma << ")s" << std::setprecision(2);
            cout << "]\tContains " << static_cast<float>(i-last_interval_index)/static_cast<float>(temps_size)*100 << "%\n";
            cout << "\n" << mu+delta << " ; ";
            last_interval_index = i;
            delta += sigma;
        }
    cout << "+inf\nFrom : [mu";
    cout << "+(" << fixed << setprecision(0) << (delta-sigma)/sigma << ")s" << std::setprecision(2);
    cout << " ; +inf]\tContains " << static_cast<float>(temps_size-last_interval_index)/static_cast<float>(temps_size)*100<<"%";
    cout.flags(default_cout_flags);
}

vector<vector<float>> transpose_matrix(vector<vector<float>>* base_matrix)
{
    int size1 = static_cast<int>(base_matrix->size());
    int size2 = static_cast<int>(base_matrix->at(0).size());
    vector<vector<float>> transposed_matrix;
    transposed_matrix.resize(size2, vector<float>(size1));
    for (int i = 0; i < size1; i++)
        for (int j = 0; j < size2; j++)
            transposed_matrix[j][i] = base_matrix->at(i)[j];
    return transposed_matrix;
}

vector<float> mult_matrix_vector(vector<vector<float>>* matrix, vector<float>* v)
{
    int size1 = static_cast<int>(matrix->size());
    int size2 = static_cast<int>(matrix->at(0).size());
    int sizev = static_cast<int>(v->size());
    vector<float> res;
    res.resize(size1);

    for (int i = 0; i < size1; i++)
    {
        res[i] = 0;
        for (int j = 0; j < size2; j++)
            res[i] += matrix->at(i)[j] * v->at(j);
    }
    
    return res;
}

void show_matrix(vector<vector<float>>* matrix)
{
    int size1 = static_cast<int>(matrix->size());
    int size2 = static_cast<int>(matrix->at(0).size());
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
            cout << matrix->at(i)[j] << "\t";
        cout << "\n";
    }
}

vector<vector<float>> create_transit_matrix(vector<float>* temps, vector<vector<float>>* temp_intervals, int decade1, int decade2)
{
    vector<vector<float>> transit_matrix;
    transit_matrix.resize(temp_intervals->at(decade1).size()+1, vector<float>(temp_intervals->at(decade2).size()+1));
	
    const int size_t = static_cast<int>(temps->size());
    const int size_d1 = static_cast<int>(temp_intervals->at(decade1).size());
    const int size_d2 = static_cast<int>(temp_intervals->at(decade2).size());
    int row = -1;
	
    for (int i = 0; i < size_t /10; i++)
    {
        row = size_d1;
        for (int j = 0; j < size_d1; j++)
            if (temps->at(decade1 + i*10) < temp_intervals->at(decade1)[j])
            {
                row = j;
                break;
            }
        for (int j = 0; j < size_d2; j++)
            if (temps->at(decade2 + i*10) < temp_intervals->at(decade2)[j])
            {
                transit_matrix[row][j] += 1;
                break;
            }
            else if (j == size_d2 - 1)
                transit_matrix[row][size_d2] += 1;
    }
	
    float row_sum;
    for (int i = 0; i < size_d1+1; i++)
    {
        row_sum = 0.0f;
        for (int j = 0; j < size_d2+1; j++)
            row_sum += transit_matrix[i][j];
        if (row_sum == 0.0f)
            cerr << "Wtf, row_sum is 0" << endl;
        for (int j = 0; j < size_d2+1; j++)
            transit_matrix[i][j] /= row_sum;
    }
    return transit_matrix;
}

void out_file_vector_matrix(string file_name, vector<vector<float>>* matrix, size_t size1, size_t size2, bool key_console_output)
{
    string file_root = "../Output/General/";
    ofstream file_stream(file_root+file_name+".txt");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
            file_stream << fixed << setprecision(2) << matrix->at(i)[j] << "\t";
        file_stream << "\n";
    }
    file_stream.close();

    if (key_console_output)
        for (int i = 0; i < size1; i++)
        {
            for (int j = 0; j < size2; j++)
                cout << matrix->at(i)[j] << "\t";
            cout << "\n";
        }
}