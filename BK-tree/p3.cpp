#include <fstream>
#include <iostream>
#include <locale>
#include <clocale>
#include <vector>
#include "bk_tree.cc"
using namespace std;

typedef vector<vector<int> > Matrix;

wstring vowels_list;
vector<vector<wstring> > classes_list;

struct info_word {
	wstring word;
	int d1;
	int d2;
	int vowels;
	bool has_accent;
	vector<int> classes;
	int length;
	bool has_h;
	bool has_double_letter;
	bool difficult;
};

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
			if (i>1 and j>1 and a[i-1] == b[j-2] and a[i-2] == b[j-1])//transposition
				m[i][j] = min(m[i][j],m[i-2][j-2]+cost);			
		}
	return m[a.size()][b.size()];		
}

int num_vowels(wstring s) {
	int nvowels = 0;
	for (int i=0; i<s.size(); i++) 
		for (int j=0; j<vowels_list.size(); j++)
			if (s[i] == vowels_list[j]) {				
				nvowels++;
				break;
			}
			
	return nvowels;
}

bool has_accent(wstring s) {
	for (int i=0; i<s.size(); i++)
		for (int j=5; j<vowels_list.size(); j++) 
			if (s[i] == vowels_list[j]) return true;
		
	return false;
}

bool has_h(wstring s) {
	for (int i=0; i<s.size(); i++) {
		if (s[i] == 'h') return true;
	}
	return false;
}

bool has_double_letter(wstring s) {
	int d = 0;
	for (int i=0; i<s.size()-1; i++) {
		if (s[i] == s[i+1]) return true;
	}
	return false;
}

int length(wstring s) {
	return s.size();
}

vector<int> get_classes(wstring s) {
	wstring b_string = L"b";
	wstring s_string = L"s";
	vector<int> w(classes_list.size());
	for (int k=0; k<s.size(); k++) {
		bool b = false;
		for (int i=0; not b and i<classes_list.size(); i++) {
			for (int j=0; not b and j<classes_list[i].size(); j++) {
				if (classes_list[i][j].size() == 1) {
					if (s[k] == classes_list[i][j][0]) {
						w[i]++;
						if (s[k] == b_string[0]) w[8]++; //to update [b,d] at the time I update [b,v]
						if (s[k] == s_string[0]) w[5]++; //to update [r,s,l] at the time I update [s,z,ce,ci,x]
						b = true;
					}
				}
				else {
					if (k<s.size()-1 and s[k] == classes_list[i][j][0] and s[k+1] == classes_list[i][j][1]) {
						w[i]++;
						b = true;
					}
				}
			}
		}
	}
	return w;
}

void process_diff(const bk_tree<wstring>& bk, vector<info_word>& v, wstring s) {
	info_word info;
	info.word = s;
	info.vowels = num_vowels(s);
	info.has_accent = has_accent(s);
	info.classes = get_classes(s);
	info.d1 = bk.count_within_distance(s,1);
	info.d2 = bk.count_within_distance(s,2);
	info.length = length(s);
	info.has_h = has_h(s);
	info.has_double_letter = has_double_letter(s);
	info.difficult = true;
	v.push_back(info);
}

void process_easy(const bk_tree<wstring>& bk, vector<info_word>& v, wstring s) {
	info_word info;
	info.word = s;
	info.vowels = num_vowels(s);
	info.has_accent = has_accent(s);
	info.classes = get_classes(s);
	info.d1 = bk.count_within_distance(s,1);
	info.d2 = bk.count_within_distance(s,2);
	info.length = length(s);
	info.has_h = has_h(s);
	info.has_double_letter = has_double_letter(s);
	info.difficult = false;
	v.push_back(info);
}

void init_classes_list() {
	vector<wstring> v1;
	v1.push_back(L"b");
	v1.push_back(L"v");
	classes_list.push_back(v1);
	vector<wstring> v2;
	v2.push_back(L"g");
	v2.push_back(L"j");
	classes_list.push_back(v2);
	vector<wstring> v3;
	v3.push_back(L"h");
	classes_list.push_back(v3);
	vector<wstring> v4;
	v4.push_back(L"s");
	v4.push_back(L"z");
	v4.push_back(L"ce");
	v4.push_back(L"ci");
	v4.push_back(L"x");
	classes_list.push_back(v4);
	vector<wstring> v5;
	v5.push_back(L"y");
	classes_list.push_back(v5);
	vector<wstring> v6;
	v6.push_back(L"r");
	v6.push_back(L"s");
	v6.push_back(L"l");
	classes_list.push_back(v6);
	vector<wstring> v7;
	v7.push_back(L"m");
	v7.push_back(L"n");
	v7.push_back(L"ñ");
	classes_list.push_back(v7);
	vector<wstring> v8;
	v8.push_back(L"ca");
	v8.push_back(L"co");
	v8.push_back(L"cu");
	v8.push_back(L"q");
	v8.push_back(L"k");
	classes_list.push_back(v8);
	vector<wstring> v9;
	v9.push_back(L"b");
	v9.push_back(L"d");
	classes_list.push_back(v9);
}

void print(const vector<info_word>& v) {
	for (int i=0; i<v.size(); i++) {
		info_word info = v[i];
		wcout << info.word << L"\n";
		wcout << L"#distance 1: " << info.d1 << L"\n";
		wcout << L"#distance 2: " << info.d2 << L"\n";
		wcout << L"#vowels: " << info.vowels << L"\n";
		if (info.has_accent)	wcout << L"accent: yes\n";
		else wcout << L"accent: no\n";
		for (int j=0; j<info.classes.size(); j++) {
			if (j == 0) wcout << L"#[b.v]: ";
			else if (j == 1) wcout << L"  #[g,j]: ";
			else if (j == 2) wcout << L"  #[h]: ";
			else if (j == 3) wcout << L"  #[s,z,ce,ci,x]: ";
			else if (j == 4) wcout << L"  #[y]: ";
			else if (j == 5) wcout << L"  #[r,s,l]: ";
			else if (j == 6) wcout << L"  #[m,n,ñ]: ";
			else if (j == 7) wcout << L"  #[ca,co,cu,q,k]: ";
			else if (j == 8) wcout << L"  #[b,d]: ";
			wcout << info.classes[j] << L";";
		}
		wcout << L"\n" << L"###########################################################################################################################" << L"\n\n";
	}	
}

void print_csv(const vector<info_word>& v) {
	//print the labels
	wcout << L"word, #distance 1, #distance 2, #vowels, has_accent?, #[b,v], #[g,j], #[h], #[s,z,ce,ci,x], #[y], #[r,s,l], #[m,n,ñ], #[ca,co,cu,q,k], #[b,d]\n";
	for (int i=0; i<v.size(); i++) {
		info_word info = v[i];
		wcout << info.word;
		wcout << L", " << info.d1;
		wcout << L", " << info.d2;
		wcout << L", " << info.vowels;
		if (info.has_accent)	wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		for (int j=0; j<info.classes.size(); j++)
			wcout << L", " << info.classes[j];
		
		wcout << L"\n";
	}	
}

void print_arff(const vector<info_word>& v) {
	//print the labels
	for (int i=0; i<v.size(); i++) {
		info_word info = v[i];
		wcout << info.d1;
		wcout << L", " << info.d2;
		wcout << L", " << info.vowels;
		//if (info.has_accent)	wcout << L", " << L"yes";
		//else wcout << L", " << L"no";
		if (info.classes[0] > 1) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[1] > 1) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[2] > 0) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[3] > 2) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[4] > 0) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[5] > 3) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[6] > 2) wcout << L", " << L"yes";
		else wcout << L", " << L"no";
		if (info.classes[8] > 1) wcout << L", " << L"yes";
		else wcout << L", " << L"no";	
		
		wcout << L", " << info.length;
		//if (info.has_h) wcout << L", " << L"yes";
		//else wcout << L", " << L"no";
		//if (info.has_double_letter) wcout << L", " << L"yes";
		//else wcout << L", " << L"no";
		if (info.difficult) wcout << L", " << L"yes";//classification label meaning "difficult word"
		else wcout << L", " << L"no";
		wcout << L"\n";
	}	
}

int main()
{
	

	// open the stream
	std::wifstream fdict("words-lemas_processed.txt", std::ifstream::in);
	std::locale loc ("");
	std::locale::global (loc);   
	fdict.imbue (loc);
	
	//init parameters	
	vowels_list = L"aeiouáéíóú";
	init_classes_list();

	//create the BK-tree
	bk_tree<wstring> bk = bk_tree<wstring>(dist);

	// read and insert the dictionary   
	for(wstring s; fdict >> s; )
		bk.insert(s);
	
	//close this stream
	fdict.close();

	//open the new stream with the difficult words
	std::wifstream fin("../learning_words/sets/difficult_words_unique_artificially_balanced_down6.txt", std::ifstream::in);	
	fin.imbue (loc);

	//process the difficult words
	vector<info_word> v;
	for(wstring s; getline(fin,s); )
		process_diff(bk,v,s);

	//open the new stream with the easy words
	std::wifstream fin2("../learning_words/sets/easy_words2.txt", std::ifstream::in);	
	fin2.imbue (loc);

	//process the easy words
	for(wstring s; getline(fin2,s); )
		process_easy(bk,v,s);

	//close this stream
	fin.close();
	fin2.close();

	print_arff(v);	
}
