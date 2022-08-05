#include<stdio.h>
/* ci sono molti programmi in brainfuck disponibili su wiki e internet
 * et al. visto che non so scrivere cose del genere da me ho fatto sto
 * cosino per tradurre quei programmi in almanacchi e avere qualcosa con
 * cui testare il funzionamento di questo mostro che ho creato */

#define STRING_WAS_EMPTY (1)

char* bfchar2al(int c) { /* tutto è un int che tu lo voglia o meno */
    switch(c) {
    case '>':
	return "almanacchi";
    case '<':
	return "almanacchi!";
    case '+':
	return "nuovi";
    case '-':
	return "nuovi!";
    case '[':
	return "lunari";
    case ']':
	return "lunari!";
    case '.':
	return "gia";
    case ',':
	return "signore";
    default:
	return "";
    }
}

int put_str(char* str, FILE* f) {
    if(str[0]=='\0') return STRING_WAS_EMPTY;
    for(int i = 0; str[i]; ++i) {
	putc(str[i],f);
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc <= 1) {
	puts("ti sei scordato di darmi un file da leggere");
	return 1;
    }

    FILE* in = fopen(argv[1],"rb");
    FILE* out = fopen("brainfuck_convertito.al","w");

    int c = getc(in);
    while(c!=EOF) {
	int err = put_str(bfchar2al(c),out);
	if(err!=STRING_WAS_EMPTY)
	    putc(' ',out);
	c = getc(in);
    }

    fclose(in);
    fclose(out);
}
