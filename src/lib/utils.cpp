#include <iostream>
#include <cstdint>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector> 
#include <fstream>
#include <algorithm>

#include "utils.h"

#define AUDIT

uint64_t max_size = 64UL;   // Maximum size for the bitstring (correponds to uint64_t max size)


/** 
 * Generate EncryptionParamaters, needed to encrypt / decrypt 
 * 
 * @param poly_mode_degree Polynomial modulus degree size (in bits)
 *
 * return                  EncryptionParameters instance
 * */
EncryptionParameters get_params(size_t poly_mode_degree)
{
    EncryptionParameters params(scheme_type::bfv);
	params.set_poly_modulus_degree(poly_mode_degree);
	params.set_coeff_modulus(CoeffModulus::BFVDefault(poly_mode_degree));
	params.set_plain_modulus(PlainModulus::Batching(poly_mode_degree, 20));

    return params;
}


/**
 * Open a dataset, convert each line into a string and write in a vector  
 *
 * @param path  Dataset path
 *
 * @return      Vector of strings
 * */
vector<string> read_dataset_from_file(string path)
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
 * Convert a dataset of plain bitstrings into uint64_t
 * An exception can be raised (and catch) if the bitstring is not valid and it 
 * causes the impossibilty to convert it into an integer
 *
 * @param dataset_path  Path of the dataset 
 * 
 * @return              Vector on uint64_t
 * */
vector<uint64_t> bitstring_to_long_dataset(string dataset_path)
{
	vector<string> dataset = read_dataset_from_file(dataset_path);
    
    vector<uint64_t> longint_dataset;
	for(string s : dataset){
		try{
			longint_dataset.push_back(stoull(s, 0, 2));
		}
		catch (exception& e){
			printf("An error occurred: the dataset passed is not in a valid format. Only bitstring are accepted\n");
	        return longint_dataset;
		}
	}

	return longint_dataset;
}


/** 
 * Print a line on the screen
 * */
void print_line()
{
    int line_size = 30;
    int i;
    string line = "";
    for(i = 0; i < line_size; i++)
        line += "*--";
    line += "*";
    cout << line << endl;
}


/** 
 * Print a sort of banner that delimits each run of the program. It also prints the parameters for 
 * the current run 
 *
 * @param params PsiParams object that contains information to print 
 * */
void print_start_computation(PsiParams params)
{
    print_line();
    printf("\n");
    string table_line(40, '-');
    table_line = "|" + table_line + "|";
    printf(" Running with the following parameters:\n");
    cout << table_line << endl;
    cout << " Modulus size: " << params.getPolyModDegree() << "\n" << table_line+"\n" << " Sender set size: " << params.getSendNumEntries() << "\n" << table_line+"\n" << " Receiver set size: " << params.getRecvNumEntries() << "\n" << table_line << endl;
    printf("\n\n");
}
