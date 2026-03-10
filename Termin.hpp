#ifndef __TERMIN__
#define __TERMIN__

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class Termin
{
public:
    string predmet;
    string nastavnik;
    string tip;
    string grupe;
    vector<string> verme;
    vector<string> soba;

    vector<string> grupe_razd;
    string grupe_;
    bool ismer = false;

    static vector<string> split(const string &s, const string &delimiter);

    Termin(vector<string>& tokens);
    bool nastavak(vector<string>& tokens);
    void print(ofstream& outFile);
};

#endif