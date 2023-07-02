#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MIN 100
#define SEED    0x12345678
#define SEED2   0x811c9dc5

typedef struct _municipios{
    char cod_ibge[9];
    char nome[MIN];
    double coord[2];
    int capital;
    int codigo_uf;
    char siafi_id[5];
    int ddd;
    char fuso_horario[MIN];
    char regiao[15];
    char uf[3];
}muni;

char * get_key(void * reg){
    return (*(muni *)reg).cod_ibge;
}


void *aloca_muni(char *cod_ibge, char *nome, double coord0, double coord1, int capital, int codigo_uf, char *siafi_id, int ddd, char *fuso_horario){
    muni *municipios;
    municipios = malloc(sizeof(muni));

    strcpy(municipios->cod_ibge, cod_ibge);
    strcpy(municipios->nome ,nome);
    municipios->coord[0] = coord0;
    municipios->coord[1] = coord1; 
    municipios->capital = capital;
    municipios->codigo_uf = codigo_uf;
    strcpy(municipios->siafi_id, siafi_id);
    municipios->ddd = ddd;
    strcpy(municipios->fuso_horario, fuso_horario);

    if(municipios->codigo_uf == 11){
        strcpy(municipios->uf,"RO");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 12){
        strcpy(municipios->uf,"AC");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 13){
        strcpy(municipios->uf,"AM");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 14){
        strcpy(municipios->uf,"RR");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 15){
        strcpy(municipios->uf,"PA");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 16){
        strcpy(municipios->uf,"AP");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 17){
        strcpy(municipios->uf,"TO");
        strcpy(municipios->regiao,"Norte");
    }
    else if(municipios->codigo_uf == 21){
        strcpy(municipios->uf,"MA");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 22){
        strcpy(municipios->uf,"PI");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 23){
        strcpy(municipios->uf,"CE");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 24){
        strcpy(municipios->uf,"RN");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 25){
        strcpy(municipios->uf,"PB");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 26){
        strcpy(municipios->uf,"PE");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 27){
        strcpy(municipios->uf,"AL");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 28){
        strcpy(municipios->uf,"SE");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 29){
        strcpy(municipios->uf,"BA");
        strcpy(municipios->regiao,"Nordeste");
    }
    else if(municipios->codigo_uf == 31){
        strcpy(municipios->uf,"MG");
        strcpy(municipios->regiao,"Sudeste");
    }
    else if(municipios->codigo_uf == 32){
        strcpy(municipios->uf,"ES");
        strcpy(municipios->regiao,"Sudeste");
    }
    else if(municipios->codigo_uf == 33){
        strcpy(municipios->uf,"RJ");
        strcpy(municipios->regiao,"Sudeste");
    }
    else if(municipios->codigo_uf == 35){
        strcpy(municipios->uf,"SP");
        strcpy(municipios->regiao,"Sudeste");
    }
    else if(municipios->codigo_uf == 41){
        strcpy(municipios->uf,"PR");
        strcpy(municipios->regiao,"Sul");
    }
    else if(municipios->codigo_uf == 42){
        strcpy(municipios->uf,"SC");
        strcpy(municipios->regiao,"Sul");
    }
    else if(municipios->codigo_uf == 43){
        strcpy(municipios->uf,"RS");
        strcpy(municipios->regiao,"Sul");
    }
    else if(municipios->codigo_uf == 50){
        strcpy(municipios->uf,"MS");
        strcpy(municipios->regiao,"Centro-Oeste");
    }
    else if(municipios->codigo_uf == 51){
        strcpy(municipios->uf,"MT");
        strcpy(municipios->regiao,"Centro-Oeste");
    }
    else if(municipios->codigo_uf == 52){
        strcpy(municipios->uf,"GO");
        strcpy(municipios->regiao,"Centro-Oeste");
    }
    else if(municipios->codigo_uf == 53){
        strcpy(municipios->uf,"DF");
        strcpy(municipios->regiao,"Centro-Oeste");
    }

    return municipios;
}


typedef struct {
     uintptr_t * table;
     int size;
     int max;
     uintptr_t deleted;
     char * (*get_key)(void *);
}thash;


uint32_t hashf(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int hash_insere(thash * h, void * bucket){
    uint32_t hash = hashf(h->get_key(bucket),SEED);
    uint32_t hash_aux = hashf(h->get_key(bucket),SEED2);
    int pos = hash % (h->max);
    int incrementa = 0;
    if (h->max == (h->size+1)){
        free(bucket);
        return EXIT_FAILURE;
    }else{
        while(h->table[pos] != 0){
            if (h->table[pos] == h->deleted)
                break;
            pos = (pos + incrementa++ * (hash_aux % h->max)) % h->max;

            if(incrementa >= h->max){
                free(bucket);
                return EXIT_FAILURE;    
            }
        }
        h->table[pos] = (uintptr_t) bucket;
        h->size +=1;
    }
    return EXIT_SUCCESS;
}

int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(sizeof(uintptr_t),nbuckets +1);
    if (h->table == NULL){
        return EXIT_FAILURE;
    }
    h->max = nbuckets +1;
    h->size = 0;
    h->deleted = (uintptr_t) & (h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}


void *hash_busca(thash h, const char * key){
    uint32_t hash = hashf(key, SEED);
    uint32_t hash_aux = hashf(key, SEED2);
    int pos = hash % (h.max);
    int incrementa = 0;
    while(h.table[pos] != 0){
        if (strcmp (h.get_key((void*)h.table[pos]),key) == 0)
            return (void *)h.table[pos];
        else
            pos = (pos + incrementa++ * (hash_aux % h.max)) % h.max;
    }
    return NULL;
}

int hash_remove(thash * h, const char * key){
    uint32_t hash = hashf(key, SEED);
    uint32_t hash_aux = hashf(key, SEED2);
    int pos = hash % (h->max);
    int incrementa = 0;
    while(h->table[pos]!=0){
        if (strcmp (h->get_key((void*)h->table[pos]),key) ==0){
            free((void *) h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -=1;
            return 1;
        }else{
            pos = (pos + incrementa++ * (hash_aux % h->max)) % h->max;
        }

    }
    return 0;
}

void hash_apaga(thash *h){
    int pos;
    for(pos =0;pos< h->max;pos++){
        if (h->table[pos] != 0){
            if (h->table[pos]!=h->deleted){
                free((void *)h->table[pos]);
            }
        }
    }
    free(h->table);
}

/*
void test_hash(){
    thash h;
    int nbuckets = 10;
    hash_constroi(&h,nbuckets,get_key);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_FAILURE);
    hash_apaga(&h);
}

void test_search(){
    thash h;
    int nbuckets = 10;
    taluno * aluno;
    hash_constroi(&h,nbuckets,get_key);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("takashi","1123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("matsubara","2123456789"))==EXIT_SUCCESS);

    aluno = hash_busca(h,"edson");
    assert(aluno->cpf[0]=='0');
    aluno = hash_busca(h,"takashi");
    assert(aluno->cpf[0]=='1');
    aluno = hash_busca(h,"matsubara");
    assert(aluno->cpf[0]=='2');
    aluno = hash_busca(h,"patricia");
    assert(aluno == NULL);

    hash_apaga(&h);
}

void test_remove(){
    thash h;
    int nbuckets = 10;
    taluno * aluno;
    hash_constroi(&h,nbuckets,get_key);
    assert(hash_insere(&h,aloca_aluno("edson","0123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("takashi","1123456789"))==EXIT_SUCCESS);
    assert(hash_insere(&h,aloca_aluno("matsubara","2123456789"))==EXIT_SUCCESS);

    aluno = hash_busca(h,"edson");
    assert(aluno->cpf[0]=='0');
    aluno = hash_busca(h,"takashi");
    assert(aluno->cpf[0]=='1');
    aluno = hash_busca(h,"matsubara");
    assert(aluno->cpf[0]=='2');
    aluno = hash_busca(h,"patricia");
    assert(aluno == NULL);

    assert(h.size == 3);
    assert(hash_remove(&h,"edson")==EXIT_SUCCESS);
    aluno = hash_busca(h,"edson");
    assert(aluno == NULL);
    assert(h.size == 2);

    assert(hash_remove(&h,"edson")==EXIT_FAILURE);

    aluno = hash_busca(h,"matsubara");
    assert(aluno->cpf[0]=='2');


    hash_apaga(&h);
}
*/

int main(){
    thash h;
    int nbuckets = 5571;
    muni temp;
    muni *aux;
    FILE *arq;
    int opcao;
    int remove;
    char cod_ibge[9];


    hash_constroi(&h, nbuckets, get_key);

    arq = fopen("municipios.csv","r");

    if(arq == NULL){
        printf("\nErro ao abrir o arquivo municipios.\n\n");
        exit(1);
    }
    else{
        while(feof(arq) == 0){
            fscanf(arq, " %[^,], %[^,],%lf,%lf,%d,%d, %[^,],%d, %[^\n]", 
                temp.cod_ibge, temp.nome, &temp.coord[0], &temp.coord[1], 
                &temp.capital, &temp.codigo_uf, temp.siafi_id, &temp.ddd,
                temp.fuso_horario);
            hash_insere(&h, aloca_muni(temp.cod_ibge, temp.nome, temp.coord[0], temp.coord[1], 
                temp.capital, temp.codigo_uf, temp.siafi_id, temp.ddd,
                temp.fuso_horario));
        }

    }
    fclose(arq);

    do{
        printf("1 - Buscar municipio pelo código IBGE\n2 - Remover municipio pelo código IBGE\n0 - Sair\n");
        scanf("%d", &opcao);
        if(opcao == 1){
            printf("Digite o código IBGE: ");
            scanf("%s", cod_ibge);

            aux = hash_busca(h, cod_ibge);

            if(aux == NULL)
                printf("\nMunicipio não encontrado.\n");
            else
                printf("\nMunicipio encontrado.\n");
        }

        else if(opcao == 2){
            printf("Digite o código IBGE: ");
            scanf("%s", cod_ibge);

            remove = hash_remove(&h,cod_ibge);

            if(remove == 0)
                printf("\nFalha ao remover.\n");
            else
                printf("\nRemovido com sucesso.\n");
        }
    }while(opcao != 0);

    return EXIT_SUCCESS;
}
