#include <stdafx.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#define NrMaxStari 20005
#define Q 10005
using namespace std;
ifstream fin("LFA.in");
ofstream fout("LFA.out");
bool ok = 0;
void Citire(int &StareInit, int  &NrStariFin, int  &NrLitere, int &NrStari, int &NrMuchii, vector <int> &stare, vector <char> &litera, map <int, bool> &StareFin, vector <vector <pair <int, char>>> &muchie)
{
	int str;
	pair <int, char> p;
	char c;
	//map <int, >
	fin >> NrStari;

	for (int i = 0; i < NrStari; i++)
	{
		fin >> str;
		if (ok == 1)str += Q; //cout << str<<" ";
		stare.push_back(str);
	}

	fin >> NrLitere;

	for (int i = 0; i < NrLitere; i++)
	{
		fin >> c;
		litera.push_back(c);
	}

	fin >> StareInit;
	if (ok == 1)StareInit += Q;
	fin >> NrStariFin;
	for (int i = 0; i < NrStariFin; i++)
	{
		fin >> str;
		if (ok == 1)str += Q;
		StareFin[str] = true;
	}

	fin >> NrMuchii;
	//cout << NrMuchii;
	muchie.resize(NrMaxStari);
	for (int i = 0; i < NrMuchii; i++)
	{
		fin >> str >> p.second >> p.first;
		if (ok == 1)p.first += Q;
		if (ok == 1)str += Q;
		//cout <<str<<" "<< p.second << " " << p.first<<"\n";
		muchie[str].push_back(p);
	}
}

void InchidereLambdaX(int stareInit, int stare, map <int, vector<int>> &inchidX, vector <vector <pair <int, char>>> muchie, bool viz[])
{
	viz[stare] = 1;
	//cout << stare << "\n";
	inchidX[stareInit].push_back(stareInit);
	for (int i = 0; i < muchie[stare].size(); i++)
	{
		if (muchie[stare][i].second == '.' && viz[muchie[stare][i].first] == 0)
		{
			inchidX[stareInit].push_back(muchie[stare][i].first);
			InchidereLambdaX(stareInit, muchie[stare][i].first, inchidX, muchie, viz);
			//cout << stare<<"\n";
		}
	}
}

void InchideriLambda(int NrStari, vector <int>stare, map <int, vector<int>> &inchidX, vector <vector <pair <int, char>>> muchie)
{
	bool viz[NrMaxStari];
	for (int i = 0; i < NrStari; i++)
	{
		for (int j = 0; j < NrStari; j++)
			viz[stare[j]] = 0;
		//cout << "\n" << stare[i] << ": ";
		InchidereLambdaX(stare[i], stare[i], inchidX, muchie, viz);
		sort(inchidX[stare[i]].begin(), inchidX[stare[i]].end());
	}

}

void Transformare(map < vector<int>, vector<pair<int, char>>>& DFA, map <int, vector<int>> inchidX, vector <int>stare, int NrLitere, vector <char> &litera, vector <vector <pair <int, char>>> muchie)
{
	int ct = 0;
	vector<pair<int, char>>temp;
	for (int k = 0; k < NrLitere; k++)
	{
		for (int i = 0; i < stare.size(); i++)
		{
			for (int j = 0; j < inchidX[stare[i]].size(); j++)
			{
				temp.push_back({ inchidX[stare[i]][j],litera[k] });
				ct++;
			}
		}
	}
	sort(temp.begin(), temp.end());

	for (int i = 1; i < ct; ++i)
		if (temp[i - 1].first == temp[i].first && temp[i - 1].second == temp[i].second)
		{
			temp.erase(temp.begin() + i);
			--i;
			--ct;
		}

	for (int i = 0; i < ct; i++)
	{
		for (int j = 0; j < muchie[temp[i].first].size(); j++)
		{
			if (muchie[temp[i].first][j].second != '.')
				temp.push_back(muchie[temp[i].first][j]);

		}
	}

	for (int i = 0; i < ct; ++i)
	{
		temp.erase(temp.begin() + i);
		--i;
		--ct;
	}
	ct = temp.size();
	sort(temp.begin(), temp.end());
	for (int i = 1; i < ct; ++i)
		if (temp[i - 1].first == temp[i].first && temp[i - 1].second == temp[i].second)
		{
			temp.erase(temp.begin() + i);
			--i;
			--ct;
		}

	ct = temp.size();
	//for(int k=0;k<NrLitere;k++)
	for (int i = 0; i < ct; ++i)
		for (int j = 0; j < inchidX[temp[i].first].size(); j++)
		{
			temp.push_back({ inchidX[temp[i].first][j], temp[i].second });
		}

	sort(temp.begin(), temp.end());
	ct = temp.size();
	for (int i = 1; i < ct; ++i)
		if (temp[i - 1].first == temp[i].first && temp[i - 1].second == temp[i].second)
		{
			temp.erase(temp.begin() + i);
			--i;
			--ct;
		}

	for (int i = 0; i < temp.size(); ++i)
	{
		DFA[stare].push_back(temp[i]);
		//cout << temp[i].first << " " << temp[i].second << "\n";
	}
	//cout << "\n";
}

void DFS(vector<vector<int>> &stareDFA, map < vector<int>, vector<pair<int, char>>>& DFA, map<vector<int>, bool>&viz, map <int, vector<int>> inchidX, vector<int> stare, int NrLitere, vector <char>litera, vector <vector<pair <int, char>>> muchie, int &ct)
{
	//map < vector<int>, vector<pair<int, char>>> DFA;
	vector <int> temp;
	//for (int i = 0; i < stare.size(); i++)
	//	cout << stare[i] << " ";
	//cout << "\n";
	stareDFA.push_back(stare); ct++;
	viz[stare] = 1;
	//for (int i = 0; i < stareDFA.size(); i++)
	//	if (StareFin[stareDFA[ct - 1][i]] == 1)StareFinDFA[stareDFA[ct - 1]] = 1;
	Transformare(DFA, inchidX, stare, NrLitere, litera, muchie);
	for (int i = 0; i < DFA[stare].size(); i++)
	{
		//mp.push_back(DFA[stare][i].first);
		if (i > 0 && DFA[stare][i].second != DFA[stare][i - 1].second)
		{
			if (viz[temp] == 0)
				DFS(stareDFA, DFA, viz, inchidX, temp, NrLitere, litera, muchie, ct);
			temp.clear();
		}
		temp.push_back(DFA[stare][i].first);
	}
	if (DFA[stare].size() > 0 && viz[temp] == 0)DFS(stareDFA, DFA, viz, inchidX, temp, NrLitere, litera, muchie, ct);

}
void Rezolvare()
{
	int StareInit, NrStariFin, NrLitere, NrStari, NrMuchii, ct, NrStariFinDFA = 0;
	vector <int>stare;
	vector <char>litera;
	vector<vector<int>> stareDFA;
	map <int, bool>StareFin;
	map <vector<int>, bool>StareFinDFA;
	vector <vector<pair <int, char>>> muchie;
	map <int, vector<int>> inchidX;
	map < vector<int>, vector<pair<int, char>>> DFA;
	vector<int> starep;
	vector<int> temp;
	map<vector<int>, bool>viz;
	map<vector<int>, int>DFAN;
	Citire(StareInit, NrStariFin, NrLitere, NrStari, NrMuchii, stare, litera, StareFin, muchie);
	ok = 1;

	int StareInit2, NrStariFin2, NrLitere2, NrStari2, NrMuchii2, ct2, NrStariFinDFA2 = 0;
	vector <int>stare2;
	vector <char>litera2;
	vector<vector<int>> stareDFA2;
	map <int, bool>StareFin2;
	map <vector<int>, bool>StareFinDFA2;
	vector <vector<pair <int, char>>> muchie2;
	map <int, vector<int>> inchidX2;
	map < vector<int>, vector<pair<int, char>>> DFA2;
	vector<int> starep2;
	vector<int> temp2;
	map<vector<int>, bool>viz2;
	map<vector<int>, int>DFAN2;
	Citire(StareInit2, NrStariFin2, NrLitere2, NrStari2, NrMuchii2, stare2, litera2, StareFin2, muchie2);
	//StareInit = 0;

	for (int i = NrStari; i < NrStari2 + NrStari; i++)
		stare.push_back(stare2[i - NrStari]);
	NrStari += NrStari2;
	NrStari++;
	stare.push_back(0);
	for (int i = NrLitere; i < NrLitere2 + NrLitere; i++)
		litera.push_back(litera2[i - NrLitere]);
	NrLitere += NrLitere2;
	ct = NrLitere;
	//cout << ct;
	sort(litera.begin(), litera.end());
	for (int i = 1; i < ct; ++i)
		if (litera[i - 1] == litera[i])
		{
			cout << char(litera[i - 1]) << " ";
			litera.erase(litera.begin() + i);
			--i;
			--ct;
		}
	NrLitere = ct;
	ct = 0;
	for (int i = 0; i < NrStari; i++)
		if (StareFin2[stare[i]] == 1)StareFin[stare[i]] = 1;
	NrStariFin += NrStariFin2;
	for (int i = NrStari - NrStari2; i < NrStari; i++)
	{
		for (int j = 0; j < muchie2[stare[i]].size(); j++)
			muchie[stare[i]].push_back(muchie2[stare[i]][j]);
	}
	NrMuchii += NrMuchii2;
	NrMuchii += 2;
	fout << StareInit << " " << StareInit2 << "\n";
	muchie[0].push_back({ StareInit,'.' });
	muchie[0].push_back({ StareInit2,'.' });
	StareInit = 0;
	//stare[0] = 1;
	fout << NrStariFin<<"\n";
	fout << NrLitere;
	for (int i = 0; i < NrStari; i++)
	{  for(int j=0;j<muchie[stare[i]].size();j++)
		fout<<stare[i]<<" "<<muchie[stare[i]][j].second<<" "<<muchie[stare[i]][j].first<<"\n";
    }
	fout << StareInit;

	InchideriLambda(NrStari, stare, inchidX, muchie);
	for (int i = 0; i < inchidX[StareInit].size(); i++)
	{
		if (StareFin[inchidX[StareInit][i]] == 1)StareFin[StareInit] = 1;
	}
	starep.push_back(StareInit);
	//Transformare(DFA, inchidX, starep, NrLitere, litera, muchie);
	
	DFS(stareDFA, DFA, viz, inchidX, starep, NrLitere, litera, muchie, ct);
	ct = 0;
	for (int i = 0; i < stareDFA.size(); i++)
	{
		ct++;
		DFAN[stareDFA[i]] = ct;
		for (int j = 0; j < stareDFA[i].size(); j++)
		{
			if (StareFin[stareDFA[i][j]] == 1) {
				StareFinDFA[stareDFA[i]] = 1; //cout << "**"<<<<"**";
				NrStariFinDFA++; break;
			}
			//NrStariFinDFA++;
		}
		//cout << "\n";
	}



	fout << stareDFA.size() << "\n";
	for (int i = 0; i < stareDFA.size(); i++)
		fout << DFAN[stareDFA[i]] << " ";
	fout << "\n";
	fout << NrLitere << "\n";
	for (int i = 0; i < NrLitere; i++)
		fout << litera[i] << " ";
	fout << "\n";
	starep.clear(); starep.push_back(StareInit);
	fout << DFAN[starep] << "\n";
	fout << NrStariFinDFA << "\n";
	for (int i = 0; i < stareDFA.size(); i++)
		if (StareFinDFA[stareDFA[i]] == 1)fout << DFAN[stareDFA[i]] << " ";
	//cout << stareDFA[NrStari][0];
	fout << "\n";
	//fout << stareDFA.size();
	ct = 0;
	for (int i = 0; i < stareDFA.size(); i++)
	{
		temp.clear();
		sort(DFA[stareDFA[i]].begin(), DFA[stareDFA[i]].end());
		for (int j = 0; j < DFA[stareDFA[i]].size(); j++)
		{
			if (j > 0 && DFA[stareDFA[i]][j].second != DFA[stareDFA[i]][j - 1].second)
			{
				//cout << DFAN[stareDFA[i]] << " ";
				//for (int k = 0; k < temp.size(); k++)
					//cout << temp[k]<<" ";
					//fout << DFAN[stareDFA[i]] << " " << DFA[stareDFA[i]][j - 1].second << " " << DFAN[temp] << "\n";
				ct++;
				temp.clear(); //cout << "\n";
			}
			temp.push_back(DFA[stareDFA[i]][j].first);
			//cout << temp[temp.size()-1] << " ";
		}
		//cout << "\n";
		if (DFA[stareDFA[i]].size() > 0)//fout << DFAN[stareDFA[i]] << " " << DFA[stareDFA[i]][DFA[stareDFA[i]].size() - 1].second << " " << DFAN[temp] << "\n";
			ct++;
	}

	fout << ct << "\n";
	for (int i = 0; i < stareDFA.size(); i++)
	{
		temp.clear();
		sort(DFA[stareDFA[i]].begin(), DFA[stareDFA[i]].end());
		for (int j = 0; j < DFA[stareDFA[i]].size(); j++)
		{
			if (j > 0 && DFA[stareDFA[i]][j].second != DFA[stareDFA[i]][j - 1].second)
			{
				//cout << DFAN[stareDFA[i]] << " ";
				//for (int k = 0; k < temp.size(); k++)
					//cout << temp[k]<<" ";
				fout << DFAN[stareDFA[i]] << " " << DFA[stareDFA[i]][j - 1].second << " " << DFAN[temp] << "\n";
				temp.clear(); //cout << "\n";
			}
			temp.push_back(DFA[stareDFA[i]][j].first);
			//cout << temp[temp.size()-1] << " ";
		}
		//cout << "\n";
		if (DFA[stareDFA[i]].size() > 0)fout << DFAN[stareDFA[i]] << " " << DFA[stareDFA[i]][DFA[stareDFA[i]].size() - 1].second << " " << DFAN[temp] << "\n";
	}
	//cout << stareDFA.size();
	

}

int main()
{
	Rezolvare();
	system("Pause");
}
