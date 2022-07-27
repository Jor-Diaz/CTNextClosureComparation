#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime> 

#include <sdsl/bit_vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/rank_support_v.hpp>
#include <sdsl/rank_support_v5.hpp>


#include "src/intersection.cpp"
#include "src/flatBinTrie.hpp"
#include "src/binTrie_il.hpp"

using namespace std;
using namespace sdsl;


bool check_atributos (vector<uint64_t> &actual_clausura, uint64_t value )
{
    for (int i = 0; i < actual_clausura.size(); ++i)
    {        
        if (actual_clausura[i] <= value)
        {
            if (actual_clausura[i]==value)
           {
                return true;               
           }       
        }else{
            return false;
        }
    }
    return false;
}



bool comparar(vector<uint64_t> &actual_clausura, vector<uint64_t> &nueva_clausura, int pos)
{            
    int menor=0;
    int i=0;    
    if (actual_clausura.size()<nueva_clausura.size()){menor=actual_clausura.size();}else{menor=nueva_clausura.size();};    
    while((i < menor)) // problema aca
    {   
        
        if(actual_clausura[i]!=nueva_clausura[i] )
        {
            if (actual_clausura[i]<nueva_clausura[i]){menor=actual_clausura[i];}else{menor=nueva_clausura[i];};
            if(menor<pos)
            {
                return false;
            }else{
                return true;
            }
            
        }
        i++;
    }        
    return true;
}

flatBinTrie<rank_support_v<1>> calcular_clausura(int cant_obj, int cant_atr, vector <flatBinTrie<rank_support_v<1>>>  &data_filas_atributos,vector <flatBinTrie<rank_support_v<1>>>  &data_filas_objetos, flatBinTrie<rank_support_v<1>> &array_atributos)
{        
    vector<uint64_t> atributos_revisar;    
    vector<uint64_t> objetos_revisar;    
    array_atributos.decode(atributos_revisar);
    //cout << "Entrada: "<< atributos_revisar << endl;              
    vector<flatBinTrie<rank_support_v<1>>> filas_objetos_revisar;                 
    vector<flatBinTrie<rank_support_v<1>>> filas_atributos_revisar; 
    flatBinTrie<rank_support_v<1>>* intermedio;                
    flatBinTrie<rank_support_v<1>>* final_calculo;    
    flatBinTrie<rank_support_v<1>>* tmp_1;      
    uint64_t auxiliar ; 
    bool flag_init = false;               
    for (int i = 0; i < atributos_revisar.size(); ++i)
    {                   
        //cout << atributos_revisar.size()<<endl;                
        filas_objetos_revisar.push_back(data_filas_objetos[atributos_revisar[i]]);                 
        if(i%14==0 && i!=0)
        {            
            intermedio=joinTries<flatBinTrie<rank_support_v<1>>>(filas_objetos_revisar,true,auxiliar);    
            if (flag_init)
            {
                (*tmp_1).free();
            }           
            tmp_1=intermedio;            
            filas_objetos_revisar.clear();
            filas_objetos_revisar.push_back((*intermedio));
            flag_init=true;        
        }                    
    }                
    if (filas_objetos_revisar.size()==1)
    {
        (filas_objetos_revisar[0]).decode(objetos_revisar);                    
    }else{
        intermedio=joinTries<flatBinTrie<rank_support_v<1>>>(filas_objetos_revisar,true,auxiliar);        
        (*intermedio).decode(objetos_revisar);            
    }
    
    if (objetos_revisar.size()==0){
        vector<uint64_t> fin_clausuras;
        for (int i = 0; i < cant_atr; ++i)
        {
              fin_clausuras.push_back((uint64_t) i);
        }  
        flatBinTrie<rank_support_v<1>> final = flatBinTrie<rank_support_v<1>>(fin_clausuras,cant_atr);
        return final;
    }                      
    flatBinTrie<rank_support_v<1>>* aux_final;
    flatBinTrie<rank_support_v<1>>* tmp;       
    bool flag_etc = false;
    for (int i = 0; i < objetos_revisar.size(); ++i)
    {                                   
        filas_atributos_revisar.push_back(data_filas_atributos[objetos_revisar[i]]);                    
        atributos_revisar.clear();
        data_filas_atributos[objetos_revisar[i]].decode(atributos_revisar);        
        if(i%14==0 && i!=0){
            //cout << "que pasa ahora matias, siempre pasa algo"<<endl;
            
            aux_final=joinTries<flatBinTrie<rank_support_v<1>>>(filas_atributos_revisar,true,auxiliar);                            
            if (flag_etc)
            {
                (*tmp).free();
            }                    
            tmp=aux_final;            
            filas_atributos_revisar.clear();            
            filas_atributos_revisar.push_back((*aux_final));
            flag_etc=true;            
        }             
    }                
    //(*intermedio).free();    
    final_calculo=joinTries<flatBinTrie<rank_support_v<1>>>(filas_atributos_revisar,true,auxiliar);        
    //if(flag_etc){
    //    (*aux_final).free();    
    //}                 
    return (*final_calculo);    
} 
    
int main(int argc, char *argv[])
{     
    unsigned t0, t1; 
    t0=clock();
    string nombreArchivo = argv[1];
    cout << "[°] Abriendo archivo: " << nombreArchivo <<endl;
    string linea;        
    int cant_atr,cant_obj,value;    
    ifstream archivo(nombreArchivo.c_str());        
    getline(archivo, linea);
    stringstream  lineStream(linea);    
    lineStream >> value;
    cant_obj=value;
    lineStream >> value;
    cant_atr=value;
    uint64_t cant_memory=0;  
    cout << "[°] Cantidad obj: " << cant_obj <<endl;
    cout << "[°] Cantidad  atr: " << cant_atr <<endl;
    
    vector <flatBinTrie<rank_support_v<1>>> data_filas_atributos;
    vector <flatBinTrie<rank_support_v<1>>> data_filas_objetos;
    vector <vector<uint64_t>> auxset_objetos;
    //auxset_objetos -> reserve(cant_atr);
    for (int i = 0; i < cant_atr; ++i)
    {
        vector<uint64_t> auxset1;        
        //auxset1 -> reserve(cant_obj);
        auxset_objetos.push_back(auxset1);        
    }        
    int pos = 0;
    while (getline(archivo, linea)) { //Acepta desde 0       
        //vector<uint64_t>* auxset = new vector<uint64_t>();
        //auxset -> reserve(cant_atr);
        //while(lineStream >> value)
        //{                                                
            //auxset -> push_back((uint64_t)value);                    
        //}    

        stringstream  lineStream(linea);   
        vector<uint64_t> auxset;
        //auxset ->reserve(cant_atr);
        while(lineStream >> value)
        {                                                
            auxset.push_back((uint64_t)value);
            auxset_objetos[value].push_back(pos);

        }            
        //flatBinTrie<rank_support_v<1>> *objeto = new flatBinTrie<rank_support_v<1>>(auxset,cant_atr);     //no acepta conjuntos vacios        
        flatBinTrie<rank_support_v<1>> objeto = flatBinTrie<rank_support_v<1>>(auxset,cant_atr);     //no acepta conjuntos vacios        
        cant_memory+=objeto.size_in_bytes();
        data_filas_atributos.push_back(objeto);
        //delete [] auxset;
        pos++;                
    }            
    for (int i = 0; i < cant_atr; ++i)
    {                
        //flatBinTrie<rank_support_v<1>> *set_atributos = new flatBinTrie<rank_support_v<1>>(auxset_objetos[i],cant_obj);        
        flatBinTrie<rank_support_v<1>> set_atributos = flatBinTrie<rank_support_v<1>>(auxset_objetos[i],cant_obj);        
        cant_memory+=set_atributos.size_in_bytes();        
        data_filas_objetos.push_back(set_atributos);                                
    }             

    cant_memory+=(uint64_t) cant_atr;
    cant_memory+=(uint64_t) cant_obj;
    //vector<uint64_t> tmp;
    vector<uint64_t> actual_vector_clausura;        
    vector<uint64_t> nuevo_vector_clausura;      
    flatBinTrie<rank_support_v<1>>  actual_clausura;    
    flatBinTrie<rank_support_v<1>>  nueva_clausura; 
    int clausuras_calculadas=1;   
    int numero_conceptos=1;               
    /////

    while(actual_vector_clausura.size()<cant_atr)
    {                        
        //cout << cant_atr - actual_vector_clausura.size()<<endl;
        for (int i = cant_atr-1; i >= 0; --i)
        {                                     
            if(check_atributos(actual_vector_clausura,i))
            {
                for (int j = 0; j < actual_vector_clausura.size(); ++j)
                {
                    if (actual_vector_clausura[j] == i)
                    {                                               
                        actual_vector_clausura.erase(actual_vector_clausura.begin() + j);                             
                        break;
                    }                    
                }                
            }else{
                clausuras_calculadas++;
                actual_vector_clausura.push_back(i);                     
                actual_clausura = flatBinTrie<rank_support_v<1>>(actual_vector_clausura,cant_atr);                
                nueva_clausura=calcular_clausura(cant_obj,cant_atr,data_filas_atributos,data_filas_objetos,actual_clausura);                                    
                nuevo_vector_clausura.clear();
                nueva_clausura.decode(nuevo_vector_clausura);                
                //cout<<"salida: " <<nuevo_vector_clausura<<endl;
                //actual_clausura.free();
                //nueva_clausura.free();                
                if(comparar(actual_vector_clausura,nuevo_vector_clausura,i)==true){                                        
                    numero_conceptos++;                    
                    //cout << numero_conceptos << " : " << nuevo_vector_clausura << endl;                                    
                    //cout << numero_conceptos <<endl;
                    break;
                }else{                                                            
                    actual_vector_clausura.pop_back();
                }                                         
            }                                        
        }                       
        //tmp = actual_vector_clausura;
        actual_vector_clausura=nuevo_vector_clausura;
        //nuevo_vector_clausura = tmp;                
    }    
    t1 = clock(); 
    ////
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    
    cout << "Execution Time: " << time << endl;    
    cout << "N° Conceptos :" <<numero_conceptos <<endl;
    cout << "N° Clausuras :" <<clausuras_calculadas <<endl;

    string filename("results/results_compressed-binary-tries.txt");
    fstream file_out;
    file_out.open(filename, std::ios_base::app);
    if (!file_out.is_open()) {
        cout << "failed to open " << filename << '\n';
    } else {
        file_out << nombreArchivo  << ";" << time <<";" << cant_memory << endl;
        cout << "Resultados registrados" << endl;
    }
    return 0;
}


