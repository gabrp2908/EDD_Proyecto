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
};