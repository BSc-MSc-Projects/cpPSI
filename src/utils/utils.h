#include <string>
#include <vector>

#include "seal/seal.h"

using namespace std; 
using namespace seal;

// Function prototypes
vector<string> convert_dataset(string path);
vector<uint64_t> bitstring_to_long_dataset(vector<string> dataset);

// Usefull class definition 
class Receiver
{
public:
	void setRecvDataset(vector<string> dataset){ 
		this->recv_dataset = dataset;
		if (dataset.size() > 0)
			setBitsSize(dataset[0].length());
	}
	void setRecvSk(SecretKey sk){ this->recv_sk = sk; } 
	void setRecvPk(PublicKey pk){ this->recv_pk = pk; }
	void setBitsSize(long size) { this->bits_size = size; }
	
	SecretKey getRecvSk(){ return this->recv_sk; } 
	PublicKey getRecvPk(){ return this->recv_pk; } 
	vector<string> getRecvDataset(){ return this->recv_dataset; }
	long getDatasetSize(){ return this->bits_size; }

private:
	SecretKey recv_sk;
	PublicKey recv_pk;
	vector<string> recv_dataset;
	long bits_size;
};
