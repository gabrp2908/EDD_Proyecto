#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Contribuidor {
    string name;
    int age;
    int id;
    string descripcion;
    int grado_contribucion;
    Contribuidor *next;

    Contribuidor(const string &_name, int _age, int _id, 
                const string &_desc, int _grade)
        : name(_name), age(_age), id(_id), 
          descripcion(_desc), grado_contribucion(_grade), next(nullptr) {}
};

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
    Contribuidor *contribuidores;

    Node(int _id, const string &_name, const string &_last_name, char _gender, int _age,
           int _id_father, bool _is_dead, bool _was_chief, bool _is_chief)
        : id(_id), name(_name), last_name(_last_name), gender(_gender), age(_age),
          id_father(_id_father), is_dead(_is_dead), was_chief(_was_chief),
          is_chief(_is_chief), left(nullptr), right(nullptr),
          contribuidores(nullptr) {}
};

class ArbolClan{
    private:
        Node *root;
        bool insertarNodo(Node *&actual, Node *newNode);
        void borrarArbol(Node* node);
        Node *buscarLider(Node* liderActual);
        Node *buscarSucesor(Node *liderActual);
        void mostrarMiembrosClan(Node *actual);
        Node *buscarNodo(Node *actual, int id);
        Node *buscarTio(Node *actual);
        Node *buscarAncestroConDosHijos(Node *actual);
        void cargarContribuidores(Node *node, const string &filename);
        void mostrarContribuidores(Node *node);
        void liberarContribuidores(Contribuidor *contrib);
        static bool compararContribuidores(const Contribuidor *a, const Contribuidor *b);

    public:
        ArbolClan(): root(nullptr){};
        ~ArbolClan(){
            borrarArbol(root);}
        void leerArchivo(const string &filename);
        void mostrarSucesionActual();
        void mostrarSucesionActual(Node *actual);
        void mostrarLiderActual_Sucesor();
        void asesinarLider();
        void asignarLider();
        void mostrarMiembros();
        void actualizarDatos(int id, const string &name, const string &last_name, char gender, int age, bool is_dead, bool was_chief, bool is_chief);
        void cargarTodosContribuidores(const string &filename);
        void mostrarContribuidoresLider();
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
        liberarContribuidores(node->contribuidores);
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

        getline(ss, field, ';');
        id = stoi(field);
        getline(ss, name, ';');
        getline(ss, last_name, ';');
        getline(ss, field, ';');
        gender = field[0];
        getline(ss, field, ';');
        age = stoi(field);
        getline(ss, field, ';');
        id_father = stoi(field);
        getline(ss, field, ';');
        is_dead = stoi(field);
        getline(ss, field, ';');
        was_chief = stoi(field);
        getline(ss, field, ';');
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

void ArbolClan::mostrarLiderActual_Sucesor(){
    Node *liderActual = buscarLider(root);
    if (liderActual == nullptr){
        cout << "Actualmente no hay un lider asignado." << endl;
        return;
    }
    cout << "Lider actual: " << liderActual->name << " " << liderActual->last_name << endl;
    Node *sucesor = buscarSucesor(liderActual->left);
    if (!sucesor)
        sucesor = buscarSucesor(liderActual->right);
    if (sucesor){
        cout << "Sucesor: " << sucesor->name << " " << sucesor->last_name << endl;
    }
    else{
        cout << "No se encontro sucesor valido en la rama actual." << endl;
    }
}

void ArbolClan::asesinarLider() {
    Node* liderActual = buscarLider(root);
    if (liderActual == nullptr) {
        cout << "No hay lider actual." << endl;
        return;
    }
    
    // Marcar al líder como muerto y como ex-líder
    liderActual->is_dead = true;
    liderActual->was_chief = true;  // Actualizar was_chief a 1
    liderActual->is_chief = false;  // Ya no es el líder actual
    
    cout << "El lider " << liderActual->name << " " << liderActual->last_name 
         << " ha sido asesinado y ahora es un ex-líder." << endl;
    
    asignarLider();
}

void ArbolClan::asignarLider(){
    Node *liderActual = buscarLider(root);
    if (liderActual == nullptr){
        cout << "No hay lider actual" << endl;
        return;
    }

    if (liderActual->is_dead || liderActual->age > 70){
        liderActual->is_chief = false;
        Node *sucesor = buscarSucesor(liderActual->left);
        if (!sucesor)
            sucesor = buscarSucesor(liderActual->right);
        if (sucesor){
            sucesor->is_chief = true;
            cout << "Nuevo lider asignado: " << sucesor->name << " " << sucesor->last_name << endl;

            Node *proximoSucesor = buscarSucesor(sucesor->left);
            if (!proximoSucesor)
                proximoSucesor = buscarSucesor(sucesor->right);
            if (proximoSucesor){
                cout << "Siguiente sucesor: " << proximoSucesor->name << " " << proximoSucesor->last_name << endl;
            }
            else{
                cout << "No se encontro siguiente sucesor valido." << endl;
            }
        }
        else{
            Node *tio = buscarTio(liderActual);
            if (tio){
                sucesor = buscarSucesor(tio->left);
                if (!sucesor)
                    sucesor = buscarSucesor(tio->right);
                if (sucesor){
                    sucesor->is_chief = true;
                    cout << "Nuevo lider asignado: " << sucesor->name << " " << sucesor->last_name << endl;

                    Node *proximoSucesor = buscarSucesor(sucesor->left);
                    if (!proximoSucesor)
                        proximoSucesor = buscarSucesor(sucesor->right);
                    if (proximoSucesor){
                        cout << "Siguiente sucesor: " << proximoSucesor->name << " " << proximoSucesor->last_name << endl;
                    }
                    else{
                        cout << "No se encontro siguiente sucesor valido." << endl;
                    }
                }
                else if (!tio->is_dead){
                    tio->is_chief = true;
                    cout << "Nuevo lider asignado: " << tio->name << " " << tio->last_name << endl;

                    Node *proximoSucesor = buscarSucesor(tio->left);
                    if (!proximoSucesor)
                        proximoSucesor = buscarSucesor(tio->right);
                    if (proximoSucesor){
                        cout << "Siguiente sucesor: " << proximoSucesor->name << " " << proximoSucesor->last_name << endl;
                    }
                    else{
                        cout << "No se encontro siguiente sucesor válido." << endl;
                    }
                }
                else{
                    Node *ancestor = buscarAncestroConDosHijos(liderActual);
                    if (ancestor){
                        sucesor = buscarSucesor(ancestor->left);
                        if (!sucesor)
                            sucesor = buscarSucesor(ancestor->right);
                        if (sucesor){
                            sucesor->is_chief = true;
                            cout << "Nuevo lider asignado: " << sucesor->name << " " << sucesor->last_name << endl;

                            Node *proximoSucesor = buscarSucesor(sucesor->left);
                            if (!proximoSucesor)
                                proximoSucesor = buscarSucesor(sucesor->right);
                            if (proximoSucesor){
                                cout << "Siguiente sucesor: " << proximoSucesor->name << " " << proximoSucesor->last_name << endl;
                            }
                            else{
                                cout << "No se encontro siguiente sucesor valido." << endl;
                            }
                        }
                        else{
                            cout << "No se encontro sucesor valido." << endl;
                        }
                    }
                    else{
                        cout << "No se encontro sucesor valido." << endl;
                    }
                }
            }
            else{
                cout << "No se encontro sucesor valido." << endl;
            }
        }
    }
    else
    {
        cout << "El lider actual esta vivo: " << liderActual->name << " " << liderActual->last_name << endl;
    }
}

Node *ArbolClan::buscarLider(Node *actual){
    if (actual == nullptr)
        return nullptr;
    if (actual->is_chief)
        return actual;
    Node *chief = buscarLider(actual->left);
    if (chief == nullptr)
        chief = buscarLider(actual->right);
    return chief;
}

Node *ArbolClan::buscarSucesor(Node *actual) {
    if (actual == nullptr) return nullptr;
    if (!actual->is_dead && actual->gender == 'M') return actual;
    Node* sucesor = buscarSucesor(actual->left);
    if (sucesor == nullptr) sucesor = buscarSucesor(actual->right);
    return sucesor;
}

void ArbolClan::mostrarMiembros() {
    cout << "Lista de todos los miembros de la familia real:" << endl;
    mostrarMiembrosClan(root);
}

void ArbolClan::mostrarMiembrosClan(Node* actual) {
    if (actual == nullptr) return;

    cout << "ID: " << actual->id << ", Nombre: " << actual->name << " " << actual->last_name
        << ", Genero: " << actual->gender << ", Edad: " << actual->age
        << ", ID Padre: " << actual->id_father << ", Muerto: " << (actual->is_dead ? "Si" : "No")
        << ", Fue Lider: " << (actual->was_chief ? "Si" : "No") 
        << ", Es Lider: " << (actual->is_chief ? "Si" : "No") << endl;

    mostrarMiembrosClan(actual->left);
    mostrarMiembrosClan(actual->right);
}

void ArbolClan::actualizarDatos(int id, const string &name, const string &last_name, char gender, int age, bool is_dead, bool was_chief, bool is_chief){
    Node *node = buscarNodo(root, id);
    if (node == nullptr){
        cout << "Miembro con ID " << id << " no encontrado" << endl;
        return;
    }

    node->name = name;
    node->last_name = last_name;
    node->gender = gender;
    node->age = age;
    node->is_dead = is_dead;
    node->was_chief = was_chief;
    node->is_chief = is_chief;

    cout << "Datos actualizados para el nodo con ID " << id << ":" << endl;
    cout << "Nombre: " << node->name << " " << node->last_name << endl;
    cout << "Género: " << node->gender << endl;
    cout << "Edad: " << node->age << endl;
    cout << "Muerto: " << node->is_dead << endl;
    cout << "Fue Lider: " << node->was_chief << endl;
    cout << "Es Lider: " << node->is_chief << endl;
}

Node *ArbolClan::buscarNodo(Node *actual, int id){
    if (actual == nullptr)
        return nullptr;
    if (actual->id == id)
        return actual;
    Node *found = buscarNodo(actual->left, id);
    if (found == nullptr)
        found = buscarNodo(actual->right, id);
    return found;
}

Node *ArbolClan::buscarTio(Node *actual){
    if (actual == nullptr || actual->id_father == 0)
        return nullptr;
    Node *father = buscarNodo(root, actual->id_father);
    if (father == nullptr || father->id_father == 0)
        return nullptr;
    Node *grandfather = buscarNodo(root, father->id_father);
    if (grandfather == nullptr)
        return nullptr;
    if (grandfather->left && grandfather->left != father)
        return grandfather->left;
    if (grandfather->right && grandfather->right != father)
        return grandfather->right;
    return nullptr;
}

Node *ArbolClan::buscarAncestroConDosHijos(Node *actual){
    if (actual == nullptr || actual->id_father == 0)
    return nullptr;
    Node *ancestor = buscarNodo(root, actual->id_father);
    while (ancestor != nullptr){
        if (ancestor->left && ancestor->right)
        return ancestor;
        ancestor = buscarNodo(root, ancestor->id_father);
    }
    return nullptr;
}

void ArbolClan::liberarContribuidores(Contribuidor* contrib) {
    while (contrib != nullptr) {
        Contribuidor* temp = contrib;
        contrib = contrib->next;
        delete temp;
    }
}

bool ArbolClan::compararContribuidores(const Contribuidor* a, const Contribuidor* b) {
    if (a->grado_contribucion != b->grado_contribucion) {
        return a->grado_contribucion > b->grado_contribucion;
    }
    return a->age > b->age;
}

void ArbolClan::cargarContribuidores(Node* node, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de contribuidores." << endl;
        return;
    }

    string line;
    getline(file, line); // Saltar cabecera

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        int node_id;

        getline(ss, field, ';');
        node_id = stoi(field);

        if (node_id == node->id) {
            string name, desc;
            int age, id, grade;

            getline(ss, name, ';');
            getline(ss, field, ';');
            age = stoi(field);
            getline(ss, field, ';');
            id = stoi(field);
            getline(ss, desc, ';');
            getline(ss, field, ';');
            grade = stoi(field);

            Contribuidor* nuevoContribuidor = new Contribuidor(name, age, id, desc, grade);

            // Insertar ordenado
            if (node->contribuidores == nullptr) {
                node->contribuidores = nuevoContribuidor;
            } else {
                Contribuidor* current = node->contribuidores;
                Contribuidor* prev = nullptr;

                while (current != nullptr && compararContribuidores(current, nuevoContribuidor)) {
                    prev = current;
                    current = current->next;
                }

                if (prev == nullptr) {
                    nuevoContribuidor->next = node->contribuidores;
                    node->contribuidores = nuevoContribuidor;
                } else {
                    prev->next = nuevoContribuidor;
                    nuevoContribuidor->next = current;
                }
            }
        }
    }
    file.close();
}

void ArbolClan::cargarTodosContribuidores(const string &filename) {
    if (root == nullptr) return;
    
    // Usaremos un enfoque iterativo para recorrer el árbol
    Node* current = root;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (prev == nullptr || prev->left == current || prev->right == current) {
            cargarContribuidores(current, filename);
            prev = current;
            current = current->left;
        } else if (current->left == prev) {
            prev = current;
            current = current->right;
        } else {
            prev = current;
            current = buscarNodo(root, current->id_father);
        }
    }
}

void ArbolClan::mostrarContribuidores(Node* node) {
    if (node == nullptr || node->contribuidores == nullptr) {
        cout << "No hay contribuidores para este lider." << endl;
        return;
    }

    cout << "Contribuidores de " << node->name << " " << node->last_name << ":" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Nombre\tEdad\tID\tContribucion\t\tGrado" << endl;
    cout << "--------------------------------------------------" << endl;

    Contribuidor* actual = node->contribuidores;
    while (actual != nullptr) {
        cout << actual->name << "\t" << actual->age << "\t" << actual->id << "\t"
             << actual->descripcion << "\t" << actual->grado_contribucion << endl;
        actual = actual->next;
    }
}

void ArbolClan::mostrarContribuidoresLider() {
    Node* lider = buscarLider(root);
    if (lider == nullptr) {
        cout << "No hay un lider actual." << endl;
        return;
    }
    mostrarContribuidores(lider);
}

int main(){
    ArbolClan clan;
    clan.leerArchivo("data_clan.csv");
    clan.cargarTodosContribuidores("data_contribuidores.csv");

    int opcion;
    do {
        cout << "\nMENU DE OPCIONES:\n";
        cout << "1. Imprimir linea de sucesion actual \n";
        cout << "2. Imprimir lider actual y sucesor\n";
        cout << "3. Asesinar al lider y asignar nuevo\n";
        cout << "4. Imprimir todos los miembros del clan\n";
        cout << "5. Actualizar datos de un Nodo\n";
        cout << "6. Visualizar lista de contribuidores al clan\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                clan.mostrarSucesionActual();
                break;
            case 2:
                clan.mostrarLiderActual_Sucesor();
                break;
            case 3:
                clan.asesinarLider();
                break;
            case 4:
                clan.mostrarMiembros();
                break;
            case 5: {
                int id;
                string name, last_name;
                char gender;
                int age;
                bool is_dead, was_chief, is_chief;

                cout << "Ingrese el ID del nodo a actualizar: ";
                cin >> id;
                cout << "Ingrese el nuevo nombre: ";
                cin >> name;
                cout << "Ingrese el nuevo apellido: ";
                cin >> last_name;
                cout << "Ingrese el nuevo genero (M/F): ";
                cin >> gender;
                cout << "Ingrese la nueva edad: ";
                cin >> age;
                cout << "Esta muerto? (1 para si, 0 para no): ";
                cin >> is_dead;
                cout << "Fue lider? (1 para si, 0 para no): ";
                cin >> was_chief;
                cout << "Es lider? (1 para si, 0 para no): ";
                cin >> is_chief;

                clan.actualizarDatos(id, name, last_name, gender, age, is_dead, was_chief, is_chief);
                break;
            }
            case 6:
                clan.mostrarContribuidoresLider();
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