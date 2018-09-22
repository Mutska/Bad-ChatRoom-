Chat

Chat con un protocolo sencillo implementado.

El chat fue escrito en C++ y python, dentro de la carpeta '/src' se encuentra el codigo del servidor
con sus archivos auxiliares, el cliente esta escrito en c++ pero sin interfaz gráfica, para esto se
agrego dentro de '/src' la carpeta '/GUI client' que es un cliente simple escrito en python pero con
interfaz gráfica integrada.
Mutska(Marco Antonio Velasco Flores, 418004087)

Dependencias

El proyecto fue generado en Ubuntu 18.04 con las siguientes dependencias

1.-Compilador g++ : g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0, necesario para compilar los archivos escritos
en c++ se puede instalar desde los repositorios oficiales de Ubuntu con el siguiente
comando: 'sudo apt-get install g++'

2.-tkinter(modulo de python): Necesario para la interfaz grafica del cliente escrito en python,
de la  misma manera se puede instalar desde los repositorios oficiales de Ubuntu con el
siguiente comando: 'sudo apt-get install python-tk'

3.-Intérprete python: Para correr el cliente es necesario 'python3', por defecto ya viene instalado
en Ubuntu.


Instrucciones

Para compilar el  proyecto dentro de la carpeta 'src' correr el siguiente comando para el servidor:
'g++ -fpermissive -pthread -O3 -Wall  servidor.cpp -o servidor' esto generara el ejecutable 'servidor'

Correr el siguiente comando para el cliente sin interfaz gráfica:
'g++ -fpermissive -pthread -O3 -Wall  cliente.cpp -o cliente' esto generara el ejecutable 'cliente'

Para ejecutar el cliente en python con interfaz gráfica correr el siguiente comando:
python3 client.py


Cuestiones tecnicas
El servidor y los clientes estan configurados por defecto con la siguiente ip: '127.0.0.1' sobre el puerto '1234'



Posibles errores

Por alguna razón si cierras el servidor antes que todos los clientes se desconecten y vuelves a correr
el servidor, los clientes no se pueden conectar.
Para arreglar esto, basta eliminar el ejecutable del servidor, recompilarlo y volverlo a correr.
Con esto el cliente en python no tendra problemas, por otro lado si el cliente escrito en c++ no
se conecta al servidor, de igual manera elimina su ejecutable y vuelve a recompilarlo.



