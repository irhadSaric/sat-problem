#ifndef SATH
#define SATH

#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class SAT{
    int broj_klauza;
    int broj_varijabli;

    vector<vector<int>> lista_klauza;
    vector<int> rjesenje;

    bool zadovoljena_klauza(vector<int> klauza);
    int izracunaj_poboljsanje(vector<int> trenutno, int k);
    int broj_zadovoljenih(vector<int> rj);
    vector<int> random_rj();
public:
    SAT(string fajl);
    void set_rjesenje(vector<int> rj);
    int get_broj_klauza();
    int get_broj_varijabli();
    void ispisi_klauze();
    int broj_zadovoljenih();

    int local_search();
    int simultano_kaljenje(double tmin, double tmax, int max_pokusaja);
    int tabu_search(int broj_ponavljanja_algoritma, int broj_okretanja);
};

vector<int> extractIntegerWords(string str)
{
    stringstream ss;
    vector<int> result;
    /* Storing the whole string into string stream */
    ss << str;

    /* Running loop till the end of the stream */
    string temp;
    int found;
    while (!ss.eof()) {

        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
            result.push_back(found);

        /* To save from space at the end of string */
        temp = "";
    }
    return result;
}

SAT::SAT(string fajl)
{
    string line;
    ifstream myfile (fajl);
    int granica = 100;
    vector<int> klauza;
    if (myfile.is_open())
    {
        int brojac = 0;
        vector<int> brojevi;
        while ( getline (myfile,line) )
        {
            //cout << brojac << ": " << line << endl;
            if(brojac >= 7 && brojac <= 7 + granica)
            {
                brojevi = extractIntegerWords(line);

                if(brojac == 7)
                {
                    granica = brojevi[1];
                    broj_klauza = brojevi[1];
                    broj_varijabli = brojevi[0];
                }
                else
                {
                    for(int i = 0; i < brojevi.size() - 1; i++){
                        klauza.push_back(brojevi[i]);
                    }
                    lista_klauza.push_back(klauza);
                    klauza.clear();
                }
                brojevi.clear();
            }
            brojac++;
        }
        myfile.close();
    }

    else cout << "Unable to open file";
}

void SAT::set_rjesenje(vector<int> rj)
{
    rjesenje = rj;
}

int SAT::get_broj_klauza()
{
    return broj_klauza;
}

int SAT::get_broj_varijabli()
{
    return broj_varijabli;
}

void SAT::ispisi_klauze()
{
    for(int i = 0; i < lista_klauza.size(); i++)
    {
        cout << "(";
        for (int j = 0; j < lista_klauza[i].size(); j++)
        {
            if(j == lista_klauza[i].size()-1)
            {
                cout << lista_klauza[i][j] << ") ^ ";
            }
            else
            {
                cout << lista_klauza[i][j] << " v ";
            }
        }
    }
    cout << endl;
}

bool SAT::zadovoljena_klauza(vector<int> klauza)
{
    int varijabla;
    bool vrijednost;
    for(int i = 0; i < klauza.size(); i++)
    {
        varijabla = abs(klauza[i]);
        vrijednost = rjesenje[varijabla];
        if((klauza[i] < 0 && !vrijednost) || (klauza[i] > 0 && vrijednost))
            return true;
    }
    return false;
}

int SAT::broj_zadovoljenih(){
    int brojac = 0;
    for(int i = 0; i < lista_klauza.size(); i++)
    {
        if(zadovoljena_klauza(lista_klauza[i]))
        {
            //cout << i << " zadovoljena" << endl;
            brojac++;
        }
    }
    return brojac;
}

int SAT::broj_zadovoljenih(vector<int> rj){
    int brojac = 0;
    vector<int> pom = rjesenje;
    rjesenje = rj;
    for(int i = 0; i < lista_klauza.size(); i++)
    {
        if(zadovoljena_klauza(lista_klauza[i]))
        {
            //cout << i << " zadovoljena" << endl;
            brojac++;
        }
    }
    rjesenje = pom;
    return brojac;
}

vector<int> SAT::random_rj()
{
    srand(time(0));
    int random_bool;
    vector<int> rj;
    for(int i = 0; i <= broj_varijabli; i++)
    {
        random_bool = rand() % 2;
        rj.push_back(random_bool);
    }
    return rj;
}

int SAT::izracunaj_poboljsanje(vector<int> trenutno, int k)
{
    int delta_trenutno = broj_zadovoljenih(trenutno);
    int delta;
    if(trenutno[k])
        trenutno[k] = 0;
    else
        trenutno[k] = 1;

    delta = broj_zadovoljenih(trenutno);
    return delta_trenutno-delta;
}

void ispisi_rjesenje(vector<int> rj)
{
    for(int i = 0; i < rj.size(); i++){
        cout << rj[i] << " ";
    }
    cout << endl;
}

float sigmoid(float delta,float temperature){

    return 1/(1+exp(-delta/temperature));

}

int SAT::local_search()
{
    bool poboljsanje = true;
    vector<int> rj = random_rj();
    vector<int> najbolje = rj;
    while(poboljsanje)
    {
        poboljsanje = false;
        for(int i = 1; i <= broj_varijabli; i++)
        {
            rj[i] = !rj[i];
            if(broj_zadovoljenih(rj) > broj_zadovoljenih(najbolje))
            {
                najbolje = rj;
                poboljsanje = true;
            }
            rj[i] = !rj[i];
        }
        rj = najbolje;
    }
    return broj_zadovoljenih(najbolje);
}


int SAT::simultano_kaljenje(double tmin, double tmax, int max_pokusaja)
{
    int pokusaji = 1;
    //int max_pokusaja = 10000;
    vector<int> trenutno = random_rj();
    vector<int> najbolje = trenutno;
    rjesenje = trenutno;
    int j;
    double t;// = temperatura;
    int delta;
    double random;
    float hladenje;

    while(pokusaji <= max_pokusaja)
    {
        rjesenje = random_rj();
        j = 0;
        //t = temperatura;
        hladenje = 1.0/(pokusaji*broj_varijabli);
        while(true)
        {
            t = tmax * exp(-j*hladenje);
            //cout << t << ", " << hladenje << endl;
            if(t <= tmin) break;
            if(broj_zadovoljenih() == broj_klauza)
            {
                return broj_zadovoljenih();
            }

            for(int k = 1; k <= broj_varijabli; k++)
            {
                delta = izracunaj_poboljsanje(rjesenje, k);
                if(delta < 0)
                {
                    //ZAMIJENI
                    if(rjesenje[k]) rjesenje[k] = 0;
                    else rjesenje[k] = 1;
                }
                else
                {
                    random = ((double) rand() / (RAND_MAX));
                    float delta2 = sigmoid(delta, t);
                    if(random > delta)
                    {
                        if(rjesenje[k]) rjesenje[k] = 0;
                        else rjesenje[k] = 1;
                    }
                }

            }
            if(broj_zadovoljenih(rjesenje) > broj_zadovoljenih(najbolje))
                najbolje = rjesenje;
            j++;
//            for(int i = 0; i < rjesenje.size(); i++){
//                if(i == rjesenje.size()-1)
//                    cout << rjesenje[i] << endl;
//                else
//                    cout << rjesenje[i] << ", ";
//            }
        }
        pokusaji++;

        //cout << "pokusaji: " << pokusaji << " " << t << " " << broj_zadovoljenih(najbolje) << " , RJ: ";ispisi_rjesenje(rjesenje);cout << endl;
    }
    rjesenje = najbolje;
    return broj_zadovoljenih();
}

int SAT::tabu_search(int broj_ponavljanja_algoritma, int broj_okretanja)
{
    vector<int> najbolje;
    vector<int> trenutno;
    vector<int> najbolji_kandidat;
    int rj;

    int trenutno_ponavljanja_algoritma = 0;
    while(trenutno_ponavljanja_algoritma < broj_ponavljanja_algoritma)
    {
        najbolje = random_rj();
        trenutno = najbolje;
        najbolji_kandidat = trenutno;

        //cout << trenutno_ponavljanja_algoritma << " " << broj_zadovoljenih(najbolje) << endl;
        int najbolji_index;
        vector<int> tabu(broj_varijabli+1, 0);

        if(broj_zadovoljenih(trenutno) == broj_klauza)
            return broj_klauza;
        for(int i = 1; i < broj_okretanja; i++)
        {
            for(int index = 1; index <= broj_varijabli; index++)
            {
                if(!tabu[index])
                {
                    trenutno[index] = !trenutno[index];
                    if(broj_zadovoljenih(trenutno) > broj_zadovoljenih(najbolje))
                    {
                        najbolji_kandidat = trenutno;
                        najbolji_index = index;
                    }
                    else
                    {
                        trenutno[index] = !trenutno[index];
                    }
                }
            }

            if(broj_zadovoljenih(najbolji_kandidat) > broj_zadovoljenih(najbolje))
            {
                najbolje = najbolji_kandidat;
                if(broj_zadovoljenih(najbolje) == broj_klauza)
                    return broj_klauza;
            }
            tabu[najbolji_index] = 3;
            for(int k = 0; k < tabu.size(); k++)
            {
                if(tabu[k])
                {
                    tabu[k]--;
                }
            }
        }

        trenutno_ponavljanja_algoritma++;
    }

    return broj_zadovoljenih(najbolje);
}
#endif // SATH
