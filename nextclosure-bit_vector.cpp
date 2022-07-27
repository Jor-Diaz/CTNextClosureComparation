#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> 


#include <sdsl/bit_vectors.hpp>

using namespace sdsl;
using namespace std;


bool comparar(int cant_atr, sdsl::int_vector<1> actual_clausura, sdsl::int_vector<1> nueva_clausura, int pos)
{    
    for (int i = 0; i < pos ; ++i)
    {        
        if(actual_clausura[i]!=nueva_clausura[i])
        {                                
            return false;            
        }
    }     
    return true;
}


sdsl::int_vector<1> calcular_clausura(int cant_obj, int cant_atr, vector<sdsl::int_vector<1>>  &data_filas_atributos,vector<sdsl::int_vector<1>>  &data_filas_objetos, sdsl::int_vector<1> &array_atributos)
{
    //cout << "Entrada: "<< array_atributos << endl;                        
    bit_vector intermedio;
    bool flag_init = false;
    for (int i = 0; i < cant_atr; ++i)
    {
        if (array_atributos[i] == 1)
        {
            if (flag_init == false)
            {
                intermedio = data_filas_objetos[i];
                flag_init = true;
            }else{
                intermedio &= data_filas_objetos[i];
            }
        }        
        
        //aux &= data_filas_atributos[i];
        //cout << "aux : " <<aux << endl;
        //if (aux==array_atributos)
        //{
        //    intermedio[i]=1;
        //}
    }    
    bit_vector final = bit_vector(cant_atr, 0);    
    bit_vector aux_final = bit_vector(cant_obj, 0);    
    if (intermedio == aux_final){
        final = bit_vector(cant_atr, 1);    
        return final;
    }        
    bool flag=false;
    for (int i = 0; i < cant_obj; ++i)
    {
        if( intermedio[i]==1)
        {
            if (flag==false)
            {         
                final = data_filas_atributos[i];
                flag = true;
            }else{
                final &= data_filas_atributos[i];   

            }            
        }        
    }        
    return final;    
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

    cout << "[°] Cantidad obj: " << cant_obj <<endl;
    cout << "[°] Cantidad  atr: " << cant_atr <<endl;
    float cant_memory=0;    
    
    vector <bit_vector> data_filas_atributos;
    vector <bit_vector> data_filas_objetos;
    for (int i = 0; i < cant_atr; ++i)
    {

    }
    for (int i = 0; i < cant_atr; ++i)
    {
        bit_vector fila = bit_vector(cant_obj, 0);
        data_filas_objetos.push_back(fila);                
    }    
    for (int i = 0; i < cant_obj; ++i)
    {
        bit_vector fila = bit_vector(cant_atr, 0);
        data_filas_atributos.push_back(fila);        

    }            
    int pos = 0;
    while (getline(archivo, linea)) { //Acepta desde 0       
        stringstream  lineStream(linea);   
        while(lineStream >> value)
        {                                                    
            data_filas_atributos[pos][value] = 1;            
            data_filas_objetos[value][pos] = 1;            
        }    
        pos++;
    }    
    cout<< size_in_bytes(data_filas_atributos)<<endl;
    cout<< size_in_bytes(data_filas_objetos)<<endl;
    cant_memory+=size_in_bytes(data_filas_atributos);    
    cant_memory+=size_in_bytes(data_filas_objetos);    
    //for (int i = 0; i < cant_obj; ++i)
    //{
    //    cout << data_filas_atributos[i] << endl;
    //    cout << "Tam Bytes: " << size_in_bytes(data_filas_atributos[i])<<endl;
    //}    
    //cout << data_filas_atributos.size() << endl;
    //cout << data_filas_atributos[0].size() << endl;    
    //cout << "Tam Vector casillas: " << data_filas_atributos.size() + (cant_obj*data_filas_atributos[0].size()) <<endl;
    //cout << "Tam Vector Mb: " << size_in_mega_bytes(data_filas_atributos) + (size_in_mega_bytes(data_filas_atributos[0])*cant_obj) <<endl;
    //cout << "Tam Vector Bytes: " << size_in_bytes(data_filas_atributos)<<endl;

    int largo_clausura = 0;
    int numero_conceptos = 1;    
    int clausuras_calculadas = 1;
    

    bit_vector actual_clausura = bit_vector(cant_atr, 0);
    bit_vector nueva_clausura;
        
    while(largo_clausura<cant_atr)
    {                
        for (int i = cant_atr-1; i >= 0; --i)
        {        
            if(actual_clausura[i]==1)
            {
                actual_clausura[i]=0;
            }else{
                clausuras_calculadas++;
                actual_clausura[i]=1;
                nueva_clausura=calcular_clausura(cant_obj,cant_atr,data_filas_atributos,data_filas_objetos,actual_clausura);                                    
                if(comparar(cant_atr,actual_clausura,nueva_clausura,i)==true){                                        
                    numero_conceptos++;                    
                    //cout << numero_conceptos << " : " << nueva_clausura << endl;
                    sdsl::rank_support_v<1> b_rank(&nueva_clausura);
                    largo_clausura = b_rank(nueva_clausura.size());                    
                    break;
                }else{                    
                    actual_clausura[i]=0;
                }                         

            }                    
        
        }                                   
        bit_vector tmp = actual_clausura;
        actual_clausura = nueva_clausura;
        nueva_clausura = actual_clausura;            
    }    
    t1 = clock(); 
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    //liberar_matriz(cant_obj,data);    
    //delete[] actual_clausura;   
    //delete[] nueva_clausura; 
    //delete[] largo_clausura; 
    cout << "N° Conceptos :" <<numero_conceptos <<endl;
    cout << "N° Clausuras :" <<clausuras_calculadas <<endl;

    string filename("results/results_bitvector.txt");
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

