#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct User {
    string username;
    string password;
    string name;
    string mail;
    int privilege;
};

static User users[10000];
static int user_count = 0;
static string logged_in[10000];
static int logged_count = 0;

int find_user(const string &u){
    for(int i=0;i<user_count;i++) if(users[i].username==u) return i;
    return -1;
}
bool is_logged(const string &u){
    for(int i=0;i<logged_count;i++) if(logged_in[i]==u) return true;
    return false;
}
void login_add(const string &u){
    if(is_logged(u)) return;
    logged_in[logged_count++]=u;
}
bool logout_remove(const string &u){
    for(int i=0;i<logged_count;i++) if(logged_in[i]==u){
        logged_in[i]=logged_in[logged_count-1];
        logged_count--;
        return true;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cmd;
    while (cin >> cmd){
        string line; getline(cin, line);
        if(cmd=="exit"){
            cout << "bye\n";
            break;
        } else if(cmd=="clean"){
            user_count=0; logged_count=0; cout<<0<<"\n";
        } else if(cmd=="add_user"){
            string c,u,p,n,m; int g=-1;
            istringstream ss(line);
            string tok;
            while(ss>>tok){
                if(tok=="-c") ss>>c;
                else if(tok=="-u") ss>>u;
                else if(tok=="-p") ss>>p;
                else if(tok=="-n") ss>>n;
                else if(tok=="-m") ss>>m;
                else if(tok=="-g") ss>>tok, g=stoi(tok);
            }
            if(user_count==0){
                if(find_user(u)!=-1){ cout<<-1<<"\n"; continue; }
                users[user_count++]={u,p,n,m,10};
                cout<<0<<"\n"; continue;
            }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || !is_logged(c) || iu!=-1 || g<0){ cout<<-1<<"\n"; continue; }
            if(users[ic].privilege<=g){ cout<<-1<<"\n"; continue; }
            users[user_count++]={u,p,n,m,g};
            cout<<0<<"\n";
        } else if(cmd=="login"){
            string u,p; istringstream ss(line); string tok;
            while(ss>>tok){ if(tok=="-u") ss>>u; else if(tok=="-p") ss>>p; }
            int iu=find_user(u);
            if(iu==-1 || users[iu].password!=p || is_logged(u)){ cout<<-1<<"\n"; }
            else { login_add(u); cout<<0<<"\n"; }
        } else if(cmd=="logout"){
            string u; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-u") ss>>u; }
            if(logout_remove(u)) cout<<0<<"\n"; else cout<<-1<<"\n";
        } else if(cmd=="query_profile"){
            string c,u; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-c") ss>>c; else if(tok=="-u") ss>>u; }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || iu==-1 || !is_logged(c)) { cout<<-1<<"\n"; }
            else if(users[ic].privilege>users[iu].privilege || c==u){
                cout<<users[iu].username<<' '<<users[iu].name<<' '<<users[iu].mail<<' '<<users[iu].privilege<<"\n";
            } else cout<<-1<<"\n";
        } else if(cmd=="modify_profile"){
            string c,u,p,n,m; int g_present=0,g=0;
            istringstream ss(line); string tok;
            while(ss>>tok){
                if(tok=="-c") ss>>c; else if(tok=="-u") ss>>u; else if(tok=="-p") ss>>p; else if(tok=="-n") ss>>n; else if(tok=="-m") ss>>m; else if(tok=="-g") { ss>>tok; g=stoi(tok); g_present=1; }
            }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || iu==-1 || !is_logged(c)) { cout<<-1<<"\n"; continue; }
            bool allowed = (users[ic].privilege>users[iu].privilege || c==u);
            if(!allowed){ cout<<-1<<"\n"; continue; }
            if(g_present && !(g<users[ic].privilege)){ cout<<-1<<"\n"; continue; }
            if(!p.empty()) users[iu].password=p;
            if(!n.empty()) users[iu].name=n;
            if(!m.empty()) users[iu].mail=m;
            if(g_present) users[iu].privilege=g;
            cout<<users[iu].username<<' '<<users[iu].name<<' '<<users[iu].mail<<' '<<users[iu].privilege<<"\n";
        } else {
            cout<<-1<<"\n";
        }
    }
    return 0;
}
