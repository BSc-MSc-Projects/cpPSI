/* Sender code of the PSI scheme: it receives an array of ciphertext (c_1, ... , c_n) form the receiver and has to do the following 
 * homomorphic computation: given that it has a dataset N of size n', the intersection computed is: 
 * 
 * 				d_i = r_i * [(c_i - s_1)*(c_i - s_2)*...*(c_i - s_n')]
 * 
 * (so c_i can be seen as the x in the polynom ?? Yes because it is the ecnrypted point)
 * computed for each c_i in the array that was sent by the receiver. Now, the (d_1,...,d_n') vector is sent back to the receiver that can
 * compute the final intersection between the two datasets
 * */

#include <iostream>
#include <iterator>
#include <list>
#include <string> 
#include <vector>
#include "seal/seal.h"

using namespace std;
using namespace seal;


// Convert a bitstring into a negative plain value, as string too
string convert_plain_value(string bitstring)
{
	int int_value = stoi(bitstring, 0, 2);
	return to_string(int_value);
}


/* The second step of the PSI scheme: homomorphically subtract each value of the receiver's dataset from 
 * each of the sender's one, and finally multiply for a random value 
 * */
vector<Ciphertext> homomorphic_computation(vector<Ciphertext> recv_ct_array, EncryptionParameters params)
{
	SEALContext send_context(params);	// this class checks the validity of the parameters set
	
	/* Used to evalutate each single ciphertext value sent by the recevier */
	Evaluator send_evaluator(send_context);
	
	
	/* Relinearization is the operation performed to decrease the size of the ciphertext. 
	 * This is used to avoid the noise used in the homomorphic computations to get to 0 (??) so that decrypt is possibile 
	 * */
	KeyGenerator send_keygen(send_context);
	RelinKeys send_relin_keys;
	send_keygen.create_relin_keys(send_relin_keys);

	/* Here we evaluate the polynomial expressed at the top of this file. It is the PSI scheme polynomial that has to be computed for each element of the received dataset */
	vector<string> sender_dataset;
	vector<Ciphertext> sender_ct;

	sender_dataset.assign(3, "00100");

	for(Ciphertext ct: recv_ct_array){
		int send_index = 0;
		Plaintext rand_val("1");	// this must become a random value
		Ciphertext d_i;		// the final result
		
		// Compute the first subtraction and product
		send_evaluator.sub_plain(ct, Plaintext(convert_plain_value(sender_dataset[0])), d_i);	// homomorphic computation of c_i - s_j
		send_evaluator.multiply_plain_inplace(d_i, rand_val);
		//send_evaluator.relinearize_inplace(first_ct, send_relin_keys);

		for(size_t index = 1; index < sender_dataset.size(); index++){
			Ciphertext sub_ct;	// keeps the sum of c_i - s_j
			send_evaluator.sub_plain(ct, Plaintext(convert_plain_value(sender_dataset[index])), sub_ct);	// homomorphic computation of c_i - s_j
			send_evaluator.multiply(d_i, sub_ct, d_i); 
			//send_evaluator.relinearize_inplace(d_i, send_relin_keys);
		}

		// Finally, multiply for the random value
		send_evaluator.multiply_plain_inplace(d_i, rand_val); 
		//send_evaluator.relinearize_inplace(d_i, send_relin_keys);
		
		sender_ct.insert(sender_ct.begin()+send_index, d_i);
		send_index++;
	}
	
	cout << "Second step completed" << endl;
	return sender_ct;
}
