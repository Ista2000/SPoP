#include <iostream>
#include "v1.cpp"
#include <string>
using namespace std;

int main() 
{
	kvStore kvs(5);
	Slice key1, key2, key3;
	Slice value1, value2;
	key1.size = 2;
	char sun[3] = "ab";
	key1.data = sun;
	value1.size = 7;
	char shradha[8] = "shradha";
	value1.data = shradha;

	key2.size = 2;
	char sunrisers[3] = "bc";
	key2.data = sunrisers;
	value2.size = 7;
	char istasis[8] = "istasis";
	value2.data = istasis;
	kvs.put(key1, value1);
	kvs.put(key2, value2);

	Slice value3;
	kvs.del(1);
	if(kvs.get(1, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	else
		cout<<"LULLI"<<endl;
	kvs.del(key2);
	if(kvs.get(1, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	else
		cout<<"LULLI"<<endl;


	return 0;
}