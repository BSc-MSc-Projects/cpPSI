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
#include "dataset_gen.h"


/** Generate both sender and receiver datasets to run the tests 
 *
 * @param n_entries 		Number of bitstrings in each dataset 
 * @param string_length 	Lenght of each bitstring
 * @param n_intersect 		Number of bitstrings that will be in the inresection between the datasets
 * @param recv_path         Path of receiver dataset
 * @param send_path         Path of sender dataset
 *
 * */
vector<string> generate_dataset(int n_entries, int string_length, int n_intersect, string recv_path, string send_path)
{
	vector<string> intersection;	// keeps the strings that will be in the intersection between the two datasets
	intersection = gen_rand_dataset(n_entries, string_length, n_intersect, recv_path, intersection);
	printf("First dataset generated\n");
	gen_rand_dataset(n_entries, string_length, 0, send_path, intersection);

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
			result_file << params_vector[index].getPolyModDegree() << "," 
                << params_vector[index].getStringLength() 
                << "," << params_vector[index].getRecvNumEntries() << "," 
                << test_class_vector[index].getTimeVector().count() << "," 
                << test_class_vector[index].getNoiseBudget() << "\n";
	    }
    }
	else{
		printf("Error while opening output data file\n");
	}
	result_file.close();
}


/** 
 * Generate a vector of PsiParams, where each one is a distinct test case configuration 
 * 
 * return Vector of PsiParams instances
 * */
vector<PsiParams> getTestCases()
{
    vector<PsiParams> params_vector;
	vector<int> n_entries = {4};//,6,8,10};
	vector<size_t> poly_mod_degrees = {8192};//, 16384};
	int string_lengths = 24;
	int num_intersects = 4;

    for(size_t poly_mod : poly_mod_degrees){
        for(int entry : n_entries)
            params_vector.push_back(PsiParams(entry, entry, string_lengths, num_intersects, poly_mod));
    }
    return params_vector;
}


int main (int argc, char *argv[])
{
	if(argc < 3){
        printf("Usage:\n1) prog\n2) path for recevier dataset\n3) Path for sender dataset\n");
        return 0;
    }

    string recv_path = argv[1];
    string send_path = argv[2];

    cout << "Paths:" << recv_path << "," << send_path << endl;

    vector<ComputationResult> test_class_vector;
	vector<PsiParams> params_vector = getTestCases(); 
	vector<string> intersection;

    // Run a test for each configuration
    for(PsiParams param : params_vector){
	
        // Setup the test class
        print_start_computation(param);
		intersection = generate_dataset(param.getSendNumEntries(), param.getStringLength(), 
                param.getIntersectLength(), recv_path, send_path);
			
		// Setup the parameters for the homomorphic scheme
    	EncryptionParameters params = get_params(param.getPolyModDegree());
		
        // Setup receiver dataset
        Dataset recv_dataset;
        recv_dataset.setLongDataset(string_to_int_dataset(recv_path));
        recv_dataset.setStringDataset(read_dataset_from_file(recv_path));
        recv_dataset.setSigmaLength(recv_dataset.getStringDataset()[0].length());

        Receiver recv = setup_pk_sk(params);
        recv.setDataset(recv_dataset);

        // Convert sender dataset into uint64_t
        vector<uint64_t> sender_dataset = string_to_int_dataset(send_path);

		// Set up the high res clock
		chrono::high_resolution_clock::time_point before, after;
		before = chrono::high_resolution_clock::now();
 
        // The full scheme
		Ciphertext recv_encr_data = crypt_dataset(recv, param.getPolyModDegree());
		Ciphertext send_encr_result = homomorphic_computation(recv_encr_data, param.getPolyModDegree(),
                sender_dataset, recv.getRelinKeys());
	    ComputationResult result = decrypt_and_intersect(param.getPolyModDegree(), send_encr_result, recv);
			
        // Acquire timing
        after = chrono::high_resolution_clock::now();

		result.setTimeVector(chrono::duration_cast<chrono::duration<double>>(after-before));
		test_class_vector.push_back(result);
		
        // Test assetion 
		if(check_result(intersection, result.getIntersection()) == 0)
			cout << "\033[1;32mTest success \033[0m\n";
		else
			cout << "\033[1;31mTest failed \033[0m\n";
		}
        printf("\n");

	write_result(test_class_vector, params_vector);
	return 0;
}
