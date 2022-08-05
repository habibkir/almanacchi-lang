#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include<stdbool.h> /* non propio c-ese come cosa */

/* visto che almanacchi-lang è solo un brainfuck allungato
 * prima ho fatto un interpreter per il brainfuck, e lo sto allungando qui
 * buona parte del codice sarà quindi copiata verbatim e/o copiata con poche modifiche
 * da brainfuck.c */

#define DIMENSIONE_MEMORIA (30000)
/* da "standard" di brainfuck, la "macchina virtuale" ha 30.000 celle */
long dimensione_codice = -1;

char* file_as_string(char* path) {
    FILE* fp = fopen(path, "rb");
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    dimensione_codice = size; /* serve per poi */
    rewind(fp);

    char* buffer = (char*)malloc((size+1)*sizeof(char));
    long bytes_read = fread(buffer,1,size,fp);
    buffer[bytes_read] = 0;
    if(bytes_read!=size) {
	puts("vaffacnulo, muori");
	return NULL;
    }
    return buffer;
}

bool comincia_con(char* codice, char*prefisso, int pc) {
    int i = 0;
    int j = pc;
    for(; prefisso[i]&&codice[j]; ++i,++j) {
	if(prefisso[i]!=codice[j])
	    return false;
    }
    if(codice[j]=='\0'&&prefisso[i]!='\0')
	return false;

    return true;
}

bool pc_valido(int pc, char* codice) {
    return (comincia_con(codice,"almanacchi",pc) ||
	    comincia_con(codice,"nuovi",pc) ||
	    comincia_con(codice,"lunari",pc) ||
	    comincia_con(codice,"signore",pc) ||
	    comincia_con(codice,"gia",pc));
}

void avanti_pc(char* codice, int* pc_ptr) {
    /* abbiamo trovato un '[' (lunari) e vogliamo andare
     * al ']' (lunari!) corrispondente */
    int pc = *pc_ptr;
    int apertaggine = 1;
    while(apertaggine) {
	pc = (pc+1)%dimensione_codice;
	if(comincia_con(codice,"lunari!",pc))
	    apertaggine--;
	else if (comincia_con(codice,"lunari",pc))
	    apertaggine++;
    }

    /* vai avanti fino alla prossima parola */
    pc = (pc+1)%dimensione_codice;
    while(!pc_valido(pc,codice))
	pc = (pc+1)%dimensione_codice;

    *pc_ptr = pc;
}

void indietro_pc(char* codice, int* pc_ptr) {
    /* abbiamo trovato un ']' (lunari!) e vogliamo anare
     * al '[' (lunari) corrispondente */
    int pc = *pc_ptr;
    int chiusaggine = 1;
    while(chiusaggine) {
	pc = (pc-1+dimensione_codice)%dimensione_codice;
	if(comincia_con(codice,"lunari!",pc))
	    chiusaggine++;
	else if (comincia_con(codice,"lunari",pc))
	    chiusaggine--;
    }

    /* vai avanti fino alla prossima parola */
    pc = (pc+1)%dimensione_codice;
    while(!pc_valido(pc,codice))
	pc = (pc+1)%dimensione_codice;

    *pc_ptr = pc;
}

bool fine(int pc, char* codice) {
    /* se c'è una parola valida davanti allora nessun problema
     * altrimenti è la fine */
    //puts("inizio della fine");
    pc++;
    while((!pc_valido(pc,codice)) && codice[pc])
	++pc;
    if(codice[pc] == '\0') return true;
    //puts("fine della fine");
    return false;
}

void print_parola(int pc, char* codice) {
    putchar(codice[pc]);
    pc = (pc+1)%dimensione_codice;
    while(!pc_valido(pc,codice)) {
	if(!isspace((int)(codice[pc])))putchar(codice[pc]);
	pc = (pc+1)%dimensione_codice;
    }
}

void run_as_almanacchi(char* codice) {
    char memoria[DIMENSIONE_MEMORIA];
    memset(memoria,0,DIMENSIONE_MEMORIA*sizeof(char));
    /* se qualche psicopatico usa char di 2 byte
     * almeno il codice non darà problemi */

    int mem_ptr = 0; /* pointer in memoria */
    int pc = 0; /* program counter */

    while(codice[pc]) {
	int dimensione_parola = 0;
	/* yanderedev mi fa na pippa */

	//printf("pc = %d, mem_ptr = %d e ",pc,mem_ptr);

	if(comincia_con(codice,"almanacchi!",pc)) {
	    //puts("comincio com almanacchi!");
	    mem_ptr = (mem_ptr-1+DIMENSIONE_MEMORIA)%DIMENSIONE_MEMORIA;
	    dimensione_parola = 11; //strlen("almanacchi! ");
	}
	else if(comincia_con(codice,"almanacchi",pc)) {
	    //puts("comincio com almanacchi");
	    mem_ptr = (mem_ptr+1)%DIMENSIONE_MEMORIA;
	    dimensione_parola = 10; //strlen("almanacchi ");
	}
	else if(comincia_con(codice,"nuovi!",pc)) {
	    //puts("comincio com nuovi!");
	    memoria[mem_ptr]--;
	    dimensione_parola = 6; //strlen("nuovi! ");
	}
	else if(comincia_con(codice,"nuovi",pc)) {
	    //puts("comincio com nuovi");
	    memoria[mem_ptr]++;
	    dimensione_parola = 5; //strlen("nuovi ");
	}
	else if(comincia_con(codice,"signore",pc)) {
	    //puts("comincio com signore");
	    char c = ' ';
	    scanf("%c",&c);
	    memoria[mem_ptr] = c;
	    dimensione_parola = 7; //strlen("signore ");
	}
	else if(comincia_con(codice,"gia",pc)) {
	    //puts("comincio com gia");
	    putchar(memoria[mem_ptr]);
	    dimensione_parola = 3; //strlen("gia ");
	}

	/* aggiorna program counter */
	if(dimensione_parola) {
	    /* la parola corrente non è un equivalente di '['']' */
	    /* visto che coi loop puoi andare dall'altra parte del file e
	     * che la fine dell'esecuzione avviene con la fine del file
	     * questo credo che sia l'unico caso in cui puoi finire l'esecuzione */
	    if(fine(pc,codice)) {/* se questa era l'ultima parola */
		break;
	    }
	    
	    pc= pc+dimensione_parola;
	    /* non c'è bisogno di wrap around per avanzate normali,
	     * solo mem_ptr e loop */
	    pc++;
	    while(!pc_valido(pc,codice))
		pc++;
	}
	else if(comincia_con(codice,"lunari!",pc)) {
	    //puts("comincio com lunari!");
	    if(memoria[mem_ptr] != 0)
		indietro_pc(codice,&pc);
	    else {
		// vai alla parola dopo
		pc = (pc+7)%dimensione_codice; // 7==strlen("lunari!");
		while(!pc_valido(pc,codice)) {
		    pc = (pc+1)%dimensione_codice;
		}
	    }
	}
	else if(comincia_con(codice,"lunari",pc)) {
	    //puts("comincio com lunari");
	    if(memoria[mem_ptr] == 0)
		avanti_pc(codice,&pc);
	    else {
		// vai alla parola dopo
		pc = (pc+6)%dimensione_codice; // 6==strlen("lunari");
		while(!pc_valido(pc,codice)) {
		    pc = (pc+1)%dimensione_codice;
		}
	    }
	}

	//printf("post pc che va : \"");print_parola(pc,codice);
	//printf("\", pc = %d, mem_ptr = %d\n",pc,mem_ptr);
    }
}

int main(int argc, char** argv) {
    if(argc <= 1) {
	puts("Guarda, dovresti darmi un file");
	return 1;
    }

    char* codice = file_as_string(argv[1]);
    printf("output di almanacchi:\n");
    run_as_almanacchi(codice);

    /* aspetta un input così la finestra non si chiude subito */
    getchar();

    return 0;
}
