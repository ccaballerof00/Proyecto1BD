#include <bits/stdc++.h>

using namespace std;


class Bucket {

private:


    int ProfundidadLocal;

    int tamanio;

    string BucketFileName;

    int NumeroBucket;

    map<int, string> valores;

public:

    Bucket() {
        this->ProfundidadLocal = 1;
        this->tamanio = 0;

        srand(time(NULL));

        int   num=1+rand()%(1001-1);;

        this->NumeroBucket = num;
        CrearBucket();
    };

   void CrearBucket(){

    fstream BucketFile;

    string ConverNumeroBucket;

    ConverNumeroBucket = static_cast<std::ostringstream*>(&(std::ostringstream() << NumeroBucket))->str();

    this->BucketFileName = "Bucket" + ConverNumeroBucket + ".dat";


    BucketFile.open(this->BucketFileName, ios::binary | ios::app);


    if(BucketFile.is_open()){

        BucketFile.seekg(0, ios::beg);
        BucketFile.write((char*)&ProfundidadLocal, sizeof(int));
        BucketFile.seekg(4, ios::beg);
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

