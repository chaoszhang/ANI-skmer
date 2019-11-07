#include "ACGTkmer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <utility>


using namespace std;

class SeqReader{
	ifstream fin;
	string filename;
	
public:
	
	SeqReader(string file): filename(file), fin(file){}
	
	void reopen(){
		fin.close();
		fin.open(filename);
	}
	
	string next(){
		string dummy, seq;
		if (getline(fin, dummy) && getline(fin, seq) && getline(fin, dummy) && getline(fin,dummy)) return seq;
		return "";
	}
};

template<int K, int N, int X> class SizeEstimator{
	uint64_t a[N], x[N], y[N], Y = 1;
	
	static uint64_t random(){
		return (((uint64_t) rand()) << 60) ^ (((uint64_t) rand()) << 45) ^ (((uint64_t) rand()) << 30) ^ (((uint64_t) rand()) << 15) ^ ((uint64_t) rand());
	}
	
public:
	
	SizeEstimator(){
		for (int i = 0; i < N; i++) x[i] = 2 * random() + 1;
		for (int i = 0; i < N; i++) y[i] = random();
		for (int i = 0; i < N; i++) a[i] = ~0LL;
		for (int i = 0; i < N; i++) Y *= X;
	}
	
	void process(const string s){
		int b = 0;
		for (int i = 0; i < 2 * K + 1; i++){
			b = b * X + s[i];
		}
		for (int j = 0; j < N; j++) a[j] = min(a[j], b * x[j] + y[j]);
		for (int i = 2 * K + 1; i < s.size(); i++){
			b = b * X + s[i] - s[i - 2 * K - 1] * Y;
			for (int j = 0; j < N; j++) a[j] = min(a[j], b * x[j] + y[j]);
		}
	}
	
	uint64_t estimate(){
		sort(a, a + N);
		return a[N / 2];
	}
};

int main(int argc, char** argv) {
	///*
	ifstream fin1("randomS1.txt"), fin2("randomS2.txt");
	string s1, s2;
	fin1 >> s1;
	fin2 >> s2;
	
	ACGTkmer<13> test(~0);
	test.add2set(s1, true);
	test.add2set(s2, false);
	test.compare();
	
	return 0;
	//*/
	
	const int K = 13;
	
	SeqReader f1("../../Drosophila_ananassae.fq"), f2("../../Drosophila_biarmipes.fq");
	
	SizeEstimator<K, 16, 5> se;
	string s = f1.next();
	while (s != ""){
		se.process(s);
		s = f1.next();
	}
	s = f2.next();
	while (s != ""){
		se.process(s);
		s = f2.next();
	}
	unsigned th = min( 3 * se.estimate() >> (32 - 28 + 2), (uint64_t) ~((unsigned) 0));
	cerr << "threshold = " << th << endl;
	double r1, r2, r;
	{
		ACGTkmer<K> kmer(th);
		f1.reopen();
		s = f1.next();
		while (s != ""){
			kmer.add2set(s, rand() & 1);
			s = f1.next();
		}
		auto p = kmer.compare();
		r1 = 1 - p.second / (double) p.first;
		cerr << "r1 = " << r1 << endl;
	}
	{
		ACGTkmer<K> kmer(th);
		f2.reopen();
		s = f2.next();
		while (s != ""){
			kmer.add2set(s, rand() & 1);
			s = f2.next();
		}
		auto p = kmer.compare();
		r2 = 1 - p.second / (double) p.first;
		cerr << "r2 = " << r2 << endl;
	}
	{
		ACGTkmer<K> kmer(th);
		f1.reopen();
		s = f1.next();
		while (s != ""){
			kmer.add2set(s, true);
			s = f1.next();
		}
		f2.reopen();
		s = f2.next();
		while (s != ""){
			kmer.add2set(s, false);
			s = f2.next();
		}
		auto p = kmer.compare();
		r = 1 - p.second / (double) p.first;
		cerr << "r = " << r << endl;
	}
	cout << (1 - (1 - r / 0.75) / sqrt((1 - r1 / 0.75) * (1 - r2 / 0.75))) * 0.75;
	return 0;
}


