#include <iostream>
#include <vector>
#include <string>

using namespace std;
const int infinite = 1000;

void printMatrix(vector<vector<int>> M);
int editDistanceMemoization(string S,string T);

// Edit Distance con Memoization utilizando Delete y Replace

int main(){
    string S = "DREAMS COME TRUE"; 
    string T = "HERO OF WINDS";
    int min;
    min = editDistanceMemoization(S, T);
    cout << "La distancia minima es: " << min << endl;
}

int editDistanceMemoization(string S,string T)
{
    int n_plus = S.length() + 1; 
    int m_plus = T.length() + 1;                               // Extiende el largo, el primer parametro de M es el largo n, el segundo es el vector de largo m inicializado en -1
    vector<vector<int>> M(n_plus, vector<int>(m_plus, infinite)); // Genera la matriz de memoria. Llamadas "recursivas", y el llenado de la matriz de memoria. Vector de vectores de enteros.
    int del, rep;

    //vector<vector<int>> M;
    for (int i = 0; i < n_plus; i++)                         // Casos bases
        M[i][0] = i;
    //for (int j = 0; j < m_plus; j++)
    //   M[0][j] = infinite;
    for (int i = 1; i < n_plus; i++)                         // Casos intermedios
    {
        for (int j = 1; j < m_plus; j++)
        {
            if (S[i - 1] == T[j - 1])                   // Ultima letra igual
                M[i][j] = M[i - 1][j - 1];
            else                                        // Ultima letra distinta
            {
                del = M[i - 1][j];
                rep = M[i - 1][j - 1];
                M[i][j] = 1 + min(del, rep);
            }
        }
    }

    //printMatrix(M);
    return M[n_plus - 1][m_plus - 1];
}

void printMatrix(vector<vector<int>> M){
    int n_plus = M.size(); 
    int m_plus = M[0].size();                  //Tamano matriz
    for (int i = 0; i < n_plus; i++)
    {
        for (int j = 0; j < m_plus; j++)
        {
            cout << M[i][j] << "\t";
        }
        cout << endl;
    }
}