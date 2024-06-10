#include <iostream>
#include <vector>
#include <string>
#include <fstream> //Para escritura y lectura de datos
#include <chrono> // Calculo del tiempo con alta precision
#include <cmath>

using namespace std;
const int infinite = 1000;

int editDistanceRecursive(string S, string T);
int editDistanceMemoization(string S, string T);
double timeRecursive(string S, string T);
double timeMemoization(string S, string T);
double prom(vector<double> vec);

template <typename T>
void printVector(vector<T> vec);

int main()
{
    ifstream input("frases.txt");
    if (!input.is_open())
        cerr << "No se pudo abrir el archivo" << endl;
        
    int n = 0;
    string line;
    
    while (getline(input, line)) {  // Obtenemos la cantidad de frases que tenemos
        n++;
    }

    input.clear();      // Reiniciamos el puntero para leer el archivo nuevamente desde 0 sin tener que cerrarlo
    input.seekg(0); 

    vector<string> vec(n);
    int k = 0;
    while (getline(input, line)) // Leemos para generar un vector con el que trabajaremos
    {
        vec[k] = line; 
        k++;
    }
    input.close();
    //printVector(vec);
    
    int exper = 50;
    vector<double> all_rec(exper);
    vector<double> all_mem(exper);
    
    int cases = pow(n,2) - n;
    vector<string> string_S(cases);
    vector<string> string_T(cases);
    vector<int> n_values(cases);
    vector<int> m_values(cases);
    vector<double> all_prom_rec(cases);
    vector<double> all_prom_mem(cases);
    vector<int> space_rec(cases);
    vector<int> space_mem(cases);
    vector<int> caso(cases);
    int S_size, T_size;
    int case_num = 0;
    k = 0;   

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {  
            if (i != j)
            {
                S_size = vec[i].size();
                T_size = vec[j].size();
                k = 0;
                while (k < exper)
                {
                    all_rec[k] = timeRecursive(vec[i], vec[j]); 
                    all_mem[k] = timeMemoization(vec[i], vec[j]); 
                    //cout << vec[i] << " " << vec[j] << endl;
                    //cout << all_rec[k] << " " << all_mem[k] << endl;
                    k++;
                }
                //printVector(all_rec);
                //printVector(all_mem);
                string_S[case_num] = vec[i];
                string_T[case_num] = vec[j];
                n_values[case_num] = vec[i].size();
                m_values[case_num] = vec[j].size();
                all_prom_rec[case_num] = prom(all_rec);
                all_prom_mem[case_num] = prom(all_mem);
                space_rec[case_num] = S_size*4;
                space_mem[case_num] = S_size*T_size*4;
                caso[case_num] = case_num + 1;
                case_num++;
            }
        }
    }
    printVector(all_prom_rec);
    printVector(all_prom_mem);

    ofstream output("experimentacion.csv");
    output << "Caso,S,T,n,m,PR[us],PM[us],ER[bytes],EM[bytes]" << endl;

    for (int i = 0; i < cases; i++)
    {
        output << caso[i] << "," << string_S[i] << "," << string_T[i] << "," << n_values[i] << "," << m_values[i] << "," 
        << all_prom_rec[i] << "," << all_prom_mem[i] << "," << space_rec[i] << "," << space_mem[i] << endl;
    }
    output.close();
    return 0;
}

double timeRecursive(string S, string T)
{   
    auto start_time = chrono::high_resolution_clock::now();     // Calculamos el tiempo total restando el tiempo posterior al edit distance menos el anterior
    int aux_rec = editDistanceRecursive(S, T);
    auto end_time = chrono::high_resolution_clock::now();
    double total = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    //cout << aux_rec << endl;
    return total;
}

double timeMemoization(string S, string T)
{   
    auto start_time = chrono::high_resolution_clock::now();     // Calculamos el tiempo total restando el tiempo posterior al edit distance menos el anterior
    int aux_mem = editDistanceMemoization(S, T);
    auto end_time = chrono::high_resolution_clock::now();
    double total = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    //cout << aux_mem << endl;
    return total;
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

int editDistanceMemoization(string S,string T)
{
    int n_plus = S.length() + 1; 
    int m_plus = T.length() + 1;                               // Extiende el largo, el primer parametro de M es el largo n, el segundo es el vector de largo m inicializado en -1
    vector<vector<int>> M(n_plus, vector<int>(m_plus, infinite)); // Genera la matriz de memoria. Llamadas "recursivas", y el llenado de la matriz de memoria. Vector de vectores de enteros.
    int del, rep;
    for (int i = 0; i < n_plus; i++)                         // Casos bases
        M[i][0] = i;
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
    return M[n_plus - 1][m_plus - 1];
}

template <typename T>
void printVector(vector<T> vec)
{
    int n = vec.size();
    for (int i = 0; i < n; i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}

double prom(vector<double> vec)
{
    int n = vec.size();
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += vec[i];
    }   
    return sum / n; 
}