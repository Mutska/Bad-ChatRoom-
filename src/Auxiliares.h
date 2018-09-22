#include "Usuario.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
using namespace std;
//newNode es la funcion que regresa un objeto de la clase "Usuarios", con todos los datos necesarios por cliente.
Usuario* newNode(int socket,char* ip ,string name){
  Usuario* cliente = new Usuario;//Reservando memoriax
  cliente->data = socket;
  strncpy(cliente->ip, ip, 16);
  cliente->name = name;
  cliente->status = "ACTIVE";
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
void send_to_all(map<string,Usuario*> list,Usuario* cliente,string buffer){
  std::string ultimate = buffer;
  size_t tam = ultimate.size();
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if(it->second->name == "Server"){
      continue;
    }
    send(it->second->data,ultimate.c_str(),tam,0);    
  }
  ultimate.clear();
}


//Esta es la funcion util que ocupare para mandar un msg a un solo cliente
void send_to_one(map<string,Usuario*> list,Usuario* cliente,string buffer){
  std::string ultimate = buffer;
  size_t tam = ultimate.size();
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if((cliente->data == it->second->data) &&  (it->second->name != "Server")){
      send(it->second->data,ultimate.c_str(),tam,0);
      break;
    }
  }
  ultimate.clear();
}

//Esta es la funcion util que ocupare para mandar un msg privado 
void send_private_msg(map<string,Usuario*> list,Usuario* cliente,string buffer,string name){
  std::string ultimate = buffer;
  size_t tam = ultimate.size();
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if((cliente->data != it->second->data) &&  (it->second->name != "Server") && (it->second->name == name)){
      send(it->second->data,ultimate.c_str(),tam,0);
      break;
    }
  }
  ultimate.clear();
}

//Esta es la funcion util que ocupare para mandar una invitacion
void send_invitation(map<string,Usuario*> list,Usuario* cliente,vector<string> lista,string room){
  std::string ultimate = cliente->name;
  ultimate += " te ha invitado a unirte al chatroom: ";
  ultimate += room;
  ultimate += " \n";
  size_t tam = ultimate.size();
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    for(int i = 2 ; i < lista.size();i++){
      if(it->second->name == lista[i] && (it->second->name != cliente->name)){
	send(it->second->data,ultimate.c_str(),tam,0);
	it->second->chatrooms[room] = 0;
      }}
  }
}
//Esta es la funcion util que ocupare para mandar un msg a una sala especifica
void send_to_room(map<string,Usuario*> list,Usuario* cliente,string  message,string room){
  std::string room_message = "(Sala: ";
  room_message += room;
  room_message += ") ";
  room_message += cliente->name;
  room_message += ": ";
  room_message += message;
  room_message += " \n";
  size_t tam = room_message.size();
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if((it->second->chatrooms[room] == 2 || it->second->chatrooms[room] == 1) && it->second->name != cliente->name){
      send(it->second->data,room_message.c_str(),tam,0);
    }
  }
  room_message.clear();
}


int detect(string command){
  int flag;
  while(1){
    if(command.find("IDENTIFY ") != string::npos && command.size() > 9 ){
      flag = 1;
      break;
    }
    if(command.find("STATUS ") != string::npos && command.size() > 7 ){
      flag = 2;
      break;
    }
    if(command.find("USERS") != string::npos && command.size() < 8){
      flag = 3;
      break;
    }
    if(command.find("PUBLICMESSAGE ") != string::npos && command.size() > 14){
      flag = 4;
      break;
    }
    if(command.find("ROOMESSAGE ") != string::npos && command.size() > 11 ){
      flag = 9;
      break;
    }
    if(command.find("MESSAGE ") != string::npos && command.size() > 11 && command.find("PUBLICMESSAGE ") == string::npos &&
       command.find("ROOMESSAGE ") == string::npos){
      flag = 5;
      break;
    }
    if(command.find("CREATEROOM ") != string::npos && command.size() > 11 ){
      flag = 6;
      break;
    }
    if(command.find("INVITE ") != string::npos && command.size() > 8){
      flag = 7;
      break;
    }
    if(command.find("JOINROOM ") != string::npos && command.size() >  9){
      flag = 8;
      break;
    }
    if(command.find("DISCONNECT") != string::npos){
      flag = 10;
      break;
    }
    if(command.find("ZZZZZZZZZZ") == string::npos){
      flag = command.find("ZZZZZZZZZZ");
      break;
    }
      }
  return flag;
}

//Esta funcion me permite parsear la entrada del usuario para tomar los argumentos necesarios;
string send_arguments(int i, string comando){
  string name = "Ya tienes asignado un nombre";
  string message;
  string roomname;
  int found;
  int len;
  istringstream iss(comando);
  vector<string> split((istream_iterator<string>(iss)), istream_iterator<string>());
  switch(i){
  case 1:
    return name;
    break;
  case 2:
    if(split[1] == "ACTIVE" || split[1] == "AWAY" || split[1] == "BUSY"){
      return split[1];
    }else{
      return "ERROR";
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
    roomname = split[1];
    return roomname;
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
//Funcion que regresa la lista de los usuarios con su status
string show_users(map<string,Usuario*> list){
  string lista = "La lista de usuarios es:\n";
  for (map<string,Usuario*>::iterator it = list.begin(); it!=list.end(); ++it){
    if(it->second->name == "Server")
      continue;
    lista += it->second->name;
    lista += "(";
    lista += it->second->status;
    lista += ")";
    lista += "\n";    
  }
  return lista;
}
