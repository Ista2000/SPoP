#include <iostream>
#include "v1.cpp"
#include <string>
#include "unistd.h"
using namespace std;

int main() 
{
	kvStore kvs(5);
	Slice key1, key2, key3, key4, key5;
	Slice value1, value2, value3, value4, value5;

	key1.size = 2;
	char sun[3] = "ab";
	key1.data = sun;
	value1.size = 7;
	char shradha[8] = "shradha";
	value1.data = shradha;

	key2.size = 2;
	char sunrisers[3] = "ad";
	key2.data = sunrisers;
	value2.size = 7;
	char istasis[8] = "istasis";
	value2.data = istasis;

	key3.size = 2;
	char kolata[3] = "aa";
	key3.data = kolata;
	value3.size = 7;
	char onepluis[8] = "manvith";
	value3.data = onepluis;

	key4.size = 2;
	char kolaata[3] = "ac";
	key4.data = kolaata;
	value4.size = 7;
	char oneapluis[8] = "agarwal";
	value4.data = oneapluis;

	key5.size = 2;
	char kolaaata[3] = "aa";
	key5.data = kolaaata;
	value5.size = 7;
	char oneplluis[8] = "rishabh";
	value5.data = oneplluis;


	kvs.put(key1, value1);
	kvs.put(key2, value2);
	kvs.put(key3, value3);
	kvs.put(key4, value4);
	kvs.put(key5, value5);

	if(kvs.get(1, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	if(kvs.get(2, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	if(kvs.get(3, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	if(kvs.get(4, key3, value3)) {
		string out(value3.data);
		cout<<out<<endl;
	}
	// if(kvs.get(5, key3, value3)) {
	// 	string out(value3.data);
	// 	cout<<out<<endl;
	// }
	// cout<<kvs.del(2)<<endl;
	// cout<<kvs.del(2)<<endl;
	// cout<<kvs.get(key2,value2);

	// string key = "abc";
	// int keyLength = key.length();
	// char *keyArray;
	// keyArray = (char *)malloc(keyLength);

	// for (int i = 0; i < keyLength; i++)
	// 	keyArray[i] = key[i];

	// Slice keySlice, valueSlice;
	// keySlice.size = (uint8_t)keyLength;
	// keySlice.data = keyArray;
	// // cout << rem << endl;
	// bool check = kvs.del(5);
	// bool check2 = kvs.get(keySlice, valueSlice);
	// cout<<check<<endl;
	// cout<<kvs.del(1)<<endl;
	// cout<<kvs.del(1)<<endl;
	// cout<<kvs.del(1)<<endl;

	// else
	// 	cout<<"LULLI"<<endl;

	// cout<<kvs.del(key3)<<endl;
	// sleep(5);
	// char news[4] = "abb";
	// key3.size = 3;
	// key3.data = news;
	// cout<<kvs.del(key3);

	// if(kvs.get(1, key3, value3)) {
	// 	string out(value3.data);
	// 	cout<<out<<endl;
	// }
	// else
	// 	cout<<"LULLI"<<endl;


	return 0;
}