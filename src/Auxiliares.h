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
  
  
//Funcion que se encargara de detectar el comando introducido
int detect(std::string command){
  int flag;
  while(1){
    if(command.find("IDENTIFY ") != std::string::npos){
      flag = 1;
      break;
    }
    if(command.find("STATUS") != std::string::npos){
      flag = 2;
      break;
    }
    if(command.find("USERS") != std::string::npos){
      flag = 3;
      break;
    }
    if(command.find("MESSAGE") != std::string::npos){
      flag = 4;
      break;
    }
    if(command.find("PUBLICMESSAGE ") != std::string::npos){
      flag = 5;
      break;
    }
    if(command.find("CREATEROOM ") != std::string::npos){
      flag = 6;
      break;
    }
    if(command.find("INVITE") != std::string::npos){
      flag = 7;
      break;
    }
    if(command.find("JOINROOM") != std::string::npos){
      flag = 8;
      break;
    }
    if(command.find("ROOMESSAGE") != std::string::npos){
      flag = 9;
      break;
    }
    if(command.find("DISCONNECT") != std::string::npos){
      flag = 10;
      break;
    }
    flag = 11;
  }
  return flag;
}
