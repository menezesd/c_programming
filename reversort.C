#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main () 
{
    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int N;
        cin >> N;
        vector <int> L(N);
        vector <int> P(N+1);
        for (int i = 0; i < N; i++) {
            cin >> L[i];
            P[L[i]] = i+1; /* P[i] = position of element i */
        }
        int sum  = 0;
        for (int i = 1; i < N; i++) {
             sum += P[i] - (i) + 1;
            int *p = &L[i-1];
            int *q = &L[0] + P[i];
            while (p != q && p != --q) {  /* reverse between p and q */
                int t = *p;
                *p = *q;
                *q = t;
                p++;
            }
             for (int j = 0; j < N; j++) 
                P[L[j]] = j+1;
        }
        cout << "Case #" << t << ": " << sum << '\n';
    }
    return 0;

}

