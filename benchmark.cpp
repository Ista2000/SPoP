#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "kvStore.cpp"
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
	for (int i = 1; i <= 127; i++)
		letters += char(i);
	for (int i = 1; i <= 31; i++)
		letters[i] = (i % 26 + 'a');

	for (int i = 0; i < stringLength; i++)
		value = value + letters[rand() % 127];

	// value = value + '\0';
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
		int v = rand() % 255 + 1;
		string key = random_key(k);
		int keyLength = key.length();
		char *keyArray;
		keyArray = (char *)malloc(keyLength + 1);

		// for (int i = 0; i < keyLength; i++)
		// keyArray[i] = key[i];
		strcpy(keyArray, key.c_str());

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

		if(db[key].size())
			cout<<"OVERWRITTEN: "<<key<<" "<<value<<endl;
		else if(key.size() <= 2)
			cout<<"FIRST ADD: "<<key<<" "<<value<<endl;

		db.insert(pair<string, string>(key, value));
		db[key] = value;
		kv.put(keySlice, valueSlice);
		db_size++;
	}

	bool incorrect = false;

	int count = 0;
	for (int i = 0; i < 100000; i++)
	{
		int x = rand() % 5;
		if (x == 0)
		{
			string k = random_key(10);
			int keyLength = k.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			strcpy(keyArray, k.c_str());

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			bool ans = kv.get(keySlice, valueSlice);
			map<string, string>::iterator itr = db.find(k);
			if ((ans == false && itr != db.end()) || (ans == true && itr == db.end()))
				incorrect = true;
		}

		else if (x == 1)
		{
			int k = rand() % 64 + 1;
			string key = random_key(k);
			int keyLength = key.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength);

			strcpy(keyArray, key.c_str());

			int v = rand() % 256 + 1;
			string value = random_value(v);
			int valueLength = value.length();
			char *valueArray;
			valueArray = (char *)malloc(valueLength);

			strcpy(valueArray, value.c_str());

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			valueSlice.size = (uint8_t)valueLength;
			valueSlice.data = valueArray;

			db.insert(pair<string, string>(key, value));
			db[key] = value;

			bool check1 = kv.get(keySlice, valueSlice);
			bool ans = kv.put(keySlice, valueSlice);
			bool check2 = kv.get(keySlice, valueSlice);
			db_size++;
			if (check2 == false || check1 != ans)
				incorrect = true;
		}

		else if (x == 2)
		{
			int max_size = db.size();
			int rem = rand() % max_size;
			map<string, string>::iterator itr = db.begin();
			for (int i = 0; i < rem; i++)
				itr++;
			string key = itr->first;

			int keyLength = key.length();
			char *keyArray;
			keyArray = (char *)malloc(keyLength + 1);

			strcpy(keyArray, key.c_str());

			Slice keySlice;
			keySlice.size = (uint8_t)keyLength;
			keySlice.data = keyArray;

			bool check = kv.del(keySlice);
			db_size--;
			db.erase(itr);
			bool check2 = kv.del(keySlice);
			if (check2 == true)
				incorrect = true;
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

			// if (check.first != itr->first || check.second != itr->second)
			if (strcmp(check.first.c_str(), itr->first.c_str()) || strcmp(check.second.c_str(), db[itr->first].c_str()))
			{
				cout << (check.first != itr->first) << " " << (check.second != db[itr->first]) << endl;

				cout << count << endl;
				cout << check.first << " " << itr->first << endl;
				// cout << "OUR: " << check.second << " THEIR OLD: " << itr->second << endl;
				cout << "OUR: " << check.second << endl 
					 << "HIS: " << db[itr->first] << endl;
				cout << strcmp(check.first.c_str(), itr->first.c_str()) << endl;
				cout << strcmp(check.second.c_str(), db[itr->first].c_str()) << endl;
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

			strcpy(keyArray, key.c_str());

			Slice keySlice, valueSlice;
			keySlice.size = (uint8_t)(keyLength);
			keySlice.data = keyArray;

			bool check = (kv.del(rem + 1));
			db.erase(itr);
			db_size--;

			bool check2 = kv.get(keySlice, valueSlice);

			if (check2 == true)
				incorrect = true;
		}
	}

	if (incorrect == true)
	{
		cout << 0 << endl;
		return 0;
	}
	cout<<"Worked"<<endl;
	return 0;
}