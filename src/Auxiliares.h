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
      arr[i] = ' ';
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

int detect(string command){
  int flag;
  while(1){
    if(command.find("IDENTIFY ") != std::string::npos && command.size() > 9 ){
      flag = 1;
      break;
    }
    if(command.find("STATUS ") != std::string::npos && command.size() > 7 ){
      flag = 2;
      break;
    }
    if(command.find("USERS") != std::string::npos && command.size() < 6){
      flag = 3;
      break;
    }
    if(command.find("PUBLICMESSAGE ") != std::string::npos && command.size() > 14){
      flag = 4;
      break;
    }
    if(command.find("ROOMESSAGE ") != std::string::npos && command.size() > 11 ){
      flag = 9;
      break;
    }
    if(command.find("MESSAGE ") != std::string::npos && command.size() > 11 && command.find("PUBLICMESSAGE ") == std::string::npos &&
       command.find("ROOMESSAGE ") == std::string::npos){
      flag = 5;
      break;
    }
    if(command.find("CREATEROOM ") != string::npos && command.size() > 12 ){
      flag = 6;
      break;
    }
    if(command.find("INVITE ") != std::string::npos && command.size() > 8){
      flag = 7;
      break;
    }
    if(command.find("JOINROOM ") != std::string::npos && command.size() >  10){
      flag = 8;
      break;
    }
    if(command.find("DISCONNECT") != std::string::npos){
      flag = 10;
      break;
    }
    if(command.find("ZZZZZZZZZZ") == std::string::npos){
      flag = command.find("ZZZZZZZZZZ");
      break;
    }
      }
  return flag;
}

//Esta funcion me permite parsear la entrada del usuario para tomar los argumentos necesarios;
string send_arguments(int i, string comando){
  string name = "Ya tienes asignado un nombre";
  string error = "El status que introduciste es incorrecto";
  string message;
  int found;
  int len;
  istringstream iss(comando);
  vector<string> split((istream_iterator<string>(iss)), istream_iterator<string>());
  switch(i){
  case 1:
    return name;
    break;
  case 2:
    if(split[1] != "ACTIVE" || split[1] != "AWAY" || split[1] != "BUSY"){
      return error;
    }else{
      return split[1];
    }
    break;
  case 4:
    message = comando.substr(14);
    return message;
    break;
  case 5:
    found = comando.find(split[1]);
    len = split[1].size();
    message = comando.substr(found+len+1);
    return message;
    break;
  case 6:
    return split[1];
    break;
  case 7:
    break;
  case 8:
    return split[1];
    break;
  case 9:
    message = comando.substr(11);
    return message;
    break;
  case 10:
    return "DISCONNECT";
    break;
  default:
    return "NADA QUE DECIR";
  }
  
  return "NO CREI LLEGAR HASTA AQUI :v";
}

//Funcion especial para el caso cinco de la funcion detect
string send_five_case(string comando){
  istringstream iss(comando);
  vector<string> split((istream_iterator<string>(iss)), istream_iterator<string>());
  return split[1];  
}
//Funcion especial para el caso siete de la funcion detect
vector<string> send_seven_case(string comando){
  istringstream iss(comando);
  vector<string> split((istream_iterator<string>(iss)), istream_iterator<string>());
  return split;  
}

