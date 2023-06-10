// ASD2gr4 Wieloch Kinga - Turystyka

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

void znajdz_ilosc_kursow(
	vector < pair <int, pair <int, int> > > dane,
	pair <int, pair <int, int> > transport
);

void stworz_mst(
	vector < pair <int, pair <int, int> > > dane,
	vector < pair <int, pair <int, int> > >& mst
);

vector <pair <int, int>> znajdz_las(vector < pair <int, pair <int, int> > > dane);

bool znajdz_wartosc(vector<pair <int, int>> las, int k);

int znajdz_zestaw_wierzcholkow(vector <pair <int, int>> rodzice, int i);

void polacz_zestaw_wierzcholkow(vector <pair <int, int>>& rodzice, int w1, int w2);

int znajdz_najmniejsza_wage(
	vector < pair <int, pair <int, int> > > mst,
	pair <int, pair <int, int> > transport
);

void zeruj_odwiedzone(bool* odwiedzone, int rozmiar);

bool dfs(vector<pair<int, int>>* mapa_polaczen, bool* visited, int& waga, int start, int koniec, bool pierwsza_iteracja);


int main()
{
	vector < pair <int, pair <int, int> > > dane;
	vector < pair <int, pair <int, int> > > mst;
	vector < pair <int, pair <int, int> > > szukane;

	float miastaF, drogiF;
	int miasta, drogi;
	int maksymalna_ilosc_miast = 100;
	int maksymalna_liczba_pasazerow = 1000000;

	for (;;) {
		cin >> miastaF >> drogiF;
		miasta = (int)miastaF;
		drogi = (int)drogiF;
		if (miasta >= maksymalna_ilosc_miast) {
			cout << "Za duza ilosc miast!" << endl;
			continue;
		}

		break;
	}

	float c1, c2, p;
	float s, e, t;

	for (int i = 0; i < drogi; i++) {
		cin >> c1;
		cin >> c2;
		cin >> p;

		if (p <= 1 || p >= maksymalna_liczba_pasazerow) {
			cout << "Niepoprawna przepustowosc linii (1 < przepustowosc linii < 1000000) (L:" << i + 1 << ") - prosze wpisac poprawna wartosc" << endl;
			i--;
			continue;
		}

		p = p * -1;

		dane.push_back({ (int)p, {(int)c1, (int)c2} });
	}

	sort(dane.begin(), dane.end());

	do {
		cin >> s;
		cin >> e;
		if (s == 0 && e == 0) {
			break;
		}

		cin >> t;
		if (t <= 1 || t >= maksymalna_liczba_pasazerow) {
			cout << "Niepoprawna ilosc pasazerow do przewozu (1 < ilosc pasazerow < 1000000) " << t << " - prosze wpisac poprawna wartosc" << endl;
			continue;
		}

		szukane.push_back({ (int)t, {(int)s, (int)e} });
	} while (s != 0 && e != 0);

	if (szukane.size() == 0) {
		cout << "Nie podano poprawnych kursow - koniec programu" << endl;
		return 0;
	}

	stworz_mst(dane, mst);
	for (auto& transport : szukane) {
		znajdz_ilosc_kursow(mst, transport);
	}

	return 0;
}

void znajdz_ilosc_kursow(
	vector < pair <int, pair <int, int> > > mst,
	pair <int, pair <int, int> > transport
) {
	int ilosc_pasazerow = transport.first;
	int najmniejsza_przepustowosc = ilosc_pasazerow;
	int ilosc_kursow = 0;

	najmniejsza_przepustowosc = znajdz_najmniejsza_wage(mst, transport);
	if (najmniejsza_przepustowosc == 0) {
		cout << "Wystapil blad. Nie znaleziono trasy" << endl;
		return;
	}

	najmniejsza_przepustowosc = najmniejsza_przepustowosc - 1; //bez kierowcy

	ilosc_kursow = ilosc_pasazerow / najmniejsza_przepustowosc;
	if (ilosc_pasazerow % najmniejsza_przepustowosc) {
		ilosc_kursow++;
	}

	cout << ilosc_kursow << endl;
}

int znajdz_najmniejsza_wage(
	vector < pair <int, pair <int, int> > > mst,
	pair <int, pair <int, int> > transport
) {
	const int maksymalny_rozmiar = 100;
	vector<pair<int, int>> mapa_polaczen[maksymalny_rozmiar];
	bool odwiedzone[maksymalny_rozmiar];
	int miasto_poczatkowe = transport.second.first;
	int miasto_koncowe = transport.second.second;
	int ilosc_pasazerow = transport.first;
	int waga = 0;

	for (auto polaczenie : mst) {
		mapa_polaczen[polaczenie.second.first].push_back({ polaczenie.second.second, polaczenie.first });
		mapa_polaczen[polaczenie.second.second].push_back({ polaczenie.second.first, polaczenie.first });
	}

	zeruj_odwiedzone(odwiedzone, maksymalny_rozmiar);
	dfs(mapa_polaczen, odwiedzone, waga, miasto_poczatkowe, miasto_koncowe, true);

	return waga;
}

void zeruj_odwiedzone(bool* odwiedzone, int rozmiar)
{
	for (int i = 0; i < rozmiar; i++) {
		odwiedzone[i] = false;
	}
}

bool dfs(vector<pair<int, int>>* mapa_polaczen, bool* odwiedzone, int& waga, int start, int koniec, bool pierwsza_iteracja = false)
{
	if (start == koniec) {
		return true;
	}

	odwiedzone[start] = true;

	int x, w;
	for (auto tmp : mapa_polaczen[start]) {
		x = tmp.first;
		w = tmp.second * -1;

		if (!odwiedzone[x]) {
			if (pierwsza_iteracja) {
				waga = w;
			}

			if (dfs(mapa_polaczen, odwiedzone, waga, x, koniec, false)) {
				if (w < waga) {
					waga = w;
				}

				return true;
			}
		}
	}

	return false;
}

void stworz_mst(
	vector < pair <int, pair <int, int> > > dane,
	vector < pair <int, pair <int, int> > >& mst
) {
	vector <pair <int, int>> las = znajdz_las(dane);
	sort(las.begin(), las.end());
	int w1, w2;

	for (auto miasto : dane) {
		w1 = znajdz_zestaw_wierzcholkow(las, miasto.second.first);
		w2 = znajdz_zestaw_wierzcholkow(las, miasto.second.second);
		if (w1 != w2) {
			mst.push_back({ miasto.first, {miasto.second.first, miasto.second.second} });
			polacz_zestaw_wierzcholkow(las, w1, w2);
		}
	}
}

int znajdz_zestaw_wierzcholkow(vector <pair <int, int> > rodzice, int i)
{
	int index, rodzic;
	for (auto miasto : rodzice) {
		index = miasto.first;
		rodzic = miasto.second;
		if (index == i) {
			if (i == rodzic) {
				return i;
			}
			else {
				return znajdz_zestaw_wierzcholkow(rodzice, rodzic);
			}
		}
	}

	return 1; //nigdy tu nie wejdzie
}

void polacz_zestaw_wierzcholkow(vector <pair <int, int> >& rodzice, int w1, int w2)
{
	int index, miasto;
	for (auto& row : rodzice) {
		index = row.first;
		miasto = row.second;
		if (index == w1) {
			for (auto row2 : rodzice) {
				if (row2.first == w2) {
					row = { index, row2.second };
				}
			}
		}
	}
}

bool znajdz_wartosc(vector <pair <int, int> > las, int k)
{
	int index;
	for (auto miasto : las) {
		index = miasto.first;
		if (index == k) {
			return true;
		}
	}

	return false;
}

vector <pair <int, int>> znajdz_las(vector < pair <int, pair <int, int> > > dane)
{
	vector <pair <int, int> > las;
	int m1, m2;
	for (auto& miasto : dane) {
		m1 = miasto.second.first;
		m2 = miasto.second.second;
		if (false == znajdz_wartosc(las, m1)) {
			las.push_back({ m1, m1 });
		}

		if (false == znajdz_wartosc(las, m2)) {
			las.push_back({ m2, m2 });
		}
	}

	return las;
}