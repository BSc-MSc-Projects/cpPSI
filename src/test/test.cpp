/** Application tests */


#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <ratio>
#include <filesystem>

#include "../lib/sender.h"
#include "../lib/receiver.h"


string recv_path = "src/dataset/receiver.csv";
string send_path = "src/dataset/sender.csv";


/** Class used to keep the parameters that will be used in the test */
class PsiTestClass : public ComputationResult
{
public:
	PsiTestClass(int num_entries, int bits_length, int intersect_length, size_t poly_mod_degree){
		this->num_entries = num_entries;
		this->bits_length = bits_length;
		this->intersect_length = intersect_length;
		this->poly_mod_degree = poly_mod_degree;
	}
	
	void setTimeVector(chrono::duration<double> time_diff) {
		this->time_diff = time_diff;
	}

	int getNumEntries(){ return this->num_entries; }
	int getStringLength(){ return this->bits_length; }
	int getIntersectLength(){ return this->intersect_length; }
	int getPolyModDegree(){ return this->poly_mod_degree; }
	chrono::duration<double> getTimeVector() { return this->time_diff; }

private:
	// Fields used to construct the datasets 
	int num_entries;
	int bits_length;
	int intersect_length;

	// Psi scheme values
	size_t poly_mod_degree;

	// For time performance
	chrono::duration<double> time_diff;
};


/** Generate both sender and receiver datasets to run the tests 
 *
 * @param n_entries 		Number of bitstrings in each dataset 
 * @param string_length 	Lenght of each bitstring
 * @param n_intersect 		Number of bitstrings that will be in the inresection between the datasets
 * 
 * */
vector<string> generate_dataset(int n_entries, int string_length, int n_intersect)
{
	vector<string> intersection;	// keeps the strings that will be in the intersection between the two datasets
	intersection = write_on_file(n_entries, string_length, n_intersect, recv_path, intersection);
	printf("First dataset generated\n");
	write_on_file(n_entries, string_length, 0, send_path, intersection);

	return intersection;
}


/** 
 * Compare the result obtained with the expected vector, to check if the the test passed 
 *
 * @param   expected Vector of expected strings 
 * @param   actual Vector of the actual result 
 *
 * @return  0 in case of success, -1 in case of failure 
 * */
int check_result(vector<string> expected, vector<string> actual)
{
	if (actual.size() == 0)		// the intersection is null
		return -1;
	
	int result = -1;
	bool found = false;
	for(string s_e : expected){
	    for (string s_a : actual){
			if(s_e.substr(0, s_e.length()-1).compare(s_a) == 0){
				found = true;
				break;
			}
		}
		if (!found)
			return -1;
		found = false;
	}
	return 0;
}


/** 
 * Write in a .csv file the result of the computation for each test. The file can be used to derive tables and graphs 
 * 
 * @param test_class Vector containing test results
 * 
 * */
void write_result(vector<PsiTestClass> test_class_vector)
{
	ofstream result_file("src/test/result.csv", ios::out | ios::app);	
	if(result_file.is_open()){
		filesystem::path p{"src/test/result.csv"};
		if(filesystem::file_size(p) == 0)
			result_file << "Modulus length,Bitstring size,Dataset size,Computation Time,Remaining noise\n";	
		
		for(PsiTestClass test_class : test_class_vector)
			result_file << test_class.getPolyModDegree() << "," << test_class.getStringLength() 
                        << "," << test_class.getNumEntries() << "," << test_class.getTimeVector().count() 
                        << "," << test_class.getNoiseBudget() << "\n";
	}
	else{
		printf("Error while opening output data file\n");
	}
	result_file.close();
}


int main (int argc, char *argv[])
{
	vector<PsiTestClass> test_class_vector;
	vector<int> n_entries = {4,6,8,10};
	int string_lengths = 4;
	int num_intersects = 4;
	vector<size_t> poly_mod_degrees = {8192, 16384};
	ComputationResult result;				                                        // result of the computation
	vector<string> intersection;		                                            // known intersection 

	// Tests consider a different size of the datasets and different poly_mod_degrees values 
	for(size_t mod_degree : poly_mod_degrees){
        for(int entry : n_entries){
			intersection = generate_dataset(entry, string_lengths, num_intersects); // generate the datasets
			
			// Setup the parameters for the homomorphic scheme
    		EncryptionParameters params(scheme_type::bfv);
			params.set_poly_modulus_degree(mod_degree);
			params.set_coeff_modulus(CoeffModulus::BFVDefault(mod_degree));
			params.set_plain_modulus(PlainModulus::Batching(mod_degree, 20));	
		
			Receiver recv;
			recv = setup_pk_sk(params);
			vector<string> recv_dataset;
			recv.setRecvDataset(convert_dataset(recv_path));

			// Setup the test class
			PsiTestClass psi_test(entry, string_lengths, num_intersects, mod_degree);
			
			// Set up the high res clock
			chrono::high_resolution_clock::time_point before, after;
			before = chrono::high_resolution_clock::now();
            
            // The full scheme
			Ciphertext recv_encr_data = crypt_dataset(recv, params);
			Ciphertext send_encr_result = homomorphic_computation(recv_encr_data, params, convert_dataset(send_path), 
                    recv.getRelinKeys());
			result = decrypt_and_intersect(params, send_encr_result, recv);
			
            // Acquire timing
            after = chrono::high_resolution_clock::now();
			psi_test.setTimeVector(chrono::duration_cast<chrono::duration<double>>(after-before));
			psi_test.setIntersection(result.getIntersection());
			psi_test.setNoiseBudget(result.getNoiseBudget());
			
			test_class_vector.push_back(psi_test);
			
			if(check_result(intersection, result.getIntersection()) == 0)
				cout << "\033[1;32mTest success \033[0m\n";
			else
				cout << "\033[1;31mTest failed \033[0m\n";
		}
	}
	write_result(test_class_vector);
	return 0;
}
