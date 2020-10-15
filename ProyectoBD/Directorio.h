#include <bits/stdc++.h>
#include "Empleado.h"
#include "Bucket.h"


using namespace std;


class Directorio {

private:

    int ProfundidadGlobal;

    map<int, string> index;

    vector<Bucket *> buckets;


public:
    Directorio()
    {

    }
    Directorio(int ProfundidadGlobal) {

        this->ProfundidadGlobal = ProfundidadGlobal;

        CrearDirectorio(ProfundidadGlobal);

        CargarDirectorioAMeroriaPrincipal(ProfundidadGlobal);

    };


    string ConvertirDaB(int numero) {
        string resultado;

        if (numero > 0) {

            while (numero > 0) {
                if (numero % 2 == 0) {
                    resultado = "0" + resultado;
                } else {
                    resultado = "1" + resultado;
                }
                numero = (int) numero / 2;
            }
        } else if (numero == 0) {
            resultado = "0";
        } else {
            throw exception();
        }

        return resultado;
    }


    void CrearDirectorio(int GlobalDepth) {

        fstream DirectorioFile;

        DirectorioFile.open("Directorio.dat", ios::binary | ios::app);


        if (DirectorioFile.is_open()) {
            string temp2;

            //Lleno el string de ceros.
            for (int i = 0; i < pow(2, GlobalDepth); i++) {

                int contador = GlobalDepth;

                while (contador > 0) {

                    temp2.push_back('0');

                    contador--;
                }

            }

            //
            for (int i = 0; i < pow(2, GlobalDepth); i++) {
                string hola = temp2;
                string temp = ConvertirDaB(i);

                int contador = GlobalDepth;

                int iterador = 0;

                //Itero el string de ceros y pongo en la variable hola puros ceros y al final el valor binario
                for (int j = hola.size(); j > 0; j--) {

                    if (contador >= 0) {

                        if (iterador < temp.size()) {

                            hola[j] = temp[iterador];

                        }
                        contador--;
                    } else {
                        break;
                    }
                    iterador++;

                }

                char Copia[GlobalDepth];
                int corre = 0;
                int it = GlobalDepth;

                for (int j = hola.size(); j > 0; j--) {

                    if (it > 0) {
                        char temp3 = hola[j];
                        int num = GlobalDepth - 2;

                        if (i == 2) {

                            if (temp3 == '1') {

                                Copia[num] = '1';
                            } else {
                                if (corre != num) {
                                    Copia[corre] = temp3;

                                } else {
                                    corre++;
                                    continue;
                                }
                                corre++;

                            }

                        } else {
                            Copia[corre] = temp3;
                            corre++;
                        }
                        it--;

                    } else if (it == 0) {
                        break;
                    }


                }

                char CopiaInvertida[GlobalDepth];

                for (int x = 0, y = (GlobalDepth - 1); x <= GlobalDepth; x++, y--) {

                    CopiaInvertida[x] = Copia[y];

                    if (y < 0) {
                        break;
                    }
                }

                //Escribir en el file los binarios
                DirectorioFile.seekg(0, ios::end);


                if ((i % 2) == 0) {
                    DirectorioFile.write((char *) &Copia, sizeof(GlobalDepth));

                } else {
                    DirectorioFile.write((char *) &CopiaInvertida, sizeof(GlobalDepth));
                }

            }
            DirectorioFile.close();

        }

    }

    int GetProfundiadGlobal(){
        return this->ProfundidadGlobal;
    }

    void CargarDirectorioAMeroriaPrincipal(int GlobalDepth) {

        char DatosIndex[GlobalDepth];

        //CARGAR ABRIR EL ARCHIVO
        ifstream file;

        file.open("Directorio.dat", ios::binary);

        file.seekg(0, ios::beg);
        int pos = 0;

        cout << "Index: " << endl;
        while (file.good()) {

            file.read((char *) &DatosIndex, sizeof(GlobalDepth));
            if (pos < pow(2, GlobalDepth)) {

                cout << DatosIndex << endl;
                this->index.insert({pos, DatosIndex});
            }

            pos++;

        }

    }


    string funcionHash(int key) {

        int NumeroParaElmodulo;
        string keyFinal;

        //cin>> NumeroParaElmodulo;

        NumeroParaElmodulo = 4;

        NumeroParaElmodulo = key % NumeroParaElmodulo;

        keyFinal = ConvertirDaB(NumeroParaElmodulo);

        return keyFinal;
    }

    void insert(int key, Empleado registro) {


        string KeyEnBinario = funcionHash(key);


        char temp[this->ProfundidadGlobal];

        for (int i = 0; i < this->ProfundidadGlobal; i++) {
            temp[i] = '0';
        }
/*
        for (int i = 0; i < this->ProfundidadGlobal; i++) {
            temp[i] = KeyEnBinario[i];

            if (KeyEnBinario[i] == this->ProfundidadGlobal) {
                temp[i] = '0';
            }
        }*/

        char temp2[this->ProfundidadGlobal];

        for (int x = 0, y = (this->ProfundidadGlobal - 1); x <= this->ProfundidadGlobal && y >= 0; x++, y--) {
            temp2[x] = temp[y];
        }


        for (auto it = this->index.begin(); it != this->index.end(); ++it) {

            //Para cuando la profundidad global es 1
            if (this->ProfundidadGlobal == 1) {
                int pos = 0;
                if (it->second[pos] == temp2[pos]) {


                    if (this->buckets.empty()) {

                        //Crear el bucker y escribir donde esta pauntando
                        auto NuevoBucket = new Bucket();

                        auto NombreBucket = NuevoBucket->GetBucketFileName();

                        fstream Bfile;
                        Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                        if (Bfile.is_open()) {

                            Bfile.seekg(0,ios::end);

                            Bfile.write((char *) &temp2, sizeof(this->ProfundidadGlobal));

                            Bfile.seekg(0, ios::end);

                            Bfile.write((char *) &registro, sizeof(Empleado));

                        }

                        Bfile.close();

                        buckets.push_back(NuevoBucket);
                    } else {
                        for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                            auto NombreBucket = (*it2)->GetBucketFileName();

                            fstream Bfile;

                            Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                            if (Bfile.is_open()) {

                                char lecturaIndex[ProfundidadGlobal];
                                int ProfLocal;

                                Bfile.seekg(0, ios::beg);

                                //En duda
                                Bfile.seekg(8 + (temp2[this->ProfundidadGlobal] * sizeof(this->ProfundidadGlobal)),
                                        ios::beg);

                                Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                                //En duda
                               Bfile.seekg(4 +((*it2)->GetProfundidadLocal() * sizeof(int)) ,
                                        ios::beg);

                                //Leo la profundidad local
                                Bfile.read((char *) &ProfLocal, sizeof(int));


                                if((*it2)->GetTamanio() == 100 ){

                                    ProfLocal++;

                                    if(ProfLocal > this->ProfundidadGlobal)
                                    {
                                        //RECONTRUIR INDICE UNO MAS
                                        cout << "Recontruir el indice";

                                        int ProfGlobal = GetProfundiadGlobal();

                                        ProfGlobal = ProfGlobal + 1 ;

                                        CrearDirectorio(ProfGlobal);


                                        //Tengo que iterar mi buckes


                                    }

                                }

                                if(ProfLocal > this->ProfundidadGlobal){

                                }




                                for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                    if (temp2[i] == lecturaIndex[i]) {
                                        continue;
                                    }
                                    if (temp2[i] == NULL) {



                                        //Escribo el record
                                        Bfile.seekg(0, ios::end);
                                        Bfile.write((char *) &registro, sizeof(Empleado));
                                    } else {
                                        break;
                                    }
                                }
                            }

                        }
                    }
                }
            }

            //Si la profundida Glonal es 2
            if (this->ProfundidadGlobal == 2) {

                int pos = 0;

                if (it->second[pos] == temp2[pos]) {
                    pos++;
                    if (it->second[pos] == temp2[pos]) {

                        if (this->buckets.empty()) {

                            //Crear el bucker y escribir donde esta pauntando
                            auto NuevoBucket = new Bucket();

                            auto NombreBucket = NuevoBucket->GetBucketFileName();

                            fstream Bfile;
                            Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                            if (Bfile.is_open()) {

                                Bfile.seekg(0, ios::beg);

                                Bfile.seekg(NuevoBucket->GetProfundidadLocal() * sizeof(int) +
                                            sizeof(this->ProfundidadGlobal), ios::beg);

                                Bfile.write((char *) &temp2, sizeof(this->ProfundidadGlobal));

                                Bfile.seekg(0, ios::end);

                                Bfile.write((char *) &registro, sizeof(Empleado));

                            }

                            Bfile.close();

                            buckets.push_back(NuevoBucket);
                        } else {

                            for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                                auto NombreBucket = (*it2)->GetBucketFileName();

                                fstream Bfile;
                                Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                                if (Bfile.is_open()) {

                                    char lecturaIndex[ProfundidadGlobal];

                                    Bfile.seekg(0, ios::beg);

                                    Bfile.seekg((*it2)->GetProfundidadLocal() * sizeof(int) +
                                                sizeof(this->ProfundidadGlobal), ios::beg);

                                    Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                                    for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                        if (temp2[i] == lecturaIndex[i]) {
                                            continue;
                                        }
                                        if (temp2[i] == NULL) {

                                            //Escribo el record
                                            Bfile.seekg(0, ios::end);
                                            Bfile.write((char *) &registro, sizeof(Empleado));
                                        } else {
                                            break;
                                        }
                                    }

                                    Bfile.seekg(0, ios::end);

                                    Bfile.close();
                                }
                            }
                        }
                    }
                }
            }

                //Para una profundidad Global mayor a 2
            else {
                int pos = 0;

                for (auto it2 = 0; it2 < it->second.size(); it2++) {

                    if (it->second[pos] == temp2[pos]) {
                        pos++;
                        continue;
                    }

                    if (pos == (this->ProfundidadGlobal-1)) {

                        if (this->buckets.empty()) {

                            //Crear el bucker y escribir donde esta pauntando
                            auto NuevoBucket = new Bucket();

                            auto NombreBucket = NuevoBucket->GetBucketFileName();

                            fstream Bfile;
                            Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                            if (Bfile.is_open()) {

                                Bfile.seekg(0, ios::beg);

                                Bfile.seekg(NuevoBucket->GetProfundidadLocal() * sizeof(int) +
                                            sizeof(this->ProfundidadGlobal), ios::beg);

                                Bfile.write((char *) &temp2, sizeof(this->ProfundidadGlobal));

                                Bfile.seekg(0, ios::end);

                                Bfile.write((char *) &registro, sizeof(Empleado));

                            }

                            Bfile.close();

                            buckets.push_back(NuevoBucket);
                        } else {

                            for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                                auto NombreBucket = (*it2)->GetBucketFileName();

                                fstream Bfile;
                                Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                                if (Bfile.is_open()) {

                                    char lecturaIndex[ProfundidadGlobal];

                                    Bfile.seekg(0, ios::beg);

                                    Bfile.seekg((*it2)->GetProfundidadLocal() * sizeof(int) +
                                                sizeof(this->ProfundidadGlobal), ios::beg);

                                    Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                                    for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                        if (temp2[i] == lecturaIndex[i]) {

                                            Bfile.seekg(0, ios::end);
                                            Bfile.write((char *) &registro, sizeof(Empleado));

                                        } else {
                                            break;
                                        }
                                    }

                                    Bfile.seekg(0, ios::end);

                                    Bfile.close();

                                }


                            }

                        }
                    } else {
                        break;
                    }

                }

            }

        }

    }

    void search(int key) {

        string KeyEnBinario = funcionHash(key);


        char temp[this->ProfundidadGlobal];

        for (int i = 0; i < this->ProfundidadGlobal; i++) {
            temp[i] = '0';
        }

        for (int i = 0; i < this->ProfundidadGlobal; i++) {
            temp[i] = KeyEnBinario[i];
            if (KeyEnBinario[i] == NULL) {
                temp[i] = '0';
            }
        }

        char temp2[this->ProfundidadGlobal];

        for (int x = 0, y = (this->ProfundidadGlobal - 1); x <= this->ProfundidadGlobal && y >= 0; x++, y--) {
            temp2[x] = temp[y];
        }


        for (auto it = this->index.begin(); it != this->index.end(); ++it) {

            //Para cuando la profundidad global es 1
            if (this->ProfundidadGlobal == 1) {
                int pos = 0;
                if (it->second[pos] == temp2[pos]) {


                    if (this->buckets.empty()) {

                        cout << "Hasta el momento no se han encontrado registro. Deberias insertar uno.";
                    }
                } else {
                    for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                        auto NombreBucket = (*it2)->GetBucketFileName();

                        fstream Bfile;

                        Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                        if (Bfile.is_open()) {

                            char lecturaIndex[ProfundidadGlobal];

                            Bfile.seekg(0, ios::beg);

                            Bfile.seekg((*it2)->GetProfundidadLocal() * sizeof(int) + sizeof(this->ProfundidadGlobal),
                                        ios::beg);

                            Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                            for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                if (temp2[i] == lecturaIndex[i]) {
                                    continue;
                                }
                                if (temp2[i] == NULL) {
                                    cout << "Se encontro el registro con la key: " << key << endl;
                                } else {
                                    cout << "No se encontro el registro con la key: " << key << endl;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            //Si la profundida Glonal es 2
            if (this->ProfundidadGlobal == 2) {

                int pos = 0;

                if (it->second[pos] == temp2[pos]) {
                    pos++;
                    if (it->second[pos] == temp2[pos]) {

                        if (this->buckets.empty()) {

                            cout << "Hasta el momento no se han encontrado registro. Deberias insertar uno.";
                        }

                    } else {

                        for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                            auto NombreBucket = (*it2)->GetBucketFileName();

                            fstream Bfile;
                            Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                            if (Bfile.is_open()) {

                                char lecturaIndex[ProfundidadGlobal];

                                Bfile.seekg(0, ios::beg);

                                Bfile.seekg(
                                        (*it2)->GetProfundidadLocal() * sizeof(int) + sizeof(this->ProfundidadGlobal),
                                        ios::beg);

                                Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                                for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                    if (temp2[i] == lecturaIndex[i]) {
                                        continue;
                                    }
                                    if (temp2[i] == NULL) {

                                        cout << "Se encontro el registro con la key: " << key << endl;
                                    } else {
                                        cout << "No se encontro el registro con la key: " << key << endl;
                                        break;
                                    }
                                }

                                Bfile.seekg(0, ios::end);

                                Bfile.close();
                            }
                        }
                    }
                }
            }

            //Para una profundidad Global mayor a 2
        else {
            int pos = 0;

            for (auto it2 = 0; it2 < it->second.size(); it2++) {

                if (it->second[pos] == temp2[pos]) {
                    pos++;
                    continue;
                }

                if (temp2[pos] == NULL) {

                    if (this->buckets.empty()) {

                        cout << "Hasta el momento no se han encontrado registro. Deberias insertar uno.";

                    } else {

                        for (auto it2 = buckets.begin(); it2 != buckets.end(); it2++) {


                            auto NombreBucket = (*it2)->GetBucketFileName();

                            fstream Bfile;
                            Bfile.open(NombreBucket, ios::binary | ios::in | ios::out);

                            if (Bfile.is_open()) {

                                char lecturaIndex[ProfundidadGlobal];

                                Bfile.seekg(0, ios::beg);

                                Bfile.seekg(
                                        (*it2)->GetProfundidadLocal() * sizeof(int) + sizeof(this->ProfundidadGlobal),
                                        ios::beg);

                                Bfile.read((char *) &lecturaIndex, sizeof(this->ProfundidadGlobal));

                                for (int i = 0; i < this->ProfundidadGlobal; i++) {
                                    if (temp2[i] == lecturaIndex[i]) {

                                        cout << "Se encontro el registro con la key: " << key << endl;

                                    } else {
                                        cout << "No se encontro el registro con la key: " << key << endl;
                                        break;
                                    }
                                }

                                Bfile.seekg(0, ios::end);

                                Bfile.close();

                            }


                        }

                    }
                }

                else {
                    break;
                }

            }

        }

    }

}


    void split(){





    }





/*
    void insert(int key,string valor,bool reinserted)
    {
        int bucket_no;
        int status = buckets[bucket_no]->insert(key,valor);
        if(status==1)
        {
            if(!reinserted)
                cout<<"Inserted key "<<key<<" in bucket "<<bucket_id(bucket_no)<<endl;
            else
                cout<<"Moved key "<<key<<" to bucket "<<bucket_id(bucket_no)<<endl;
        }
        else if(status==0)
        {
            split(bucket_no);
            insert(key,valor,reinserted);
        }
        else
        {
            cout<<"Key "<<key<<" already exists in bucket "<<bucket_id(bucket_no)<<endl;
        }

    };
*/

/*
    void remove(int key,int mode){
        int bucket_no = hash(key);
        if(buckets[bucket_no]->remove(key))
            cout<<"Deleted key "<<key<<" from bucket "<<bucket_id(bucket_no)<<endl;
        if(mode>0)
        {
            if(buckets[bucket_no]->isEmpty() && buckets[bucket_no]->ObtenerProfundidadLocal()>1)
                merge(bucket_no);
        }
        if(mode>1)
        {
            shrink();
        }

    };
*/

/*
    void update(int key, string valor){

        int bucket_no = hash(key);
        buckets[bucket_no]->update(key,valor);

    };
*/


/*
    void search(int key){

        int bucket_no = hash(key);
        cout<<"Searching key "<<key<<" in bucket "<<bucket_id(bucket_no)<<endl;
        buckets[bucket_no]->search(key);


    };
*/


/*
    void display(bool duplicates){
        int i,j,d;
        string s;
        std::set<string> shown;
        cout<<"Global depth : "<<ProfundidadGlobal<<endl;
        for(i=0;i<buckets.size();i++)
        {
            d = buckets[i]->ObtenerProfundidadLocal();
            s = bucket_id(i);
            if(duplicates || shown.find(s)==shown.end())
            {
                shown.insert(s);
                for(j=d;j<=ProfundidadGlobal;j++)
                    cout<<" ";
                cout<<s<<" => ";
                buckets[i]->Print();
            }
        }

    };
*/


};