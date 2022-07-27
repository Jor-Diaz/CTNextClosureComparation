#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> 

using namespace std;

void swap(bool* *x, bool* *y)
{
    bool* tmp = *x;
    *x = *y;
    *y = tmp;
}

bool comparar(int cant_atr, bool* actual_clausura, bool* nueva_clausura, int pos)
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


void imprimir_concepto_formal(int cant_atr, bool* concepto_formal, int numero_conceptos)
{
    cout << numero_conceptos  <<" : " ;
    for (int i = 0; i < cant_atr; ++i)    
    {
        cout << concepto_formal[i] << " ";        
    }
    cout << " " <<endl;
}
    

int calcular_clausura(int cant_obj, int cant_atr, bool** matriz, bool* array_atributos,int cant_atr_array,int* largo_clausura, bool* nueva_clausura)
{
    //cout << "Entrada  " <<endl;
    //for (int i = 0; i < cant_atr_array; ++i)    
    //{
    //    cout << array_atributos[i] << " ";        
    //}
    //cout << " " << endl;
    bool flag_add;
    bool* array_intermedio=new bool[cant_obj]();   
    int aux_largo_clausura=0;     
    int cant_aux_intermedio=0;

    //derivada de atributos
    for (int i = 0; i < cant_obj; ++i)
    {
        flag_add = true;
        for (int j = 0; j < cant_atr_array; ++j)
        {
            if (array_atributos[j]==1 && matriz[i][j]==0)
            {
                flag_add=false;
                break;
            }
        }
        if (flag_add)
        {
            array_intermedio[i]=1;
            cant_aux_intermedio++;
        }
    }     
    if(cant_aux_intermedio==0)
    {
        delete[] array_intermedio;        
        for (int i = 0; i < cant_atr; ++i)
        {
            nueva_clausura[i]=1;
        }
        largo_clausura[0]=cant_atr;
        return 0;
    }
    if (cant_atr_array==0)
    {
        delete[] array_intermedio;
        return 0;
    }        

    //Derivada Objetos
    flag_add = false;    
    for (int i = 0; i < cant_obj; ++i)
    {
        if (array_intermedio[i]==1)
        {            
            for (int j = 0; j < cant_atr; ++j)
            {                
                if (flag_add && nueva_clausura[j]==1)
                {
                    if (matriz[i][j]==0)
                    {
                        nueva_clausura[j]=0;
                        aux_largo_clausura--;
                    }                                                        
                }else if(flag_add==false)
                {
                    if (matriz[i][j]==1)
                    {
                        nueva_clausura[j]=1;
                        aux_largo_clausura++;   
                    }else{
                        nueva_clausura[j]=0;
                    }                
                }
            }
            flag_add=true;            
        }        
    }
    delete[] array_intermedio;    
    largo_clausura[0]=aux_largo_clausura;
    return 0;
} 

void imprimir_matriz(int cant_obj, int cant_atr, bool** matriz)
{
    for (int i = 0; i < cant_obj; ++i)
    {
        for (int j = 0; j < cant_atr; ++j)
        {
            cout << matriz[i][j]<< " ";            
        }
        cout<< " " <<endl;
    }   
}

void liberar_matriz(int cant_obj, bool** matriz)
{
    for (int i = 0; i < cant_obj; ++i)
    {
        delete[] matriz[i];
    }
    delete[] matriz;
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

    int cant_memory=0;
    
    bool** data = new bool*[cant_obj];    
    cant_memory+=cant_obj*sizeof(bool);
    cant_memory+=(cant_obj*(cant_atr*sizeof(bool)));
    for (int i = 0; i < cant_obj; ++i)
    {
        data[i]=new bool[cant_atr]();        
    }
    int pos = 0;
    while (getline(archivo, linea)) { //Acepta desde 0       
        stringstream  lineStream(linea);   
        while(lineStream >> value)
        {                                                
            data[pos][value]=1;
        }    
        pos++;
    }
    //imprimir_matriz(cant_obj,cant_atr,data);  

    bool* actual_clausura = new bool[cant_atr]();   
    bool* nueva_clausura= new bool[cant_atr]();    
    int* largo_clausura= new int[1];
    //bool* intermedio;
    
    int numero_conceptos=1;    
    int clausuras_calculadas=1;
    cant_memory+=(2*(cant_atr*sizeof(bool)));

    //Calculo clausuras conjunto vacio    
    calcular_clausura(cant_obj, cant_atr, data,actual_clausura,0,largo_clausura,nueva_clausura);                        
    //cout << "1 : "<< endl;
        
    //next closure        
    largo_clausura[0]=0;        
    while(largo_clausura[0]<cant_atr)
    {                
        for (int i = cant_atr-1; i >= 0; --i)
        {        
            if(actual_clausura[i]==1)
            {
                actual_clausura[i]=0;
            }else{
                clausuras_calculadas++;
                actual_clausura[i]=1;
                calcular_clausura(cant_obj, cant_atr, data,actual_clausura,cant_atr,largo_clausura,nueva_clausura);                            
                if(comparar(cant_atr,actual_clausura,nueva_clausura,i)==true){                                        
                    numero_conceptos++;                    
                    //imprimir_concepto_formal(cant_atr,nueva_clausura,numero_conceptos);                                                            
                    break;
                }else{                    
                    actual_clausura[i]=0;
                }                         
            }                    
        }                   
        swap(&actual_clausura,&nueva_clausura);            
        //intermedio=actual_clausura;            
        //actual_clausura=nueva_clausura;                
        //nueva_clausura=intermedio;        
    }    
    t1 = clock(); 
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    liberar_matriz(cant_obj,data);    
    delete[] actual_clausura;   
    delete[] nueva_clausura; 
    delete[] largo_clausura; 
    cout << "N° Conceptos :" <<numero_conceptos <<endl;
    cout << "N° Clausuras :" <<clausuras_calculadas <<endl;

    string filename("results/results_boolarray.txt");
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
