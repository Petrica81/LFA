#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <iomanip>
#include <deque>
#include <algorithm>
using namespace std;
ifstream f("NFA.in");
ofstream g("DFA.out");
int n,m,nf;
int start;
string alfabet;
vector<vector<int>> stari,starifinale;
deque<vector<int>> starinoi;
map<vector<int>,map<string,vector<int>>> mat;
void citire(){
    f>>n>>m;
    for(int i=1; i<=m; i++){
        int aux1,aux2;
        string aux3;
        f>>aux1>>aux2>>aux3;
        mat[{aux1}][aux3].push_back(aux2);
        if(alfabet.find(aux3) == -1)alfabet.append(aux3);
    }
    for(int i=0;i<n;i++)stari.push_back({i});
    f>>start>>nf;
    for(int i=1; i<=nf; i++){
        int aux1;
        f>>aux1;
        starifinale.push_back({aux1});
    }
}
void ordonare(vector<int> &vector){
    sort(vector.begin(),vector.end());
    for(int i = 0 ; i<vector.size()-1;i++){
            if(vector[i]==vector[i+1]) { vector.erase(vector.begin() + i+1); i--; }
        }
}
void afisare(){
    g<<"Nod";
    for(int i=0;i<alfabet.size();i++){
        g<<setw(10)<<alfabet[i];
    }
    g<<'\n';
    g<<">";
    for(int i=0;i<stari.size();i++){
        for (int j = 0; j < starifinale.size(); j++) {
            if (stari[i] == starifinale[j]) g << '*';
        }
        g<<setw(3);
        for(int j = 0; j < stari[i].size(); j++){
            g<<stari[i][j];
        }
        for(int j=0;j<alfabet.size();j++){
            g<<setw(10);
            stringstream ss;
            ss<<alfabet[j];
            if(mat[stari[i]][ss.str()].empty()) g<<"X";
            else {
                for (int k = 0; k < mat[stari[i]][ss.str()].size(); k++) {
                    g << mat[stari[i]][ss.str()][k];
                }
            }
        }
        g<<'\n';
    }
    g<<'\n';

}
void transformare(){
    while(!stari.empty()) stari.pop_back();
    int marime = 1;
    stari.push_back({0});
    map<vector<int>,map<string,vector<int>>> newmat;
    for(int i=0; i<alfabet.size();i++){
        stringstream ss;
        ss<<alfabet[i];
        if(mat[{0}][ss.str()].empty()) newmat[{0}][ss.str()] = {0};
        else{
            vector<int> aux;
            for(int j=0;j<mat[{0}][ss.str()].size();j++){
                aux.push_back(mat[{0}][ss.str()][j]);
            }
            ordonare(aux);
            newmat[{0}][ss.str()] = aux;
            starinoi.push_back(aux);
        }
    }
    for(int j=0;j<nf;j++){
        if(0 == starifinale[j][0]){
            starifinale.push_back({0});
            j=nf+1;
        }
    }
    while(!starinoi.empty()){
       if(!(newmat[starinoi[0]].empty())) { starinoi.pop_front();}
       else{
           for(int i=0;i<starinoi[0].size();i++)
               for(int j=0;j<nf;j++){
                   if(starinoi[0][i] == starifinale[j][0]){
                       starifinale.push_back(starinoi[0]);
                       j=nf+1;
                       i=starinoi[0].size()+1;
                   }
               }
           stari.push_back(starinoi[0]);
            for(int i = 0; i<alfabet.size();i++){
                vector<int> v;
                bool ceva = false;
                stringstream ss;
                ss<<alfabet[i];
                for(int j=0;j<starinoi[0].size();j++){
                    vector<int> vaux;
                    vaux.push_back(starinoi[0][j]);
                    if(!mat[vaux][ss.str()].empty()){
                        for(int k=0;k<mat[vaux][ss.str()].size(); k++){
                            v.push_back(mat[vaux][ss.str()][k]);
                        }
                        ceva = true;
                    }
                    vaux.pop_back();
                }
                if(ceva){
                    ordonare(v);
                    newmat[starinoi[0]][ss.str()] = v;
                    starinoi.push_back(v);
                }
                else{
                    newmat[starinoi[0]][ss.str()] = starinoi[0];
                }
            }
            marime++;
        }
    }
    starifinale.erase(starifinale.begin(),starifinale.begin()+nf);
    mat = newmat;
    n = marime;
}
int main() {
    citire();
    transformare();
    afisare();
    return 0;
}
