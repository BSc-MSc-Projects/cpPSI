#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector> 
#include <fstream>


using namespace std;


/** Generate random codes and write them into the output dataset file 
 *
 * @param n_entries 		Number of bitstrings in each dataset 
 * @param string_length 	Lenght of each bitstring
 * @param n_intersect 		Number of bitstrings that will be in the intersection between the datasets
 * @param path				Path for the file
 * @param prev_intersect	vector containing the eventual strings that will belong to the intersection
 * 
 * @return 					The strings that will belong to the intersection
 * */
vector<string> gen_rand_dataset(int n_entries, int string_length, int n_intersect, string path, 
        vector<string> prev_intersect)
{
    vector<string> intersection;
	string bitstring = "";
	ofstream ds_steam(path);
	
	if(ds_steam.is_open()){
		for(int i = 0; i < n_entries - prev_intersect.size(); i++){
			for(int j = 0; j < string_length; j++)
				bitstring += to_string(rand()%2);
			
			bitstring += '\n';
			if(i < n_intersect)
				intersection.push_back(bitstring);
			ds_steam << bitstring;
			bitstring = "";
		}
		
		for(int i = 0; i < prev_intersect.size(); i++)
			ds_steam << prev_intersect[i];
		
		ds_steam.close();
	}
	else
        cout << "Cannot open file " << path << endl;
	
	return intersection;
}
