#include <list> 
#include <vector>

#include "../utils/utils.h"
#include <seal/seal.h>


using namespace seal;

// Functions prototypes
std::vector<Ciphertext> crypt_dataset(Receiver recv, EncryptionParameters params);
void decrypt_and_insersect(EncryptionParameters params, vector<Ciphertext> sender_computation, Receiver recv);
