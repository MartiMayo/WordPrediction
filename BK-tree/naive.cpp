#include <fstream>
#include <iostream>
#include <locale>
#include <clocale>
#include <vector>
#include <ctime>
using namespace std;

typedef vector<vector<int> > Matrix;

int min(int a, int b) {
	if (a<b) return a;
	return b;
}

int minim(int a, int b, int c) {
	return min(min(a,b),min(a,c));
}

int dist(const wstring& a, const wstring& b) {
	Matrix m(a.size()+1,vector<int> (b.size()+1));
	int cost;
	m[0][0] = 0;
	for (int i=1; i<=a.size(); i++) m[i][0] = i;
	for (int j=1; j<=b.size(); j++) m[0][j] = j;
	for (int i=1; i<=a.size(); i++)
		for (int j=1; j<=b.size(); j++) {
			if (a[i-1] == b[j-1]) cost = 0;
			else cost = 1;
		
			m[i][j] = minim(m[i-1][j-1]+cost,m[i-1][j]+1,m[i][j-1]+1); //substitution, deletion and insertion			
		}
	return m[a.size()][b.size()];		
}

int count_within_distance(const vector<wstring>& v, wstring s, int k) {
	int conta = 0;
	for (int i=0; i<v.size(); i++) {
		if (dist(s,v[i]) <= k) conta++;
	}
	return conta;
}

int main() {
	//  Start the building timer
	std::clock_t start_build;
    start_build = std::clock();

	// open the stream
	//std::wifstream fdict("words-lemas_processed.txt", std::ifstream::in);	

	std::wifstream fdict("100kwords-lemas_processed_unique.txt", std::ifstream::in);
	std::locale loc ("");
	std::locale::global (loc);   
	fdict.imbue (loc);

	//long long comp = 0;
	
	//create the BK-tree
	vector<wstring> v;

	// read and insert the dictionary   
	for(wstring s; getline(fdict,s); )
		v.push_back(s);
	
	//close this stream
	fdict.close();

	//open the new stream with the difficult words
	std::wifstream fin("../learning_words/sets/all_words_unique.txt", std::ifstream::in);	
	fin.imbue (loc);
	
    // output of the building timer
    std::cerr << "Building structure time: " << (std::clock() - start_build) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

    //std::cerr << "Building comparisons: " << comp << std::endl;
    //comp = 0;

	//  Start the process timer
	std::clock_t start_process;
     	start_process = std::clock();

	//process the difficult words
	for(wstring s; getline(fin,s); )
		cout << count_within_distance(v, s, 2) << endl;

	// output of the process timer
	std::cerr << "Process time: " << (std::clock() - start_process) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	//std::cerr << "Processing comparisons: " << comp << std::endl;

	//close this stream
	fin.close();
	
}
