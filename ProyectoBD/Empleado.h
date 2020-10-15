#pragma once
#include <bits/stdc++.h>

using namespace std;


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
