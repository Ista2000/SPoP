#include<bits/stdc++.h>
#include <time.h>
#include "v3.cpp"
using namespace std;

string sliceToStr(Slice& a) {
    string ret = "";

    for (int i = 0; i < a.size; i++)
        ret += a.data[i];

    return ret;
}

void strToSlice(string l, Slice& a) {
    a.size = l.length();
    a.data = (char*)malloc(a.size);
    strncpy(a.data, l.c_str(), a.size);
    // a.data[a.size] = 0;
}

string random_key(int stringLength){
	string k = "";
	string letters = "";
	for(char i = 'a';i<='z';i++)letters+=i;
	for(char i = 'A';i<='Z';i++)letters+=i;
	for(int i=0;i<stringLength;i++)
		k += letters[rand()%(letters.size())];

	return k;
}

string random_value(int stringLength){
	string v = "";
	string letters = "";
	for(int i = 32;i<=127;i++)letters+=char(i);

	for(int i=0;i<stringLength;i++)
		v += letters[rand()%(letters.size())];

	return v;
}

long CLOCKS_PER_SECOND = 1000000;
kvStore kv(10000000);
map<string,string> db;
long long db_size = 0;

void *myThreadFun(void *vargp) 
{ 
	int transactions=0;
	clock_t start = clock();
	int time = 10;
	clock_t tt = clock();
	while((float(tt-start)/CLOCKS_PER_SECOND)<=time)
	{

		for(int i=0;i<10000;i++)
		{
			transactions+=1;
			int x = rand()%5;
			// cout<<x<<endl;
			if(x==0)
			{
				string key = random_key(rand()%64 + 1);
				Slice s_key,s_value;
				strToSlice(key,s_key);
				bool ans = kv.get(s_key,s_value);
			}
			else if(x==1)
			{
				string key = random_key(rand()%64 + 1);
				string value = random_value(rand()%255 + 1);
				Slice s_key,s_value,temp;
				strToSlice(key,s_key);
				strToSlice(value,s_value);

				bool check = kv.get(s_key,temp);
				bool ans = kv.put(s_key,s_value);

				if(check == false)
					db_size++;
			}
			else if(x==2)
			{
				int temp=db_size;
				if (temp == 0)
					continue;		
				int rem = rand()%temp;
				Slice s_key,s_value;
				bool check = kv.get(rem,s_key,s_value);
				check = kv.del(s_key);
				db_size--;
			}
			else if(x==3)
			{
				int temp=db_size;
				if (temp == 0)
					continue;
				int rem = rand()%temp;
				Slice s_key,s_value;
				bool check = kv.get(rem,s_key,s_value);
			}
			else if(x==4)
			{
				int temp=db_size;
				if (temp == 0)
					continue;
				int rem = rand()%temp;
				bool check = kv.del(rem);
				db_size--;
			}
		}
		tt=clock();
	}
	cout<<transactions/time<<endl;
	return NULL;  
} 

int main()
{
	srand(time(0));
	struct timespec start, end;
	double total=0;
	for(int i=0;i<100000;i++)
	{
		string key = random_key(rand()%64 + 1);
		string value = random_key(rand()%255 + 1);
		db[key] = value;
		Slice k,v;
		strToSlice(key,k);
		strToSlice(value,v);
		clock_gettime(CLOCK_MONOTONIC, &start);
		kv.put(k,v);
		clock_gettime(CLOCK_MONOTONIC, &end);
    	total += (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);

		db_size = db.size();
	}

	cout<<"Put worked, time: "<<total<<endl;

	bool incorrect = false;

	for(int i=0;i<10000;i++)
	{
		int x = rand()%5;
		// cout<<i/1000<<endl;
		if(x==0)
		{
			// cout<<"GET"<<endl;
			string key = random_key(rand()%64 + 1);
			Slice s_key,s_value;
			strToSlice(key,s_key);
			bool ans = kv.get(s_key,s_value);
			map<string,string>:: iterator itr = db.find(key);
			if((ans==false && itr != db.end()) || (ans==true && itr->second != sliceToStr(s_value) ))
			{
				cout<<x<<" "<<ans<<" "<<itr->first<<endl;
				incorrect = true;
				break;
			}
		}
		else if(x==1)
		{
			// cout<<"PUT"<<endl;
			int k = rand()%64 + 1;
			int v = rand()%255 + 1;
			string key = random_key(k);
			string value = random_key(v);
			db[key] = value;
			Slice s_key,s_value;
			strToSlice(key,s_key);
			strToSlice(value,s_value);
			bool ans = kv.put(s_key,s_value);

			Slice check;
			bool check2 = kv.get(s_key,check);
			db_size = db.size();
			if(check2 == false || value != sliceToStr(check))
			{
				cout<<x<<" "<<check2<<" "<<key<<" "<<value<<" "<<db[key]<<endl;
				incorrect = true;
				break;
			}
		}
		else if(x==2)
		{
			int rem = rand()%db_size;
			map<string,string>:: iterator itr = db.begin();
			advance(itr,rem);
			string key = itr->first;
			Slice s_key,s_value;
			strToSlice(key,s_key);
			bool check = kv.del(s_key);
			db.erase(itr);
			db_size--;

			bool check2 = kv.get(s_key,s_value);
			if(check2 == true)
			{
				cout<<"UMM: "<<x<<" "<<check<<" "<<key<<endl;
				incorrect = true;
				break;
			}
		}
		else if(x==3)
		{
			int rem = rand()%db_size;
			Slice s_key,s_value;
			bool check = kv.get(rem,s_key,s_value);
			map<string,string>:: iterator itr = db.begin();
			for(int i=0;i<rem;i++)itr++;
			if( itr->first != sliceToStr(s_key) || itr->second != sliceToStr(s_value))
			{
				cout<<x<<endl;
				incorrect = true;
				break;
			}
		}
		else if(x==4)
		{
			int rem = rand()%db_size;
			map<string,string>:: iterator itr = db.begin();
			for(int i=0;i<rem;i++)itr++;
			string key = itr->first;
			bool check = kv.del(rem);
			db.erase(itr);
			db_size--;
			Slice s_key,s_value;
			strToSlice(key,s_key);
			bool check2 = kv.get(s_key,s_value);
			if(check2 == true)
			{
				cout<<x<<endl;
				incorrect = true;
				break;
			}
		}
	}
	if(incorrect == true)
	{
		cout<<0<<endl;
		return 0;
	}
	cout<<"worked"<<endl;
	int threads = 4;

	pthread_t tid[threads];
	for (int i = 0; i < threads; i++) 
	{
		tid[i] = i;
        pthread_create(&tid[i], NULL, myThreadFun, (void *)&tid[i]); 
	}
	for(int i=0;i<threads;i++)
		pthread_join(tid[i],NULL);
	return 0;
}