/* This is a class that shows a proof of work for the PSI scheme implemented in the "sender.cpp" and "receiver.cpp" files 
 * This file is used to allow both parties to "communicate" each other. Some steps are trivial, such as the agreement on the 
 * homomorphic scheme, that is BFV. 
 * */

#include <iostream>
#include <ostream>
#include <string>
#include <list>
#include <vector>

#include "../sender/sender.h"
#include "../receiver/receiver.h"


using namespace std;
using namespace seal;


/* Main function */
int main (int argc, char *argv[])
{
	if (argc < 3){
		printf("Usage: prog file path of receiver file path of sender file");
		return -1;
	}
	/* For simpicity, the agreement between sender and receiver is supposed to be done here */
	EncryptionParameters params(scheme_type::bfv);	// set up the encryption scheme

	/* The poly modulus degree, one of the critical parameters to choose. The higher it is, the slower the operations will be but 
	 * more complicated operations will be supported. Recommended values are: 
	 * 						1024, 2048, 4096, 8192, 16384, ..., 32768 
	 * */
	size_t poly_mod_degree = 16384;
	params.set_poly_modulus_degree(poly_mod_degree);

	/* Set up the coeff modulus, using the general good choice given the value of the poly mod degree */
	params.set_coeff_modulus(CoeffModulus::BFVDefault(poly_mod_degree));
	//params.set_plain_modulus(1024);				// only for the BFV scheme
	
	/* Batch encoding to speed up the computation and obtain better performance*/
	params.set_plain_modulus(PlainModulus::Batching(poly_mod_degree, 20));

	
	/* Set up an instance of Receiver class, that will be used to keep the public/secret keys and passed to the receiver functions
	 * for encryption/decryption
	 * */
	Receiver recv;
	recv = setup_pk_sk(params);
	vector<string> recv_dataset;
	recv.setRecvDataset(convert_dataset(argv[1]));
	
	/* This is the full PSI scheme:
	 *  - the receiver encrypts its dataset;
	 *  - the sender computes the homomorphic product on the ecnrypted data 
	 *  - the receiver decrypt the computation received by the sender and computes the interception
	 *  */
	Ciphertext recv_encr_data = crypt_dataset(recv, params);

	Ciphertext send_encr_result = homomorphic_computation(recv_encr_data, params, convert_dataset(argv[2]));
	decrypt_and_intersect(params, send_encr_result, recv);
	
	return 0;
}
