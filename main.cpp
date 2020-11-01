#include <bits/stdc++.h>
using namespace std;
int logat = 0;	// pentru a vedea daca utilizatorul este logat sau nu
class utilizator
{
private:
	char nume[50];
	char email[50];
	char parola[50];
	int victorie;
	int infrangere;

	void modifica();
public:
	utilizator()
	{
		victorie = 0;
		infrangere = 0;
	}
	int creeaza_utilizator_nou(char nume[], char email[], char parola[]);
	int login(char email[], char parola[]);
	void arata_date(int arata_nume);
	void victorii()
	{
		victorie++;
		modifica();
	}
	void infrangeri()
	{
		infrangere++;
		modifica();
	}
} utilizator;
void utilizator::modifica()
{
	fstream fil;
	fil.open("utilizator.dat",ios::in| ios::out|ios::binary);
	utilizator x;
	fil.read((char*)&x, sizeof(x));
	while (!fil.eof())
        {
		if (!strcmp(x.email, utilizator.email))
            {
			fil.seekg(0,ios::cur);
			fil.seekp(fil.tellg() - sizeof(utilizator));
			fil.write((char*)&(utilizator), sizeof(utilizator));
			break;
            }
		fil.read((char*)&x, sizeof(u));
        }
	fil.close();
}
int utilizator::creeaza_utilizator_nou(char nume[], char email[], char parola[])
{
	ifstream fin;
	ofstream fout;
	fin.open("utilizator.dat", ios::in);
	fout.open("utilizator.dat", ios::app);
	utilizator x;
	while (!fin.eof())
        {
		fin.read((char*)&x, sizeof(x));
		if (!strcmp(x.email, email))
            {
			fin.close();
			fout.close();
			return -1;					// utilizatorul este deja existent
            }
        }
	strcpy(x.nume, nume);
	strcpy(x.email, email);
	strcpy(x.parola, parola);
	fout.write((char*)&x, sizeof(x));
	fin.close();
	fout.close();
	return 1;							// utilizator creat cu succes
}
int utilizator::login(char email[], char parola[])
{
	if (logat == 1)
		return 1;
	ifstream fin;
	fin.open("utilizator.dat", ios::in);
	utilizator x;
	while (!fin.eof())
        {
		fin.read((char*)&x, sizeof(x));
		if (!strcmp(x.email, email))
            {
			fin.close();
			if (!strcmp(x.parola, parola))
                {
				logat = 1;
				utilizator = z;
				return 1;	// logat cu succes
                }
			else
				return 0;	// parola incorecta
            }
        }
	fin.close();
	return -1;				// utilizatorul nu exista
}
void utilizator::arata_date(int arata_nume)
{
	cout << "\n\n\n";
	if (arata_nume)
        {
		cout << "Nume: " << nume << endl;
		cout << "Email: " << email << endl;
        }
	cout << "Numarul de jocuri jucate: " << victorie + infrangere << endl;
	cout << "Victorii: " << victorie << endl;
	cout << "Infrangeri: " << infrangere << endl;
	cin.ignore();
}
class linked_list
{
private:
	struct node         // un nod al datelor unei liste
        {
		char data[100];
		node* next;
        };
	node* head; 		// pointerul listei
	int size;
public:
	linked_list()
        {
		size = 0;
		head = NULL;
        }
	void push(char x[])       // functie pentru a pune datele in lista
        {
		node* temp = new node;
		strcpy(temp->data, x);
		temp -> next = head;
		head = temp;
		size++;
        }
	void show()              // functie pentru a printa toate datele listei
        {
		node* temp = head;
		while (temp!=NULL)
            {
			cout << temp -> data << "\n";
			temp = temp -> next;
            }
        }
	int getSize()            // functie pentru a determina marimea listei
        {
		return size;
        }
	char* get(int idx)       // functie pentru a gasi un element din lista folosind indexul
        {
		node* temp = head;
		int i = 0;
		while (temp!=NULL && i < idx)
            {
			temp = temp->next;
			i++;
            }
		return temp->data;
        }
};
void login()
{
	utilizator x;
	while (!logat)
        {
		cout << "\n\nEnter (1) pentru a te loga sau (2) creeaza un utilizator nou";
		cout << "\n-> ";
		char ch;
		cin >> ch;
		switch(ch)
            {
			case '2':
                {
				char nume[50], email[50], parola[50];
				cout << "\nEnter nume: ";
				cin >> nume;
				cout << "\nEnter email: ";
				cin >> email;
				cout << "\nEnter parola: ";
				cin >> parola;
				int status = x.creeaza_utilizator_nou(nume, email, parola);
				switch(status)
                    {
					case 1:
						cout << "\n\nUtilizator creat cu succes";
						break;
					default:
						cout << "\n\nEmail-ul este deja folosit!";
						break;
                    }
				break;
                }
			case '1':
                {
				char email[50], parola[50];
				cout << "\nEnter email: ";
				cin >> email;
				cout << "\nEnter parola: ";
				cin >> parola;
				int status = x.login(email, parola);
				switch (status)
                    {
					case 1:
						cout << "\nHi ";
						utilizator.arata_date(1);
						break;
					default:
						cout << "\n\nEmail-ul sau parola sunt incorecte.";
                    }
				break;
                }
            }
        }
}
class Spanzuratoare
{
private:
	char* cuvant;
	char* litere_folosite;
	linked_list cuvinte;
	void primeste_cuvant();
	void litere_folosite_pana_acum();
	int contine(char*, char);
	int ghicite();
	char* temp_string(char);
public:
	Spanzuratoare();
	void reguli();
	void Incepe_jocul();
};
void Spanzuratoare::primeste_cuvant()      // primeste un cuvant aleatoriu din lista de cuvinte
    {
	int size = cuvinte.getSize();
	if (cuvinte.getSize() == 0)
        {
		ifstream fin("cuvinte.txt", ios::in);
		char cuvant[100];
		while(!fin.eof())
            {
			fin >> cuvant;
			cuvinte.push(cuvant);
            }
		fin.close();
        }
	size = cuvinte.getSize();
	srand(time(NULL));
	int r = rand()%size;
	cuvant = cuvinte.get(r);
}
void Spanzuratoare::litere_folosite_pana_acum()     // arata literele folosite pana acum
{
	int len = strlen(litere_folosite);
	for (int i = 0; i < len; i++)
		cout << litere_folosite[i] << ", ";
}
int Spanzuratoare::contine(char r[], char ch)      // verifica daca un string contine o anumita litera
{
	int len = strlen(r);
	for (int i = 0; i < len; i++)
		if (r[i] == ch)
			return 1;
	return 0;
}
Spanzuratoare::Spanzuratoare()       // constructor
{
	cout << "Bine ai venit! Apasa (1) pentru a incepe jocul, apasa (2) pentru a iesi.";
	cout << "\n->";
	char alegere;
	cin >> alegere;
	// apasa orice tasta pentru a continua
	switch(alegere)
        {
		case '1':
			cout << "Sa incepem jocul!\n";
			reguli();
			Incepe_jocul();
			break;
		case '2':
			cout << "O zi placuta!";
			exit(0);
        }
}
void Spanzuratoare::reguli()     // regulile jocului
{
	cout << "Reguli:\n";
	cout << "Ghiceste cuvantul corect ghicind literele din care este alcatuit.\n";
	cout << "5 greseli si jocul se termina.\n\n";
}
void Spanzuratoare::Incepe_jocul()      // manipulator de joc
{
	while (!logat)
        {
		cout << "\n\nNu esti logat!";
		login();
        }
	primeste_cuvant();
	int greseli = 0;
	litere_folosite = new char[26];
	strcpy(litere_folosite, "");
	while (greseli < 5)
        {
		cout << "\nGhiceste o litera\n";
		cout << "Litere folosite pana acum: "; litere_folosite_pana_acum();
		if (ghicite())
            {
			cout << "\n\n";
			cout << "Ai ghicit! Cuvantul este: " << cuvant << endl << endl;
			utilizator.victorie();
			break;
            }
		cout << "->";
		char a;
		cin >> a;
		if (!isalpha(a))
            {
			cout << "\n\nIntrare nevalida." << endl;
            }
		else if (contine(litere_folosite, tolower(a)))
            {
			cout << "\nLitera a fost deja folosita!" << endl;
            }
		else
            {
			char temp[2];
			temp[0] = tolower(a);
			temp[1] = '\0';
			strcat(litere_folosite, temp);
			if (contine(cuvant, tolower(a)))
				cout << "\nAi ghicit!" << endl;
			else
                {
				greseli++;
				if (greseli == 5)
                    {
					cout << "Ai facut 5 greseli! Jocul s-a terminat." << endl;
					utilizator.infrangere();
					cout << "Cuvantul este: " << cuvant << endl << endl;
					break;
                    }
				cout << "\nAi gresit! ";
				cout << (5 - greseli) << " greseli ramase!" << endl;
                }
            }
        }
	utilizator.arata_date(0);
	cout << "Vrei sa joci din nou? (d/[n])\n";
	char ch;
	cin >> ch;
	if (tolower(ch) == 'y')
		Incepe_jocul();		// joaca din nou
	else
        {
		cout << "O zi placuta!";
		exit(0);
        }
}
int Spanzuratoare::ghicite()       // verifica daca cuvantul ghicit este corect
{
	int m = strlen(litere_folosite);
	int n = strlen(cuvant);
	int len = 0;
	char string[100];		// cuvinte ghicite pana acum
	strcpy(string, "");
	for (int i = 0; i < n; i++)
        {
		if (contine(litere_folosite, cuvant[i]))
            {
			len++;
			char temp[2];
			temp[0] = cuvant[i];
			temp[1] = '\0';
			strcat(string, temp);
            }
		else
            {
			strcat(string, "?");
            }
        }
	if (len == n)
		return 1;
	cout << "\nCuvinte ghicite pana acum: " << string << endl;
	return 0;
}
int main()
{
	Spanzuratoarea;
}
