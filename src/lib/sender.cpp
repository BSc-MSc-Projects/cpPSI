/** Sender code of the PSI scheme: it receives a ciphertext c that is a matrix of encrypted values 
 *  form the receiver and has to do the following homomorphic computation: 
 *  given that it has a dataset N of size n', the intersection computed is
 * 
 * 				d_i = r_i * [(c_i - s_1)*(c_i - s_2)*...*(c_i - s_n')]
 * 
 *  (so c_i can be seen as the x in the polynom ?? Yes because it is the encrypted point)
 *  computed for each c_i in the array that was sent by the receiver. 
 *  Now, the ciphertext d, again an encrypted matrix, is sent back to the receiver that can compute 
 *  the final intersection between the two datasets
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
#include "utils.h"

using namespace std;
using namespace seal;

#define SEND_AUDIT
#define SEED 987654321


/**
 * Generate a vector of random values that will be used in the homomorphic computation
 *
 * @param slot_count    Slot count of the matrix
 * @param dataset_size  Size of the sender's dataset
 *
 * @return              A vector of random uint64_t values
 * */
vector<uint64_t> gen_rand(size_t slot_count, size_t dataset_size)
{
	vector<uint64_t> rand_val_matrix(slot_count, 0ULL);
    srand(SEED);
	
    for(size_t index = 0; index < dataset_size; index++)
		rand_val_matrix[index] = rand();	

	return rand_val_matrix;
}


/** 
 * The second step of thr PSI scheme: homomorphically subtract each value of the receiver's dataset from each of 
 * the sender's one, and finally multiply for a random value.
 *
 * @param recv_ct           Ciphertext matrix sent by the receiver
 * @param poly_mod_degree   size of the polynomial modulus (bits), used to configure the parameters
 * @param sender_dataset    Set of bitstrings of the sender
 * @param send_relin_keys   Relinearization keys used to reduce chipertext size after homomorphic operations
 *
 * @return                  Homomorphic computation of the sender, the resulting ciphertext d
 * */
//Ciphertext homomorphic_computation(Ciphertext recv_ct, size_t poly_mod_degree, vector<string> sender_dataset, 
        //RelinKeys send_relin_keys)
Ciphertext homomorphic_computation(Ciphertext recv_ct, size_t poly_mod_degree, vector<uint64_t> sender_dataset, 
        RelinKeys send_relin_keys)
{
	Ciphertext d; 			                               // the final result
	
	if (sender_dataset.size() == 0 || recv_ct.size() == 0){
#ifdef SEND_AUDIT
		printf("Sender: an error occurred, cannot go on with the computation\n");
#endif
		return d;
	}

    EncryptionParameters prams = get_params(poly_mod_degree);
	SEALContext send_context(prams);

	/* Used to evalutate each single ciphertext value sent by the recevier */
	Evaluator send_evaluator(send_context);	
	KeyGenerator send_keygen(send_context);
	

	/* Evaluation of the polynomial expressed at the top of this file. It is the PSI scheme polynomial, 
     * that has to be computed for each element of the received ciphertext 
	 * */
	BatchEncoder encoder(send_context);
	size_t index;
	size_t slot_count = encoder.slot_count();
	size_t row_size = slot_count/2;

	// Compute the first subtraction
	vector<uint64_t> first_val_matrix(slot_count, sender_dataset[0]);
	Plaintext first_plain;
	encoder.encode(first_val_matrix, first_plain);
		
	send_evaluator.sub_plain(recv_ct, first_plain, d);	// homomorphic computation of c_i - s_j

	/* For each value of the sender dataset, compute the difference between the matrices. 
	 * Then, multiply with the previous value to keep up with the polynomial computation 
     * */
	for(long index = 1; index < sender_dataset.size(); index++){
		vector<uint64_t> prod_matrix(slot_count, sender_dataset[index]);
		Plaintext sub_plain;
		Ciphertext sub_encrypted; 
		encoder.encode(prod_matrix, sub_plain);
        
        // Subtract, multiply and relinearize the result to keep the size of the ciphertext = 2
		send_evaluator.sub_plain(recv_ct, sub_plain, sub_encrypted);
        send_evaluator.multiply_inplace(d, sub_encrypted);
	    send_evaluator.relinearize_inplace(d, send_relin_keys);
    }
		
	// Finally, multiply for the random value
	Plaintext rand_plain;
	encoder.encode(gen_rand(slot_count, sender_dataset.size()), rand_plain);	

	send_evaluator.multiply_plain_inplace(d, rand_plain);
	send_evaluator.relinearize_inplace(d, send_relin_keys);
	
#ifdef SEND_AUDIT
    printf("Second step completed\n");
#endif

	return d;
}
