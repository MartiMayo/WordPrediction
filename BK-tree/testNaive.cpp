#include <fstream>
#include <iostream>
#include <locale>
#include <clocale>
#include <vector>
using namespace std;
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

int main() {
	// open the stream
	std::wifstream fdict("words-lemas_processed.txt", std::ifstream::in);
	std::locale loc ("");
	std::locale::global (loc);   
	fdict.imbue (loc);
	
	//create the vector to store the dataset
	vector<wstring> v;

	// read and insert the dictionary   
	for(wstring s; fdict >> s; )
		v.push_back(s);
	
	//close this stream
	fdict.close();

	//open the new stream with the difficult words
	std::wifstream fin("utf8.dat", std::ifstream::in);	
	fin.imbue (loc);

	//process the difficult words
	for(wstring s; fin >> s; ) {		
		int count = 0;
		for (int i=0; i<v.size(); i++) {
			if (dist(s,v[i]) <= 2) count++;
		}
		cout << count << endl;
	}
		

	//close this stream
	fin.close();
	
}
