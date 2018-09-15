#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


int main(){
  stringstream reloaded;
  string testing;
  string flag;
  //Mensaje de prueba para enviar al servidor
  char msg[10000] = "Mensaje de prueba por parte del cliente";
  reloaded.str(msg);
  testing  = reloaded.str();
  //Esta variable almacenara al descriptor de archivo(File Descriptor) del  el cliente
  int client_socket;
  //Estas son las estructuras que corresponden al servidor y al cliente, las cuales hay que configurar
  struct sockaddr_in server_info,client_info;
  //Estas variables guardan la cantidad de memoria necesaria que se debe reservar para el cliente y el servidor.
  int server_space = sizeof(server_info);
  int client_space = sizeof(client_info);
  //Esta variable almacenar el estado de la conexion
  int connection_status;
  //En este arreglo se guarda el mensaje del servidor
  char server_response[10000];
  //Creamos el socket del cliente
  client_socket = socket(AF_INET,SOCK_STREAM,0);
  //Pedimos memoria al sistema para las estructuras del servidor y cliente
  memset(&server_info,0,server_space);
  memset(&client_info,0,client_space);
  //Configuramos las estructura del servidor 
  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = inet_addr("192.168.0.13");
  server_info.sin_port = htons(8080);
  
  connection_status =  connect(client_socket,(struct sockaddr *) &server_info,sizeof(server_info));

  if(connection_status == -1){
    printf("El cliente no se pudo conectar");

  }
  //Mensajes de prueba para comprobacion de los comandos
  char correcto[1000] = "IDENTIFY MARKOS";
  char incorrecto[1000] = "IDENTIFYMUTSKA";

  while(1){
  //Esta funcion recibe el mensaje del servidor y lo guarda en la variable server_response
  recv(client_socket,&server_response,sizeof(server_response),0);
  reloaded.str(server_response);
  testing  = reloaded.str();
  
  //Imprime el mensaje del servidor con la conversion de char[] al tipo string
  cout<<"El servidor envia el siguiente mensaje: "<<testing<<endl;
  
  //Mandando mensaje personalizado en tiempo real;
  send(client_socket,correcto,sizeof(correcto),0);
  recv(client_socket,&server_response,sizeof(server_response),0);
  reloaded.str(server_response);
  testing  = reloaded.str();
  cout<<"El servidor envia el siguiente mensaje: "<<testing<<endl;  
  cout<<"Quieres continuar el ciclo?(y/n): "<<endl;
  cin>>flag;
  if(flag == "n")
    break;
  }

  //Cerramos el socket del cliente
  close(client_socket);
 

  return 0;

}
