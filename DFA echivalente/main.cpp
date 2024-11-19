#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

struct DFA
{
    int initialState;
    unordered_set<int> finalStates;
    unordered_map<int, unordered_map<char, int>> transitions;

    bool accepts(const string& input) const
    {
        int currentState = initialState;
        for (char c : input)
        {
            if (transitions.find(currentState) == transitions.end() ||
                    transitions.at(currentState).find(c) == transitions.at(currentState).end())
            {
                return false;
            }
            currentState = transitions.at(currentState).at(c);
        }
        return finalStates.find(currentState) != finalStates.end();
    }
};

DFA readDFAFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    DFA dfa;
    int numStates;

    // Citirea numărului de stări
    file >> numStates;

    unordered_set<int> states;
    for (int i = 0; i < numStates; ++i) {
        int state;
        file >> state;
        states.insert(state);
    }

    int numSymbols;

    // Citirea simbolurilor
    file >> numSymbols;

    unordered_set<char> symbols;
    for (int i = 0; i < numSymbols; ++i) {
        char symbol;
        file >> symbol;
        symbols.insert(symbol);
    }

    // Citirea stării inițiale
    file >> dfa.initialState;

    int numFinalStates;

    // Citirea stărilor finale
    file >> numFinalStates;

    for (int i = 0; i < numFinalStates; ++i) {
        int finalState;
        file >> finalState;
        dfa.finalStates.insert(finalState);
    }

    int numTransitions;

    // Citirea tranzițiilor
    file >> numTransitions;

    for (int i = 0; i < numTransitions; ++i) {
        int fromState;
        char symbol;
        int toState;
        file >> fromState >> symbol >> toState;

        dfa.transitions[fromState][symbol] = toState;
    }

    file.close();

    return dfa;
}

bool verificaEchivalenta(const DFA& A, const DFA& B)
{
    queue<pair<int, int>> q;
    unordered_set<pair<int, int>, hash<pair<int, int>>> visited;

    auto hashPair = [](const pair<int, int>& p)
    {
        return hash<int>()(p.first) ^ hash<int>()(p.second);
    };

    q.push({A.initialState, B.initialState});
    visited.insert({A.initialState, B.initialState});

    while (!q.empty())
    {
        auto [stateA, stateB] = q.front();
        q.pop();

        bool isFinalA = (A.finalStates.find(stateA) != A.finalStates.end());
        bool isFinalB = (B.finalStates.find(stateB) != B.finalStates.end());

        if (isFinalA != isFinalB)
        {
            return false;
        }

        // Get all symbols from A and B
        unordered_set<char> symbols;
        if (A.transitions.find(stateA) != A.transitions.end())
        {
            for (auto& t : A.transitions.at(stateA))
            {
                symbols.insert(t.first);
            }
        }
        if (B.transitions.find(stateB) != B.transitions.end())
        {
            for (auto& t : B.transitions.at(stateB))
            {
                symbols.insert(t.first);
            }
        }

        for (char symbol : symbols)
        {
            int nextA = -1;
            int nextB = -1;

            if (A.transitions.find(stateA) != A.transitions.end() &&
                    A.transitions.at(stateA).find(symbol) != A.transitions.at(stateA).end())
            {
                nextA = A.transitions.at(stateA).at(symbol);
            }

            if (B.transitions.find(stateB) != B.transitions.at(stateB).end() &&
                    B.transitions.at(stateB).find(symbol) != B.transitions.at(stateB).end())
            {
                nextB = B.transitions.at(stateB).at(symbol);
            }

            if (nextA != nextB)
            {
                if (visited.find({nextA, nextB}) == visited.end())
                {
                    q.push({nextA, nextB});
                    visited.insert({nextA, nextB});
                }
            }
        }
    }

    return true;
}

int main()
{
    DFA dfa1 = citireFisier("date1.in");
    DFA dfa2 = citireFisier("date2.in");

    if (verificaEchivalenta(dfa1, dfa2))
    {
        cout << "DFA-urile sunt echivalente." << endl;
    }
    else
    {
        cout << "DFA-urile NU sunt echivalente." << endl;
    }

    return 0;
}
