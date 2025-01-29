#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;
    
class Alumno{
    protected:
        string nya;
        int codigo;
        vector<float> notas;
    public:
        Alumno(int codigo, string nya, vector<float> notas) 
            : codigo(codigo),nya(nya),notas(notas){};

        string getNombre(){return nya;}
        int getCodigo(){return codigo;}
        vector<float> getNotas(){return notas;}

        float getPromedio(){
            float suma=0;
            for(auto nota : notas)
                suma+=nota;
            return suma/notas.size();
        }

        virtual bool aprueba() = 0;
        virtual string tipoAlumno() = 0;
};

class Invitado : public Alumno{
    public:
        Invitado(int codigo, string nya, vector<float> notas)
            : Alumno(codigo,nya,notas){}

        bool aprueba(){return notas.size() == 1 && getPromedio()>60;}
        string tipoAlumno(){return "Invitado";}
};

class Medio : public Alumno{
    public:
        Medio(int codigo, string nya, vector<float> notas)
            : Alumno(codigo,nya,notas){}

        bool aprueba(){return notas.size() == 3 && getPromedio()>70;}
        string tipoAlumno(){return "Medio";}
};

class Premium : public Alumno{
    public:
        Premium(int codigo, string nya, vector<float> notas)
            : Alumno(codigo,nya,notas){}

        bool aprueba(){
            if (notas.size()<5)return false;
            for(auto nota : notas) if (nota <=70) return false;
            return getPromedio()>80;
        }
        string tipoAlumno(){return "Premium";}
};

class Curso{
    private:
        string codigo;
        string nombre;
        vector<Alumno*>alumnos;
    public:
        Curso(string codigo, string nombre,vector<Alumno*>alumnos)
        : codigo(codigo),nombre(nombre),alumnos(alumnos){};

        void guardarArchivo();
        void getMayoresPromedios(){}
        void getCantXTipo(){}
        void agregarAlumno(Alumno* alumno){alumnos.push_back(alumno);}
        void listaAlumnosAprobados(){
            cout<<"Alumnos aprobados:\n";
            for(auto alumno : alumnos ) if (alumno->aprueba()) cout<<alumno->getNombre()<<endl;
        } 
};

void Curso:: getMayoresPromedios(){
    string mayorPromedioStr="";
    float mayorPromedio=0;

    for(auto alumno:alumnos){
        if(alumno->getPromedio()>mayorPromedio){
            mayorPromedio=alumno->getPromedio();
            mayorPromedioStr=alumno->getNombre();
        }
    }
    cout<<"Alumno con mayor promedio: "<<mayorPromedioStr<<". Promedio: "<<mayorPromedio<<endl;
}

void Curso:: getCantXTipo(){
    map <string, int> alumnado;
    for(auto alumno : alumnos) alumnado[alumno->tipoAlumno()]++;

    for(auto [tipo, cantidad] : alumnado)
        cout << tipo << ": " << cantidad << " alumnos"<<endl;
} 


void Curso::guardarArchivo(){
    string direccion = "curso_"+ codigo +".bin";
    ofstream archivo(direccion, ios::binary);
    if (!archivo){
        cout<<"Error al guardar datos del curso: "<<codigo;return;
    }

    for(auto alumno:alumnos){
        string tipo = alumno->tipoAlumno();
        size_t tipoLen = tipo.size();
        archivo.write(reinterpret_cast<const char*>(&tipoLen), sizeof(size_t));
        archivo.write(tipo.data(), tipoLen);  

        int codigo = alumno->getCodigo();
        archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(int));

        string nombre = alumno->getNombre();
        size_t nombreLen = nombre.size();
        archivo.write(reinterpret_cast<const char*>(&nombreLen), sizeof(size_t));
        archivo.write(nombre.data(), nombreLen);

        vector<float> notas = alumno->getNotas();
        size_t cantidadNotas = notas.size();
        archivo.write(reinterpret_cast<const char*>(&cantidadNotas), sizeof(size_t));
        archivo.write(reinterpret_cast<const char*>(notas.data()), cantidadNotas * sizeof(float));
    }
}