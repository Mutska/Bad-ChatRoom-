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
//Prototipos de funciones
void msg_sender();
void msg_receiver();
void trimm(char *,int);
void prompt();

//Esta variable almacenara al descriptor de archivo(File Descriptor) del  el cliente
int client_socket;

int main(){
  stringstream stream;
  string cadena;
  string flag;
  //Mensaje de prueba para enviar al servidor
  char msg[10000] = "Mensaje de prueba por parte del cliente";
  stream.str(msg);
  cadena  = stream.str();
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
  server_info.sin_addr.s_addr = inet_addr("192.168.15.8");
  server_info.sin_port = htons(8080);
  
  connection_status =  connect(client_socket,(struct sockaddr *) &server_info,sizeof(server_info));

  if(connection_status == -1){
    printf("El cliente no se pudo conectar");

  }
  //Mensajes de prueba para comprobacion de los comandos
  char name[1000] = {};
  char incorrecto[1000] = "IDENTIFYMUTSKA";

  while(1){
  //Esta funcion recibe el mensaje del servidor y lo guarda en la variable server_response
  recv(client_socket,&server_response,sizeof(server_response),0);
  stream.str(server_response);
  cadena  = stream.str();
  
  //Imprime el mensaje del servidor con la conversion de char[] al tipo string
  cout<<"El servidor envia el siguiente mensaje: "<<cadena<<endl;
  fgets(name, 500, stdin);
  trimm(name, 500);

  
  //Mandando mensaje personalizado en tiempo real;
  send(client_socket,name,sizeof(name),0);
  recv(client_socket,&server_response,sizeof(server_response),0);
  stream.str(server_response);
  cadena  = stream.str();
  cout<<"El servidor envia el siguiente mensaje: "<<cadena<<endl;  
  cout<<"Quieres continuar el ciclo?(y/n): "<<endl;
  cin>>flag;
  if(flag == "n")
    break;
  }

  //Cerramos el socket del cliente
  close(client_socket);
 

  return 0;

}

//Esta funcion se encarga de recibir msg del socket del cliente
void msg_receiver() {
  char message[1000] = {};
  int receive;
  while (1) {
    receive = recv(client_socket, message, 1000, 0);
    if (receive > 0) {
      printf("\r%s\n", message);
      prompt();
    } else if (receive == 0) {
      break;
    } else { 
      //Nada que hacer aqui
    }
  }
}

//Esta funcion se encargar de mandar msg por parte del cliente
void msg_sender() {
  char message[500] = {};
  while (1) {
    prompt();
    while (fgets(message, 500, stdin) != NULL) {
      trimm(message, 500);
      if (strlen(message) == 0) {
	prompt();
	cout<<"No escribiste nada";
      } else {
	break;
      }
    }
    send(client_socket, message, 500, 0);
  }
}

  
  //Esta funcion me ayuda a sacar los saltos de linea de un arreglo de caracteres de c
  void trimm (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { 
      if (arr[i] == '\n') {
	arr[i] = '\0';
	break;
      }
    }
  }

//Esta funcion me permite cambiar el prompt de la terminal 
void prompt(){
  printf("\r%s", "$");
  fflush(stdout);
}
