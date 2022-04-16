#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector> 
#include <fstream> 

using namespace std;

#define AUDIT


/**
 * Helper function that open the dataset of the dataset and convert each value 
 *
 * @param path Dataset path
 *
 * @return vector of strings
 * */
vector<string> convert_dataset(string path)
{
	vector<string> conv_dataset;

	// Try to open the file
	ifstream dataset;
	dataset.open(path, ios::in);
	if(!dataset.is_open()){
#ifdef  AUDIT
		cout << "cannot open file with path: " << path << endl;
#endif 
		return conv_dataset;														// return an empty dataset
	}
	string dataset_line;
	
	while(getline(dataset, dataset_line))
		conv_dataset.push_back(dataset_line.substr(0, dataset_line.length()));		// cut the final '\n' character
		
	dataset.close();																// close the resource
	return conv_dataset;	
}
