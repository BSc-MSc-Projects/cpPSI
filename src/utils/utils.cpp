#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector> 
#include <iostream> 
#include <fstream> 

using namespace std;

#define AUDIT

/* Convert a bitstring into an hexadecimal string */
string bit_to_hex(string bitstring)
{
	const string hex_alphab = "0123456789ABCDEF";
	string conv_result = "";
	
	// Eventually pad the string if the lenght is not a multiple of 4
	int r = bitstring.length()%4;
	if(r%2 != 0)
		r = (r+2)%4;
	int bits_to_add = r;
	while(r--)
		bitstring.insert(0, "0");

	// Actual conversion into hexadecimal
	for(size_t index = 0; index <= bitstring.length()-4; index+=4)
		conv_result += hex_alphab[stoi(bitstring.substr(index, 4), 0, 2)];
	
	return conv_result;
}


/**
 * Helper function that open the dataset of the dataset and convert each value according to the parameter conv_type. In this case,
 * the parameter can have 2 values:
 * 	- 0: convert the bitstring into hexadecimal string (receiver)
 * 	- 1: convert the bitstring into integer value, again rappresented as a string (sender)
 *
 * @param path Dataset path
 * @param conv_type The type of conversion to make
 *
 * @return vector of strings
 * */
vector<string> convert_dataset(string path, int conv_type)
{
	vector<string> conv_dataset;
	
	// Try to open the file
	ifstream dataset;
	dataset.open(path, ios::in); //| ios::binary);
	if(!dataset.is_open()){
#ifdef  AUDIT
		cout << "cannot open file with path: " << path << endl;
#endif 
		return conv_dataset;												// return an empty dataset
	}
	string dataset_line;
	
	while(getline(dataset, dataset_line)){
		dataset_line = dataset_line.substr(0, dataset_line.length());		// cut the final '\n' character
		//if (!conv_type)
		//	conv_dataset.push_back(bit_to_hex(dataset_line));
		//else
			conv_dataset.push_back(to_string(stoi(dataset_line, 0, 2)));
	}
	
	dataset.close();														// close the resource
	return conv_dataset;	
}
