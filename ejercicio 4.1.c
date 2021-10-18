#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>



int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando el socket\n");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	
	
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error en bind\n");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen\n");
	
	int i;
	for (;;){
		printf ("Escuchando\n");
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("Conexi?n establecida\n");
		
		
		int end =0;

		while (end ==0)
		{
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			peticion[ret]='\0';
			printf ("Peticion: %s\n",peticion);
			

			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			char nombre[20];
			
			if (codigo !=0)
			{
				p = strtok( NULL, "/");
				
				strcpy (nombre, p);
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo ==0)
				end=1;
			else if (codigo ==1) 
				sprintf (respuesta,"%d",strlen (nombre));
			else if (codigo ==2)
				if((nombre[0]=='I') || (nombre[0]=='J'))
					strcpy (respuesta,"SI");
				else
					strcpy (respuesta,"NO");
			else if (codigo ==4){
				char nombre[30];
				char nombre_inv[30] = {'\0'};
				int i;
				int len = strlen(nombre);
				int End;
			
				for (i = len - 1; i >= 0; i--)
					{
						nombre_inv[len - i - 1] = nombre[i];
					}
				
				for (i = 0; i < len; i++)
					{
						if (nombre_inv[i] == nombre[i])
							End = 1;
						else
							End = 0;
					}
				if (End == 1)
					sprintf(respuesta,"%s es palindrome \n", nombre);
				else
					sprintf(respuesta,"%s NO es palindrome \n", nombre);
			}
			else
				{
					p = strtok( NULL, "/");
					float altura =  atof (p);
					if (altura > 1.75)
						sprintf (respuesta, "%s: eres alto",nombre);
					else
						sprintf (respuesta, "%s: eresbajo",nombre);
				}
				
				if (codigo !=0)
				{
					
					printf ("Respuesta: %s\n", respuesta);
					write (sock_conn,respuesta, strlen(respuesta));
				}
		}
		close(sock_conn); 
	}
}