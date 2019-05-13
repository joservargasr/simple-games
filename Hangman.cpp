#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<time.h>
using namespace std;

void PrintMessage(string message, bool printTop = true, bool printBottom = true)
{
	if (printTop)
	{
		cout << "+---------------------------------+" << endl;
	}
	cout << "|";
	bool start=true;
	for (int i = message.length(); i < 33; i++)
	{
		if (start)
		{
			message= " " + message;
		}
		else 
		{
			message= message + " ";
		}
		start = !start;
	}
	cout << message<< "|" << endl;
	if (printBottom)
	{
		cout << "+---------------------------------+" << endl;
	}
}
void DrawHangman(int failCount=0)
{
	if (failCount >= 1) {
		PrintMessage("|", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
	if (failCount >= 2) {
		PrintMessage("|", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
	if (failCount >= 3) {
		PrintMessage("O", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
	if (failCount == 4) {
		PrintMessage("/  ", false, false);
	}
	if (failCount == 5) {
		PrintMessage("/| ",false, false);
	}
	if (failCount >= 6) {
		PrintMessage("/|\\", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
	if (failCount >= 7) {
		PrintMessage("|", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
	if (failCount == 8) {
		PrintMessage("/  ", false, false);
	}
	if (failCount >= 9) {
		PrintMessage("/ \\", false, false);
	}
	else {
		PrintMessage("", false, false);
	}
}
void PrintLetters(string input,char from,char to)
{
	string s;
	for(char i=from;i<=to;i++)
	{
		if (input.find(i) == string::npos)
		{
			s += i;
			s += " ";
		}
		else {
			s += "  ";
		}
	}
	PrintMessage(s, false, false);
}
void PrintAvailableLetters(string input)
{
	PrintMessage("AVAILABLE LETTERS");
	PrintLetters(input, 'A', 'M');
	PrintLetters(input, 'N', 'Z');
}
bool PrintWordsAndCheckWind(string words,string guessed) 
{
	bool won = true;
	string s;
	for(int i=0;i<words.length();i++)
	{
		if (guessed.find(words[i]) == string::npos)
		{
			won = false;
			s += "_";
		}
		else 
		{
			s += words[i];
		}
	}
	PrintMessage(s,false);
	return won;
}
string LoadRandomWord(string path)
{
	string word;
	vector<string> v;
	ifstream reader(path);
	if (reader.is_open()) 
	{
		while (getline(reader, word))
			v.push_back(word);

		int randomLine = rand() % v.size();
		word = v.at(randomLine);
		reader.close();
	}
	return word;
}
int TriesLeft(string word,string guesses)
{
	int error = 0;
	for(int i=0;i<guesses.length();i++)
	{
		if (word.find(guesses[i]) == string::npos)
			error++;
	}
	return error;
}
int main()
{
	srand(time(NULL));
	string guesses;
	string wordToGuess;
	wordToGuess = LoadRandomWord("words.txt");
	int tries = 0;
	bool win = false;
	do {
		system("cls");
		PrintMessage("HANGMAN");
		DrawHangman(tries);
		PrintAvailableLetters(guesses);
		PrintMessage("Guess the word");
		win=PrintWordsAndCheckWind(wordToGuess, guesses);

		if (win)
			break;

		char x;
		cout << ">"; cin >> x;
		if (guesses.find(x) == string::npos)
			guesses += x;
		tries = TriesLeft(wordToGuess, guesses);
	} while (tries < 10);
	
	if (win)
		PrintMessage("YOU WON!");
	else
		PrintMessage("GAME OVER");
	system("pause");
	return 0;
}

