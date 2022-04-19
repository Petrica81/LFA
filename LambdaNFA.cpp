#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
std::ifstream f("automat.in");
std::ofstream g("automat.out");
enum class tip {
    DFA,
    NFA
};
class automat{
protected:
    int n,m,start,fin;
    std::vector<int> vfin;
    std::vector<std::vector<std::pair<std::string,std::string>>> mat;

public:
    automat(){
        n=0;
        m=0;
        start=0;
        fin=0;
        vfin={};
        mat={};
    }
    void setn(int n_){
        n = n_;
    };
    friend std::istream &operator>>(std::istream &is, automat &x){
        is>>x.n;
        is>>x.m;
        std::vector<std::pair<std::string,std::string>> v2 = {};
        for( int i = 0; i < x.n; i++) {
            x.mat.push_back(v2);
            for(int j = 0; j< x.n; j++){
                x.mat[i].push_back(std::make_pair("",""));
            }
        }
        int aux1,aux2;
        std::string aux3;
        for(int i = 0; i < x.m; i++){
            is>>aux1;
            is>>aux2;
            is>>aux3;
            if(x.mat[aux1][aux2].second.size() == 0) x.mat[aux1][aux2] = std::make_pair(std::to_string(aux2),aux3);
            else x.mat[aux1][aux2].second.append(aux3);
        }
        is>>x.start>>x.fin;
        for(int i = 0; i < x.fin; i++){
            is>>aux1;
            x.vfin.push_back(aux1);
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream &os, automat &x){
        os<<x.n<<'\n'<<x.m<<'\n';
        for(int i = 0;i < x.n; i++) {
            for (int j = 0; j < x.n; j++) {
                if(x.mat[i][j].second.compare("")!=0) {
                    os << i << ' ' << x.mat[i][j].first << ' ' << x.mat[i][j].second;
                    os<<'\n';
                }
            }
        }
        os<<x.start<<'\n'<<x.fin<<'\n';
        for(int i = 0; i < x.fin; i++)
            os<<x.vfin[i]<<' ';
        os<<std::endl;
        return os;
    }
};

class NFA : automat{
private:
    std::vector<int> ciclu;
public:
    friend std::istream& operator>>(std::istream &is, NFA &x){
        automat *aut = &x;
        is>>*aut;
        return is;
    }
    friend std::ostream& operator<<(std::ostream &os, NFA &x){
        automat *aut = &x;
        os<<*aut;
        return os;
    }
    int verificare(std::string cuv){
        int suma = 0;
        int loc = this->start;
        if(cuv.empty()) {
            bool corect = false;
            for (int i = 0; i < this->fin; i++)
                if (loc == this->vfin[i]) corect = true;
            if (corect) return 1;
            else return 0;
        }
        for (int i = 0; i < this->n; i++) {
            if (this->mat[loc][i].second.find(cuv.front()) != -1){
                this->start = i;
                std::string cuv2 = cuv.substr(1);
                suma = suma + verificare(cuv2);
                this->start = loc;
            }
        }
        return suma;
    }
    void afver(std::string cuv){
        if(this->verificare(cuv) == 0) std::cout<<"NU ";
        else std::cout<<"DA ";
    }
    int lverificare(std::string cuv){
        int suma = 0;
        int loc = this->start;
        if(cuv.empty()) {
            bool corect = false;
            for (int i = 0; i < this->fin; i++)
                if (loc == this->vfin[i]) corect = true;
            if (corect) return 1;
            else {
                for (int i = 0; i < this->n; i++) {
                    if (this->mat[loc][i].second.find("0") != -1 && ciclu[i] == 0) {
                        this->start = i;
                        ciclu[i] = 1;
                        suma = suma + lverificare(cuv);
                        ciclu[i] = 0;
                        this->start = loc;
                    }
                }
            }
        }
        for (int i = 0; i < this->n; i++) {
            if (this->mat[loc][i].second.find("0") != -1 && ciclu[i] == 0){
                this->start = i;
                ciclu[i] = 1;
                suma = suma + lverificare(cuv);
                ciclu[i] = 0;
                this->start = loc;
            }
            if (this->mat[loc][i].second.find(cuv.front()) != -1){
                this->start = i;
                std::vector<int> ciclu2;
                for(int i = 0; i < this->n; i++) ciclu2.push_back(0);
                ciclu.swap(ciclu2);
                std::string cuv2 = cuv.substr(1);
                suma = suma + lverificare(cuv2);
                ciclu.swap(ciclu2);
                this->start = loc;
            }
        }
        return suma;
    }
    void lambdaver(std::string cuv){
        for(int i = 0; i < this->n; i++) ciclu.push_back(0);
        if(this->lverificare(cuv) == 0) std::cout<<"NU ";
        else std::cout<<"DA ";
    }
};

class DFA : automat {
public:
    friend std::istream &operator>>(std::istream &is, DFA &x) {
        automat *aut = &x;
        is >> *aut;
        return is;
    }
    friend std::ostream &operator<<(std::ostream &os, DFA &x) {
        automat *aut = &x;
        os << *aut;
        return os;
    }
    int verificare(std::string cuv){
        int loc = this->start;
        while(!cuv.empty()) {
            bool corect = false;
            for (int i = 0; i < this->n; i++) {
                if (this->mat[loc][i].second.find(cuv.front()) != -1) {
                    corect = true;
                    loc = i;
                    cuv.erase(0,1);
                }
            }
            if(!corect) return 0;
        }
        bool corect = false;
        if(cuv.empty()) {
            for (int i = 0; i < this->fin; i++)
                if (loc == this->vfin[i]) corect = true;
            if (corect) return 1;
            else return 0;
        }
    }
    void afverificare(std::string cuv){
        if(this->verificare(cuv) == 0) std::cout<<"NU ";
        else std::cout<<"DA ";
    }
};

int main() {
    NFA x1;
    f>>x1;
    int n;
    std::string cuvinte;
    f>>n;
    for(int i = 1; i <= n; i++){
        f>>cuvinte;
        x1.lambdaver(cuvinte);
    }
    g<<x1;
    return 0;
}
