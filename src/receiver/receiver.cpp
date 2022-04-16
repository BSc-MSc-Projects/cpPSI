/* This is a class that shows a proof of work for the PSI scheme implemented in the "sender.cpp" and "receiver.cpp" files 
 * This file is used to allow both parties to "communicate" each other. Some steps are trivial, such as the agreement on the 
 * homomorphic scheme. 
 * */

#include <iostream>
#include <ostream>
#include <string>
#include <list>
#include <cmath>
#include <vector>
#include <bitset>
#include <algorithm>

#include "../utils/utils.h"
#include "seal/seal.h"

using namespace std;
using namespace seal;

#define RECV_AUDIT

// Function prototypes
void print_intersection(vector<string> intersection);


/** 
 * Apply homomorphic encryption on the dataset r_dataset, to produce n1_size ciphertexts that will be deilvered to the sender. 
 * 
 * @param recv Instance of Receiver class
 * @param params EncryptionParameters class that contains the constants for the scheme
 *
 * @return A matrix that contains the ecnrypted values of the dataset
 * */
Ciphertext crypt_dataset(Receiver recv, EncryptionParameters params)
{   
	SEALContext recv_context(params);     								// this class checks the validity of the parameters set 
	Encryptor encryptor(recv_context, recv.getRecvPk());
	Plaintext plain_recv_matrix;
	Ciphertext encrypted_recv_matrix;
	vector<Ciphertext> cipher_dataset;
	vector<string> recv_dataset = recv.getRecvDataset();
	
	BatchEncoder recv_batch_encoder(recv_context);
	size_t slot_count = recv_batch_encoder.slot_count();
	size_t row_size = slot_count/2;
	vector<uint64_t> batch_recv_matrix(slot_count, 0ULL);
	
	/* In this part, the receiver moves the first step of the PSI scheme: 
	 * the dataset is encrypted using the encryptor class and the obtained dataset is then sent to the sender (returned) 
	 * */
	for(size_t index = 0; index < recv_dataset.size(); index++)
		batch_recv_matrix[index] = stoull(recv_dataset[index], 0, 2);
	recv_batch_encoder.encode(batch_recv_matrix, plain_recv_matrix);	// encode the batch matrix
	encryptor.encrypt(plain_recv_matrix, encrypted_recv_matrix);

	printf("First step completed\n");
	return encrypted_recv_matrix;
}


/** 
 * Last part of the PSI scheme, where the receiver computes the intersection between the two dataset.
 * 
 * @param params EncryptionParameters class instance, containing the information about the scheme
 * @param sender_computation Ciphertext resulting after the homomorphic computation performed by the sender
 * @param recv Receiver class instance containing the secret key used to decrypt the sender computaiton
 * 
 * @return The size of the intersection
 * */
int decrypt_and_intersect(EncryptionParameters params, Ciphertext sender_computation, Receiver recv)
{
	SEALContext recv_context(params);     							// this class checks the validity of the parameters set
	Decryptor recv_decryptor(recv_context, recv.getRecvSk());		// needed to check if everything worked
	Plaintext plain_result;
	vector<string> intersection;
	vector<uint64_t> pod_result;

#ifdef RECV_AUDIT
	cout << "noise budget in encrypted x: " << recv_decryptor.invariant_noise_budget(sender_computation) << " bits" << endl;
#endif

	BatchEncoder encoder(recv_context);
	long size = recv.getDatasetSize();
	
	/* First, encode the plaintext matrix, where each entry is a value of the dataset.
	 * Then, encrypt the matrix using the public key 
	 * */
	recv_decryptor.decrypt(sender_computation, plain_result);
	encoder.decode(plain_result, pod_result);
	for(long index = 0; index < recv.getRecvDataset().size(); index++)
		if(pod_result[index] == 0)									// the value belongs to the intersection
			intersection.push_back(recv.getRecvDataset()[index]);
	printf("Last step completed\n");
	if(intersection.size() > 0)
		print_intersection(intersection);
	else
		printf("The intersection between sender and receiver is null \n");
	return intersection.size();
}


/** 
 * Generate public and secret keys for recevier operations
 *
 * @param params EncryptionParameters class instance, containing the information about the scheme
 * 
 * */
Receiver setup_pk_sk(EncryptionParameters params)
{
	SEALContext recv_context(params);
	Receiver recv;

	/* Generate public and private keys for the receiver */
	KeyGenerator recv_keygen(recv_context);
    SecretKey recv_sk = recv_keygen.secret_key();
    PublicKey recv_pk;
	recv_keygen.create_public_key(recv_pk);
	
	// Save the keys for later decryption
	recv.setRecvPk(recv_pk); 
	recv.setRecvSk(recv_sk);

	return recv;
}


/** 
 * Pretty print of a nice table :) 
 *
 * @param intersection Intersection between the two dataset
 * */
void print_intersection(vector<string> intersection)
{
	string o_line = "";
	string v_line = " | ";
	string spaces = "";
	size_t i = 0;

	int middle_point = intersection[0].length()+2;
	long line_size = 2*middle_point;

	for (i = 0; i <= line_size; i++)
		o_line += "-";
	o_line[middle_point] = '|';
	
	cout << "\nPrinting the intersection between the two datasets: (bitstring, integer value)\n" << endl;
	cout << o_line << endl;
	for (string s : intersection){
		cout << ' ' <<  s << v_line << stoull(s, 0, 2) << endl;
		cout << o_line << endl;
	}
}
