#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef struct {
	int value;
	bool correct;
} digit;
typedef struct {
	int x;
	int y;
} int2;

// kamus: 6 = ^ ; 7 = v ; 8 = < ; 9 = >

int2 CheckFalse (digit number[5][5]);
/*Fungsi untuk mengecek adakah slot yang masih kosong pada matriks digit. Bila semua telah terisi mengembalikan <5,5>. */
bool CheckHV (digit number[5][5], int x, int y, int n);
/*Fungsi untuk mengecek apakah angka n bisa diletakkan pada kotak ke-(x,y) pada array dua dimensi (matriks) number. x dan y berada pada range 0-4. True jika bisa, false jika tidak.*/
bool int2eq (int2 a, int2 b);
/*Fungsi memberi nilai true jika a.x == b.x dan a.y == b.y*/
int2 FindPrev (digit number[5][5], int x, int y);
/*Membalikan kotak yang berisi angka yang bukan berasal dari soal, sebelum kotak ke [x][y]*/

void PrintFS (digit number [5][5], int hpointer[5][4], int vpointer[4][5]);
/*IS: matriks number, hpointer dan vpointer terisi
FS: Matriks-matriks tercetak pada layar*/

bool CheckSign (digit number[5][5], int hpointer[5][4], int vpointer[4][5], int2 pnt);
/*Fungsi untuk mengecek apakah angka n bisa diletakkan pada kotak pnt, dengan syarat pointer-pointer. Mengembalikan true jika bisa diletakkan pada titik pnt, false jika tidak.*/
bool checkLeft (digit number[5][5], int hpointer[5][4], int2 pnt);
bool checkRight (digit number[5][5], int hpointer[5][4], int2 pnt);
bool checkUp (digit number[5][5], int vpointer[4][5], int2 pnt);
bool checkDown (digit number[5][5], int vpointer[4][5], int2 pnt);

int main()
{
	digit number[5][5];
	int hpointer [5][4];
	int vpointer [4][5];
	int i, j, n;
	int movecount = 0;
	int2 cf, cfloop;
	clock_t op, ed;
	double milisec; 

	cf.x = 5; cf.y = 5;
	
	ifstream testfile;

	testfile.open ("in.dat");
	// copying file to array
	
	for (i = 0; i <= 4; i++) {
		for (j = 0; j <= 3; j++) {	
			testfile >> number[i][j].value >> hpointer[i][j]; 
		}
		testfile >> number[i][4].value; 
		if (i < 4)	{
			for (j = 0; j <= 4; j++) {	
				testfile >> vpointer[i][j]; 
			}
		}
	}

	op = clock();
	for (i = 0; i <= 4; i++) {
		for (j = 0; j <= 4; j++) {
			if (number[i][j].value != 0)
				number[i][j].correct = true;
			else
				number[i][j].correct = false;
		}
	}
 	cout << endl;

	cfloop = CheckFalse(number);
	while (!int2eq(cfloop, cf)){
		number[cfloop.x][cfloop.y].value++;
		movecount ++;

		while (number[cfloop.x][cfloop.y].value < 6 && (!(CheckHV(number, cfloop.x, cfloop.y, number[cfloop.x][cfloop.y].value) && CheckSign(number, hpointer, vpointer, cfloop))))
		{
			number[cfloop.x][cfloop.y].value++;
		}
		if (number[cfloop.x][cfloop.y].value == 6)
		{
			number[cfloop.x][cfloop.y].value = 0;
			cfloop = FindPrev(number, cfloop.x, cfloop.y);
		}
		else
		{
			cfloop = CheckFalse(number);
		}
	}

	PrintFS (number, hpointer, vpointer);
	ed = clock();
	milisec = ((double) (ed - op)) * 1000 / CLOCKS_PER_SEC;
	cout << "Total usaha pengisian adalah: " << movecount << "kali" << endl; 
	cout << "Total waktu adalah: " << milisec << "ms" << endl;

	testfile.close();
	return 0;
}


bool CheckHV (digit number[5][5], int x, int y, int n){
	int i, j;
	bool found = false;

	for (i = 0; i <= 4; i++){
		if (number[x][i].value == n){
			if (i != y)
				found = true;
		}
		if (number[i][y].value == n){
			if (i != x)
				found = true;
		}
	}
	return !found;
}

int2 CheckFalse (digit number[5][5]){
	bool cf = true;
	int2 cf2;

	cf2.x = 5; cf2.y = 5;
	int i = 0;
	int j = 0;
	while (cf == true && i <= 4){
		while (cf == true && j <= 4){
			if (number[i][j].value == 0){
				cf = false;
				cf2.x = i; cf2.y = j;
			}
			else
				j++;
		}
		j = 0;
		i++;
	}
	return cf2;
}

bool int2eq (int2 a, int2 b){
	return ((a.x == b.x) && (a.y == b.y));
}
int2 FindPrev (digit number[5][5], int x, int y){
	int2 prev;
	bool foundprev = false;
	int i, j;
	i = x; j = y-1;
	while (i >= 0 && !foundprev){
		while (j >= 0 && !foundprev){
			if (!number[i][j].correct){
				prev.x = i;
				prev.y = j;
				foundprev = true;
			}
			else
			{
				j--;
			}
		}
		j = 4;
		i--;
	}
	return prev;
}

void PrintFS (digit number [5][5], int hpointer[5][4], int vpointer[4][5]){
	int i, j;
	for (i = 0; i <= 4; i++)
	{
		for (j = 0; j <= 4; j++)
		{
			cout << number[i][j].value << " ";
			if (j < 4){
				if (hpointer[i][j] == 0)
					cout << " ";
				else if (hpointer[i][j] == 6)
					cout << "^"; 
				else if (hpointer[i][j] == 7)
					cout << "v";
				else if (hpointer[i][j] == 8)
					cout << "<";
				else if (hpointer[i][j] == 9)
					cout << ">";
				cout << " ";
			}
		}
		cout << endl;

		if (i < 4){
			for (j = 0; j <= 4; j++){
		
				if (vpointer[i][j] == 0)
					cout << " ";
				else if (vpointer[i][j] == 6)
					cout << "^"; 
				else if (vpointer[i][j] == 7)
					cout << "v";
				else if (vpointer[i][j] == 8)
					cout << "<";
				else if (vpointer[i][j] == 9)
					cout << ">";
				cout << "   ";
			}
		}

		cout << endl;
	}
}

bool CheckSign (digit number[5][5], int hpointer[5][4], int vpointer[4][5], int2 pnt){
	bool sign = true;
	if (pnt.x == 0){
		if (pnt.y == 0){
			return (checkDown(number, vpointer, pnt) && checkRight(number, hpointer, pnt));
		}
		else if (pnt.y == 4)
			return (checkDown(number, vpointer, pnt) && checkLeft(number, hpointer, pnt));
		else 
			return (checkDown(number, vpointer, pnt) && checkLeft(number, hpointer, pnt) && checkRight(number, hpointer, pnt));
	}
	else if (pnt.y == 0){
		return (checkDown(number, vpointer, pnt) && checkRight(number, hpointer, pnt) && checkUp(number, vpointer, pnt));
	}
	else
		return (checkDown(number, vpointer, pnt) && checkRight(number, hpointer, pnt) && checkUp(number, vpointer, pnt) && checkLeft(number, hpointer, pnt));
}

bool checkLeft (digit number[5][5], int hpointer[5][4], int2 pnt){
	bool sign = true;
	if (hpointer[pnt.x][pnt.y -1] == 8 && number[pnt.x][pnt.y - 1].value != 0){
		if (number[pnt.x][pnt.y].value <= number[pnt.x][pnt.y - 1].value){
			sign = false;
		}
	}
	else if (hpointer[pnt.x][pnt.y - 1] == 9 && number[pnt.x][pnt.y - 1].value != 0){
		if (number[pnt.x][pnt.y].value >= number[pnt.x][pnt.y - 1].value){
			sign = false;
		}
	}
	return sign;
}

bool checkRight (digit number[5][5], int hpointer[5][4], int2 pnt){
	bool sign = true;
	if (hpointer[pnt.x][pnt.y] == 8 && number[pnt.x][pnt.y + 1].value != 0){
		if (number[pnt.x][pnt.y].value >= number[pnt.x][pnt.y + 1].value){
			sign = false;
		}
	}
	else if (hpointer[pnt.x][pnt.y] == 9 && number[pnt.x][pnt.y + 1].value != 0){
		if (number[pnt.x][pnt.y].value <= number[pnt.x][pnt.y + 1].value){
			sign = false;
		}
	}
	return sign;
}

bool checkUp (digit number[5][5], int vpointer[4][5], int2 pnt){
	bool sign = true;
	if (vpointer[pnt.x - 1][pnt.y] == 6 && number[pnt.x - 1][pnt.y].value){
		if (number[pnt.x - 1][pnt.y].value >= number[pnt.x][pnt.y].value){
			sign = false;
		}
	}
	else if  (vpointer[pnt.x - 1][pnt.y] == 7 && number[pnt.x - 1][pnt.y].value){
		if (number[pnt.x - 1][pnt.y].value <= number[pnt.x][pnt.y].value){
			sign = false;
		}
	}
	return sign;
}

bool checkDown (digit number[5][5], int vpointer[4][5], int2 pnt){
	bool sign = true;
	if (vpointer[pnt.x][pnt.y] == 6 && number[pnt.x + 1][pnt.y].value != 0){
			if (number[pnt.x][pnt.y].value >= number[pnt.x + 1][pnt.y].value){
				sign = false;
			}
		}
		else if (vpointer[pnt.x][pnt.y] == 7 && number[pnt.x + 1][pnt.y].value != 0){
			if (number[pnt.x][pnt.y].value <= number[pnt.x + 1][pnt.y].value){
				sign = false;
			}
		}

	return sign;
}