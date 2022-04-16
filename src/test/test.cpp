/** Application tests */


#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>

#include "../sender/sender.h"
#include "../receiver/receiver.h"


string recv_path = "src/dataset/receiver.csv";
string send_path = "src/dataset/sender.csv";


/* Class used to keep the parameters that will be used in the test */
class PsiTestClass
{
public:
	PsiTestClass(int num_entries, int bits_length, int intersect_length, size_t poly_mod_degree){
		this->num_entries = num_entries;
		this->bits_length = bits_length;
		this->intersect_length = intersect_length;
		this->poly_mod_degree = poly_mod_degree;
	}

	int getNumEntries(){ return this->num_entries; }
	int getStringLength(){ return this->bits_length; }
	int getIntersectLength(){ return this->intersect_length; }
	int getPolyModDegree(){ return this->getPolyModDegree(); }

private:
	// Fields used to construct the datasets 
	int num_entries;
	int bits_length;
	int intersect_length;

	// Psi scheme values
	size_t poly_mod_degree;
};


/** Write bitstrings in a given dataset 
 *
 * @param n_entries 		Number of bitstrings in each dataset 
 * @param string_length 	Lenght of each bitstring
 * @param n_intersect 		Number of bitstrings that will be in the intersection between the datasets
 * @param path				Path for the file
 * @param prev_intersect	vector containing the eventual strings that will belong to the intersection
 * 
 * @return 					The strings that will belong to the intersection
 * */
vector<string> write_on_file(int n_entries, int string_length, int n_intersect, string path, vector<string> prev_intersect){
	vector<string> intersection;
	string bitstring = "";
	ofstream ds_steam(path);
	
	if(ds_steam.is_open()){
		for(int i = 0; i < n_entries; i++){
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
		printf("Cannot open file\n");
	
	return intersection;
}


/** Generate both sender and receiver datasets to run the tests 
 *
 * @param n_entries 		Number of bitstrings in each dataset 
 * @param string_length 	Lenght of each bitstring
 * @param n_intersect 		Number of bitstrings that will be in the inresection between the datasets
 * 
 * */
void generate_dataset(int n_entries, int string_length, int n_intersect)
{
	vector<string> intersection;	// keeps the strings that will be in the intersection between the two datasets
	intersection = write_on_file(n_entries, string_length, n_intersect, recv_path, intersection);
	printf("First dataset generated\n");
	write_on_file(n_entries, string_length, 0, send_path, intersection);
}


int main (int argc, char *argv[])
{
	vector<PsiTestClass> test_class_vector;
	vector<int> n_entries = {4,6,8,10};
	int string_lengths = 10;
	int num_intersects = 4;
	vector<size_t> poly_mod_degrees = {4096, 8192, 16384};
	int result;

	// Tests consider a different size of the datasets and different poly_mod_degrees values 
	for(int entry : n_entries){
		for(size_t mod_degree : poly_mod_degrees){
			generate_dataset(entry, string_lengths, num_intersects);
			
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
			test_class_vector.push_back(psi_test);

			Ciphertext recv_encr_data = crypt_dataset(recv, params);
			Ciphertext send_encr_result = homomorphic_computation(recv_encr_data, params, convert_dataset(send_path));
				
			result = decrypt_and_intersect(params, send_encr_result, recv);
			if(result)
				cout << "\033[1;32mTest success \033[0m\n";
			else
				cout << "\033[1;31mTest failed \033[0m\n";
		}
	}
	return 0;
}
