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
        bool insertarNodo(Node *&actual, Node *newNode);
        void borrarArbol(Node* node);

    public:
        ArbolClan(): root(nullptr){};
        ~ArbolClan(){
            borrarArbol(root);}
        void leerArchivo(const string &filename);
        void mostrarSucesionActual();
        void mostrarSucesionActual(Node *actual);
        void mostrarLiderActual_Sucesor();
        void asesinarLider();
        void asignar_NuevoLider();
        void mostrarMiembros();
        void actualizarDatos();
};

bool ArbolClan::insertarNodo(Node *&actual, Node *newNode){
    if (actual == nullptr){ //Si el arbol esta vacío
        actual = newNode;
        cout << "Raiz del arbol: " << newNode->name << " " << newNode->last_name << endl;
        return true;
    }

    if (actual->id == newNode->id_father){ //Si el id del nodo actual es igual al del padre
        if (actual->left == nullptr){ //Si no tiene hijos se inserta a la izquierda
            actual->left = newNode;
            cout << "Hijo izquierdo de " << actual->name << " " << actual->last_name << ": " << newNode->name << " " << newNode->last_name << endl;
        }
        else{ //Si ya tiene un hijo, inserta a la derecha
            Node *child = actual->left;
            while (child->right != nullptr){
                child = child->right;
            }
            child->right = newNode;
            cout << "Hijo derecho de " << actual->name << " " << actual->last_name << ": " << newNode->name << " " << newNode->last_name << endl;
        }
        return true;
    }

    bool inserted = false;
    if (actual->left != nullptr){
        inserted = insertarNodo(actual->left, newNode);
    }
    if (!inserted && actual->right != nullptr){
        inserted = insertarNodo(actual->right, newNode);
    }

    return inserted;
}

void ArbolClan::borrarArbol(Node* node) {
    if (node) {
        borrarArbol(node->left);
        borrarArbol(node->right);
        delete node;
    }
}

void ArbolClan::leerArchivo(const string &filename){
    ifstream file(filename);
    if (!file.is_open()){
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Leer la primera linea

    int count = 0; //Contador de registros
    while (getline(file, line)){
        stringstream ss(line);
        string field;

        int id, age, id_father;
        char gender;
        bool is_dead, was_chief, is_chief;
        string name, last_name;

        getline(ss, field, ',');
        id = stoi(field);
        getline(ss, name, ',');
        getline(ss, last_name, ',');
        getline(ss, field, ',');
        gender = field[0];
        getline(ss, field, ',');
        age = stoi(field);
        getline(ss, field, ',');
        id_father = stoi(field);
        getline(ss, field, ',');
        is_dead = stoi(field);
        getline(ss, field, ',');
        was_chief = stoi(field);
        getline(ss, field, ',');
        is_chief = stoi(field);

        Node *newNode = new Node(id, name, last_name, gender, age, id_father, is_dead, was_chief, is_chief);
        cout << "Insertando: " << newNode->name << " " << newNode->last_name << " (ID: " << newNode->id << ", is_dead: " << newNode->is_dead << ")" << endl;
        insertarNodo(root, newNode);
        count++;
    }

    cout << "Total de miembros insertados: " << count << endl;
    file.close();
}

void ArbolClan::mostrarSucesionActual(){
    cout << "Lista de familiares vivos:" << endl;
    mostrarSucesionActual(root);
}

void ArbolClan::mostrarSucesionActual(Node *actual){
    if (actual == nullptr)
        return;
    if (!actual->is_dead){
        cout << actual->name << " " << actual->last_name << " (ID: " << actual->id << ")" << endl;
    }
    mostrarSucesionActual(actual->left);
    mostrarSucesionActual(actual->right);
}

int main(){
    ArbolClan clan;
    clan.leerArchivo("data_clan.csv");

    int opcion;
    do {
        cout << "\nMENU DE OPCIONES:\n";
        cout << "1. Imprimir linea de sucesion actual \n";
        cout << "2. Imprimir lider actual y sucesor\n";
        cout << "3. Asesinar al lider y asignar nuevo\n";
        cout << "4. Imprimir todos los miembros del clan\n";
        cout << "5. Actualizar datos de una Nodea\n";
        cout << "6. Visualizar lista de contribuidores al clan\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                //Mostrar linea de sucesion actual (Solo vivos)
                clan.mostrarSucesionActual();
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
                //Modificar datos de Nodea
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