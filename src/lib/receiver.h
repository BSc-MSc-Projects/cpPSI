#include <list> 
#include <vector>

#include "utils.h"
#include <seal/seal.h>


using namespace seal;

Ciphertext crypt_dataset(Receiver recv, size_t poly_mod_degree);
ComputationResult decrypt_and_intersect(size_t poly_mod_degree, Ciphertext sender_computation, Receiver recv);
Receiver setup_pk_sk(EncryptionParameters params);
