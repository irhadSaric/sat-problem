#include <iostream>
#include "sat.h"

using namespace std;



int main()
{
    SAT sat("uf50-01.cnf");
    /*vector<int> rj;
    for(int i = 0; i <= 20; i++){
        rj.push_back(0);
    }
    sat.ispisi_klauze();
    sat.set_rjesenje(rj);
    cout << "br: " << sat.broj_zadovoljenih() << endl;*/
    //vector<int> randomRj;
//    randomRj = sat.random_rj();
//    sat.set_rjesenje(randomRj);
    cout << "lokalna: " << sat.local_search() << endl;
    cout << "tabu: " << sat.tabu_search(1, 100) << endl;
    cout << "simultano: " << sat.simultano_kaljenje(0.01, 0.3, 1) << endl;


    return 0;
    /*
    SAT sat(10, 5);
    vector<int> rj;
    rj.push_back(-11);
    rj.push_back(1);
    rj.push_back(0);
    rj.push_back(0);
    rj.push_back(0);
    rj.push_back(1);
    sat.set_rjesenje(rj);
    for(int i = 0; i < 6; i++)
        cout << rj[i] << " ";
    cout << endl;
    sat.ispisi_klauze();
    cout << sat.broj_zadovoljenih();
    */
    return 0;
}
