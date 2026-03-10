#include "Termin.hpp"

vector<string> Termin::split(const string &s, const string &delimiter) {
    vector<string> tokens;
    size_t last = 0;
    size_t next = 0;
    while ((next = s.find(delimiter, last)) != string::npos) {
        tokens.push_back(s.substr(last, next - last));
        last = next + delimiter.length();
    }
    tokens.push_back(s.substr(last));
    return tokens;
}

Termin::Termin(vector<string>& tokens)
{
    verme.push_back(tokens[0]);
    nastavnik = tokens[1];
    grupe    = tokens[2];
    tip      = tokens[3];
    predmet  = tokens[4];
    soba.push_back(tokens[5]);

    grupe_razd = split(grupe, " , ");
    grupe_ = "";
    for (string s : grupe_razd) {
        if (s[1] == 'i' || s[1] == 'I') {
            ismer = true;
            grupe_ += s + ", ";
        }
    }
    grupe_ = grupe_.substr(0, grupe_.size() - 2);
}

bool Termin::nastavak(vector<string>& tokens)
{
    if (nastavnik == tokens[1] && grupe == tokens[2] && tip == tokens[3] && predmet == tokens[4]) {
        verme.push_back(tokens[0]);
        soba.push_back(tokens[5]);
        return true;
    }
    return false;
}

void Termin::print(ofstream& outFile)
{
    outFile << "\t\t\t\t{" << endl;
    outFile << "\t\t\t\t\t\"nastavnik\": \"" << nastavnik << "\"," << endl;
    outFile << "\t\t\t\t\t\"grupe\": \"" << grupe_ << "\"," << endl;
    outFile << "\t\t\t\t\t\"termini\": [" << endl;
    bool first = true;
    for (int i = 0; i < verme.size(); i++) {
        if (first) first = false;
        else outFile << "," << endl;
        outFile << "\t\t\t\t\t\t{\"vreme\": \"" << verme[i] << "\", \"soba\": \"" << soba[i] << "\"}";
    }
    outFile << endl << "\t\t\t\t\t]" << endl;
    outFile << "\t\t\t\t}";
}