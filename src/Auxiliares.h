#include "Usuario.h"
#include <iostream>
#include <string>
#include <map>

//newNode es la funcion que regresa un objeto de la clase "Usuarios", con todos los datos necesarios por cliente.
Usuario* newNode(int socket,char* ip ,std::string name){
  Usuario* cliente = new Usuario;//Reservando memoriax
  cliente->data = socket;
  strncpy(cliente->ip, ip, 16);
  cliente->name = name;
  return cliente;
}
