#if ! defined ( REQUETE_H_ )
#define REQUETE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

//Maximum d'un mot dans les requetes du client
#define MAX_TOCKEN 256

//Protocoles
#define TCP_PULL 0
#define TCP_PUSH 1
#define UDP_PULL 2
#define UDP_PUSH 3

//Type de requete
#define BAD_REQUEST (-2)
#define NON_DEFINI (-1)
#define GET 1
#define START 2
#define PAUSE 3
#define END 4
#define ALIVE 5

//Erreur de converion char->int
#define PARSE_ERROR -2

struct requete {
	int type;
    
    int isOver;
    
    int imgId;
    int listenPort;
    int fragmentSize;
    
    int inWord;
    int space;
    int crlfCounter;
    
    char* mot;
    int motPosition; //position dans le mot lu
    
    int reqPosition; //position du mot dans la requete
};

int parseInt(char* entier);

void initReq(struct requete* req);

void traiteRequete(struct requete* req, int clientSocket);

void traiteChaine(char* chaine, struct requete* req, int clientSocket);

#endif // REQUETE_H_