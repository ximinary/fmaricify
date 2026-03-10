#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Termin.hpp"
#include "Predmet.hpp"

using namespace std;


int main() {
    ifstream inFile("/home/ximinary/Desktop/python venv/output.txt");
    if (!inFile.is_open()) {
        cerr << "Nema fajla" << endl;
        return 1;
    }

    vector<Termin*> termini;

    string line;
    while (getline(inFile, line)) {
        vector<string> tokens = Termin::split(line, " | ");
        if (tokens.size() != 6) {
            cerr << "Pogresna linija: " << line << endl;
            exit(EXIT_FAILURE);
        }

        bool jeste_nastavak = false;
        for (Termin* t : termini) {
            if (t->nastavak(tokens)) {
                jeste_nastavak = true;
                break;
            }
        }
        if (!jeste_nastavak) {
            Termin* novi_termin = new Termin(tokens);
            if (novi_termin->ismer) {
                termini.push_back(novi_termin);
            } else {
                delete novi_termin;
            }
        }
    }
    inFile.close(); 


    vector<Predmet*> predmeti;

    for (Termin* t : termini) {
        bool isthis = false;
        for (Predmet* p : predmeti) {
            if (p->isThis(t)) {
                isthis = true;
                break;
            }
        }
        if (!isthis) {
            predmeti.push_back(new Predmet(t));
        }
    }

    sort(predmeti.begin(), predmeti.end(), Predmet::compare);

    ofstream outFile("index.html");
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return 1;
    }

    outFile << "<table border=\"1\">" << endl;
    outFile << "<tr>" << endl;
    outFile << "<th>Предмет</th>" << endl;
    outFile << "<th></th>" << endl;
    outFile << "<th>Предавања</th>" << endl;
    outFile << "<th>Вежбе</th>" << endl;
    outFile << "<th>Практикум</th>" << endl;
    outFile << "</tr>" << endl;

    int god = 0;
    for(Predmet* p : predmeti) {
        p->printHTML(outFile, god);
    }

    outFile << "</table>" << endl;
    outFile << "<br>" << endl;
    outFile << "<button onclick=\"generate()\">ГЕНЕРИШИ РАСПОРЕД</button>" << endl;
    outFile << "<br>" << endl;
    outFile << "<div id=\"results\"></div>" << endl;
    outFile << "<script src=\"script.js\"></script>" << endl;

    outFile.close();




    outFile = ofstream("out.json");
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return 1;
    }

    outFile << "{" << endl;
    outFile << "\t\"predmeti\": [" << endl;
    bool first = true;
    for(Predmet* p : predmeti) {
        if (first) first = false;
        else outFile << "," << endl;
        outFile << "\t\t{" << endl;
        p->print(outFile);
        outFile << "\t\t}";
    }
    outFile << endl << "\t]" << endl;
    outFile << "}" << endl;

    outFile.close();

    return 0;
}


/*
{
  "predmeti": [
    {
      "naziv": "Анализа 1",
      "p": [
        {
          "nastavnik": "Јелена Катић",
          "grupe": "1i1a, 1i1b, 1i1v",
          "termini": [
            {"vreme": "sre11", "soba": "706"},
            {"vreme": "sre12", "soba": "706"},
            {"vreme": "sre13", "soba": "706"}
          ]
        },
        {
          "nastavnik": "Јована Николић",
          "grupe": "1i2a, 1i2b, 1i2v",
          "termini": [
            {"vreme": "cet08", "soba": "706"},
            {"vreme": "cet09", "soba": "706"},
            {"vreme": "cet10", "soba": "706"}
          ]
        }
      ],
      "v": [
        {
          "nastavnik": "Павле Касом",
          "grupe": "1i2v",
          "termini": [
            {"vreme": "pon08", "soba": "N225"},
            {"vreme": "pon09", "soba": "N225"},
            {"vreme": "pon10", "soba": "N225"}
          ]
        },
        {
          "nastavnik": "Павле Касом",
          "grupe": "1i2b",
          "termini": [
            {"vreme": "pon11", "soba": "N201"},
            {"vreme": "pon12", "soba": "N201"},
            {"vreme": "pon13", "soba": "N201"}
          ]
        },
        {
          "nastavnik": "Павле Касом",
          "grupe": "1i1v",
          "termini": [
            {"vreme": "uto08", "soba": "N152"},
            {"vreme": "uto09", "soba": "N152"},
            {"vreme": "uto10", "soba": "N152"}
          ]
        },
        {
          "nastavnik": "Иван Ћурић",
          "grupe": "1i1a",
          "termini": [
            {"vreme": "uto08", "soba": "JAG3"},
            {"vreme": "uto09", "soba": "JAG3"},
            {"vreme": "uto10", "soba": "JAG3"}
          ]
        },
        {
          "nastavnik": "Иван Ћурић",
          "grupe": "1i1b",
          "termini": [
            {"vreme": "uto11", "soba": "JAG3"},
            {"vreme": "uto12", "soba": "JAG3"},
            {"vreme": "uto13", "soba": "JAG3"}
          ]
        },
        {
          "nastavnik": "Павле Касом",
          "grupe": "1i2a",
          "termini": [
            {"vreme": "sre09", "soba": "JAG1"},
            {"vreme": "sre10", "soba": "JAG1"},
            {"vreme": "sre11", "soba": "JAG1"}
          ]
        }
      ],
      "k": []
    },
    {
      "naziv": "Дискретне структуре 2",
      "p": [
        {
          "nastavnik": "Зоран Станић",
          "grupe": "1i1a, 1i1b, 1i1v",
          "termini": [
            {"vreme": "pon08", "soba": "N253"},
            {"vreme": "pon09", "soba": "N253"},
            {"vreme": "pon10", "soba": "N253"}
          ]
        },
        {
          "nastavnik": "Зоран Станић",
          "grupe": "1i2a, 1i2b, 1i2v",
          "termini": [
            {"vreme": "uto08", "soba": "706"},
            {"vreme": "uto09", "soba": "706"},
            {"vreme": "uto10", "soba": "706"}
          ]
        }
      ],
      "v": [
        {
          "nastavnik": "Кристина Ивановић",
          "grupe": "1i2b",
          "termini": [
            {"vreme": "pon09", "soba": "N252"},
            {"vreme": "pon10", "soba": "N252"}
          ]
        },
        {
          "nastavnik": "Кристина Ивановић",
          "grupe": "1i2v",
          "termini": [
            {"vreme": "pon11", "soba": "N252"},
            {"vreme": "pon12", "soba": "N252"}
          ]
        },
        {
          "nastavnik": "Јелена Тасић",
          "grupe": "1i1b",
          "termini": [
            {"vreme": "uto09", "soba": "JAG2"},
            {"vreme": "uto10", "soba": "JAG2"}
          ]
        },
        {
          "nastavnik": "Јелена Тасић",
          "grupe": "1i1a",
          "termini": [
            {"vreme": "uto11", "soba": "JAG2"},
            {"vreme": "uto12", "soba": "JAG2"}
          ]
        },
        {
          "nastavnik": "Кристина Ивановић",
          "grupe": "1i2a",
          "termini": [
            {"vreme": "cet11", "soba": "843"},
            {"vreme": "cet12", "soba": "843"}
          ]
        },
        {
          "nastavnik": "Јелена Тасић",
          "grupe": "1i1v",
          "termini": [
            {"vreme": "pet10", "soba": "843"},
            {"vreme": "pet11", "soba": "843"}
          ]
        }
      ],
      "k": []
    },
    {
      "naziv": "Енглески језик 2",
      "p": [
        {
          "nastavnik": "Павле Павловић",
          "grupe": "1i1a, 1i1b, 1i1v, 1i2a, 1i2b, 1i2v",
          "termini": [
            {"vreme": "pet12", "soba": "840"},
            {"vreme": "pet13", "soba": "840"}
          ]
        }
      ],
      "v": [],
      "k": []
    }
  ]
}

*/
