/* Sender code of the PSI scheme: it receives an array of ciphertext (c_1, ... , c_n) form the receiver and has to do the following 
 * homomorphic computation: given that it has a dataset N of size n', the intersection computed is: 
 * 
 * 				d_i = r_i * [(c_i - s_1)*(c_i - s_2)*...*(c_i - s_n')]
 * 
 * (so c_i can be seen as the x in the polynom ?? Yes because it is the ecnrypted point)
 * computed for each c_i in the array that was sent by the receiver. Now, the (d_1,...,d_n') vector is sent back to the receiver that can
 * compute the final intersection between the two datasets
 * */

#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <string> 
#include <vector>
#include <random>
#include <limits>
#include "seal/seal.h"
#include "../utils/utils.h"

using namespace std;
using namespace seal;


/* The second step of the PSI scheme: homomorphically subtract each value of the receiver's dataset from 
 * each of the sender's one, and finally multiply for a random value 
 * */
Ciphertext homomorphic_computation(Ciphertext recv_ct_array, EncryptionParameters params, vector<string> sender_dataset)
{
	SEALContext send_context(params);	// this class checks the validity of the parameters set
	
	/* Used to evalutate each single ciphertext value sent by the recevier */
	Evaluator send_evaluator(send_context);	
	KeyGenerator send_keygen(send_context);
	
	/* Here we evaluate the polynomial expressed at the top of this file. It is the PSI scheme polynomial, that has to be computed 
	 * for each element of the received dataset 
	 * */
	BatchEncoder encoder(send_context);
	size_t index;
	size_t slot_count = encoder.slot_count();
	size_t row_size = slot_count/2;

	// TODO: is this ok? Should it be a truly random num gen?	
	vector<uint64_t> rand_val_matrix(slot_count, 0ULL);
	random_device rd;     //Get a random seed from the OS entropy device, or whatever
  	mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                             //and seed it with entropy.
	//Define the distribution, by default it goes from 0 to MAX(unsigned long long)
  		//or what have you.
  	uniform_int_distribution<unsigned long long> distr;
	
	for(index = 0; index < sender_dataset.size(); index++)
		rand_val_matrix[index] = distr(eng);
	
	Plaintext rand_plain;
	//for(index = 0; index < recv_ct_array.size(); index++)
	//	rand_val_matrix[index] = 25;
	encoder.encode(rand_val_matrix, rand_plain);

	//Plaintext rand_val("25");	// this must become a random value
	Ciphertext d_i;				// the final result
	//RelinKeys send_relin_keys;
	//send_keygen.create_relin_keys(send_relin_keys);
		
	// Compute the first product
	vector<uint64_t> first_val_matrix(slot_count, 0ULL);
	Plaintext first_plain;
	for(index = 0; index < sender_dataset.size(); index++)
		first_val_matrix[index] = stoull(sender_dataset[0]);
	encoder.encode(first_val_matrix, first_plain);
		
	send_evaluator.sub_plain(recv_ct_array, first_plain, d_i);	// homomorphic computation of c_i - s_j
	
	/* For each value of the sender dataset, compute the difference between the matrices. 
	 * Then, multiply with the previous value to keep up with the polynomial computation */
	for(long index = 1; index < sender_dataset.size(); index++){
		vector<uint64_t> prod_matrix(slot_count, stoull(sender_dataset[index]));
		Plaintext sub_plain;
		Ciphertext sub_ecnrypted; 
		Ciphertext prod_ecnrypted;
		encoder.encode(prod_matrix, sub_plain);
		send_evaluator.sub_plain(recv_ct_array, sub_plain, sub_ecnrypted);
		send_evaluator.multiply_inplace(d_i, sub_ecnrypted); 
		//send_evaluator.relinearize_inplace(d_i, send_relin_keys);
	}
		
	// Finally, multiply for the random value
	send_evaluator.multiply_plain_inplace(d_i, rand_plain); 
	//send_evaluator.relinearize_inplace(d_i, send_relin_keys);
	
	cout << "Second step completed" << endl;
	return d_i;
}
