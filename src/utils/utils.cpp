#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector> 
#include <fstream>
#include <algorithm>

using namespace std;

#define AUDIT
uint64_t max_size = 64UL;

/**
 * Helper function that opens a dataset and converts each value 
 *
 * @param path  Dataset path
 *
 * @return      Vector of strings
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
		return conv_dataset;
	}
	string dataset_line;
	
	while(getline(dataset, dataset_line))
		conv_dataset.push_back(dataset_line.substr(0, min(max_size, dataset_line.length())));
	
	dataset.close();
	return conv_dataset;	
}


/** 
 * Helper function to convert a dataset of plain bitstrings into uint_64
 * An exception can be raised (and catch) if the bitstring is not valid and it 
 * causes the impossibilty to convert it into an integer
 *
 * @param dataset   String dataset, each string is expressed in bits 
 * 
 * @return          Vector on uint64_t
 * */
vector<uint64_t> bitstring_to_long_dataset(vector<string> dataset)
{
	vector<uint64_t> longint_dataset;
	vector<uint64_t> empty_dataset;
	for(string s : dataset){
		try{
			longint_dataset.push_back(stoull(s, 0, 2));
		}
		catch (exception& e){
			printf("An error occurred: the dataset passed is not in a valid format. Only bitstring are accepted\n");
			return empty_dataset;
		}
	} 
	return longint_dataset;
}
