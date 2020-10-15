#include <bits/stdc++.h>
#include <thread>




using namespace std;

class SequentialFile;

struct Empleado
{
    int Id;
    char Nombre [15];
    char Apellido[15];
    char Correo[30];
    char Ciudad[20];
    char Pais[40];
    char CodigoPais[2];
    int Inext = -1;
    char Cnext = 'd';

    void print ()
    {
        cout<<this->Id<<" ";
        for(int i = 0; i < 15; i++)
        {
            cout<<this->Nombre[i];
        }
        for(int i = 0; i < 15; i++)
        {
            cout<<this->Apellido[i];
        }
        for(int i = 0; i < 30; i++)
        {
            cout<<this->Correo[i];
        }
        for(int i = 0; i < 20; i++)
        {
            cout<<this->Ciudad[i];
        }
        for(int i = 0; i < 40; i++)
        {
            cout<<this->Pais[i];
        }
        for(int i = 0; i < 2; i++)
        {
            cout<<this->CodigoPais[i];
        }
        cout<<" "<<this->Cnext<<" ";
        cout<<this->Inext<<" "<<endl;
    }
};

    


template <typename T>
bool compare(T registro1, T registro2)
{
    return registro1.Id < registro2.Id; 
};

class SequentialFile 
{
public:
    string filename;
    string auxname;
    SequentialFile(string name)
    {
        this->filename = name;
        this->auxname = "auxi.dat";
        ofstream auxfile;
        auxfile.open(this->auxname, ios::binary | ios::app);
        int cont = 0;
        int freelist = -5;
        auxfile.write((char*)&cont,sizeof(int));
        auxfile.write((char*)&freelist,sizeof(int));
        auxfile.close();
    }

template <typename T>
    void ConvertirCsvaDat()
    {
        ifstream file;
        T registro;
        vector<T> registros;
        int i = 0;
        string line;
        file.open(this->filename);
        while(file.is_open())
        {
            while(getline(file,line))
            {
                stringstream stream(line);
                string valor;
                while(getline(stream,valor,',')){
                    switch(i)
                    {
                        case 0:
                            registro.Id = atoi(valor.c_str());
                            i++;
                            break;
                        case 1:
                            for(int i = (int)valor.size(); i < 15; i++)
                            {
                                valor = valor + " ";
                            }
                            strcpy(registro.Nombre, valor.c_str());
                            i++;
                            break;
                        case 2:
                            for(int i = (int)valor.size(); i < 15; i++)
                            {
                                valor = valor + " ";
                            }
                            strcpy(registro.Apellido, valor.c_str());
                            i++;
                            break;
                        case 3:
                            for(int i = (int)valor.size(); i < 30; i++)
                            {
                                valor = valor + " ";
                            }
                            strcpy(registro.Correo, valor.c_str());
                            i++;
                            break;
                        case 4:
                            for(int i = (int)valor.size(); i < 20; i++)
                            {
                                valor = valor + " ";
                            }
                            strcpy(registro.Ciudad, valor.c_str());
                            i++;
                            break;
                        case 5:
                            for(int i = (int)valor.size(); i < 40; i++)
                            {
                                valor = valor + " ";
                            }
                            strcpy(registro.Pais, valor.c_str());
                            i++;
                            break;
                        case 6:
                            strcpy(registro.CodigoPais, valor.c_str());
                            i = 0;
                            registros.push_back(registro);
                            break;
                    }
                }
            }

            file.close();
        }

        sort(registros.begin(),registros.end(),compare<T>);
        ofstream outfile;
        string nombre = this->filename + ".dat";
        outfile.open(nombre, ios::binary);

        int iheader = 0;
        char cheader = 'd';
        outfile.write((char*)&cheader,sizeof(char));
        outfile.write((char*)&iheader,sizeof(int));

        for(int i = 0; i < registros.size(); i++)
        {
            if(i<registros.size()-1)
            {
                registros[i].Inext = i+1;
            }
            outfile.write((char*)&registros[i],sizeof(T));
        }
        outfile.close();
        this->filename = nombre;
    }

template <typename T>
    void load()
    {
        ifstream file;
        file.open(filename, ios::binary);
        T registro;
        char c;
        int num;
        file.read((char*)&c,sizeof(char));
        file.read((char*)&num,sizeof(int));
        cout<<"Cargando registros del archivo "<<this->filename<<endl;
        cout<<"Encabezado: "<<num<<" "<<c<<endl;
        cout<<"     "<<"|Id  |"<<"Nombre        |"<<"Apellido       |"<<"Correo                      |"<<"Ciudad             |"<<"Pais                                   |"<<"CodPais|"<<endl;
        string guion = "-";
        for(int i = 0; i < 133;i++)
        {
            guion = guion + "-";
        }
        cout<<guion<<endl;
        int i = 1;
        while (file.read((char*) &registro, sizeof(T)))
        {
            if(i < 10)
                cout<<i<<"    |";
            else 
            {
                if(i < 100)
                    cout<<i<<"   |";
                else
                {
                if(i < 1000)
                    cout<<i<<"  |";
                else
                {
                    cout<<i<<"|";
                }
                }
            }
            registro.print();
            i++;
        }
        file.close();
    }

template<typename T>
    void loadaux()
    {
        ifstream file;
        file.open(this->auxname, ios::binary);
        T registro;
        int cont;
        int freelist;
        file.read((char*)&cont,sizeof(int));
        file.read((char*)&freelist,sizeof(int));
        cout<<"Cargando registros del archivo "<<this->auxname<<endl;
        cout<<"Contador: "<<cont<<" Freelist: "<<freelist<<endl;
        while (file.read((char*) &registro, sizeof(T)))
            {
                registro.print();
            }
        file.close();
    }

template <typename T>
    T Search(int Id, int &iindex, char &cindex)
    {
        cindex = 'd';
        ifstream file, auxfile;
        T registro;
        file.open(this->filename,ios::binary);
        if(file.is_open())
        {
            int low, mid, high;
            low = 0;
            file.seekg(0,ios::end);
            high = ((int)file.tellg()-5)/sizeof(T) - 1;
            file.seekg(5,ios::beg);
            while(low <= high)
            {
                mid = floor((low + high)/2);
                file.seekg(5 + mid*sizeof(T),ios::beg);
                file.read((char*)&registro,sizeof(T));
                if(registro.Id == Id )
                {
                    if(registro.Inext == -5) {

                        int code = -404;
                        registro.Id = code;

                        return registro;

                    }
                    file.close();
                    return registro;
                }
                if(registro.Id < Id)
                {
                    low = mid + 1;
                }
                else
                {
                    high = mid - 1;
                }
            }
            iindex = high;

            if(high == -1) //Entro a esta condición si el key que busco es menor que el primer registro del data file.
            {
                char cposi;
                int posi;
                file.seekg(0,ios::beg);
                file.read((char*)&cposi,sizeof(char));
                if(cposi=='d') //Aqui reviso si es que el primer registro del df es el menor de toda la data.
                {
                    file.close();
                    int code = -404;
                    registro.Id = code;
                    return registro;
                }
                file.read((char*)&posi,sizeof(int));
//Asigno la posición del menor registro de toda la data para empezar la busqueda de mi key desde ahí.
                registro.Cnext = cposi;
                registro.Inext = posi;
            }

            else {
//Me ubico en el anterior inmediato en donde debería estar el registro segun la busqueda.
                file.seekg(5 + high * sizeof(T),ios::beg);
                file.read((char*)&registro,sizeof(T));

            }

            if(registro.Cnext=='a')
            {
//Si este anterior tiene un siguiente en aux, entonces debo continuar mi busqueda en aux
                auxfile.open(this->auxname,ios::binary);
                if(auxfile.is_open())
                {
                    while(true)
                    {
                        if(registro.Inext==-1) //next = -1 significa que no tiene un siguiente por lo tanto se debe romper la busqueda.
                        {
                            break;
                        }
                        auxfile.seekg(sizeof(int)+ sizeof(int) + registro.Inext*sizeof(T),ios::beg);
                        int pos_anterior = registro.Inext;
                        auxfile.read((char*)&registro,sizeof(T));
                        if(registro.Id == Id)
                        {
                            file.close();
                            auxfile.close();
                            return registro;
                        }
                        if(registro.Id > Id)
                        {
                            auxfile.close();
                            break;
                        }
                        if(registro.Cnext == 'd')
                        {
                            if(registro.Id < Id)
                            {
                                cindex = 'a';
                                iindex = pos_anterior;
                            }
                            auxfile.close();
                            break;
                        }
                        cindex = 'a';
                        iindex = pos_anterior;
                    }
                }
            }
            file.close();
        }
//Para llegar aquí significa que nunca se encontró el key ni en el data file ni en el aux.
        //Trabajamos el error
        int code = -404;
        registro.Id = code;

        return registro;
    }

template<typename T>
    vector<T> search(int begin,int end)
    {
        ifstream file, auxfile;
        vector<T> registros;
        T registro;
        int begin_ipos;
        char begin_cpos;
        registro = Search<T>(begin,begin_ipos,begin_cpos);
        while(true)
        {
            registros.push_back(registro);
            if(registro.Id == end)
            {
                break;
            }
            if(registro.Cnext=='d')
            {
                file.open(this->filename,ios::in | ios::out | ios::binary);
                file.seekg(5 + registro.Inext*sizeof(T),ios::beg);
                file.read((char*)&registro,sizeof(T));
                file.close();
            }
            else
            {
                auxfile.open(this->auxname,ios::in | ios::out | ios::binary);
                auxfile.seekg(8 + registro.Inext*sizeof(T));
                auxfile.read((char*)&registro, sizeof(T));
                auxfile.close();
            }
        }
        return registros;
    }

template<typename T>
    void add(T registro)
    {
        fstream auxfile;
        fstream file;
        ofstream newfile;
        string nf = this->filename+"_1";
            T empleado;
            int contador;
            int ipos_insert = 0; //Aquí se obtiene la posición del registro anterior a donde se debe insertar.
            char cpos_insert = ' '; //Aquí se obtiene el archivo donde se encuentra el registro anterior a donde se debe insertar.
            Search<T>(registro.Id,ipos_insert,cpos_insert);
            if(cpos_insert == 'd')
            {
                file.open(this->filename,ios::in | ios::out | ios::binary);
                if(file.is_open())
                {
                    if(ipos_insert==-1) //Aquí solo se entra cuando el elemento a insertar es menor que el minimo de todo el df.
                    {
                        auxfile.open(auxname,ios::in | ios::out | ios::binary);
                        auxfile.seekg(0,ios::beg);
                        auxfile.read((char*)&contador,sizeof(int));
                        contador++;
                        int iaux1;
                        char caux1;
                        file.seekg(0,ios::beg);
                        file.read((char*)&caux1,sizeof(char));
                        file.read((char*)&iaux1,sizeof(int));
                        registro.Inext = iaux1;
                        registro.Cnext = caux1;
                        int temp = contador-1;
                        char t = 'a';
                        file.seekg(0,ios::beg);
                        file.write((char*)&t,sizeof(char));
                        file.write((char*)&temp,sizeof(int));
                        auxfile.seekg(0,ios::beg);
                        auxfile.write((char*)&contador,sizeof(int));
                        auxfile.seekg(8 + ((contador-1)*sizeof(T)));
                        auxfile.write((char*)&registro,sizeof(T));
                        auxfile.close();
                        file.close();
                    }
                    else{
                        file.seekg(5 + (ipos_insert*sizeof(T)),ios::beg);
                        file.read((char*)&empleado,sizeof(T));
                        if(empleado.Inext == -5) //Aquí se entra cuando el registro anterior a donde se va a insertar está eliminado
                        {
                            registro.Inext = ipos_insert + 1;
                            file.seekg(5 + (ipos_insert*sizeof(T)),ios::beg);
                            file.write((char*)&registro,sizeof(T));
                            if(ipos_insert > 0){
                            file.seekg(5 + ((ipos_insert-1)*sizeof(T)),ios::beg);
                            file.read((char*)&empleado,sizeof(T));
                            empleado.Inext = ipos_insert;
                            file.seekg(5 + ((ipos_insert-1)*sizeof(T)),ios::beg);
                            file.write((char*)&empleado,sizeof(T));
                            file.close();
                            }
                        }
                        else{
                        if(empleado.Cnext == 'd' && empleado.Inext != ipos_insert + 1 && empleado.Inext !=-1)
                        { //Cuando el registro posterior al anterior donde voy a insertar está eliminado.
                                registro.Cnext = 'd';
                                registro.Inext = empleado.Inext;
                                empleado.Inext = ipos_insert + 1;
                                file.seekg(5 + ((ipos_insert+1)*sizeof(T)),ios::beg);
                                file.write((char*)&registro,sizeof(T));
                                file.seekg(5 + ((ipos_insert)*sizeof(T)),ios::beg);
                                file.write((char*)&empleado,sizeof(T));
                                file.close();
                        }
                        else{
                            //Aquí se entra cuando la inserción se hará en el archivo aux.
                        auxfile.open(auxname,ios::in | ios::out | ios::binary);
                        int freelist;
                        auxfile.seekg(0,ios::beg);
                        auxfile.read((char*)&contador,sizeof(int));
                        auxfile.seekg(sizeof(int),ios::beg);
                        auxfile.read((char*)&freelist,sizeof(int));
                        int pos;
                        if(freelist != -5)
                        {
                            T eliminado;
                            pos = freelist;
                            auxfile.seekg(8 + (freelist*sizeof(T)));
                            auxfile.read((char*)&eliminado,sizeof(T));
                            freelist = eliminado.Inext;
                            registro.Inext = empleado.Inext;
                            registro.Cnext = empleado.Cnext;
                            empleado.Cnext = 'a';
                            empleado.Inext = pos;
                            file.seekg(5 + (ipos_insert * sizeof(T)));
                            file.write((char*)&empleado,sizeof(T));
                            auxfile.seekg(8 + (pos*sizeof(T)));
                            auxfile.write((char*)&registro,sizeof(T));
                            auxfile.seekg(sizeof(int),ios::beg);
                            auxfile.write((char*)&freelist,sizeof(int));
                            contador++;
                            auxfile.seekg(0,ios::beg);
                            auxfile.write((char*)&contador,sizeof(int));
                        }
                        else
                        {
                            contador++;
                            registro.Inext = empleado.Inext;
                            registro.Cnext = empleado.Cnext;
                            empleado.Inext = contador-1;
                            empleado.Cnext = 'a';
                            file.seekg(5 + (ipos_insert * sizeof(T)));
                            file.write((char*)&empleado,sizeof(T));
                            auxfile.seekg(8 + ((contador-1)*sizeof(T)),ios::beg);
                            auxfile.write((char*)&registro,sizeof(T));
                            auxfile.seekg(0,ios::beg);
                            auxfile.write((char*)&contador,sizeof(int));
                        }
                        file.close();
                        auxfile.close();
                        }
                        }
                    }
                }
            }
            else
            { //Aquí se entrará cuando la el registro anterior a donde voy a insertar se encuentra en el aux.
                        auxfile.open(auxname,ios::in | ios::out | ios::binary);
                        int freelist;
                        auxfile.seekg(8 + (ipos_insert*sizeof(T)));
                        auxfile.read((char*)&empleado,sizeof(T));
                        auxfile.seekg(sizeof(int),ios::beg);
                        auxfile.read((char*)&freelist,sizeof(int));
                        auxfile.seekg(0,ios::beg);
                        auxfile.read((char*)&contador,sizeof(int));
                        int pos;
                        if(freelist != -5)
                        {
                            T eliminado;
                            pos = freelist;
                            auxfile.seekg(8 + (freelist*sizeof(T)));
                            auxfile.read((char*)&eliminado,sizeof(T));
                            freelist = eliminado.Inext;
                            registro.Inext = empleado.Inext;
                            registro.Cnext = empleado.Cnext;
                            empleado.Cnext = 'a';
                            empleado.Inext = pos;
                            file.seekg(5 + (ipos_insert * sizeof(T)));
                            file.write((char*)&empleado,sizeof(T));
                            auxfile.seekg(8 + (pos*sizeof(T)));
                            auxfile.write((char*)&registro,sizeof(T));
                            auxfile.seekg(sizeof(int),ios::beg);
                            auxfile.write((char*)&freelist,sizeof(int));
                            contador++;
                            auxfile.seekg(0,ios::beg);
                            auxfile.write((char*)&contador,sizeof(int));
                        }
                        else
                        {

                            contador++;
                            registro.Inext = empleado.Inext;
                            registro.Cnext = empleado.Cnext;
                            empleado.Inext = contador-1;
                            empleado.Cnext = 'a';
                            auxfile.seekg(8 + (ipos_insert * sizeof(T)));
                            auxfile.write((char*)&empleado,sizeof(T));
                            auxfile.seekg(8 + ((contador-1)*sizeof(T)),ios::beg);
                            auxfile.write((char*)&registro,sizeof(T));
                            auxfile.seekg(0,ios::beg);
                            auxfile.write((char*)&contador,sizeof(int));
                        }
                        file.close();
                        auxfile.close();
                        } 
    }


};

void FuncionHilosAdd(Empleado empleado){

    auto test = SequentialFile("Data2.csv");
    test.ConvertirCsvaDat<Empleado>();
    test.load<Empleado>();
    test.loadaux<Empleado>();

    test.add(empleado);


}
void FuncionHilosSearch(int id){

    auto test = SequentialFile("Data2.csv");
    test.ConvertirCsvaDat<Empleado>();
    test.load<Empleado>();
    test.loadaux<Empleado>();

    int i = 0;
    char c = ' ';

    auto t = test.Search<Empleado>(id,i,c);

            if(t.Id == 404)
            {
                cout<<"No se encontro ninguna coincidencia para tu busqueda"<<endl;
                
            }
            else
            {
                t.print();
            }
  }


  void FuncionHilosDelete(int ide){
    cout << "Se elimino correctamente" << endl;
  }


int main()
{
  /*
    auto test = SequentialFile("Data2.csv");
    test.ConvertirCsvaDat<Empleado>();
    test.load<Empleado>();
    test.loadaux<Empleado>();

    int i = 0;
    char c = ' ';
    int opcion;
    while(true){
    cout<<endl;
    cout<<"Usted puede realizar las siguientes operaciones en este gestor de Base de datos"<<endl<<endl;
    cout<<"1. Busqueda"<<endl;
    cout<<"2. Insertar registro"<<endl;
    cout<<"3. Eliminacion de registro"<<endl;
    cout<<"4. Mostrar resultados"<<endl;
    cout<<"5. Salir"<<endl<<endl;
    cout<<"Escriba el numero de la opción que desee performar"<<endl;
    cin>>opcion;
    switch(opcion)
    {
        case 1:{
            int id;
            cout<<"Usted eligio la opción de busqueda"<<endl;
            cout<<"Ingrese el id que busca"<<endl;
            cin>>id;
            auto t = test.Search<Empleado>(id,i,c);
            if(t.Id == 404)
            {
                cout<<"No se encontro ninguna coincidencia para tu busqueda"<<endl;
                break;
            }
            else
            {
                t.print();
                break;
            }
            }
        case 2:{
            cout<<"Usted eligio la opcion de insercion"<<endl;
            cout<<"Ingrese los datos del registro a insertar"<<endl;
            auto t1 = Empleado();
            int id;
            string nombre;
            string apellido;
            string correo;
            string ciudad;
            string pais;
            string codigopais;
            cin>>id>>nombre>>apellido>>correo>>ciudad>>pais>>codigopais;
            t1.Id = id;
            strcpy(t1.Nombre,nombre.c_str());
            strcpy(t1.Apellido,apellido.c_str());
            strcpy(t1.Correo,correo.c_str());
            strcpy(t1.Ciudad,ciudad.c_str());
            strcpy(t1.Pais,pais.c_str());
            strcpy(t1.CodigoPais,codigopais.c_str());
            test.add(t1);
            cout<<"Insercion finalizada"<<endl;
            break;
        }
        case 3:
            cout<<"Usted eligio la opcion de eliminacion"<<endl;
            break;
        case 4:
            cout<<endl;
            cout<<"Mostrando resultados"<<endl;
            test.load<Empleado>();
            test.loadaux<Empleado>();
    }
    if(opcion == 5)
        break;
    }
      */
/*
            auto t1 = Empleado();
            int id;
            string nombre;
            string apellido;
            string correo;
            string ciudad;
            string pais;
            string codigopais;
            cin>>id>>nombre>>apellido>>correo>>ciudad>>pais>>codigopais;
            t1.Id = id;
            strcpy(t1.Nombre,nombre.c_str());
            strcpy(t1.Apellido,apellido.c_str());
            strcpy(t1.Correo,correo.c_str());
            strcpy(t1.Ciudad,ciudad.c_str());
            strcpy(t1.Pais,pais.c_str());
            strcpy(t1.CodigoPais,codigopais.c_str());
            
*/

  //std::thread thread01(&FuncionHilosAdd, t1);
  //thread01.join();


  int ide;
  cout<<"Ingrese el id que desea eliminar"<<endl;
  cin>>ide;

  std::thread thread02(&FuncionHilosSearch, ide);
  thread02.join();


  int ideDel;
  std::thread thread03(&FuncionHilosDelete, ideDel);
  thread03.join();




    
}