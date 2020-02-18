#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "v1.cpp"
using namespace std;

string random_key(int stringLength)
{
	string key = "";
	string letters = "";
	for (char i = 'a'; i <= 'z'; i++)
		letters += i;
	for (char i = 'A'; i <= 'Z'; i++)
		letters += i;
	for (int i = 0; i < stringLength; i++)
		key = key + letters[rand() % 52];

	return key;
}

string random_value(int stringLength)
{
	string value = "";
	string letters = "";
	for (int i = 0; i <= 255; i++)
		letters += char(i);

	for (int i = 0; i < stringLength; i++)
		value = value + letters[rand() % 26 + 'a'];

	return value;
}

long CLOCKS_PER_SECOND = 1000000;
kvStore kv(1000000);
map<string, string> db;
int db_size = 0;
int num = 0;

int main()
{
	srand(time(NULL));
	for (int i = 0; i < 100000; i++)
	{
		int k = rand() % 64 + 1;
		int v = rand() % 5 + 1;
		string key = random_key(k);
		int keyLength = key.length();
		char *keyArray;
		keyArray = (char *)malloc(keyLength);

		for (int i = 0; i < keyLength; i++)
			keyArray[i] = key[i];

		string value = random_value(v);
		int valueLength = value.length();
		char *valueArray;
		valueArray = (char *)malloc(valueLength);

		for (int i = 0; i < valueLength; i++)
			valueArray[i] = value[i];

		Slice keySlice, valueSlice;
		keySlice.size = (uint8_t)keyLength;
		keySlice.data = keyArray;

		valueSlice.size = (uint8_t)valueLength;
		valueSlice.data = valueArray;

		db.insert(pair<string, string>(key, value));
		kv.put(keySlice, valueSlice);
		db_size++;
	}

	bool incorrect = false;

	int count = 0;
	for (int i = 0; i < 10000; i++)
	{
		int x = rand() % 5;
		if (x == 0)
		{
			string k = random_key(10);
			int keyLength = k.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			// for (int i = 0; i < keyLength; i++)
			// keyArray[i] = k[i];
			// keyArray[keyLength] = '\0';
			strcpy(keyArray, k.c_str());

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			bool ans = kv.get(keySlice, valueSlice);
			map<string, string>::iterator itr = db.find(k);
			if ((ans == false && itr != db.end()) || (ans == true && itr == db.end()))
				cout << "asidhasouid" << endl, incorrect = true;
		}
		else if (x == 1)
		{
			int k = rand() % 64 + 1;
			string key = random_key(k);
			int keyLength = key.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			strcpy(keyArray, key.c_str());

			int v = rand() % 5 + 1;
			string value = random_value(v);
			int valueLength = value.length();
			char *valueArray;
			valueArray = (char *)malloc(valueLength + 1);

			strcpy(valueArray, value.c_str());

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			valueSlice.size = (uint8_t)valueLength;
			valueSlice.data = valueArray;

			db.insert(pair<string, string>(key, value));

			bool check1 = kv.get(keySlice, valueSlice);
			// string out(valueSlice.data);
			bool ans = kv.put(keySlice, valueSlice);
			bool check2 = kv.get(keySlice, valueSlice);
			// cout<<value<<" "<<valueSlice.data<<endl;
			db_size++;
			if (check2 == false || check1 != ans)
			{
				incorrect = true;
				// cout<<"asidhasouid"<<endl;
				// cout<<"asidhasouidddddddddddddd"<<endl;
			}
		}
		else if (x == 2)
		{
			int max_size = db.size();
			int rem = rand() % max_size;
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			string key = itr->first;
			// cout << key << " ";
			int keyLength = key.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			// for (int i = 0; i < keyLength; i++)
			// 	keyArray[i] = key[i];
			// keyArray[keyLength] = '\0';
			strcpy(keyArray, key.c_str());

			Slice keySlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			// cout << keySlice.data << endl;
			bool check = kv.del(keySlice);
			db_size--;
			db.erase(itr);
			bool check2 = kv.del(keySlice);
			// if (check == 1 && check2 == 1)
			// cout << key << " " << keySlice.data << endl;
			// cout << check << " " << check2 << endl;
			if (check2 == true)
			{
				// cout<<"asidhasouid"<<endl;
				incorrect = true;
			}
		}
		else if (x == 3)
		{
			count++;
			incorrect = false;
			int max_size = db.size();
			int rem = rand() % max_size;
			Slice keySlice, valueSlice;
			bool checkStatus = kv.get(rem + 1, keySlice, valueSlice);
			pair<string, string> check = make_pair(string(keySlice.data), string(valueSlice.data));

			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			cout << count << endl;
			cout << check.first << " " << itr->first << endl;
			cout << check.second << " " << itr->second << endl;
			if (check.first != itr->first || check.second != itr->second)
			{

				incorrect = true;
				break;
			}
		}
		else if (x == 4)
		{
			// count++;
			int max_size = db.size();
			if (!max_size)
				break;
			int rem = rand() % max_size;
			assert(max_size == db.size());
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			string key = itr->first;
			int keyLength = key.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			// for (int i = 0; i < keyLength; i++)
			// 	keyArray[i] = key[i];
			// keyArray[keyLength] = '\0';
			strcpy(keyArray, key.c_str());
			// cout<<key<<" "<<keyArray<<endl;

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)(keyLength);
			keySlice.data = keyArray;
			// cout << rem << endl;
			if (kv.del(rem + 1))
			{
				// cout << "Deleted " << key << endl;
				// incorrect = true;
				// break;
			}
			else
			{
				cout << "Failed delete " << key << endl;
				// incorrect = true;
				// break;
			}
			db.erase(itr);
			db_size--;
			bool check2 = kv.get(keySlice, valueSlice);

			if (check2 == true)
			{
				cout << "Unfortunately found: " << key << endl;
				incorrect = true;
				break;
			}
			// cout << "Couldn't find: " << key << endl;
		}
	}
	if (incorrect == true)
	{
		cout << 0 << endl;
		return 0;
	}

	return 0;
}