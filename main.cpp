#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<stack>
#include <bits/stdc++.h>
using namespace std;
ifstream f("pg.in");
ofstream g("pg.out");
const int MAX=100005;
class graf{
public:
    //DFS - declarari
    int N, M, S, vizitat[100010], minime[100010];
    vector<int> stocare[100010];
    queue<pair<int, int>> coada;

    //DFS - declarari
    //int N, M;
    vector<vector<int>> D;
    vector<int>ok; //pt a marca nodul ca vizitat sau nu - 0 sau 1
    graf(){}

    //CTC - declarari
    //int N, M;
    int k=0, viz[100005], viz_t[100005];
    vector <int>  rez[100005], G[100005], G_t[100005];
    stack <int> stiv;

    //CRITICAL CONNECTIONS - declarari
    vector<vector<int>>D_C;
    vector<vector<int>>ans;
    vector<int>d;
    vector<int>l;
    vector<int>tata;
    int kk=0, ck=0;

    //BICONEX - declarari
    //int N, M;
    int q=0;
    vector<int>D_B[100005];
    vector<vector<int>>rezultat;
    stack<pair<int, int>> st;
    int d_B[100005]={0}, l_B[100005]={0};


    //HAVEL HAKIMI - declarari
    //int N; // nr de elem din vector
    vector<int> v; //vectorul de grade


    //SORTARE TOPOLOGICA - declarari
    vector<int>D_ST[100005];
    stack<int>s;
    bool vizitat_st[MAX]={0};

//-----------------------------------------------------------------------
    //BFS
   void bfs(int start)
   {
        pair<int, int> curent;
        coada.push({start, 0}); // dist de la start la el insusi e 0
        minime[start] = 0; // momentan, minime va avea val 0
        vizitat[start] = 1; // vizitez nodul curent
        while(!coada.empty())
        {
            curent = coada.front();
            minime[curent.first] = curent.second;
            coada.pop();
            for(auto i : stocare[curent.first]) // parcurgem primele elem din pereche
                if(!vizitat[i]){
                    coada.push({i, curent.second+1}); //cresc contorul
                    vizitat[i] = 1; // vizitez nodul i
                }
        }

    }
    void creare_graf(int N, int M, int S)
    {
        this-> N = N;
        this-> M = M;
        this-> S = S;
        for(int i = 0; i< 100010; i++)
                minime[i] = -1; //umplem tabl de dist minime cu -1
    }
    void creare_adiacente(int M)
    {
        int nod1, nod2;
        for(int i = 1; i<= M; i++)
        {
            f>>nod1>>nod2;
            stocare[nod1].push_back(nod2);
        }

    }
//-------------------------------------------------------------------------------------------
    //DFS
    void dfs(int start)
    {
        ok[start] = 1;
        for(auto i: D[start]) // parcurg D
            if(ok[i]==0)
            {
                ok[i] =1;
                dfs(i);
            }
    }
    void creare_graf_Dfs(int N, int M)
    {
        this-> N = N;
        this -> M =M;
        ok=vector<int>(N+1);
        D=vector<vector<int>>(N+1);
    }
    void creare_adiacente_Dfs(int M)
    {
         int nod1, nod2;
         for(int i = 1; i<= M; i++)
        {
            f>>nod1>>nod2;
            D[nod1].push_back(nod2);
            D[nod2].push_back(nod1);
        }
    }
    int comp_conexe(int N)
    {
        int k= 0;
        for(int i =1; i<= N; i++)
            if(ok[i]==0)
            {
                k++;
                dfs(i);
            }
        return k;
    }
//----------------------------------------------------------------------------------------
    //CTC
    void dfs_ctc1(int nod1)
    {
        viz[nod1]=1; //vizitam nodul1 de start
        for(auto nod2: G[nod1]) //iteram prin vectorul de noduri1 cu nod2
            if(!viz[nod2]) //daca nod2 nu a fost vizitat
                dfs_ctc1(nod2);//apelam recusiv fctia de parcurgere in adancime pt nod2
        stiv.push(nod1); //adaugam nodul1 in stiva
    }
    void dfs_ctc2(int nod1)
    {
        viz_t[nod1]=1; //marcam in tabl viz_t nodul1
        rez[k].push_back(nod1); //retinem in dreptul nr de comp tari conexe indicativul nodului marcat
        for(auto nod2: G_t[nod1])
            if(!viz_t[nod2])
                dfs_ctc2(nod2);
    }
    void creare_graf_ctc(int N, int M)
    {
        this-> N=N;
        this-> M=M;
    }
    void creare_adiacente_ctc(int M)
    {
        for(int i=1; i<=M; i++)
        {
            int nod1, nod2;
            f>>nod1>>nod2;
            G[nod1].push_back(nod2);
            G_t[nod2].push_back(nod1);
        }
    }
    void fct_ctc(int N)
    {
        for(int i=1; i<=N; i++)
            if(viz[i]==0)
                dfs_ctc1(i); //pt fiecare nod nevizitat aplicam dfs_ctc1
        while(!stiv.empty()) //cat timp mai avem elemente in stiva
        {
            if(viz_t[stiv.top()]==0) //daca in viz_t nu a fost vizitat elem din vf-ul stivei
            {
                dfs_ctc2(stiv.top()); //facem dfs_ctc2
                k=k+1; //increm nr de comp
            }
            stiv.pop();
        }
        g<<k<<"\n";
        for(int i=0; i<k; i++)
        {
            for(auto it: rez[i])
                g<<it<<" ";
            g<<"\n";
        }
    }
//------------------------------------------------------------------------------------

    //CRITICAL CONNECTIONS - leetcode
    void dfs_critical_connections(int nod)
    {
        d[nod]=kk++; //vizitam fiecare nod pt ambii vect
        l[nod]=kk++;
        for(auto it: D_C[nod]) //iteram prin nodurile grafului
        {
            if(d[it]==-1)//daca nodul curent nu este vizitat
            {
                tata[it]=nod; //ii atribuim nodului curent it ca parinte pe nod
                dfs_critical_connections(it); //facem dfs din nodul curent
                l[nod]=min(l[nod], l[it]); //mergem pe varianta minima
                if(d[nod]<l[it]) //verificam daca avem nod de intoarcere
                    ans.push_back({nod, it});
            }
            else if(it!=tata[nod]) //daca nodul adiacent nu este parinte
                l[nod]=min(l[nod], d[it]); //minimaliz valoarea l
        }
    }
    void criticalConnections(int N, int M) {
        tata.resize(N,-1);
        l.resize(N,-1);
        D_C.resize(N);
        d.resize(N,-1);
        for(int i=0; i<M; i++)
        {
            int nod1, nod2;
            f>>nod1>>nod2;
            D_C[nod1].push_back(nod2);
            D_C[nod2].push_back(nod1);
        }
        for(int i=0; i<N; i++)
            if(d[i]==-1)
                    dfs_critical_connections(i); //facem dfs pe nodurile nevizitate
        for(int i=0; i<ans.size(); i++)
        {
            g<<"[";
            for(auto it: ans[i])
                g<<it<<" ";
            g<<"]";
        }
    }
//-----------------------------------------------------------------------

    // BICONEX
    void bic(int nod1, int nod2)
    {
        vector<int>local;
        while(st.top().first!=nod1 && st.top().second!=nod2)
        {
            int y=st.top().second;
            local.push_back(y);
            st.pop();
        }
        local.push_back(nod2);
        local.push_back(nod1);
        st.pop();
        rezultat.push_back(local);
    }
    void dfs_bic(int nod_curent)
    {
        q++;
        d_B[nod_curent]=q; //marcam ambele tablouri
        l_B[nod_curent]=q;
        for(auto nod_adiac: D_B[nod_curent])//iteram prin graf
           {
               if(d_B[nod_adiac]) //daca nodul este vizitat
                    l_B[nod_curent]=min(l_B[nod_curent], d_B[nod_adiac]); //pastram min
                else
                {//daca nu este vizitat
                    st.push({nod_curent, nod_adiac}); //retinem in sitva nodul curent si perechea sa
                    dfs_bic(nod_adiac);//facem dfs din nod_adiac
                    l_B[nod_curent]=min(l_B[nod_adiac], l_B[nod_curent]); //pastram min
                    if(l_B[nod_adiac]>=d_B[nod_curent])
                        bic(nod_curent, nod_adiac);//aplicam fct bic pt nodul curent si perechea sa
                }
           }
    }
    void creare_graf_bic(int N, int M)
    {
        this->N=N;
        this->M=M;
    }
    void creare_adiacente_bic(int M)
    {
        int nod1, nod2;
         for(int i = 1; i<= M; i++)
        {
            f>>nod1>>nod2;
            D_B[nod1].push_back(nod2);
            D_B[nod2].push_back(nod1);
        }
    }
    void afisare_bic()
    {
        int t=0;
        for(int i=0; i<rezultat.size(); i++)
            t+=1;
        g<<t<<"\n";
        for(int i=0; i<rezultat.size(); i++) //pt fiecare componenta biconexa
        {
            for(int j=0; j<rezultat[i].size(); j++)
                g<<rezultat[i][j]<<' ';// afisam nodurile care ii apartin
            g<<endl;
        }
    }
//--------------------------------------------------------------------

    //HAVEL HAKIMI
    bool HavelOK(int N, vector<int>&v)
    {
        while(1)
        {
            sort(v.begin(), v.end(), greater<>()); // sortam in ord descresc
            if(!v[0])
                return 1; //prima conditie de stop=>daca toate elem sunt 0, doar atunci se verifica algo Havel Hakimi
            int x=v[0]; //retinem primul elem
            v.erase(v.begin());//in stergem pe x din vector
            if(x>v.size())
                return 0; //a doua conditie de stop=>daca nu mai am destule elem atunci
            for(int i=0; i<x;i++)
            {
                v[i]=v[i]-1;//decrementam urmatoarele x elem cu 1
                if(v[i]<0)
                    return 0;//a treia conditie de stop=>daca intampinam un element negativ
            }
        }
    }
//---------------------------------------------------------
    //SORTARE TOPOLOGICA
    void dfs_st(int nod)
    {
        vizitat_st[nod]=1;
        for(auto nod_adj: D_ST[nod])
            if(vizitat_st[nod_adj]==0) //daca nodul adiacent sau nu a fost vizitat, facem dfs din el
                        dfs_st(nod_adj);
        s.push(nod); // adaugam nodul curent in stiva
    }
    void sortare_topologica()
    {
        for(int i=1; i<=N; i++)
            if(vizitat_st[i]==0) // daca nodul nu a fost vizitat
                dfs_st(i);

    }
    void afisare_sortare_topo()
    {
        while(!s.empty())
        {
            int var = s.top();
            g<<var<<" ";
            s.pop();
        }
    }
    void creare_graf_st(int N, int M)
    {
        this->N=N;
        this->M=M;

    }
    void creare_adiacente_st(int M)
    {
        int nod1, nod2;
        for(int i=0; i<M; i++)
        {
            f>>nod1>>nod2;
            D_ST[nod1].push_back(nod2);
        }

    }

};
graf Gr;
int main()
{
    //BFS
/*
    int N, M, S;
    f>>N>>M>>S;
    Gr.creare_graf(N, M, S);
    Gr.creare_adiacente(M);
    Gr.bfs(S);
    for(int i = 1; i<=N; i++)
        g<< Gr.minime[i]<<" ";

*/
    //DFS
    /*
    int N, M;
    f>>N>>M;
    Gr.creare_graf_Dfs(N, M);
    Gr.creare_adiacente_Dfs(M);
    g<<Gr.comp_conexe(N);
    return 0;
*/

    //CTC
    /*
    int N, M;
    f>>N>>M;
    Gr.creare_graf_ctc(N, M);
    Gr.creare_adiacente_ctc(M);
    Gr.fct_ctc(N);

    */

    //critical connections
    /*
    int N, M;
    f>>N>>M;
    Gr.criticalConnections(N, M);
    */

    //Componente Biconexe
/*
    int N, M;
    f>>N>>M;
    Gr.creare_graf_bic(N, M);
    Gr.creare_adiacente_bic(M);
    Gr.dfs_bic(1);
    Gr.afisare_bic();
*/

    //Havel Hakimi

    vector<int>v;
    int N;
    int h;
    f>>N;
    for(int i=0; i<N; i++)
        {
            f>>h;
            v.push_back(h);
        }

    if(Gr.HavelOK(N, v)==1)
        g<<"Da";
    else
        g<<"Nu";



    //Sortare Topologica
    /*
    int N, M;
    f>>N>>M;
    Gr.creare_graf_st(N, M);
    Gr.creare_adiacente_st(M);
    Gr.sortare_topologica();
    Gr.afisare_sortare_topo();
    */
    return 0;
}
