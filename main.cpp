#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class Hangman
{
	private:
		char *raspuns=""; //Stocheaza cuvantul care trebuie ghicit
		string arata=""; //Asta arata utilizatorului cat de departe au ajuns cu ghicitul
		string indiciu; //Indiciu pentru cuvantul care trebuie ghicit
		int p1_nr = 0; //Runde castigate de Player 1
		int p2_nr = 0; //Runde castigate de Player 2
		int nr = 1; //Total runde jucate
		string p1 = "Player 1"; //Numele default al Player 1, se poate schimba din meniu
		string p2 = "Player 2"; //Numele default al Player 2, se poate schimba din meniu

	public:
		HANDLE H_Console = GetStdHandle(STD_OUTPUT_HANDLE);

		/* functie pentru a seta pozitia cursorului in output window */
		void gotoXY(int X, int Y)
		{
			COORD position;
			position.X = X;
			position.Y = Y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
		}

		/*functie pentru a arata casutele pe ecran*/
		void printBox(int X, int Y, int width, int length)
		{
			/*
			C_S_S = Colt stanga sus
			C_D_S = Colt dreapta sus
			C_S_J = Colt stanga jos
			C_D_J = Colt dreapta jos
			L_O	= Linie orizontala
			L_V = Linie verticala
			*/
			char C_S_S = 201, C_D_S = 187, C_S_J = 200, C_D_J = 188;
			char L_O = 205, L_V = 186;
			width -= 2;	length -= 2;

			gotoXY(X, Y);
			cout << C_S_S;
			for (int i = 0; i < width; i++)
				cout << L_O;
			cout << C_D_S;

			for (int i = 0; i < length; i++)
			{
				gotoXY(X, Y + (i + 1));
				cout << L_V;
				gotoXY(X + (width + 1), Y + (i + 1));
				cout << L_V;
			}
			gotoXY(X, Y + length);
			cout << C_S_J;

			for (int i = 0; i < width; i++)
				cout << L_O;
			cout << C_D_J;
		}

		void clearScreen()
		{
			for (int i = 0; i < 24; i++)
			{
				gotoXY(3, 1 + i);
				for (int j = 0; j < 74; j++)
					cout << " ";
			}
		}

		//Functie pentru a arata pagina de start a jocului
		void hangmanGame()
		{
			while (true)
			 {
				clearScreen();
				char logo[7][75];
				strcpy(logo[0], "HH    HH     AA     NN    NN     GGG    MM          MM     AA     NN    NN");
				strcpy(logo[1], "HH    HH   AA  AA   NNN   NN   GG   GG  MMM        MMM   AA  AA   NNN   NN");
				strcpy(logo[2], "HH    HH  AA    AA  NNNN  NN  GG        MMMM      MMMM  AA    AA  NNNN  NN");
				strcpy(logo[3], "HHHHHHHH  AAAAAAAA  NN NN NN  GG        MM MM    MM MM  AAAAAAAA  NN NN NN");
				strcpy(logo[4], "HH    HH  AA    AA  NN  NNNN  GG   GGG  MM  MM  MM  MM  AA    AA  NN  NNNN");
				strcpy(logo[5], "HH    HH  AA    AA  NN   NNN   GG   GG  MM   MMMM   MM  AA    AA  NN   NNN");
				strcpy(logo[6], "HH    HH  AA    AA  NN    NN     GGG    MM    MM    MM  AA    AA  NN    NN");

				//Printarea logoului
				for (int i = 0; i < 7; i++)
				{
					SetConsoleTextAttribute(H_Console, 12);
					gotoXY(3, 1 + i);
					puts(logo[i]);
				}
				SetConsoleTextAttribute(H_Console, 9);
				printBox(28, 8, 30, 14); //Casuta pentru meniu
				printBox(14, 22 , 22 , 4); //Casuta de scor pentru Player 1
				printBox(46, 22 , 22 , 4); //Casuta de scor pentru Player 2

				/*Meniu*/
				gotoXY(31, 10);
				cout << "(1)-> Incepe Jocul ";
				gotoXY(31, 12);
				cout << "(2)-> Schimba Numele ";
				gotoXY(31, 14);
				cout << "(3)-> Instructiuni";
				gotoXY(31, 16);
				cout << "(4)-> Cuvant Si Indiciu";
				gotoXY(31, 18);
				cout << "(5)-> Iesire";
				gotoXY(18,23);
				cout<<p1<<" : "<<p1_nr;
				gotoXY(50,23);
				cout<<p2<<" : "<<p2_nr;

				//Selectarea optiunilor
				gotoXY(0, 29);
				char ch = getch();
					if (ch == '1')
						startGame(); //Incepe jocul
					else if (ch=='2') //Ia numele jucatorilor ca input
					{
						SetConsoleTextAttribute(H_Console, 12);
						clearScreen();
						gotoXY(31, 16);
						cout<<"Player 1 : ";
						gotoXY(31,18);
						getline(cin,p1);
						gotoXY(31, 20);
						cout<<"Player 2 : ";
						gotoXY(31,22) ;
						getline(cin,p2);
						continue ;
					}
					else if (ch == '3')
						instructions(); //Arata instructiunile
					else if (ch == '4') //Ia cuvantul si indiciul ca input
					{
						SetConsoleTextAttribute(H_Console, 12);
						clearScreen();
						char word1[100];
						gotoXY(31, 16);
						cout<<"Introdu cuvantul"; //Introdu cuvantul de ghicit
						gotoXY(31,18);
						cin>>word1;
						storetheword(word1); //Retine cuvantul
						gotoXY(31,20);
						cout<<"Introdu indiciul : " ; scanf("\n"); //Introdu un indiciu pentru celalalt jucator
						gotoXY(31,22);
						getline(cin,indiciu); //Retine indiciul
						continue;
			        }
					else if (ch == '5')
						exit(0); //Inchide programul
					else
						continue;
				}
		}

		//Functie pentru initializarea jocului
		void startGame()
		{
			if(raspuns=="" or strlen(raspuns)<=1)  //Daca nu este introdus niciun cuvant sau cuvantul are o litera, jucatorul este rugat sa intrdouca un cuvant valid
			{
				SetConsoleTextAttribute(H_Console, 12);
				clearScreen();
				gotoXY(25, 12);
				cout<<"Introdu un cuvant valid!";
				gotoXY(25, 14);
				cout<<"Apasa orice tasta pentru a te intoarce la meniul principal";
				char any = getch();
				return;
			}
			while (true)
			{
				nr+=1 ; //Rundele totale cresc cu cate una
				playGame(); //Jocul incepe
//				int check = 0;
				char chek = getch() ;
				break ;
			}
			hangmanGame();
		}

		//Functie unde sunt ghicite literele
		void playGame()
		{
			clearScreen();
			int lives = 0, check = 0;
			char guess[100], input;
			char *word;
			//Retinerea cuvantului
			word = getWord();
			//Transformarea cuvantului in asterixuri
			encryptWord(word, guess);
			printBox(10, 3, 19, 4);
			printBox(56, 3, 14, 4);
			printBox(46, 9, 24, 17);
			while (true)
			 {
				//Printarea cuvantului
				gotoXY(12, 4);
				cout << "Cuvant : ";
				puts(guess);
				gotoXY(12,6);
				cout<< "Indiciu : "; //Arata indiciul
				cout<<indiciu;
				//Printarea vietilor ramase
				gotoXY(58, 4);
				cout << "Vieti : " << (7 - lives);
				//Arata spanzuratoarea
				hanging(lives);
				if (lives == 7)
					break;
				if (checkWord(word, guess))
				{
					check = 1;
					break;
				}
				//Ghicirea
				gotoXY(12, 11);
				cout << "Ghiceste : ";
				input = getche();
				gotoXY(14, 14);
				if (checkGuess(word, guess, input))
				{
					cout << "                        ";
					gotoXY(14, 14);
					cout << "Corect!";
				}
				else
				{
					cout << "Ai pierdut o viata";
					lives++;
				}
			}

			//Printarea rezultatului
			printBox(10, 18, 25, 8);
			SetConsoleTextAttribute(H_Console, 12);
			gotoXY(18, 20);
			if (check == 1)
			{
				cout << "AI CASTIGAT!";
				/* Daca runda este impara joaca Player 1, altfel joaca Player 2*/
				if (!(nr&1))
					p1_nr+=1;
				else
					p2_nr+=1;
			}
			else
				cout << "AI PIERDUT!";
			SetConsoleTextAttribute(H_Console, 9);
		}

		/*Functie care returneaza cuvantul care trebuie ghicit*/
		char* getWord()
		{
			return raspuns;
		}

		//Functie care mascheaza cuvantul
		void encryptWord(char* word, char *guess)
		{
			int i = 0;
			guess[0] = word[0];
			//Se retin toate caracterele cuvantului drept asterixuri
			while (word[i] != '\0')
			{
				guess[i] = '*';
				i++;
			}
			guess[i] = '\0';
		}

		//Fucntie care arata cat de aproape este jucatorul de a pierde runda
		void hanging(int wrong)
		{
			static char hang[12][20];
			if (wrong == 0)
			{
				strcpy(hang[0], " _____________    ");
				strcpy(hang[1], " |           |    ");
				strcpy(hang[2], " |           |    ");
				strcpy(hang[3], "             |    ");
				strcpy(hang[4], "             |    ");
				strcpy(hang[5], "             |    ");
				strcpy(hang[6], "             |    ");
				strcpy(hang[7], "             |    ");
				strcpy(hang[8], "             |    ");
				strcpy(hang[9], "             |    ");
				strcpy(hang[10], "             |    ");
				strcpy(hang[11], "         ---------");
			}
			else if (wrong == 1)
				strcpy(hang[3], " O           |   ");
			else if (wrong == 2)
				strcpy(hang[4], " |           |   ");
			else if (wrong == 3)
				strcpy(hang[4], "/|           |   ");
			else if (wrong == 4)
				strcpy(hang[4], "/|\\         |   ");
			else if (wrong == 5)
				strcpy(hang[5], " |           |   ");
			else if (wrong == 6)
				strcpy(hang[6], "/            |   ");
			else
				strcpy(hang[6], "/ \\         |   ");


			//Arata spanzuratoarea
			for (int i = 0; i < 12; i++)
			{
				if (wrong == 7)
					SetConsoleTextAttribute(H_Console, 12);
				gotoXY(49, 11 + i);	puts(hang[i]);
			}
			if (wrong == 7)
				SetConsoleTextAttribute(H_Console, 9);
		}

		//Functie care arata daca o litera este corecta
		bool checkGuess(char* word, char *guess, char ch)
		{
			bool check = false;
			int i = 0;
			ch = toupper(ch);
			char temp;
			while (word[i] != '\0')
			{
				temp = toupper(word[i]);
				if (temp == ch)
				{
					guess[i] = word[i];
					check = true;
				}
				i++;
			}
			return check;
		}

		bool checkWord(string word, char *guess)
		{
			int i = 0;
			while (word[i] != '\0')
            {
				if (word[i] != guess[i])
					return false;
				i++;
			}
			return true;
		}

		//Functie care arata instructiunile pe ecran
		void instructions()
		{
			SetConsoleTextAttribute(H_Console, 12);
			clearScreen();
			char about[19][78];
			strcpy(about[0], "                              HANGMAN                              ");
			strcpy(about[1], "                                                                   ");
			strcpy(about[2], "         IN ACEST JOC, TREBUIE SA GHICESTI CUVANTUL SECRET!        ");
			strcpy(about[3], "                					  			                     ");
			strcpy(about[4], "          AI LA DISPOZITIE 7 VIETI PENTRU A GHICI CUVANTUL         ");
			strcpy(about[5], "             DACA GHICESTI CUVANTUL, AI CASTIGAT RUNDA!            ");
			strcpy(about[6], "                                                                   ");
			strcpy(about[7], "      DACA NU GHICESTI CUVANTUL SI TI SE TERMINA CELE 7 VIETI,     ");
			strcpy(about[8], "                          AI PIERDUT RUNDA                         ");
			strcpy(about[9], "                                                                   ");
			strcpy(about[10], "        PRIMA DATA, PLAYER 1 ALEGE UN CUVANT SI UN INDICIU        ");
			strcpy(about[11], "                                                                  ");
			strcpy(about[12], "              PLAYER 2 TREBUIE SA GHICEASCA CUVANTUL              ");
			strcpy(about[13], "                                                                  ");
            strcpy(about[14], "           DUPA TERMINAREA RUNDEI SE INVERSEAZA ROLURILE          ");

			//Printarea instructiunilor
			for (int i = 0; i < 19; i++)
            {
				gotoXY(1, 5 + i); puts(about[i]);
            }
			gotoXY(0, 29);
			getch();
			hangmanGame();
		}
		void storetheword(char *x)
		{
		    raspuns=x;
		}
};

int main()
{
	Hangman instance ; //Crearea unui obiect instanta a clasei hangman
	system("title HANGMAN"); //Setarea titlului
	system("mode 80,30"); //Setarea ecranului
	system("color 08"); //Setarea culorii
	instance.printBox(0, 0, 80, 30); //Printarea marginilor casutei
	instance.hangmanGame(); //Afisarea meniului
	return 0;
}
