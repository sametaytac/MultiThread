enum DIRECTION {WEST, EAST};

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <iterator>
#include "writeOutput.h" 
using namespace std;

class Train
{


	public:
		int atime;
		int duration;
		DIRECTION yon;
		int id;
		int cik;




};
pthread_cond_t condition;
pthread_mutex_t mutRail;
vector<Train*> inrail;
class Rail
{
	private:
		int *limit;
	public:
		

		Rail(int _limit){
			int* sam=&(_limit);
			this->limit=sam;
				pthread_mutex_init(&mutRail, NULL);

			}
		void setlimit(int _limit){
			int* sam=&(_limit);
			this->limit=sam;
			}
		int getlimit(){
			return *limit;
			}
		void Enter(DIRECTION d,int trainCode){
			writeOutput(1,trainCode,d);
			}
		void Exit(DIRECTION d,int trainCode){
						
			if(trainCode==inrail[0]->id)
			{
			writeOutput(2,trainCode,d);	
			inrail.erase(inrail.begin());
				
			for(int i=0;i<inrail.size();i++)
			{				if(inrail[0]->cik==1)
							{writeOutput(2,inrail[0]->id,d);
							inrail.erase(inrail.begin());
							i--;
			}
			else
			break;
			}
			}
			else
			{
			for(int i=0;i<inrail.size();i++)
				{
					if(inrail[i]->id==trainCode)
						{
						inrail[i]->cik=1;break;						
						}
				}
			}
		if(inrail.size()==0)
			pthread_cond_broadcast(&condition);	
		
		}
};

Rail* railway=new Rail(0);

//condition_variable cv;


int counter;
int threadCount=0;
pthread_mutex_t mut2;
void* threadFunc(void *tra) {
	
	
			
	Train* train = (Train *) tra;
	writeOutput(0,train->id,train->yon);

	
	if(inrail.size()==0){
		//pthread_mutex_lock( &(mut2) );

		

		pthread_mutex_lock( &(mutRail) );
				inrail.push_back(train);
		railway->Enter(train->yon,train->id);
		pthread_mutex_unlock( &(mutRail) );
		usleep(10000*train->duration);

	        railway->Exit(train->yon,train->id);
	}

	
	else if (inrail[0]->yon==train->yon)
		{


		

		pthread_mutex_lock( &(mutRail) );
				inrail.push_back(train);
		railway->Enter(train->yon,train->id);
		pthread_mutex_unlock( &(mutRail) );
		usleep(10000*train->duration);

	        railway->Exit(train->yon,train->id);
	       


		}
	else
	{
	
	
	
		pthread_cond_wait(&condition,&mut2);

	
		pthread_mutex_unlock( &(mut2) );

		pthread_mutex_lock( &(mutRail) );
				inrail.push_back(train);
		railway->Enter(train->yon,train->id);
		pthread_mutex_unlock( &(mutRail) );
		usleep(10000*train->duration);

	        railway->Exit(train->yon,train->id);

	}	

threadCount--;

	}








int main(int argc, char** argv){
int limit;
limit=atoi(argv[1]);
//ifstream f("input2.txt");
vector<Train*> trains;
//f>>limit;
string ay;
int c=0;
pthread_cond_init(&condition,0);

pthread_attr_t ptr;
 	pthread_attr_init(&ptr);
 	pthread_attr_setdetachstate(&ptr, PTHREAD_CREATE_DETACHED);
railway->setlimit(limit);

istream_iterator<char> it(cin), eos;
string str;
while(getline(cin,str) ){
string str1,str2,str3;

int pos;
pos = str.find(' ');
str1 = str.substr(0,pos);
Train* temp=new Train();
temp->cik=0;
temp->atime=atoi( str1.c_str() );
str = str.substr(pos+1);
pos = str.find(' ');
str2 = str.substr(0,pos);
temp->id=atoi( str2.c_str() );

str = str.substr(pos+1);
pos = str.find(' ');
ay = str.substr(0,pos);
if(ay[0]=='W')
	temp->yon=WEST;
else
	temp->yon=EAST;
c++;
str = str.substr(pos+1);
temp->duration=atoi( str.c_str() );;
trains.push_back((temp));
}


for(int i=0;i<trains.size();i++){
	
	//cout<<trains[i].atime<<" "<<trains[i].id<<" "<<trains[i].yon<<" "<<trains[i].duration<<endl;
	pthread_t pt;
usleep(1000*trains[i]->atime);
pthread_create(&pt, &ptr, threadFunc, (void*) trains[i]);
	threadCount++;
	}


while(threadCount != 0) {
			usleep(1);
		}




return 0;
}
