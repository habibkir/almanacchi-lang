/* Utilizzo:
 * 1. cp almanacchi.c almanacchi.h (e togli il main)
 * 2. */
#include"almanacchi.h"
/* 3. vedi sotto */

#include<stdbool.h>

void kys(char* messaggio) {
    printf("errore:\n%s\ne quindi devo ammazzarmi\n",messaggio);
    exit(1);
}

int testa_comincia_con() {
    char* test = "almanacchi!";
    if(!comincia_con(test,test,0))
	kys("test non comincia con se stesso");

    if(comincia_con(test,test,1))
	kys("test comincia con se stesso male");

    if(comincia_con(test,test,1))
	kys("test comincia con se stesso male");

    if(!comincia_con(test,"a",0))
	kys("almanacchi non iniza con a");

    if(!comincia_con(test,"al",0))
	kys("almanacchi non inizia con al");

    if(!comincia_con(test,"lma",1))
	kys("a[...]acchi");

    if(comincia_con("shit","shitfuckery",0))
	kys("spiega a quest'affare cos'è un prefisso");

    if(!comincia_con("shit","t",3))
	kys("siamo alla fine, ma dovrebbe funzionare lo stesso");

    if(comincia_con("shitfuckery","fuckeryfuckery",4))
	kys("auguri stronzo");

    return 0;
}
	
int main() {
    testa_comincia_con();
    return 0;
}

