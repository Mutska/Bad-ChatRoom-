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
//Esta funcion me ayuda a sacar los saltos de linea de un arreglo de caracteres
void trimm (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { 
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}
//Esta es la funcion util que ocupare para mandar un msg a todos los clientes almacenados en el diccionario (PUBLIC_MESSAGE)
void send_to_all(std::map<std::string,Usuario*> list,Usuario* cliente,char buffer[]) { 
  for (std::map<std::string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if((cliente->data == it->second->data) || (it->second->name == "Server")){
      continue;
    }
    send(it->second->data,buffer,500,0);    
  }
}
  
  
