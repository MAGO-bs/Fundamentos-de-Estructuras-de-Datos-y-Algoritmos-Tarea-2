#include <iostream>
#include <string>

using namespace std;
const int infinite = 1000;

// Edit Distance Recursivo con Delete y Replace

int editDistanceRecursive(string S, string T);

int main()
{
    string S = "DREAMS COME TRUE";
    string T = "HERO OF WINDS";
    int min;
    min = editDistanceRecursive(S, T);
    cout << "La distancia minima es: " << min << endl;
}

int editDistanceRecursive(string S, string T)
{
    int n = S.length();
    int m = T.length();

    // Casos base

    if (n >= 0 && m == 0) 
        return n;
    else if (n == 0 && m > 0)
        return infinite;
    else 
    {
        // Si el ultimo caracter es igual, bajamos ambos indices
        if (S[n - 1] == T[m - 1])
        {
            return editDistanceRecursive(S.substr(0, n - 1), T.substr(0, m - 1));
        }
        else //En caso contrario, realizamos las operaciones y encontramos el minimo
        {
            // Caso caracteres diferentes, Insert, Delete y Replace
            int del = editDistanceRecursive(S.substr(0, n - 1), T);
            int rep = editDistanceRecursive(S.substr(0, n - 1), T.substr(0, m - 1));

            return 1 + min(del, rep);
        }   
    }
}