#include <iostream>
#include <string>

using namespace std;


/* Convert a bitstring into an hexadecimal string */
string bit_to_hex(string bitstring)
{
	const string hex_alphab = "0123456789ABCDEF";
	string conv_result = "";
	
	// Eventually pad the string if the lenght is not a multiple of 4
	int r = bitstring.length()%4;
	if(r%2 != 0)
		r = (r+2)%4;
	int bits_to_add = r;
	while(r--)
		bitstring.insert(0, "0");

	// Actual conversion into hexadecimal
	for(size_t index = 0; index <= bitstring.length()-4; index+=4)
		conv_result += hex_alphab[stoi(bitstring.substr(index, 4), 0, 2)];
	
	return conv_result;
}
