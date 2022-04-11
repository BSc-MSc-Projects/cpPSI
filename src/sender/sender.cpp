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


/* The second step of the PSI scheme: homomorphically subtract each value of the receiver's dataset from 
 * each of the sender's one, and finally multiply for a random value 
 * */
vector<Ciphertext> homomorphic_computation(vector<Ciphertext> recv_ct_array, EncryptionParameters params, vector<string> sender_dataset)
{
	SEALContext send_context(params);	// this class checks the validity of the parameters set
	
	/* Used to evalutate each single ciphertext value sent by the recevier */
	Evaluator send_evaluator(send_context);	
	KeyGenerator send_keygen(send_context);

	vector<Ciphertext> sender_ct;
	
	//sender_dataset = {"11111", "10000", "10001"};

	/* Here we evaluate the polynomial expressed at the top of this file. It is the PSI scheme polynomial, that has to be computed 
	 * for each element of the received dataset 
	 * */
	for(Ciphertext ct : recv_ct_array){
		//vector<Ciphertext> sub_ct;	// keeps the sum of c_i - s_j
		//vector<Ciphertext> prod_ct;	// keeps the product of all the (c_i - s_j) so far
		//cout << "Calculating ploynomial" <<endl;
		Plaintext rand_val("1");	// this must become a random value
		Ciphertext d_i;				// the final result
		//RelinKeys send_relin_keys;
		//send_keygen.create_relin_keys(send_relin_keys);
		
		// Compute the first subtraction and product
		send_evaluator.sub_plain(ct, Plaintext(to_string(stoi(sender_dataset[0],0,2))), d_i);	// homomorphic computation of c_i - s_j
		//send_evaluator.multiply_plain_inplace(d_i, rand_val);
		//send_evaluator.relinearize_inplace(d_i, send_relin_keys);
		
		//cout << "Done first mult" << endl;
		
		//RelinKeys send_relin_keys;
		//send_keygen.create_relin_keys(send_relin_keys);

		for(long index = 1; index < sender_dataset.size(); index++){ 
			//RelinKeys send_relin_keys;
			//send_keygen.create_relin_keys(send_relin_keys);
			//cout << "Inside for with integer " << sender_dataset[index] <<endl;
			Ciphertext sub_ct;	// keeps the sum of c_i - s_j
			Ciphertext prod_ct;	// keeps the product of all the (c_i - s_j) so far
			send_evaluator.sub_plain(ct, Plaintext(to_string(stoi(sender_dataset[index], 0, 2))), sub_ct);	// homomorphic computation of c_i - s_j
			send_evaluator.multiply(d_i, sub_ct, prod_ct); 
			//send_evaluator.relinearize_inplace(prod_ct, send_relin_keys);
			d_i = prod_ct;
		}
		
		// Finally, multiply for the random value
		send_evaluator.multiply_plain_inplace(d_i, rand_val); 
		
		//cout << "Encrypted element " << endl;
		sender_ct.push_back(d_i);
	}
	
	cout << "Second step completed" << endl;
	return sender_ct;
}
