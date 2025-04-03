#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Node{
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead, was_chief, is_chief;
    Node* left;
    Node* right;
    Node *contribuidores;

    Node(int _id, const string &_name, const string &_last_name, char _gender, int _age,
           int _id_father, bool _is_dead, bool _was_chief, bool _is_chief)
        : id(_id), name(_name), last_name(_last_name), gender(_gender), age(_age),
          id_father(_id_father), is_dead(_is_dead), was_chief(_was_chief),
          is_chief(_is_chief), left(nullptr), right(nullptr) {}
};

class ArbolClan{
    private:
        Node *root;
        bool insertarNodo();

    public:
        void leerArchivo();
        void mostrarSucesionActual();
        void mostrarLiderActual_Sucesor();
        void asesinarLider();
        void asignar_NuevoLider();
        void mostrarMiembros();
        void actualizarDatos();
};

int main(){
    int opcion;
    do {
        cout << "\nMENU DE OPCIONES:\n";
        cout << "1. Imprimir linea de sucesion actual \n";
        cout << "2. Imprimir lider actual y sucesor\n";
        cout << "3. Asesinar al lider y asignar nuevo\n";
        cout << "4. Imprimir todos los miembros del clan\n";
        cout << "5. Actualizar datos de una persona\n";
        cout << "6. Visualizar lista de contribuidores al clan\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                //Mostrar linea de sucesion actual (Solo vivos)
                break;
            case 2:
                //Mostrar datos de lider actual y su sucesor
                break;
            case 3:
                //Asesinar al lider actual y asignar uno nuevo
                break;
            case 4:
                //Mostrar todos los miembros
                break;
            case 5: {
                //Modificar datos de persona
                break;
            }
            case 6:
                //Mostrar lista de contribuidores del clan
                break;
            case 7:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    } while (opcion != 7);

    return 0;
}