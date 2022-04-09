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

#include "../utils/utils.h"
#include "seal/seal.h"

using namespace std;
using namespace seal;

/* Constants for the sender and the receiver */

long n1_size = 4;	// receiver dataset size
long sigma = 5;		// length of each dataset's bitstring


/* Apply homomorphic encryption on the dataset r_dataset, to produce n1_size ciphertexts that will be 
 * deilvered to the sender */
vector<Ciphertext> crypt_dataset(Receiver recv, EncryptionParameters params)
{   
    SEALContext recv_context(params);     // this class checks the validity of the parameters set 
    PublicKey recv_pk = recv.getRecvPk();		
	Encryptor encryptor(recv_context, recv_pk);

	/* In this part, the receiver moves the first step of the PSI scheme: 
	 * the dataset is encrypted using the ecnryptor class and the obtained dataset is then sent to the sender (returned) 
	 * */
	vector<Ciphertext> cipher_dataset;
	int index = 0;
	list<string>::iterator recv_it;
	list<string> recv_dataset = recv.getRecvDataset();
	for(recv_it = recv_dataset.begin(); recv_it != recv_dataset.end(); recv_it++){
		Plaintext x_plain(bit_to_hex(recv_it->c_str()));
		Ciphertext x_enc;
		Plaintext dec_show;
		encryptor.encrypt(x_plain, x_enc); 
		
		cipher_dataset.insert(cipher_dataset.begin() + index, x_enc);
		index++;
	}
	
	cout << "First step completed" << endl;
	return cipher_dataset;
}


/* Last part of the PSI scheme, where the receiver computes the intersection between the two dataset.
 * */
void decrypt_and_insersect(EncryptionParameters params, vector<Ciphertext> sender_computation, Receiver recv)
{
	SEALContext recv_context(params);     // this class checks the validity of the parameters set
	Decryptor recv_decryptor(recv_context, recv.getRecvSk());	// needed to check if everything worked
	
	int index = 0;
	for(Ciphertext ct : sender_computation){
		Plaintext x_decrypted;
		recv_decryptor.decrypt(ct, x_decrypted);
		if((x_decrypted.to_string()).compare("0") == 0)
			cout << "the element at index: " << index  << " is in the intersection\n" << endl;
		index++;
	}

	cout << "Last step completed" << endl;
}
