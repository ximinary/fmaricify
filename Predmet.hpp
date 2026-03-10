#ifndef __PREDMET__
#define __PREDMET__

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>

#include "Termin.hpp"

using namespace std;

class Predmet
{
private:
    string naziv;

    vector<Termin*> p;
    vector<pair<string, string>> p_nastavnici;

    vector<Termin*> v;
    vector<pair<string, string>> v_nastavnici;

    vector<Termin*> k;
    vector<pair<string, string>> k_nastavnici;

    int godina;

public:
    Predmet(Termin* termin);
    bool isThis(Termin* termin);
    void printHTML(ofstream& outFile, int &god);
    void print(ofstream& outFile);

    static bool compare(const Predmet* a, const Predmet* b);
};

#endif