#include <fstream>
#include <iostream>
#include <locale>
#include <clocale>
#include <vector>
#include <set>
using namespace std;

int main() {
	set<wstring> s1;

	//open the new stream with the difficult words
	std::wifstream fin("words_features.txt", std::ifstream::in);	
	std::locale loc ("");
	std::locale::global (loc);   
	fin.imbue (loc);

	for(wstring s; getline(fin,s); )
		s1.insert(s);

	//open the new stream with the difficult words
	std::wifstream fin2("../sets/easy_words.txt", std::ifstream::in);	
	fin2.imbue (loc);

	for(wstring s; getline(fin2,s); )
		if (s1.count(s) == 0) wcout << s << L"\n";
	
}
