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
#include <climits>
#include "seal/seal.h"
#include "../utils/utils.h"

using namespace std;
using namespace seal;

//#define SEND_AUDIT

/**
 * Generate a vector of random values that will be used in the homomorphic computation
 *
 * @param slot_count Slot count of the matrix
 * @param dataset_size Size of the sender's dataset
 *
 * @return A vector of random ull values
 * */
vector<uint64_t> gen_rand(size_t slot_count, size_t dataset_size)
{
	vector<uint64_t> rand_val_matrix(slot_count, 0ULL);
	random_device rd;     	// Get a random seed from the OS entropy device, or whatever
  	mt19937_64 eng(rd()); 	// Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
	
	//Define the distribution, by default it goes from 0 to MAX(unsigned long long) or what have you.
  	uniform_int_distribution<uint64_t> distr(1, LONG_MAX);
	for(size_t index = 0; index < dataset_size; index++){
		size_t value = distr(eng);
		rand_val_matrix[index] = value;	
	}
	return rand_val_matrix;
}


/** 
 * The second step of the PSI scheme: homomorphically subtract each value of the receiver's dataset from each of the sender's one, 
 * and finally multiply for a random value.
 *
 * @param recv_ct Ciphertext matrix sent by the receiver
 * @param params EncryptionParameters parameters agreed with the Receiver
 * @param sender_dataset Set of bitstrings of the sender
 *
 * @return Homomorphic computation of the sender
 * */
Ciphertext homomorphic_computation(Ciphertext recv_ct, EncryptionParameters params, vector<string> sender_dataset)
{
	//Ciphertext d_i; 											// the final result
	vector<Ciphertext> enc_computation(sender_dataset.size()+1);	// this will keep up the computation 
	
	if (sender_dataset.size() == 0 || recv_ct.size() == 0){
#ifdef SEND_AUDIT
		printf("Sender: an error occurred, cannot go on with the computation\n");
#endif
		//return d_i;
		return enc_computation[0]; 
	}

	vector<uint64_t> longint_sender_dataset = bitstring_to_long_dataset(sender_dataset);
	if (longint_sender_dataset.size() == 0){
#ifdef SEND_AUDIT
		printf("Sender dataset is malformed\n");
#endif
		//return d_i;
		return enc_computation[0]; 
	}	

	SEALContext send_context(params);							// this class checks the validity of the parameters set
	
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

	/*RelinKeys send_relin_keys;
	send_keygen.create_relin_keys(send_relin_keys);*/
		
	// Compute the first subtraction
	vector<uint64_t> first_val_matrix(slot_count, longint_sender_dataset[0]);
	Plaintext first_plain;
	encoder.encode(first_val_matrix, first_plain);
		
	//send_evaluator.sub_plain(recv_ct, first_plain, d_i);	// homomorphic computation of c_i - s_j
	send_evaluator.sub_plain(recv_ct, first_plain, enc_computation[0]);	// homomorphic computation of c_i - s_j
#ifdef SEND_AUDIT
	cout << "Size of ct: " << enc_computation[0].size() << endl;
#endif
	RelinKeys send_relin_keys;
	send_keygen.create_relin_keys(send_relin_keys);

	/* For each value of the sender dataset, compute the difference between the matrices. 
	 * Then, multiply with the previous value to keep up with the polynomial computation */
	for(long index = 1; index < sender_dataset.size(); index++){
		//vector<uint64_t> prod_matrix(slot_count, stoull(sender_dataset[index], 0, 2));
		vector<uint64_t> prod_matrix(slot_count, stoull(sender_dataset[index], 0, 2));
		
		Plaintext sub_plain;
		Ciphertext sub_encrypted; 
		Ciphertext prod_ecnrypted;
		encoder.encode(prod_matrix, sub_plain);
		send_evaluator.sub_plain(recv_ct, sub_plain, sub_encrypted);
		//send_evaluator.multiply_inplace(d_i, sub_encrypted); 
		//send_evaluator.relinearize_inplace(enc_computation[index-1], send_relin_keys);
		//send_evaluator.mod_switch_to_next_inplace(enc_computation[index-1]);

		send_evaluator.multiply(enc_computation[index-1], sub_encrypted, enc_computation[index]); 
	//	send_evaluator.relinearize_inplace(d_i, send_relin_keys);
		//send_evaluator.relinearize_inplace(enc_computation[index], send_relin_keys);
		//send_evaluator.mod_switch_to_next_inplace(enc_computation[index]);
#ifdef SEND_AUDIT
    	//send_evaluator.relinearize_inplace(enc_computation[index], relin_keys);
		cout << "Size of ct: " << enc_computation[index].size() << endl;
#endif
	}
		
	// Finally, multiply for the random value
	Plaintext rand_plain;
	encoder.encode(gen_rand(slot_count, sender_dataset.size()), rand_plain);	

	//RelinKeys send_relin_keys;
	//send_keygen.create_relin_keys(send_relin_keys);
	/*send_evaluator.multiply_plain_inplace(d_i, rand_plain); 
	send_evaluator.relinearize_inplace(d_i, send_relin_keys);*/
	send_evaluator.multiply_plain(enc_computation[enc_computation.size()-2], rand_plain, enc_computation[enc_computation.size()-1]); 
	//send_evaluator.relinearize_inplace(enc_computation[enc_computation.size()-1], send_relin_keys);
#ifdef SEND_AUDIT
	cout << "Size of ct: " << enc_computation[enc_computation.size()-1].size() << endl;
	printf("Second step completed\n");
#endif

	//return d_i;
	return enc_computation[enc_computation.size()-1];
}
