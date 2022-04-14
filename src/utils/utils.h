#include <string>
#include <vector>

#include "seal/seal.h"

using namespace std; 
using namespace seal;

// Function prototypes
vector<string> convert_dataset(string path);

// Usefull class definition 
class Receiver
{
public:
	void setRecvDataset(vector<string> dataset){ 
		recv_dataset = dataset;
		setBitsSize(dataset[0].length());
	}
	void setRecvSk(SecretKey sk){ recv_sk = sk; } 
	void setRecvPk(PublicKey pk){ recv_pk = pk; }
	void setBitsSize(long size) { 
		bits_size = size; 
	}
	
	SecretKey getRecvSk(){ return recv_sk; } 
	PublicKey getRecvPk(){ return recv_pk; } 
	vector<string> getRecvDataset(){ return recv_dataset; }
	long getDatasetSize(){ return bits_size; }

private:
	SecretKey recv_sk;
	PublicKey recv_pk;
	vector<string> recv_dataset;
	long bits_size;
};
