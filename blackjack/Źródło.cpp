#include <iostream>
#include <string>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

//klasa karty
class Card {
public:
	int idn;				//numer dla znaku
	int idc;				//numer dla koloru
	int idv;
	int t;// wartosc to sortowania
	string sign;			//znak
	string color;			//kolor
	int value;				//wartośc
	string name;			//nazwa
	bool flag = false;		//flaga do tasowania
	bool taken = false;		//czy wzieta

							//pokazuje nazw karty
	void showCard() {
		cout << t << " " << name << " " << value << endl;
	}

	Card(int x, int y, int t) {
		this->idn = x;
		this->idc = y;;
		this->t = t;
		switch (idc) {
		case 0:
			color = "H";
			idv = idn;
			break;

		case 1:
			color = "D";
			idv = idn + 100;
			break;

		case 2:
			color = "P";
			idv = idn + 200;
			break;

		case 3:
			color = "T";
			idv = idn + 300;
			break;
		}

		if (idn <= 7) {
			value = idn + 2;
			sign = '0' + idn + 2;
		}
		else {
			value = 10;

			switch (idn) {
			case 8:
				sign = "10";
				break;
			case 9:
				sign = 'J';
				break;
			case 10:
				sign = 'Q';
				break;
			case 11:
				sign = 'K';
				break;
			case 12:
				sign = 'A';
				value = 11;
				break;
			}
		}

		name = sign + color;
	}

	Card() {}
};

//klasa tali
class Talia :public Card {

public:
	int size = 2;	//ilosc tali
	Card deck[52 * 2];		//tablica kart

							//inicjajca tali
	void initiateDeck() {
		int i, j, k;
		int x = 0;
		for (k = 0; k < size; k++) {
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 13; j++) {
					deck[x] = Card(j, i, k);
					x++;
				}
			}
		}
	}

	//tasowanie tali
	Talia shuffle(Talia stara) {
		int i = 0;
		int x;
		Talia nowa = Talia();
		while (i < nowa.size * 52) {
			x = rand() % (nowa.size * 52);

			while (stara.deck[x].flag == false) {
				nowa.deck[i] = stara.deck[x];
				stara.deck[x].flag = true;
				i++;
			}
		}

		for (i = 0; i < nowa.size * 52; i++) {
			nowa.deck[i].flag = false;
		}

		return nowa;
	}

	//wyświetlenie tali
	void printDeck() {
		for (int i = 0; i < 52 * size; i++) {
			deck[i].showCard();
		}
	}

	Talia() {}
};

//klasa gracza
class Gracz : public Talia {
public:
	int points = 0;		//liczba punktów
	int cards = 0;		//liczba kart w ręku
	Card hand[10];		//ręka gracza
	bool asFlag = false;

	//sortowanie ręki gracza
	void sortHand() {
		int i, j;
		Card temp;
		for (i = 0; i < cards; i++) {

			for (j = 0; j < cards - 1; j++) {

				if (hand[j].idv > hand[j + 1].idv) {
					temp = hand[j];
					hand[j] = hand[j + 1];
					hand[j + 1] = temp;

				}
			}

		}

	}

	//sprawdzenie punktów gracza
	void checkPoints() {
		int i;
		points = 0;

		for (i = 0; i < cards; i++) {
			points += hand[i].value;
		}
	}


	void addPoints(Card hand) {
		points = points + hand.value;
	}

	//pokazanie punktów gracza
	void showPoints() {
		cout << points << endl;
	}

	//sprawdzenie AS
	void checkAs() {
		int i;
		for (i = 0; i < cards; i++) {
			if (points > 21) {
				if (hand[i].idn == 12) {
					hand[i].value = 1;

				}
			}
			checkPoints();
		}
	}

	void showme() {
		int i;
		for (i = 0; i < cards; i++) {

			cout << hand[i].value << endl;

		}
	}
	//pobranie 2 kart
	void takeDouble(Talia talia) {
		int i = 0;
		int x;
		while (i < 2) {
			x = rand() % (talia.size * 52);

			while (talia.deck[x].flag == false) {
				hand[i] = talia.deck[x];
				talia.deck[x].flag = true;
				addPoints(hand[i]);
				cards++;
				i++;
			}

		}

	}

	//pokazanie ręki gracza
	void showHand() {
		int i;
		for (i = 0; i < cards; i++) {
			hand[i].showCard();
		}
	}

	//pobranie karty
	void hit(Talia talia) {
		int i = 0;
		int x;
		while (i < 1) {
			x = rand() % (talia.size * 52);

			while (talia.deck[x].flag == false) {
				hand[cards] = talia.deck[x];
				talia.deck[x].flag = true;
				cards++;
				addPoints(talia.deck[x]);
				i++;
			}
		}
	}

	Gracz() {

	}

};


//klasa gry
class Game :public Gracz {
public:
	Talia talia;
	Gracz plejer;
	Gracz krupier;

	void startGame() {
		int z;
		char move;
		cout << "START" << endl;
		system("pause");

		talia.initiateDeck();
		talia = talia.shuffle(talia);
		plejer.takeDouble(talia);
		krupier.takeDouble(talia);

		talia = talia.shuffle(talia);
		plejer.checkAs();
		do {
			plejer.checkAs();
			system("CLS");
			cout << "Odkryta karta krupiera: " << krupier.hand[0].name << endl;
			cout << "Reka gracza: " << endl;
			plejer.sortHand();
			plejer.showHand();
			cout << "Liczba punktow gracza: " << endl;
			plejer.showPoints();
			//plejer.showme();
			cout << "Twoj ruch - hit(h) czy stand(s)?" << endl;
			cin >> move;

			if (move == 'h') {
				plejer.hit(talia);
				plejer.checkAs();
				talia = talia.shuffle(talia);
			}
			else if (move == 's') {
				break;
			}
			else {
				cout << "Blad wpisywania!" << endl;
				system("pause");

			}

		} while (plejer.points < 21);

		system("CLS");
		cout << "Odkryta karta krupiera: " << krupier.hand[0].name << endl;
		cout << "Reka gracza: " << endl;
		plejer.sortHand();
		plejer.showHand();
		cout << "Liczba punktow gracza: " << endl;
		plejer.showPoints();

		if (plejer.points > 21) {
			cout << "PRZEGRALES!" << endl;
			return;
		}
		else {
			system("CLS");
			cout << "Reka krupiera: " << endl;
			krupier.sortHand();
			krupier.showHand();
			cout << "Reka gracza: " << endl;
			plejer.sortHand();
			plejer.showHand();
			cout << "Liczba punktow krupiera: " << endl;
			krupier.showPoints();
			cout << "Liczba punktow gracza: " << endl;
			plejer.showPoints();

			while (krupier.points < 17) {
				krupier.hit(talia);
				talia = talia.shuffle(talia);
			}

			if (krupier.points < 21) {
				z = rand() % 10;
				if (z > 7) {
					krupier.hit(talia);
				}
			}

			system("CLS");
			cout << "Reka krupiera: " << endl;
			krupier.sortHand();
			krupier.showHand();
			cout << "Reka gracza: " << endl;
			plejer.sortHand();
			plejer.showHand();
			cout << "Liczba punktow krupiera: " << endl;
			krupier.showPoints();
			cout << "Liczba punktow gracza: " << endl;
			plejer.showPoints();

			if (plejer.points > krupier.points || krupier.points > 21) {
				cout << "WYGRALES!" << endl;
			}
			else {
				cout << "PRZEGRALES!" << endl;
			}
		}
	}

	Game(Talia talia, Gracz player, Gracz krupier) {
		this->talia = talia;
		this->plejer = player;
		this->krupier = krupier;

	}
};

int main() {
	srand(time(NULL));

	Talia *nowa = new Talia();
	Gracz *Janek = new Gracz();
	Gracz *Krupier = new Gracz();
	Game *black = new Game(*nowa, *Janek, *Krupier);

	black->startGame();

	/*	nowa->initiateDeck();
	nowa->printDeck();
	*nowa= nowa->shuffle(*nowa);
	cout << "==========================" << endl;
	//	nowa->printDeck();
	cz();
	/*	Janek->takeDouble(*nowa);
	Janek->showHand();
	cout << Janek->points << endl;

	Janek->hit(*nowa);
	cout << "============="<<endl;
	Janek->showHand();
	cout << Janek->points << endl;
	Janek->showPoints();*/
	system("pause");
	return 0;
}