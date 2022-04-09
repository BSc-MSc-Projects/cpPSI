#include <string>
#include <list>

#include "seal/seal.h"

using namespace std; 
using namespace seal;

// Function prototypes
string bit_to_hex(string bitstring);

// Usefull class definition 
class Receiver
{
public:
	
	// Dummy dataset generation
	static list<string> getRecvDataset() 
	{
		return {"10001", "00100", "11001"};
	}

	void setRecvSk(SecretKey sk){ recv_sk = sk; } 
	void setRecvPk(PublicKey pk){ recv_pk = pk; }
	
	SecretKey getRecvSk(){ return recv_sk; } 
	PublicKey getRecvPk(){ return recv_pk; }

private:
	SecretKey recv_sk;
	PublicKey recv_pk;
};
