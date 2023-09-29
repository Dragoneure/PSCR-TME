#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");

	vector<pair<int,string>> map;

	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
		// on affiche un mot "propre" sur 100
		cout << nombre_lu << ": "<< word << endl;

		int taillemax = map.size();
		bool test = false;
		for(int i = 0; i < taillemax ; ++i) {
		    if ((map[i].second) == word) {
		    	test = true;
		    	map[i].first += 1;
		    	break;
		    }
		}
		if (!test) {
			map.push_back({1,word});
			++nombre_lu;
		}
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

	int taillemax = map.size();
    for(int i = 0; i < taillemax ; ++i) {
	    if ((map[i].second) == "war" || (map[i].second) == "peace" || (map[i].second) == "toto") {
	        cout << map[i].second << " :" << map[i].first << endl;

	    }
	}

    return 0;
}


