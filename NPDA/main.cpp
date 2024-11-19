#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <algorithm>

using namespace std;

struct Tranzitie
{
    int stare_initiala;
    char simbol_intrare;
    char simbol_stiva;
    int stare_urmatoare;
    string actiune_stiva;
};

bool acceptat(vector<Tranzitie>& tranzitii, int stare_initiala, vector<int>& stari_finale, vector<char>& input_symbols, vector<char>& stack_symbols, string cuvant) {
    // Initializam o stiva goala
    stack<char> stiva;
    stiva.push('.'); // Adaugam simbolul de baza al stivei

    // Setam starea initiala a NPDA-ului
    int stare_curenta = stare_initiala;

    // Parcurgem fiecare caracter din cuvant
    for (char caracter : cuvant) {
        cout << "Caracter: " << caracter << ", Stare curenta: " << stare_curenta << ", Stiva: ";
        stack<char> tempStiva = stiva;
        while (!tempStiva.empty()) {
            cout << tempStiva.top();
            tempStiva.pop();
        }
        cout << endl;
        // Gasim tranzitia corespunzatoare
        bool gasit = false;
        for (Tranzitie& tranzitie : tranzitii) {
            if (tranzitie.stare_initiala == stare_curenta && tranzitie.simbol_intrare == caracter && tranzitie.simbol_stiva == stiva.top()) {
                gasit = true;
                stare_curenta = tranzitie.stare_urmatoare;

                // Actualizam stiva conform tranzitiei
                stiva.pop(); // Eliminam simbolul din varful stivei
                for (int i = tranzitie.actiune_stiva.size() - 1; i >= 0; --i) {
                    if (tranzitie.actiune_stiva[i] != '.') {
                        stiva.push(tranzitie.actiune_stiva[i]); // Adaugam noii simboli pe stiva
                    }
                }
                break;
            }
        }
        // Daca nu s-a gasit nicio tranzitie corespunzatoare, cuvantul nu este acceptat
        if (!gasit) {
            return false;
        }
    }

    // Daca am parcurs intregul cuvant si am ajuns intr-o stare finala, atunci cuvantul este acceptat
    if (find(stari_finale.begin(), stari_finale.end(), stare_curenta) != stari_finale.end()) {
        return true;
    }

    // Daca nu am ajuns intr-o stare finala, dar am parcurs tot cuvantul, acesta nu este acceptat
    return false;
}

int main()
{
    ifstream fisier("NPDA.in"); // Numele fișierului de intrare

    int numar_stari, numar_simboluri_intrare, numar_simboluri_stiva, numar_stari_finale, numar_tranzitii, numar_cuvinte;
    fisier >> numar_stari;

    // Citim starile
    vector<int> stari(numar_stari);
    for (int i = 0; i < numar_stari; ++i)
    {
        fisier >> stari[i];
    }

    fisier >> numar_simboluri_intrare;

    // Citim alfabetul de intrare
    vector<char> simboluri_intrare(numar_simboluri_intrare);
    for (int i = 0; i < numar_simboluri_intrare; ++i)
    {
        fisier >> simboluri_intrare[i];
    }

    fisier >> numar_simboluri_stiva;

    // Citim alfabetul stivei
    vector<char> simboluri_stiva(numar_simboluri_stiva);
    for (int i = 0; i < numar_simboluri_stiva; ++i)
    {
        fisier >> simboluri_stiva[i];
    }

    int stare_initiala;
    fisier >> stare_initiala;

    fisier >> numar_stari_finale;

    // Citim starile finale
    vector<int> stari_finale(numar_stari_finale);
    for (int i = 0; i < numar_stari_finale; ++i)
    {
        fisier >> stari_finale[i];
    }

    fisier >> numar_tranzitii;

    // Citim tranzitiile
    vector<Tranzitie> tranzitii(numar_tranzitii);
    for (int i = 0; i < numar_tranzitii; ++i)
    {
        fisier >> tranzitii[i].stare_initiala >> tranzitii[i].simbol_intrare >> tranzitii[i].simbol_stiva >> tranzitii[i].stare_urmatoare >> tranzitii[i].actiune_stiva;
    }

    fisier >> numar_cuvinte;

    // Citim cuvintele
    vector<string> cuvinte(numar_cuvinte);
    for (int i = 0; i < numar_cuvinte; ++i)
    {
        fisier >> cuvinte[i];
    }

    // Verificam fiecare cuvant si afisam rezultatul
    for (const string& cuvant : cuvinte)
    {
        if (acceptat(tranzitii, stare_initiala, stari_finale, simboluri_intrare, simboluri_stiva, cuvant))
        {
            cout << cuvant << " este acceptat de NPDA." << endl;
        }
        else
        {
            cout << cuvant << " nu este acceptat de NPDA." << endl;
        }
    }

    fisier.close();

    return 0;
}

