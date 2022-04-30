/** Application tests */


#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <filesystem>

#include "../lib/sender.h"
#include "../lib/receiver.h"


string recv_path = "src/dataset/receiver.csv";
string send_path = "src/dataset/sender.csv";


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
 * @param test_class    Vector containing test results
 * @param params_vector Vector containing parameters for each test
 * */
void write_result(vector<ComputationResult> test_class_vector, vector<PsiParams> params_vector)
{
	ofstream result_file("src/test/result.csv", ios::out | ios::app);	
	if(result_file.is_open()){
		filesystem::path p{"src/test/result.csv"};
		if(filesystem::file_size(p) == 0)
			result_file << "Modulus length,Bitstring size,Dataset size,Computation Time,Remaining noise\n";	
		
        for(size_t index = 0; index < test_class_vector.size(); index++){
			result_file << params_vector[index].getPolyModDegree() << "," << params_vector[index].getStringLength() 
                        << "," << params_vector[index].getRecvNumEntries() << "," << 
                        test_class_vector[index].getTimeVector().count() << "," 
                        << test_class_vector[index].getNoiseBudget() << "\n";
	    }
    }
	else{
		printf("Error while opening output data file\n");
	}
	result_file.close();
}


int main (int argc, char *argv[])
{
	vector<ComputationResult> test_class_vector;
	vector<PsiParams> params_vector;
	vector<int> n_entries = {4,6,8,10};
	int string_lengths = 4;
	int num_intersects = 4;
	vector<size_t> poly_mod_degrees = {8192, 16384};
	vector<string> intersection;		                                            // known intersection 

	// Tests consider a different size of the datasets and different poly_mod_degrees values 
	for(size_t mod_degree : poly_mod_degrees){
        for(int entry : n_entries){
			// Setup the test class
			PsiParams psi_test(entry, entry, string_lengths, num_intersects, mod_degree);
            params_vector.push_back(psi_test);
            
            print_start_computation(psi_test);
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

			// Set up the high res clock
			chrono::high_resolution_clock::time_point before, after;
			before = chrono::high_resolution_clock::now();
            
            // The full scheme
			Ciphertext recv_encr_data = crypt_dataset(recv, params);
			Ciphertext send_encr_result = homomorphic_computation(recv_encr_data, params, convert_dataset(send_path), 
                    recv.getRelinKeys());
	        ComputationResult result = decrypt_and_intersect(params, send_encr_result, recv);
			
            // Acquire timing
            after = chrono::high_resolution_clock::now();

			result.setTimeVector(chrono::duration_cast<chrono::duration<double>>(after-before));
			test_class_vector.push_back(result);
			
			if(check_result(intersection, result.getIntersection()) == 0)
				cout << "\033[1;32mTest success \033[0m\n";
			else
				cout << "\033[1;31mTest failed \033[0m\n";
		}
        printf("\n");
	}
	write_result(test_class_vector, params_vector);
	return 0;
}
