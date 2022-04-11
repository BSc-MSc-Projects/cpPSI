/* This is a class that shows a proof of work for the PSI scheme implemented in the "sender.cpp" and "receiver.cpp" files 
 * This file is used to allow both parties to "communicate" each other. Some steps are trivial, such as the agreement on the 
 * homomorphic scheme, that is CKKS. 
 * */

#include <iostream>
#include <ostream>
#include <string>
#include <list>
#include <vector>

#include "../sender/sender.h"
#include "../receiver/receiver.h"
//#include "../utils/utils.h"

//#include "seal/seal.h"


using namespace std;
using namespace seal;

/* Constants for the sender and the receiver */


int main (int argc, char *argv[])
{
	if (argc < 3){
		cout << "Usage: prog file path of receiver file path of sender file" << endl;
		return -1;
	}
	/* For simpicity, the agreement between sender and receiver is supposed to be done here */
	EncryptionParameters params(scheme_type::bfv);	// set up the encryption scheme

	/* The poly modulus degree, one of the critical parameters to choose. The higher it is, the slower the operations will be but 
	 * more complicated operations will be supported. Recommended values are: 
	 * 						1024, 2048, 4096, 8192, ..., 32768 
	 * */
	size_t poly_mod_degree = 4096;
	params.set_poly_modulus_degree(poly_mod_degree);

	/* Set up the coeff modulus, using the general good choice given the value of the poly mod degree */
	params.set_coeff_modulus(CoeffModulus::BFVDefault(poly_mod_degree));
	params.set_plain_modulus(1024);				// only for the BFV scheme
	
	SEALContext context(params);				// this class checks the validity of the parameters set

	/* Generate public and private keys for the receiver */
	KeyGenerator recv_keygen(context);
    SecretKey recv_sk = recv_keygen.secret_key();
    PublicKey recv_pk;
	recv_keygen.create_public_key(recv_pk);
	
	/* Set up an instance of Receiver class, that will be used to keep the public/secret keys and passed to the receiver functions
	 * for encryption/decryption
	 * */
	Receiver recv;
	recv.setRecvPk(recv_pk); 
	recv.setRecvSk(recv_sk);
	recv.setRecvDataset(convert_dataset(argv[1], 0));
	vector<Ciphertext> recv_encr_data = crypt_dataset(recv, params);
	
	vector<Ciphertext> send_encr_result = homomorphic_computation(recv_encr_data, params, convert_dataset(argv[2], 1));
	decrypt_and_insersect(params, send_encr_result, recv);
	return 0;
}
