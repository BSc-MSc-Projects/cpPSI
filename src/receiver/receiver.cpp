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


#include "../utils/utils.h"
#include "seal/seal.h"

using namespace std;
using namespace seal;

void print_intersection(vector<string> intersection);

/* Constants for the sender and the receiver */


/* Apply homomorphic encryption on the dataset r_dataset, to produce n1_size ciphertexts that will be 
 * deilvered to the sender */
Ciphertext crypt_dataset(Receiver recv, EncryptionParameters params)
{   
	SEALContext recv_context(params);     // this class checks the validity of the parameters set 
    //PublicKey recv_pk = recv.getRecvPk();		
	Encryptor encryptor(recv_context, recv.getRecvPk());
	
	BatchEncoder recv_batch_encoder(recv_context);
	size_t slot_count = recv_batch_encoder.slot_count();
	size_t row_size = slot_count/2;
	vector<uint64_t> batch_recv_matrix(slot_count, 0ULL);
	
	/* In this part, the receiver moves the first step of the PSI scheme: 
	 * the dataset is encrypted using the ecnryptor class and the obtained dataset is then sent to the sender (returned) 
	 * */
	vector<Ciphertext> cipher_dataset;
	vector<string> recv_dataset = recv.getRecvDataset();
	
	for(size_t index = 0; index < recv_dataset.size(); index++)
		batch_recv_matrix[index] = stoull(recv_dataset[index], 0, 10);
	Plaintext plain_recv_matrix;
	Ciphertext encrypted_recv_matrix;
	recv_batch_encoder.encode(batch_recv_matrix, plain_recv_matrix);	// encode the batch matrix
	encryptor.encrypt(plain_recv_matrix, encrypted_recv_matrix);

	cout << "First step completed" << endl;
	return encrypted_recv_matrix;
}


/* Last part of the PSI scheme, where the receiver computes the intersection between the two dataset.
 * */
void decrypt_and_insersect(EncryptionParameters params, Ciphertext sender_computation, Receiver recv)
{
	SEALContext recv_context(params);     // this class checks the validity of the parameters set
	Decryptor recv_decryptor(recv_context, recv.getRecvSk());	// needed to check if everything worked
	
	cout << "    + noise budget in encrypted x: " << recv_decryptor.invariant_noise_budget(sender_computation) << " bits" << endl;
	
	Plaintext plain_result;
	BatchEncoder encoder(recv_context);
	vector<string> intersection;
	recv_decryptor.decrypt(sender_computation, plain_result);
	vector<uint64_t> pod_result;
	encoder.decode(plain_result, pod_result);
	for(long index = 0; index < recv.getRecvDataset().size(); index++)
		if(pod_result[index] == 0)
			//cout << "The uint value: " << stoull(recv.getRecvDataset()[index], 0, 10) << " (bitstring: " <<  bitset<20>(stoull(recv.getRecvDataset()[index], 0, 10)).to_string() << 
			//	")" " is in the intersection" << endl;
			intersection.push_back(bitset<20>(stoull(recv.getRecvDataset()[index], 0, 10)).to_string());
		//intersection.push_back(recv.getRecvDataset()[index]);
	cout << "Last step completed" << endl;
	print_intersection(intersection);
}


Receiver setup_pk_sk(EncryptionParameters params)
{
	SEALContext recv_context(params);     // this class checks the validity of the parameters set 
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


/* Pretty print of a nice table :) */
void print_intersection(vector<string> intersection)
{
	string first_col = "Integer value";
	string second_col = " Bitstring";
	string o_line = "";
	string v_line = "|";
	
	size_t total = 20 - second_col.length() + 3;

	for(size_t i = 0; i < total; i++)
		second_col += " ";
	
	size_t line_length = intersection[0].length();
	for (size_t i = 0; i < intersection[0].length(); i++)
		o_line += "--";
	o_line[intersection[0].length()+1] = '|';
	
	cout << "\nPrinting the intersection between the two datasets:\n" << endl;
	cout << o_line << endl;
	cout << second_col << first_col << endl;
	cout << o_line << endl;
	for (string s : intersection){
		cout << ' ' <<  s << v_line << ' ' << stoull(s, 0, 2) << endl;
		cout << o_line << endl;
	}
}
