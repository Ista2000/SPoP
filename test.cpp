#include <iostream>
#include "v1.cpp"
#include <string>
using namespace std;

int main() 
{
	kvStore kvs(5);
	Slice key;
	Slice value;
	key.size = 7;
	char istasis[8] = "istasis";;
	key.data = istasis;
	value.size = 7;
	char shradha[8] = "shradha";
	value.data = shradha;
	kvs.put(key, value);

	Slice value1;
	if(kvs.get(key, value1)) {
		string out(value1.data);
		cout<<out<<endl;
	}
	else
		cout<<"LULLI"<<endl;

	return 0;
}