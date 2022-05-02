#include <vector>
#include <string>
#include <seal/seal.h>

using namespace std;
using namespace seal;

Ciphertext homomorphic_computation(Ciphertext recv_ct, size_t poly_mod_degree, vector<string> sender_dataset, 
        RelinKeys send_relin_keys);
