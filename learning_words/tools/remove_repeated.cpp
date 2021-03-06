#include <fstream>
#include <iostream>
#include <locale>
#include <clocale>
#include <vector>
#include <ctime>
#include <set>
using namespace std;

int main() {

	// open the stream
	//std::wifstream fdict("words-lemas_processed.txt", std::ifstream::in);	

	std::wifstream fdict("../sets/easy_words.txt", std::ifstream::in);
	std::locale loc ("");
	std::locale::global (loc);   
	fdict.imbue (loc);
	
	set<wstring> con;
	
	// read and insert the dictionary   
	for(wstring s; getline(fdict,s); ) {
		con.insert(s);
	}

	fdict.close();

	set<wstring>::iterator it = con.begin();
	while (it!=con.end()) {
		wcout << (*it) << L"\n";
		it++;
	}
}
