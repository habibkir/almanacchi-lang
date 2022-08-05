#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* TODO poi metti la roba per il teardown, se vuoi */

/* per più informazioni sul brainfuck
 * https://esolangs.org/wiki/Brainfuck */

#define DIMENSIONE_MEMORIA (30000)
/* da "standard" di brainfuck, la "macchina virtuale" ha 30.000 celle */
long dimensione_codice = -1;

char* file_as_string(char* path) {
    /* se hai problemi col codice lamentati con le risposte di
     * https://stackoverflow.com/questions/63101309/is-there-a-way-to-get-size-of-a-file-on-windows-using-c */
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

/* violazoine di DRY, non so che farci */
void avanti_pc(int* pc_ptr, char* codice) {
    /* abbiamo trovato un '[' e vogliamo avanti fino al ']' corrispondente */
    int apertaggine = 1;
    int pc = *pc_ptr;

    while(apertaggine) { /* apertaggine==0 -> abbiamo chiuso del tutto */
	pc = (pc+1)%dimensione_codice;
	if(codice[pc] == ']')
	    apertaggine--;
	else if (codice[pc] == '[')
	    apertaggine++;
    }
    *pc_ptr = pc;
}

void indietro_pc(int* pc_ptr, char*codice) {
    /* abbiamo trovato un ']' e vogliamo andare indietro fino al '[' corrispondente */
    int chiusaggine = 1;
    int pc = *pc_ptr;

    while(chiusaggine) { /* chiusaggine==0 -> apertura e chiusura bilanciati */
	pc = (pc-1+dimensione_codice)%dimensione_codice;
	if(codice[pc] == ']')
	    chiusaggine++;
	else if (codice[pc] == '[')
	    chiusaggine--;
    }
    *pc_ptr = pc;
}

void print_tattico(char* codice, int pc) {
    /* su windows sappiate che questa cosa fotterà malamente per inizio file e (forse) andare a capo */
    for(int i = 0; i<pc; ++i) {
	printf("%c",codice[i]);
    }

    printf(" [%c] ",codice[pc]);

    for(int i = pc+1; codice[i]; ++i) {
	printf("%c",codice[i]);
    }
    puts("");
}

void run_as_brainfuck(char* codice) {
    char memoria[DIMENSIONE_MEMORIA];
    memset(memoria,0,DIMENSIONE_MEMORIA*sizeof(char));
    /* se qualche psicopatico usa char di 2 byte
     * almeno il codice non darà problemi */

    int mem_ptr = 0; /* pointer in memoria */
    int pc = 0; /* program counter */

    while(codice[pc]) { /* quando si ferma a NULL possiamo supporre che abbia fatto */
	char c = codice[pc];
	/* aggiorna memoria/mem_ptr e fai IO */
	switch(c) {
	case '>':
	    mem_ptr = (mem_ptr+1)%DIMENSIONE_MEMORIA;
	    break;
	case '<':
	    mem_ptr = (mem_ptr-1+DIMENSIONE_MEMORIA)%DIMENSIONE_MEMORIA;
	    /* aritmetica modulare coi negativi in C? nah */
	    break;
	case '+':
	    memoria[mem_ptr]++;
	    break;
	case '-':
	    memoria[mem_ptr]--;
	    break;
	case '.':
	    putchar(memoria[mem_ptr]);
	    break;
	case ',':
	    char c = ' ';
	    scanf("%c",&c);
	    memoria[mem_ptr] = c;
	    break;
	default:
	    break;
	}

	/* aggiorna program counter */
	switch(c) {
	case '[':
	    if(memoria[mem_ptr] == 0)
		avanti_pc(&pc,codice);
	    else pc++;
	    break;
	case ']':
	    if(memoria[mem_ptr] != 0)
		indietro_pc(&pc,codice);
	    else pc++;
	    break;
	default:
	    pc++;
	    break;
	}

	#ifdef DEBUG_DA_STRONZO
	printf("pc = %d e mem_ptr = %d\n",pc,mem_ptr);
	for(int i = 0; i<30; ++i) {
	    printf("%d ",memoria[i]);
	}
	puts("");
	for(int i = 0; i<30; ++i) {
	    putchar(memoria[i]);
	}
	print_tattico(codice,pc);
	#endif
    }
}

int main(int argc, char** argv) {
    /* se non dai argomenti è un problemino */
    if(argc <= 1) {
	puts("Guarda, dovresti darmi un file");
	return 1;
    }
	
    /* argv[1] contiene la path al file da interpretare */
    char* codice = file_as_string(argv[1]);
    printf("output di brainfuck:\n");
    run_as_brainfuck(codice);

    /* aspetta un input così la finestra non si chiude subito */
    getchar();

    return 0;
}
