#include <bits/stdc++.h>
using namespace std;

static int mod(int a, int b) { int r = a % b; return r < 0 ? r + b : r; }

static char rotate_char(char c, int k) {
    if (c >= 'A' && c <= 'Z') return char('A' + mod((c - 'A') + k, 26));
    if (c >= 'a' && c <= 'z') return char('a' + mod((c - 'a') + k, 26));
    return c;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <rotation> <phrase...>\n";
        return 1;
    }
    int k = stoi(argv[1]);
    k = mod(k, 26);
    string phrase;
    for (int i = 2; i < argc; ++i) {
        if (i > 2) phrase += ' ';
        phrase += argv[i];
    }
    for (char &c : phrase) c = rotate_char(c, k);
    cout << phrase << "\n";
    return 0;
}
