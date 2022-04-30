#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <ratio>

#include "seal/seal.h"

using namespace std; 
using namespace seal;


/** 
 * Recevier class, used to save public/private keys and the dataset that will be used 
 * to check which strings belong to the intersection 
 * */
class Receiver
{
public:
	void setRecvDataset(vector<string> dataset)
    { 
		this->recv_dataset = dataset;
		if (dataset.size() > 0)
			setBitsSize(dataset[0].length());
	}
	void setRecvSk(SecretKey sk){ this->recv_sk = sk; } 
	void setRecvPk(PublicKey pk){ this->recv_pk = pk; }
    void setRelinKeys(RelinKeys relin_keys) { this->relin_keys = relin_keys; }
	void setBitsSize(long size) { this->bits_size = size; }
	
	SecretKey getRecvSk(){ return this->recv_sk; } 
	PublicKey getRecvPk(){ return this->recv_pk; }
    RelinKeys getRelinKeys() { return this->relin_keys; }
	vector<string> getRecvDataset(){ return this->recv_dataset; }
	long getDatasetSize(){ return this->bits_size; }

private:
	SecretKey recv_sk;
	PublicKey recv_pk;
    RelinKeys relin_keys;
	vector<string> recv_dataset;
	long bits_size;
};


/** Class used to keep the parameters used for the scheme */
class PsiParams
{
public:
	PsiParams(int sender_size, int recv_size, int sigma, int intersection, size_t poly_mod_degree)
    {
		this->sender_set_size = sender_size;
		this->recv_set_size = recv_size;
		this->intersect_length = intersection;
        this->sigma = sigma;
		this->poly_mod_degree = poly_mod_degree;
	}
	
	int getSendNumEntries(){ return this->sender_set_size; }
	int getRecvNumEntries(){ return this->recv_set_size; }
	int getStringLength(){ return this->sigma; }
	int getIntersectLength(){ return this->intersect_length; }
	int getPolyModDegree(){ return this->poly_mod_degree; }

private:
	// Fields used to construct the datasets 
	int sender_set_size;        // number of bitstrings in receiver's private set
	int recv_set_size;          // number of bitstrings in sender's private set 
	int sigma;                  // size of a single bitstring (expressed in bits)
	int intersect_length;       // number of strings that belong to both sets

	// Psi scheme values
	size_t poly_mod_degree;
};


/** Keeps information about the PSI computation result, such as the intersection between the two datasets and 
 *  the remaining noise after the homomorphic computation. Usefull for test cases and data gathering 
 *  */
class ComputationResult{
public:
    ComputationResult(size_t noise_budget, vector<string> intersection)
    {
        this->noise_budget = noise_budget;
        this->ds_intersection = intersection;
    }
    void setTimeVector(chrono::duration<double> time_diff) { this->time_diff = time_diff; }
    void setIntersection(vector<string> intersection) { this->ds_intersection = intersection; }
	void setNoiseBudget(size_t noise_budget){ this->noise_budget = noise_budget; }

	size_t getNoiseBudget(){ return this->noise_budget; }
	vector<string> getIntersection() { return this->ds_intersection; }
	chrono::duration<double> getTimeVector() { return this->time_diff; }
private:
	size_t noise_budget;
	vector<string> ds_intersection;
	
    // For time performance
	chrono::duration<double> time_diff;
};


// Function prototypes
vector<string> convert_dataset(string path);
vector<uint64_t> bitstring_to_long_dataset(vector<string> dataset);
vector<string> write_on_file(int n_entries, int string_length, int n_intersect, string path, 
        vector<string> prev_intersect);
void print_line();
void print_start_computation(PsiParams params);
