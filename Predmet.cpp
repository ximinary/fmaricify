#include "Predmet.hpp"

Predmet::Predmet(Termin* termin)
{
    naziv = termin->predmet;
    if (termin->tip == "П") {
        p.push_back(termin);
        p_nastavnici.emplace_back(termin->nastavnik, termin->grupe_);
    } else if (termin->tip == "В") {
        v.push_back(termin);
        v_nastavnici.emplace_back(termin->nastavnik, termin->grupe_);
    } else if (termin->tip == "К") {
        k.push_back(termin);
        k_nastavnici.emplace_back(termin->nastavnik, termin->grupe_);
    }
    godina = termin->grupe_razd[0][0] - '0';
}


void dodaj_nastavnika(vector<pair<string, string>>& nastavnici, Termin* termin)
{
    bool nadjen = false;
    for (auto &[nas, ter] : nastavnici) {
        if (nas == termin->nastavnik) {
            ter += ", " + termin->grupe_;
            nadjen = true;
            break;
        }
    }
    if (!nadjen) {
        nastavnici.emplace_back(termin->nastavnik, termin->grupe_);
    }
}

bool Predmet::isThis(Termin* termin)
{
    if (naziv == termin->predmet) {
        if (termin->tip == "П") {
            p.push_back(termin);
            dodaj_nastavnika(p_nastavnici, termin);
        } else if (termin->tip == "В") {
            v.push_back(termin);
            dodaj_nastavnika(v_nastavnici, termin);
        } else if (termin->tip == "К") {
            k.push_back(termin);
            dodaj_nastavnika(k_nastavnici, termin);
        }
        return true;
    }
    return false;
}


void Predmet::printHTML(ofstream& outFile, int &god)
{
    if (god != godina) {
        outFile << "<tr><td style=\"font-weight: bold; text-align: center;\" colspan=\"5\">" << godina << ". година</td></tr>" << endl;
        god = godina;
    }

    outFile << "<tr>";
    outFile << "<td>" << naziv << "</td>";
    
    outFile << "<td><input type=\"checkbox\" name='" << naziv << "_c'>";

    if(!p.empty()) {
        outFile << "<td><select style=\"width: 100px;\" name='" << naziv << "_p'>";
        outFile << "<option value='" << "0" << "'>" << "Свеједно" << "</option>";
        for (auto &nast : p_nastavnici) {
            outFile << "<option value='" << nast.first << "'>";
            outFile << nast.first << " (" << nast.second << ")";
            outFile << "</option>";
        }
        outFile << "<option value='" << "-" << "'>" << "Не идем" << "</option>";
        outFile << "</select style=\"width: 100px;\"></td>";
    } else {
        outFile << "<td></td>";
    }


    if(!v.empty()) {
        outFile << "<td><select style=\"width: 100px;\" name='" << naziv << "_v'>";
        outFile << "<option value='" << "0" << "'>" << "Свеједно" << "</option>";
        for (auto &nast : v_nastavnici) {
            outFile << "<option value='" << nast.first << "'>";
            outFile << nast.first << " (" << nast.second << ")";
            outFile << "</option>";
        }
        outFile << "<option value='" << "-" << "'>" << "Не идем" << "</option>";
        outFile << "</select style=\"width: 100px;\"></td>";
    } else {
        outFile << "<td></td>";
    }


    if(!k.empty()) {
        outFile << "<td><select style=\"width: 100px;\" name='" << naziv << "_k'>";
        outFile << "<option value='" << "0" << "'>" << "Свеједно" << "</option>";
        for (auto &nast : k_nastavnici) {
            outFile << "<option value='" << nast.first << "'>";
            outFile << nast.first << " (" << nast.second << ")";
            outFile << "</option>";
        }
        outFile << "<option value='" << "-" << "'>" << "Не идем" << "</option>";
        outFile << "</select style=\"width: 100px;\"></td>";
    } else {
        outFile << "<td></td>";
    }

    outFile << "</tr>\n";
}

void Predmet::print(ofstream& outFile)
{
    outFile << "\t\t\t\"naziv\": \"" << naziv << "\"," << endl;
    outFile << "\t\t\t\"p\": [" << endl;
    bool first = true;
    for (Termin* t : p) {
        if (first) first = false;
        else outFile << "," << endl;
        t->print(outFile);
    }
    outFile << endl << "\t\t\t]," << endl;

    outFile << "\t\t\t\"v\": [" << endl;
    first = true;
    for (Termin* t : v) {
        if (first) first = false;
        else outFile << "," << endl;
        t->print(outFile);
    }
    outFile << endl << "\t\t\t]," << endl;

    outFile << "\t\t\t\"k\": [" << endl;
    first = true;
    for (Termin* t : k) {
        if (first) first = false;
        else outFile << "," << endl;
        t->print(outFile);
    }
    outFile << endl << "\t\t\t]" << endl;
}

bool Predmet::compare(const Predmet* a, const Predmet* b) {
    if (a->godina == b->godina) {
        if (a->naziv.substr(0, 14).compare("Стручни") == 0 && b->naziv.substr(0, 14).compare("Стручни") != 0)
            return true;
        return a->naziv.compare(b->naziv) < 0;
    }
    return a->godina < b->godina;
}
