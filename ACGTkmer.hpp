#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

/**
* Character to integer
* A==0, C==1, G==2, T==C
*/
inline uint64_t c2i(char c){
	if (c == 'A') return 0;
	if (c == 'C') return 1;
	if (c == 'G') return 2;
	return 3;
}

template<int K, size_t X, size_t Y, size_t Z> struct Hasher{
	unsigned operator()(uint64_t b) const{
		return (b * X + (b >> (2 * K)) * Y + Z) >> 2;
	}
};

template<int K, int N> class HashTable{
	const unsigned MASK = N - 1;
	const int CNT[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
	unsigned threshold;
	uint64_t* table = new uint64_t[N]{};
	
	Hasher<K, 127, 65537, 2147483647> thresholdHasher;
	Hasher<K, 263, 32771, 104677> hasher;
	Hasher<K, 67, 48661, 8093> rehasher;
	
public:
	HashTable(unsigned t): threshold(t){}
	
	void insert(uint64_t b, uint64_t c, bool isFirstSet){
		if (thresholdHasher(b) > threshold) return;
		unsigned h = hasher(b) & MASK;
		if (table[h] == 0 || (table[h] >> 9) == b) {
			table[h] |= (b << 9) | (1 << 8) | (1 << (c + isFirstSet * 4));
			return;
		}
		unsigned r = rehasher(b) * 2 + 1, d = r;
		for (h = (h + d) & MASK; !(table[h] == 0 || (table[h] >> 9) == b); d += r, h = (h + d) & MASK);
		table[h] |= (b << 9) | (1 << 8) | (1 << (c + isFirstSet * 4));
	}
	
	pair<int, int> compare() const{
		int cknk = 0, ck1k = 0;
		for (int i = 0; i < N; i++){
			const uint64_t v = table[i];
			if (v == 0) continue;
			int s1 = v & 15, s2 = (v >> 4) & 15;
			
			/*
			if (CNT[s1] != 1 || CNT[s2] != 1) continue;
			cknk ++;
			ck1k += CNT[s1 & s2];
			*/
			///*
			if (s1 == 0 || s2 == 0) continue;
			cknk += max(CNT[s1], CNT[s2]);
			ck1k += CNT[s1 & s2];
			//*/
		}
		cerr << cknk << " " << ck1k << endl;
		return {cknk, ck1k};
	}
	
	~HashTable(){
		delete table;
	}
};

template<int K> class ACGTkmer{
	HashTable<K, 1 << 28> table;
	
public:
	
	ACGTkmer(unsigned threshold): table(threshold){}
	/**
	* Insert the strings to set
	* q - string to be modified
	* s - unordered_set that contains the int value of the string
	* k - user designate value used to split the string
	*/
	void add2set(const string& q, bool isFirstSet){
		const uint64_t MASK = (1 << (2 * K)) - 1;
		uint64_t L = 0, R = 0, m;
		for (int i = K + 1; i < 2 * K; i++){
			L = (L << 2) | c2i(q[i - (K + 1)]);
			R = (((3 ^ c2i(q[i])) << (2 * K)) | R) >> 2;
		}
		
		for (int i = 2 * K; i < q.size(); i++){
			L = ((L << 2) | c2i(q[i - (K + 1)])) & MASK;
			R = (((3 ^ c2i(q[i])) << (2 * K)) | R) >> 2;
			m = c2i(q[i - K]);
			if (L < R) table.insert(L << (2 * K) | R, m, isFirstSet);
			if (L > R) table.insert(R << (2 * K) | L, 3 ^ m, isFirstSet);
		}
	}
	
	/**
	* number of same knk and k1k
	*/
	pair<int, int> compare() const{
		return table.compare();
	}
};

