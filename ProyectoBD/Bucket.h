#include <bits/stdc++.h>

using namespace std;


class Bucket {

private:

//Cada Bucket tiene solo un archivo.
    //FixedRecord registros;

    int ProfundidadLocal;

    int tamanio;

    string BucketFileName;

    int NumeroBucket;

    long next_bucket;

    map<int, string> valores;




public:

//Inicio cada Bucket con su profundidad local y su tamaño
    Bucket() {
        this->ProfundidadLocal = 1;
        this->tamanio = 0;
        this->NumeroBucket = 0;
        CrearBucket();
    };

//Funcion para crear el Bucket
   void CrearBucket(){

    fstream BucketFile;

    string ConverNumeroBucket;

    ConverNumeroBucket = static_cast<std::ostringstream*>(&(std::ostringstream() << NumeroBucket))->str();

    this->BucketFileName = "Bucket" + ConverNumeroBucket + ".dat";


    BucketFile.open(this->BucketFileName, ios::binary | ios::in | ios::out);

    NumeroBucket++;


    if(BucketFile.is_open()){

        BucketFile.seekg(0, ios::beg);
        BucketFile.write((char*)&ProfundidadLocal, sizeof(int));
        BucketFile.write((char*)&tamanio, sizeof(int));
    }

    BucketFile.close();

}

   string GetBucketFileName(){
      return this->BucketFileName;
   }

   int GetProfundidadLocal(){
        return this->ProfundidadLocal;
    }

    int GetTamanio(){
       return this->tamanio;
   }

};

