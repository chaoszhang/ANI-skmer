#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <unordered_set>
#define LENTH 200

using namespace std;

/*string randString(){
	string str("ACGT");

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis( 0, 3);

	for (int i = 0; i< 2621440; i++){

	}
	
}


static void getNum (int *dst) {
    int sz, pos, i, src[10005760];
    for (i = 0; i < sizeof(src)/sizeof(*src); i++)
        src[i] = i + 1;
    sz = 10005760;
    for (i = 0; i < 100057; i++) {
        pos = rand() % sz;
        dst[i] = src[pos];
        src[pos] = src[sz-1];
        sz--;
    }
}*/

int main(){
	ofstream outputFile1;
	ofstream outputFile2;
	outputFile1.open("randomS1.txt");
	outputFile2.open("randomS2.txt");

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis( 0, 3);
	uniform_int_distribution<> dis2( 0, 99);

	srand(time(0));

	int count = 0;

	for (int i = 0; i<10005760; i++){

			char op1;
			char op2;
			int test1 = dis(gen);
			int test2 = test1;

			if(true){
				if(dis2(gen)==15){
					while(test2==test1){
						test2=dis(gen);
					}
					count++;
				}
			}

			if(test1==0){
				op1='A';
				
			}else if(test1==1){
				op1='C';
				
			}else if(test1==2){
				op1='G';
				
			}else if(test1==3){
				op1='T';
				
			}else{
				cerr<<"there is an error when converting the string1! string1 is: "<<test1<<endl;
			}

			if(test2==0){
				op2='A';
				
			}else if(test2==1){
				op2='C';
				
			}else if(test2==2){
				op2='G';
				
			}else if(test2==3){
				op2='T';
				
			}else{
				cerr<<"there is an error when converting the string2! string2 is:"<<test2<<endl;
			}

			outputFile1<<op1;
			outputFile2<<op2;
	}

	if(count<100057){
		cerr<<"less then 1%, the count is: "<<count<<endl;
	}
	if(count==100057){
		cerr<<"file2 successfully modified"<<endl;
	}
	if(count>100057){
		cerr<<"count: "<<count<<endl;
	}


	outputFile1.close();
	outputFile2.close();
   

}


















