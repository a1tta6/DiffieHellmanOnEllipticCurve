#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

int gcdex(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    int x1, y1;
    int d = gcdex(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

// Возведение в степень по модулю
long long int exponentiationModulo(long long int x, long long int y, long long int N)
{
    if (y == 0) return 1;
    long long int z = exponentiationModulo(x, y / 2, N);
    if (y % 2 == 0)
        return (z*z) % N;
    else
        return (x * z * z) % N;
}
long long int moduloInverse(int a, int m) {
    if (a < 0)
        a += m;
    int x, y;
    int g = gcdex(a, m, x, y);
    if (g != 1)
        cout << "error" << endl;
    else {
        x = (x % m + m) % m;
        return x;
    }
}

vector<vector<int>> findPoints(vector<int> Ep, int p) {
    vector<vector<int>> points;
    int a = Ep[0];
    int b = Ep[1];

    for (int i = 0; i < p; i++)
        for (int j = 1; j < p; j++)
        {
            if (exponentiationModulo(i, 2, p) == int(pow(j, 3) + a*j + b) % p)
            {
                vector<int> point{ j,i };
                points.push_back(point);
            }
        }
    return points;
                
}

//Находит первообразный корень по простому модулю р
int primitiveRoot(long long p)
{
    vector<int> fact; //динамический массив для факториала
    int phi = p - 1, n = phi; //функция пробегающая по всем числам от 1 до n-1
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0)
        {
            fact.push_back(i);//помещает факториал в конец
            while (n % i == 0)
                n /= i;

        }
    if (n > 1)
        fact.push_back(n);
    for (int res = 2; res <= p; ++res)
    {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i)
            ok &= exponentiationModulo(res, phi / fact[i], p) != 1;
        if (ok) return res;

    }
    return -1;//если нет первообразного корня, передает в результат -1
}

// Взятие случайного большого простого числа из файла
long int generateP(int n)
{
    ifstream f;
    f.open("primes1.txt");
    long long int p = 0;
    long long int r = rand() % n;
    r += r;
    for (long long int i = 0; i < r; i++)
        f >> p;
    f.close();
    return p;
}
vector<int> sumPoints(int x1, int y1, int x2, int y2, int a, int p) {
    long long int l = 0;
    if ((x1 == x2) && (y1 == y2))
        if (y1 == 0)
            return { 0,0 };
        else {
            l = ((3 * x1 * x1 + a) * moduloInverse(2 * y1, p)) % p;
            if (l < 0)
                l += p;
        }
    else if (x2 - x1 == 0)
        return { 0,0 };
    else { 
        l = ((y2 - y1) * moduloInverse(x2 - x1, p)) % p;
        if (l < 0)
            l += p;
    }

    int x3 = (l * l - x1 - x2) % p;
    if (x3 < 0)
        x3 += p;
    int y3 = (l * (x1 - x3) - y1) % p;
    if (y3 < 0)
        y3 += p;
    return { x3, y3 };

}
vector<int> multiplyPoint(int k, int x, int y, int a, int p) {
    vector<int> startPoint{ x,y };
    vector<int> secondPoint{ x,y };
    for (int i = 0; i < k; i++)
    {
        secondPoint = sumPoints(startPoint[0], startPoint[1], secondPoint[0], secondPoint[1], a, p);
        if ((secondPoint[0] == 0) && (secondPoint[1] == 0))
        {
            secondPoint = { x,y };
        }
    }
    return secondPoint;


}
int main()
{
    srand(time(0));
    setlocale(LC_ALL, "RUS");
    vector <int> Ep{ 1,3 };
    int k = 5;

    long long int p, g, a, A, B, b, ka, kb;
    p = 41;
    vector <vector<int>> points = findPoints(Ep, p);

    for (vector<int> var : points)
        cout << var[0] << ' ' << var[1] << endl;
    cout << endl;

    int x = 23;
    int y = 37;

    a = 5;
    vector<int>Pa = multiplyPoint(a, x, y, Ep[0], p);
    cout << "Pa = " << Pa[0]<<' '<<Pa[1]<<endl;

    b = 9;
    vector<int>Pb = multiplyPoint(b, x, y, Ep[0], p);
    cout << "Pb = " << Pb[0] << ' ' << Pb[1] << endl;

    vector<int> Ka = multiplyPoint(a, Pb[0], Pb[1], Ep[0], p);
    cout << "Ka = " << Ka[0] << ' ' << Ka[1] << endl;

    vector<int> Kb = multiplyPoint(b, Pa[0], Pa[1], Ep[0], p);
    cout << "Kb = " << Kb[0] << ' ' << Kb[1] << endl;

}