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

struct Train {
    string id;
    int stationNum;
    string stations[105];
    int seatNum;
    int prices[105];
    int travelTimes[105];
    int stopoverTimes[105];
    int start_hr, start_mi;
    int sale_start_m, sale_start_d;
    int sale_end_m, sale_end_d;
    char type;
    bool released;
};

static User users[10000];
static int user_count = 0;
static string logged_in[10000];
static int logged_count = 0;

static Train trains[1000];
static int train_count = 0;

int find_user(const string &u){
    for(int i=0;i<user_count;i++) if(users[i].username==u) return i;
    return -1;
}
bool is_logged(const string &u){
    for(int i=0;i<logged_count;i++) if(logged_in[i]==u) return true;
    return false;
}
void login_add(const string &u){ if(!is_logged(u)) logged_in[logged_count++]=u; }
bool logout_remove(const string &u){ for(int i=0;i<logged_count;i++) if(logged_in[i]==u){ logged_in[i]=logged_in[logged_count-1]; logged_count--; return true; } return false; }

int mdays(int m){ if(m==6) return 30; if(m==7) return 31; return 31; }
void add_minutes(int &m,int &d,int &hr,int &mi,int add){
    int total = hr*60+mi + add;
    hr = (total/60)%24; mi = total%60; int days = total/(24*60);
    while(days--){ d++; if(d>mdays(m)){ d=1; m++; } }
}

int cmp_date(int m1,int d1,int m2,int d2){ if(m1!=m2) return m1<m2?-1:1; if(d1!=d2) return d1<d2?-1:1; return 0; }

int find_train(const string &id){ for(int i=0;i<train_count;i++) if(trains[i].id==id) return i; return -1; }

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
            user_count=0; logged_count=0; train_count=0; cout<<0<<"\n";
        } else if(cmd=="add_user"){
            string c,u,p,n,m; int g=-1;
            istringstream ss(line); string tok;
            while(ss>>tok){ if(tok=="-c") ss>>c; else if(tok=="-u") ss>>u; else if(tok=="-p") ss>>p; else if(tok=="-n") ss>>n; else if(tok=="-m") ss>>m; else if(tok=="-g") { ss>>tok; g=stoi(tok);} }
            if(user_count==0){ if(find_user(u)!=-1){ cout<<-1<<"\n"; continue; } users[user_count++]={u,p,n,m,10}; cout<<0<<"\n"; continue; }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || !is_logged(c) || iu!=-1 || g<0){ cout<<-1<<"\n"; continue; }
            if(users[ic].privilege<=g){ cout<<-1<<"\n"; continue; }
            users[user_count++]={u,p,n,m,g}; cout<<0<<"\n";
        } else if(cmd=="login"){
            string u,p; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-u") ss>>u; else if(tok=="-p") ss>>p; }
            int iu=find_user(u); if(iu==-1 || users[iu].password!=p || is_logged(u)) cout<<-1<<"\n"; else { login_add(u); cout<<0<<"\n"; }
        } else if(cmd=="logout"){
            string u; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-u") ss>>u; }
            if(logout_remove(u)) cout<<0<<"\n"; else cout<<-1<<"\n";
        } else if(cmd=="query_profile"){
            string c,u; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-c") ss>>c; else if(tok=="-u") ss>>u; }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || iu==-1 || !is_logged(c)) cout<<-1<<"\n";
            else if(users[ic].privilege>users[iu].privilege || c==u) cout<<users[iu].username<<' '<<users[iu].name<<' '<<users[iu].mail<<' '<<users[iu].privilege<<"\n"; else cout<<-1<<"\n";
        } else if(cmd=="modify_profile"){
            string c,u,p,n,m; int g_present=0,g=0; istringstream ss(line); string tok;
            while(ss>>tok){ if(tok=="-c") ss>>c; else if(tok=="-u") ss>>u; else if(tok=="-p") ss>>p; else if(tok=="-n") ss>>n; else if(tok=="-m") ss>>m; else if(tok=="-g") { ss>>tok; g=stoi(tok); g_present=1; } }
            int ic=find_user(c), iu=find_user(u);
            if(ic==-1 || iu==-1 || !is_logged(c)) { cout<<-1<<"\n"; continue; }
            bool allowed = (users[ic].privilege>users[iu].privilege || c==u);
            if(!allowed){ cout<<-1<<"\n"; continue; }
            if(g_present && !(g<users[ic].privilege)){ cout<<-1<<"\n"; continue; }
            if(!p.empty()) users[iu].password=p; if(!n.empty()) users[iu].name=n; if(!m.empty()) users[iu].mail=m; if(g_present) users[iu].privilege=g;
            cout<<users[iu].username<<' '<<users[iu].name<<' '<<users[iu].mail<<' '<<users[iu].privilege<<"\n";
        } else if(cmd=="add_train"){
            Train t; t.released=false; string s_list,p_list,x_time,t_list,o_list,d_list; string y;
            istringstream ss(line); string tok;
            while(ss>>tok){
                if(tok=="-i") ss>>t.id; else if(tok=="-n") ss>>tok, t.stationNum=stoi(tok); else if(tok=="-m") ss>>tok, t.seatNum=stoi(tok);
                else if(tok=="-s") ss>>s_list; else if(tok=="-p") ss>>p_list; else if(tok=="-x") ss>>x_time; else if(tok=="-t") ss>>t_list; else if(tok=="-o") ss>>o_list; else if(tok=="-d") ss>>d_list; else if(tok=="-y") ss>>y;
            }
            if(find_train(t.id)!=-1 || t.stationNum<2 || t.seatNum<=0){ cout<<-1<<"\n"; continue; }
            // parse start time
            if(x_time.size()!=5 || x_time[2]!=':'){ cout<<-1<<"\n"; continue; }
            t.start_hr=(x_time[0]-'0')*10+(x_time[1]-'0'); t.start_mi=(x_time[3]-'0')*10+(x_time[4]-'0'); t.type = y.empty()? 'Z': y[0];
            // parse stations
            int idx=0; string cur=""; for(size_t i=0;i<=s_list.size();i++){ if(i==s_list.size() || s_list[i]=='|'){ t.stations[idx++]=cur; cur=""; } else cur.push_back(s_list[i]); }
            if(idx!=t.stationNum){ cout<<-1<<"\n"; continue; }
            // parse prices
            idx=0; cur=""; for(size_t i=0;i<=p_list.size();i++){ if(i==p_list.size() || p_list[i]=='|'){ t.prices[idx++]=cur.empty()?0:stoi(cur); cur=""; } else cur.push_back(p_list[i]); }
            if(idx!=t.stationNum-1){ cout<<-1<<"\n"; continue; }
            // parse travel times
            idx=0; cur=""; for(size_t i=0;i<=t_list.size();i++){ if(i==t_list.size() || t_list[i]=='|'){ t.travelTimes[idx++]=cur.empty()?0:stoi(cur); cur=""; } else cur.push_back(t_list[i]); }
            if(idx!=t.stationNum-1){ cout<<-1<<"\n"; continue; }
            // parse stopover times
            idx=0; cur=""; if(o_list=="_") { idx=0; } else { for(size_t i=0;i<=o_list.size();i++){ if(i==o_list.size() || o_list[i]=='|'){ t.stopoverTimes[idx++]=cur.empty()?0:stoi(cur); cur=""; } else cur.push_back(o_list[i]); } }
            if(t.stationNum==2){ /* ok */ } else if(idx!=t.stationNum-2){ cout<<-1<<"\n"; continue; }
            // parse sale date
            cur=""; int part=0; string a="", b=""; for(size_t i=0;i<=d_list.size();i++){ if(i==d_list.size() || d_list[i]=='|'){ if(part==0) a=cur; else b=cur; cur=""; part++; } else cur.push_back(d_list[i]); }
            if(part!=2 || a.size()!=5 || b.size()!=5){ cout<<-1<<"\n"; continue; }
            t.sale_start_m=(a[0]-'0')*10+(a[1]-'0'); t.sale_start_d=(a[3]-'0')*10+(a[4]-'0');
            t.sale_end_m=(b[0]-'0')*10+(b[1]-'0'); t.sale_end_d=(b[3]-'0')*10+(b[4]-'0');
            if(train_count<1000){ trains[train_count++]=t; cout<<0<<"\n"; } else { cout<<-1<<"\n"; }
        } else if(cmd=="release_train"){
            string id; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-i") ss>>id; }
            int it=find_train(id); if(it==-1 || trains[it].released){ cout<<-1<<"\n"; } else { trains[it].released=true; cout<<0<<"\n"; }
        } else if(cmd=="delete_train"){
            string id; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-i") ss>>id; }
            int it=find_train(id); if(it==-1 || trains[it].released){ cout<<-1<<"\n"; }
            else { trains[it]=trains[train_count-1]; train_count--; cout<<0<<"\n"; }
        } else if(cmd=="query_train"){
            string id,d; istringstream ss(line); string tok; while(ss>>tok){ if(tok=="-i") ss>>id; else if(tok=="-d") ss>>d; }
            int it=find_train(id); if(it==-1){ cout<<-1<<"\n"; continue; }
            Train &t=trains[it];
            int dm=(d[0]-'0')*10+(d[1]-'0'); int dd=(d[3]-'0')*10+(d[4]-'0');
            // date refers to departure from starting station
            if(cmp_date(dm,dd,t.sale_start_m,t.sale_start_d)<0 || cmp_date(dm,dd,t.sale_end_m,t.sale_end_d)>0){ cout<<-1<<"\n"; continue; }
            cout<<t.id<<' '<<t.type<<"\n";
            int m=dm,dn=dd, hr=t.start_hr, mi=t.start_mi;
            int price_cum=0;
            for(int i=0;i<t.stationNum;i++){
                if(i==0){
                    cout<<t.stations[i]<<" xx-xx xx:xx -> ";
                    char buf[32]; snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d", m,dn,hr,mi);
                    cout<<buf<<' '<<price_cum<<' '<<t.seatNum<<"\n";
                } else if(i==t.stationNum-1){
                    char arr[32]; snprintf(arr,sizeof(arr), "%02d-%02d %02d:%02d", m,dn,hr,mi);
                    cout<<t.stations[i]<<' '<<arr<<" -> xx-xx xx:xx "<<price_cum<<" x\n";
                } else {
                    char arr[32]; snprintf(arr,sizeof(arr), "%02d-%02d %02d:%02d", m,dn,hr,mi);
                    // leave after stopover
                    int l_m=m,l_d=dn,l_hr=hr,l_mi=mi; add_minutes(l_m,l_d,l_hr,l_mi, t.stopoverTimes[i-1]);
                    char lev[32]; snprintf(lev,sizeof(lev), "%02d-%02d %02d:%02d", l_m,l_d,l_hr,l_mi);
                    cout<<t.stations[i]<<' '<<arr<<" -> "<<lev<<' '<<price_cum<<' '<<t.seatNum<<"\n";
                    m=l_m; dn=l_d; hr=l_hr; mi=l_mi;
                }
                if(i<t.stationNum-1){ add_minutes(m,dn,hr,mi, t.travelTimes[i]); price_cum += t.prices[i]; }
            }
        } else if(cmd=="query_ticket"){
            string s,tgt,d, pref="cost"; istringstream ss(line); string tok;
            while(ss>>tok){ if(tok=="-s") ss>>s; else if(tok=="-t") ss>>tgt; else if(tok=="-d") ss>>d; else if(tok=="-p") ss>>pref; }
            int dm=(d.size()>=5? (d[0]-'0')*10+(d[1]-'0'):0); int dd=(d.size()>=5? (d[3]-'0')*10+(d[4]-'0'):0);
            // collect matches
            int res_idx[1000]; int res_price[1000]; int res_time[1000]; string res_id[1000]; string res_from[1000]; string res_to[1000];
            int res_dep_m[1000], res_dep_d[1000], res_dep_hr[1000], res_dep_mi[1000];
            int res_arr_m[1000], res_arr_d[1000], res_arr_hr[1000], res_arr_mi[1000];
            int res_seat[1000]; int R=0;
            for(int ti=0; ti<train_count; ti++){
                Train &tr = trains[ti]; if(!tr.released) continue;
                // find station indices
                int i=-1,j=-1; for(int k=0;k<tr.stationNum;k++){ if(tr.stations[k]==s){ i=k; break; } }
                for(int k=i+1;k<tr.stationNum;k++){ if(tr.stations[k]==tgt){ j=k; break; } }
                if(i==-1 || j==-1) continue;
                // offset minutes to departure at station i
                int off=0; for(int k=0;k<i;k++){ off += tr.travelTimes[k]; if(k<i-1) off += tr.stopoverTimes[k]; }
                if(i>0) off += tr.stopoverTimes[i-1];
                // earliest and latest boarding dates
                int em=tr.sale_start_m, ed=tr.sale_start_d, ehr=tr.start_hr, emi=tr.start_mi; add_minutes(em,ed,ehr,emi, off);
                int lm=tr.sale_end_m, ld=tr.sale_end_d, lhr=tr.start_hr, lmi=tr.start_mi; add_minutes(lm,ld,lhr,lmi, off);
                if(cmp_date(dm,dd,em,ed)<0 || cmp_date(dm,dd,lm,ld)>0) continue;
                // departure time at i
                int dhr=tr.start_hr, dmi=tr.start_mi; add_minutes(dhr,dmi,dhr,dmi, off - (dhr*60+dmi)); // simplify: compute time-of-day
                // better: compute time-of-day directly
                int dep_total = (tr.start_hr*60+tr.start_mi + off) % (24*60);
                dhr = dep_total/60; dmi = dep_total%60;
                // arrival at j
                int arr_m=dm, arr_d=dd, arr_hr=dhr, arr_mi=dmi; int arr_off=0; for(int k=i; k<j; k++){ arr_off += tr.travelTimes[k]; if(k<j-1) arr_off += tr.stopoverTimes[k]; }
                add_minutes(arr_m,arr_d,arr_hr,arr_mi, arr_off);
                // price
                int price=0; for(int k=i;k<j;k++) price+=tr.prices[k];
                res_idx[R]=ti; res_price[R]=price; res_time[R]=arr_off; res_id[R]=tr.id; res_from[R]=tr.stations[i]; res_to[R]=tr.stations[j];
                res_dep_m[R]=dm; res_dep_d[R]=dd; res_dep_hr[R]=dhr; res_dep_mi[R]=dmi;
                res_arr_m[R]=arr_m; res_arr_d[R]=arr_d; res_arr_hr[R]=arr_hr; res_arr_mi[R]=arr_mi;
                res_seat[R]=tr.seatNum; R++;
            }
            // sort
            for(int a=1;a<R;a++){
                int b=a; while(b>0){ bool swap=false;
                    if(pref=="time"){ if(res_time[b] < res_time[b-1]) swap=true; else if(res_time[b]==res_time[b-1] && res_id[b] < res_id[b-1]) swap=true; }
                    else { if(res_price[b] < res_price[b-1]) swap=true; else if(res_price[b]==res_price[b-1] && res_id[b] < res_id[b-1]) swap=true; }
                    if(swap){
                        // swap all fields
                        #define SWAP(x) do{ auto tmp=x[b]; x[b]=x[b-1]; x[b-1]=tmp; }while(0)
                        SWAP(res_idx); SWAP(res_price); SWAP(res_time); SWAP(res_id); SWAP(res_from); SWAP(res_to);
                        SWAP(res_dep_m); SWAP(res_dep_d); SWAP(res_dep_hr); SWAP(res_dep_mi);
                        SWAP(res_arr_m); SWAP(res_arr_d); SWAP(res_arr_hr); SWAP(res_arr_mi);
                        SWAP(res_seat);
                        #undef SWAP
                        b--;
                    } else break;
                }
            }
            cout<<R<<"\n";
            for(int r=0;r<R;r++){
                char dep[32]; snprintf(dep,sizeof(dep), "%02d-%02d %02d:%02d", res_dep_m[r],res_dep_d[r],res_dep_hr[r],res_dep_mi[r]);
                char arr[32]; snprintf(arr,sizeof(arr), "%02d-%02d %02d:%02d", res_arr_m[r],res_arr_d[r],res_arr_hr[r],res_arr_mi[r]);
                cout<<res_id[r]<<' '<<res_from[r]<<' '<<dep<<" -> "<<res_to[r]<<' '<<arr<<' '<<res_price[r]<<' '<<res_seat[r]<<"\n";
            }
        } else {
            cout<<-1<<"\n";
        }
    }
    return 0;
}
