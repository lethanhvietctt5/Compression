#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "Huffman.h"
#include "Folder.h"

using namespace std;

enum COLOR
{
	black,
	blue,
	green,
	cyan,
	red,
	magenta,
	brown, 
	normal,
	darkgrey,
	lightblue, 
	lightgreen,
	lightcyan,
	lightred,
	lightmagenta,
	yellow,
	white
};

void setFontSize(int a, int b)

{

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();

	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);

	GetCurrentConsoleFontEx(hStdOut, 0, lpConsoleCurrentFontEx);

	lpConsoleCurrentFontEx->dwFontSize.X = a;

	lpConsoleCurrentFontEx->dwFontSize.Y = b;

	SetCurrentConsoleFontEx(hStdOut, 0, lpConsoleCurrentFontEx);

}

ostream& operator << (ostream& out, const COLOR& s)
{
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out_handle, s);
	return out;
}

void compressFile()
{
	system("cls");
	string in, out;
	cout << yellow << "\t\t\t\tCOMPRESS A FILE" << endl << endl;
	cout << yellow << "\t\t===============================================" << endl << endl << endl;
	cout << yellow << "Enter name of file input:";
	getline(cin, in);
	cout << yellow << "Enter name of file output:";
	getline(cin, out);
	Huffman huff(in, out);
	huff.encode();
	cout << yellow << "Compression successful !";
}

void decompressFile()
{
	system("cls");
	string in, out;
	cout << yellow << "\t\t\t\tDECOMPRESS A FILE" << endl << endl;
	cout << yellow << "\t\t===============================================" << endl << endl << endl;
	cout << yellow << "Enter name of file input:";
	getline(cin, in);
	cout << yellow << "Enter name of file output:";
	getline(cin, out);
	Huffman huff(in, out);
	huff.decode();
	cout << yellow << "Decompression successful !";
}

void compressFolder()
{
	system("cls");
	string in, out;
	cout << yellow << "\t\t\t\tCOMPRESS A FOLDER" << endl << endl;
	cout << yellow << "\t\t===============================================" << endl << endl << endl;
	cout << yellow << "Enter name of folder input:";
	getline(cin, in);
	cout << yellow << "Enter name of file output:";
	getline(cin, out);
	Folder fol(in);
	fol.open();
	fol.encode(out);
	cout << yellow << "Compression successful !";
}

void decompressFolder()
{
	system("cls");
	string in, out;
	cout << yellow << "\t\t\t\tDECOMPRESS A FOLDER" << endl << endl;
	cout << yellow << "\t\t===============================================" << endl << endl << endl;
	cout << yellow << "Enter name of file input:";
	getline(cin, in);
	cout << yellow << "Enter name of folder output:";
	getline(cin, out);
	Folder fol;
	fol.decode(in, out);
	cout << yellow << "Decompression successful !";
}

int main()
{
	HWND console = GetConsoleWindow();
	RECT resize;
	GetWindowRect(console, &resize);

	MoveWindow(console, resize.left, resize.top, 600, 500, TRUE);
	setFontSize(2, 25);
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		system("cls");
		cout << yellow << "\t\t\tDATA COMPRESSION PROJECT" << endl << endl;
		cout << yellow << "\t========================================================" << endl << endl << endl;
		cout << yellow << "\t\t\t1. COMPRESS A FILE" << endl;
		cout << yellow << "\t\t\t2. DECOMPRESS A FILE" << endl;
		cout << yellow << "\t\t\t3. COMPRESS A FOLDER" << endl;
		cout << yellow << "\t\t\t4. DECOMPRESS A FOLDER" << endl;
		cout << yellow << "\t========================================================" << endl << endl << endl;
		string choose = "";
		cout << yellow << "Your choose: ";

		while (choose == "")
		{
			getline(cin, choose);
			if (choose.length() == 1)
			{
				switch (choose[0])
				{
				case '1':
					compressFile();
					break;
				case '2':
					decompressFile();
					break;
				case '3':
					compressFolder();
					break;
				case '4':
					decompressFolder();
					break;
				default:
					cout << red << "Your choose is INCONSONANT" << endl;
					break;
				}
			}
			else
				cout << "Your choose is INCONSONANT" << endl;
			cout << endl << endl;
			cout << "Press any key to continue and Press ESC to exit." << endl;
			(void)_getch();
		}
	}
	return 1;
}