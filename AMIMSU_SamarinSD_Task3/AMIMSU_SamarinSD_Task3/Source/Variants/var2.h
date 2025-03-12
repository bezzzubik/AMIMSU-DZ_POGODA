#include "base_header.h"
void var2_create_intervals_gauss(vector<float>* temps, vector<float>* temp_intervals){
	vector<float> sorted_temps = *temps;
	sort(sorted_temps.begin(), sorted_temps.end());
	ofstream out_sorted_temp_file("../Output/General/var2_sorted_temps.txt");
	for (int i = 0; i < static_cast<int>(sorted_temps.size()); i++)
		out_sorted_temp_file << sorted_temps.at(i) << "\n";
	out_sorted_temp_file.close();
	
	int temps_size = static_cast<int>(sorted_temps.size());
	//mu = sum(X(i) * P(i)), P(i) = P(i+1) = ... P(n) = 1/n => mu = X(i)/n
	float mu = 0;
	for (auto i : sorted_temps)
		mu+=i;
	mu /= static_cast<float>(temps_size);

	float sigma = 0;
	for (auto i : sorted_temps)
		sigma += static_cast<float>(pow(i - mu, 2));
	sigma = sqrt(sigma / (static_cast<float>(temps_size) - 1));
	sigma*=sigma_step;
	float delta = -3.0f * sigma;
	cout << "mu = " << mu
		<< "\nsigma = " << sigma
		<< "\nIntervals :"
		<< "\n-inf ; ";
	std::ios_base::fmtflags cout_flags( cout.flags() );
	int last_interval_index = 0;
	for (int i = 0; i < temps_size; i++)
		if (sorted_temps[i] > mu + delta)
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
	cout.flags(cout_flags);
}