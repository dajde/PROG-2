#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){

    int opt;
    opterr = 0;
    int i =0, j = 0 ,k =0,hran = -1, pomocna = 0, predpony = 0, dlzka = 0, d = 0,p = 0,u = 0 ,l = 0,r = 0,R = 0, novyretdlzka = 0;

    //Prepinace
    char* optstring = ":dpulr:R:";
    //Na riadok
    char ret[1001] = {0};
    char novyret[1001] = {0};
    char slovo[1001] = {0};
    char * pom = NULL;
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'd':
                d = 1;
                break;
            case 'p':
                p = 1;
                break;
            case 'u':
                u = 1;
                break;
            case 'l':
                l = 1;
                break;
            case 'r':
                r = 1;
                pom = optarg;
                break;
            case 'R':
                R = 1;
                pom = optarg;
                break;
            case '?':
                return 1;
            case ':':
                return 2;
        }
    }
    if(pom != NULL){
        optarg = pom;
    }

    //For na ocheckovanie options v non-opt arg.
    if(optind < argc){
        for(i=optind;i<argc;i++){
            if(argv[i][0] == '-'){
                return 1;
            }
        }
    }
    //Ak nemame non-opt argumenty, nastavime flag predpony na 1, inak bude 0 a nastavi sa az potom, co slovo obsahuje predponu
    if(optind == argc){
        predpony = 1;
    }else if(optarg != NULL){
        predpony = 0;
    }
    if(optarg != NULL){
        dlzka = strlen(optarg);
    }

    //Velky loop na riadky
    while(fgets (ret , 1000 , stdin) != NULL) {
        if (ret[0] == '\n') {
            break;
        }
        hran = -1;
        k = 0;
        //For na cisla, punct, velke a male pismenka
        for (i = 0; i < strlen(ret); i++) {
            //Ak plati !d, cize nemame prepinac -d a pismenko je cislo, preskocime
            if (!d && isdigit(ret[i])) {
                continue;
            }
                //Ak plati !p, cize nemame prepinac -o a pismenko je interpunkcne, preskocime
            if (!p && ispunct(ret[i])) {
                continue;
            }
            //Ak mame prepinac lowercase a pismeno je velke, zmensime ho
            if (l && ret[i] >= 65 && ret[i] <= 90) {
                novyret[k] = ret[i] + 32;
            }
            //Ak mame prepinac uppercase a pismeno je male, zvacsime ho
            else if (u && ret[i] >= 97 && ret[i] <= 122) {
                novyret[k] = ret[i] - 32;
            }
            else {
                novyret[k] = ret[i];
            }
            k++;

        }
        //pomocnu k dame ako dlzku upraveneho riadka
        novyretdlzka = k;
        //upravujeme upraveny riadok, -r -R
        for(i = 0; i < novyretdlzka;i++){
            //Ideme pre -r
            if (i > hran && isalnum(novyret[i]) && r) {
                //Vynulujeme dlzku slova
                j = 0;
                //Ak je alnum tak naplname slovo, j je dlzka slova
                while (isalnum(novyret[i + j])) {
                    slovo[j] = novyret[i + j];
                    j++;
                }
                //loop na ocheckovanie predpon
                for(k = optind; k < argc; k++){
                    for(pomocna = 0; pomocna < strlen(argv[k]); pomocna++){
                        if(strlen(argv[k]) > j) {
                            break;
                        }
                        if(slovo[pomocna] != argv[k][pomocna]){
                            break;
                        }
                        if(pomocna == strlen(argv[k]) - 1){
                            predpony = 1;
                        }
                    }

                }
                //nahradime pismenka v slove za pismenka v optarg
                if (predpony) {
                    for (k = 0; k < dlzka; k++) {
                        slovo[k] = optarg[k];
                    }
                }

                //dame do ret nase slovo
                pomocna = 0;
                for (k = i; k < i + j; k++) {
                    novyret[k] = slovo[pomocna];
                    pomocna++;
                }
                //hranicu posunieme
                if(optind != argc){
                    predpony = 0;
                }

                hran = j + i;
            }
            //Ideme pre -R
            if (i > hran && isalnum(novyret[i]) && R) {
                //Vynulujeme dlzku slova
                j = 0;
                //Ak je alnum tak naplname slovo, j je dlzka slova
                while (isalnum(novyret[i + j])) {
                    slovo[j] = novyret[i + j];
                    j++;
                }
                //loop na ocheckovanie predpon
                for(k = optind; k < argc; k++){
                    for(pomocna = 0; pomocna < strlen(argv[k]); pomocna++){
                        if(argv[k][pomocna] >= 65 && argv[k][pomocna] <= 90 && slovo[pomocna] >= 65 && slovo[pomocna] <= 90){
                            if(slovo[pomocna] != argv[k][pomocna]){
                                break;
                            }
                        }
                        else if(argv[k][pomocna] >= 65 && argv[k][pomocna] <= 90 && slovo[pomocna] >= 97 && slovo[pomocna] <=122){
                            if(slovo[pomocna] -32 != argv[k][pomocna]){
                                break;
                            }
                        }
                        else if(argv[k][pomocna] >= 97 && argv[k][pomocna] <= 122 && slovo[pomocna] >= 97 && slovo[pomocna] <=122 ){
                            if(slovo[pomocna] != argv[k][pomocna]){
                                break;
                            }
                        }
                        else if(argv[k][pomocna] >= 97 && argv[k][pomocna] <= 122 && slovo[pomocna] >= 65 && slovo[pomocna] <= 90){
                            if(slovo[pomocna] + 32 != argv[k][pomocna]){
                                break;
                            }
                        }
                        else{
                            if(slovo[pomocna] != argv[k][pomocna]){
                                break;
                            }
                        }
                        if(pomocna == strlen(argv[k]) - 1){
                            predpony = 1;
                        }

                    }
                }
                //nahradime pismenka v slove za pismenka v optarg
                if (predpony) {
                    for (k = 0; k < dlzka; k++) {
                        if(optarg[k] >= 65 && optarg[k] <= 90 && slovo[k] >= 65 && slovo[k] <= 90){
                            slovo[k] = optarg[k];
                        }else if(optarg[k] >= 97 && optarg[k] <= 122 && slovo[k] >= 97 && slovo[k] <= 122){
                            slovo[k] = optarg[k];
                        }else if(optarg[k] >= 97 && optarg[k] <= 122 && slovo[k] >= 65 && slovo[k] <= 90){
                            slovo[k] = optarg[k] - 32;
                        }else if(optarg[k] >= 65 && optarg[k] <= 90 && slovo[k] >= 97 && slovo[k] <= 122){
                            slovo[k] = optarg[k] + 32;
                        }else{
                            slovo[k] = optarg[k];
                        }

                    }
                }

                //dame do ret nase slovo
                pomocna = 0;
                for (k = i; k < i + j; k++) {
                    novyret[k] = slovo[pomocna];
                    pomocna++;
                }
                //hranicu posunieme
                if(optind != argc){
                    predpony = 0;
                }
                hran = j + i;
            }
            printf("%c",novyret[i]);

        }
    }
    return 0;
    }
