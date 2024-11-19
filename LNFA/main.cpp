#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

struct Tranzitie
{
    int urmatoareaStare;
    char litera;
};

int main()
{
    ifstream f("input.txt");
    ofstream g("output.txt");

    int nrStari;
    f >> nrStari;
    unordered_map<int, int> indexStare;
    vector<int> stari(nrStari);

    for (int i = 0; i < nrStari; i++)
    {
        int stare;
        f >> stare;
        indexStare[stare] = i;
        stari[i] = stare;
    }

    int nrLitere;
    f >> nrLitere;
    unordered_map<char, int> indexLitera;
    vector<char> litere(nrLitere);

    for (int i = 0; i < nrLitere; i++)
    {
        char litera;
        f >> litera;
        indexLitera[litera] = i;
        litere[i] = litera;
    }

    int stareInitiala, nrStariFinale;
    f >> stareInitiala >> nrStariFinale;

    vector<bool> stariFinale(nrStari, false);

    for (int i = 0; i < nrStariFinale; i++)
    {
        int stareFinala;
        f >> stareFinala;
        stariFinale[indexStare[stareFinala]] = true;
    }

    int nrTranzitii;
    f >> nrTranzitii;

    vector<vector<Tranzitie>> tranzitii(nrStari);

    for (int i = 0; i < nrTranzitii; i++)
    {
        int stareCurenta, urmatoareaStare;
        char litera;
        f >> stareCurenta >> litera >> urmatoareaStare;
        if (litera == '.')
            tranzitii[indexStare[stareCurenta]].push_back({indexStare[urmatoareaStare], '\0'});
        else
            tranzitii[indexStare[stareCurenta]].push_back({indexStare[urmatoareaStare], litera});
    }

    int nrCuvinte;
    f >> nrCuvinte;

    for (int i = 0; i < nrCuvinte; i++)
    {
        string cuvant;
        f >> cuvant;

        auto lambdaClosure = [&](int stare)
        {
            set<int> closure;
            set<int> toVisit = {stare};

            while (!toVisit.empty())
            {
                int currentState = *toVisit.begin();
                toVisit.erase(toVisit.begin());
                closure.insert(currentState);

                for (const auto& tranzitie : tranzitii[currentState])
                {
                    if (tranzitie.litera == '\0')
                    {
                        if (closure.find(tranzitie.urmatoareaStare) == closure.end())
                            toVisit.insert(tranzitie.urmatoareaStare);
                    }
                }
            }
            return closure;
        };


        set<int> stariCurente;
        stariCurente.insert(indexStare[stareInitiala]);

        for (int stare : lambdaClosure(indexStare[stareInitiala]))
            stariCurente.insert(stare);


        for (char litera : cuvant)
        {
            set<int> urmatoareStari;
            for (int stare : stariCurente)
            {
                for (const auto& tranzitie : tranzitii[stare])
                {
                    if (tranzitie.litera == litera)
                    {
                        urmatoareStari.insert(tranzitie.urmatoareaStare);

                        for (int lambdaStare : lambdaClosure(tranzitie.urmatoareaStare))
                            urmatoareStari.insert(lambdaStare);
                    }
                }
            }
            stariCurente = urmatoareStari;
        }

        bool acceptat = false;
        for (int stare : stariCurente)
        {
            if (stariFinale[stare])
            {
                acceptat = true;
                break;
            }
        }
        if (acceptat)
            g << "DA" << endl;
        else
            g << "NU" << endl;
    }

    return 0;
}
