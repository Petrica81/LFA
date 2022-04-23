#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <deque>
using namespace std;
ifstream f("DFA.in");
ofstream g("DFA.out");
int n,m,nf;
int start;
string alfabet;
vector<int> stari,starifinale;
map<int,map<string,int>> mat;
map<int,int> multimi,final;
void citire(){
    f>>n>>m;
    for(int i=1; i<=m; i++){
        int aux1,aux2;
        string aux3;
        f>>aux1>>aux2>>aux3;
        mat[aux1][aux3]=aux2;
        if(alfabet.find(aux3) == -1)alfabet.append(aux3);
    }
    for(int i=0;i<n;i++)stari.push_back(i);
    f>>start>>nf;
    for(int i=1; i<=nf; i++){
        int aux1;
        f>>aux1;
        starifinale.push_back(aux1);
    }
}
void afisare() {
    g << "Nod";
    for (int i = 0; i < alfabet.size(); i++) {
        g << setw(10) << alfabet[i];
    }
    g << '\n';
    g << ">";
    for (int i = 0; i < n; i++) {
        for(int j=0;j<nf;j++){
            if(stari[i] == starifinale[j]) g<<"*";
        }
        g<<setw(3)<<stari[i];
        for(int k=0;k<alfabet.size();k++){
            stringstream ss;
            ss<<alfabet[k];
            g<<setw(10)<<mat[stari[i]][ss.str()];
        }
        g<<'\n';
    }
}
void ordonare(vector<int> &vector){
    sort(vector.begin(),vector.end());
    for(int i = 0 ; i<vector.size()-1;i++){
        if(vector[i]==vector[i+1]) { vector.erase(vector.begin() + i+1); i--; }
    }
}
void minimalizare(){
    int nrmultimi =2;
    bool modificare = true;
    for(int i=0;i<n;i++){
        multimi[stari[i]]=1;
        for(int j=0;j<nf;j++)
            if(stari[i] == starifinale[j])multimi[stari[i]] = 2;
    }
    int k=0;
    while(modificare){
//        cout<<"pas "<<k++<<endl;
//        cout<<"Nod"<<setw(9)<<"Multime"<<endl;
//        for(int i=0;i<n;i++){
//            cout<<stari[i]<<setw(5)<<multimi[stari[i]]<<endl;
//        }
        vector<int> amaifost;
        modificare =  false;
        for(int i=0;i<n-1;i++){
            bool ok = false;
            for(int j=i+1;j<n;j++){
                if(ok && multimi[stari[i]]==multimi[stari[j]]) {
                    int aux = 0;
                    for (int k = 0; k < alfabet.size(); k++) {
                        stringstream ss;
                        ss << alfabet[k];
                        if (multimi[mat[stari[i]][ss.str()]] == multimi[mat[stari[j]][ss.str()]]) {
                            aux++;
                        }
                        else{
                            bool eok = false;
                            for (int l = 0; l < amaifost.size(); l++) {
                                if ((mat[stari[j]][ss.str()] == amaifost[l]) ||
                                    (mat[stari[i]][ss.str()] == amaifost[l])) {
                                    eok = true;
                                }
                            }
                            if(eok)aux++;
                        }
                    }
                    if (aux != alfabet.size()) {
                        multimi[j] = nrmultimi;
                        amaifost.push_back(j);
                    }
                }
                else if(multimi[stari[i]]==multimi[stari[j]]){
                    int aux = 0;
                    for (int k = 0; k < alfabet.size(); k++) {
                        stringstream ss;
                        ss<<alfabet[k];
                        if(multimi[mat[stari[i]][ss.str()]] == multimi[mat[stari[j]][ss.str()]])aux++;
                    }
                    if(aux != alfabet.size()){
                        nrmultimi++;
                        amaifost.push_back(j);
                        multimi[j] = nrmultimi;
                        modificare = true;
                        ok = true;
                    }
                }
            }
            if(ok && modificare) i = n;
        }
    }
//    cout<<"Nod"<<setw(9)<<"Multime"<<endl;
//    for(int i=0;i<n;i++){
//        cout<<stari[i]<<setw(5)<<multimi[stari[i]]<<endl;
//    }
    cout<<"Noduri in aceeasi multime(echivalente)\n";
    for(int i=1;i<=nrmultimi;i++){
        for(int j=0;j<n;j++){
            if(multimi[j] == i)cout<<j<<" ";
        }
        cout<<endl;
    }

    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++)
            if (multimi[stari[i]] == multimi[stari[j]] && multimi[stari[j]] != 0){
                stari[j] = stari[i];
                }
    }
    for(int i=0;i<n;i++){
        for(int k=0;k<alfabet.size();k++) {
            stringstream ss;
            ss << alfabet[k];
            mat[stari[i]][ss.str()] = stari[mat[stari[i]][ss.str()]];
        }
    }
    ordonare(stari);
    n= stari.size();
    vector<int> aux;
    for(int i=0;i<nf;i++){
        bool gasit =  false;
        for(int j=0;j<n;j++){
            if(stari[j] == starifinale[i]) gasit = true;
        }
        if(gasit){
            aux.push_back(starifinale[i]);
        }
    }
    starifinale = aux;
    nf = aux.size();

    deque<int>bucle;
    for(int i=0;i<starifinale.size();i++) {
        bucle.push_back(starifinale[i]);
        final[starifinale[i]] = 1;
    }
    while(!bucle.empty()){
        for(int i=0;i<n;i++){
            for(int j=0;j<alfabet.size();j++){
                stringstream ss;
                ss<<alfabet[j];
                if(final[mat[stari[i]][ss.str()]] == 1 && final[stari[i]] == 0){
                 final[stari[i]]=1;
                 bucle.push_back(stari[i]);
                }
            }
        }
        bucle.pop_front();
    }
    cout<<"eliminare noduri inchise:";
    for(int l=0;l<n;l++) {
        if (final[stari[l]] == 0) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < alfabet.size(); j++) {
                    stringstream ss;
                    ss << alfabet[j];
                    if(mat[stari[i]][ss.str()] == stari[l]){
                        mat[stari[i]][ss.str()] = stari[i];
                    }
                }
            }
            cout<<stari[l]<<" ";
            stari[l]=0;
        }
    }
    ordonare(stari);
    n = stari.size();
}
int main() {
    citire();
    minimalizare();
    afisare();
    return 0;
}
