#include <bits/stdc++.h>
using namespace std;

// Minimal stub to satisfy build: echoes bye on exit and -1 for others.
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cmd;
    while (cin >> cmd){
        if(cmd=="exit"){
            cout << "bye\n";
            break;
        } else if(cmd=="clean"){
            cout << 0 << "\n";
        } else {
            // Read rest of line parameters quickly, ignore
            string line;
            getline(cin, line);
            cout << -1 << "\n";
        }
    }
    return 0;
}
