#include <stdio.h>
#define Nmax 100
#define Nmin 10
#define R_MAX 2147483647

static long long unsigned int SEED = 0x1; // seed generatora
void srnd(int seed) { SEED = seed; }
int navrat[2] = {0};
int rnd(int from, int to) {
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}

int max(int one, int two){
    if(one > two){
        return one;
    }
    return two;
}
int tah(int hrac1,int hrac2, int round, int navstevy[], int t, int tunely[][2], int n, int cislohraca){
    int tmp = 0;
    int eliminacia = 0, teleport = 0;
    int i = 0;
    int R1 = 0, R2 = 0;
    R1 = rnd(1,6);
    R2 = rnd(1,6);
    //Mimo trate
    if(hrac1 == -1){
        tmp = hrac1;
        if(R1 + R2 > 7){
            if(R1 + R2 - 7 == hrac2){
                hrac2 = -1;
   
                eliminacia = 1;
            };
            hrac1 = R1 + R2 -7;
        }
        for(i = 0; i < t; i++){
            if(hrac1 == tunely[i][0]){
                if(tunely[i][1] == hrac2){
                    hrac2 = -1;
                    eliminacia=1;
                }
                hrac1 = tunely[i][1];
                teleport = 1;
                break;
            }
        }
        if(teleport && eliminacia){
            printf("[%d,%d] [%d] [%d,%d] [%d] T E\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else if(teleport){
            printf("[%d,%d] [%d] [%d,%d] [%d] T\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else if(eliminacia){
            printf("[%d,%d] [%d] [%d,%d] [%d] E\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else{
            printf("[%d,%d] [%d] [%d,%d] [%d]\n", round,cislohraca, tmp, R1, R2, hrac1);
        }
        eliminacia = 0;
        teleport = 0;
        if(hrac1 != -1){
            navstevy[hrac1]++;
        }

    }
    //Specialny
    else if(R1 + R2 == 2 && hrac1 > hrac2 && hrac1 != -1 && hrac2 != -1){
        tmp = hrac1;
        hrac1 = hrac2;
        hrac2 = tmp;
        printf("[%d,%d] [%d] [%d,%d] [%d] S\n", round,cislohraca, hrac2, R1, R2, hrac1);
        navstevy[hrac1]++;
        navstevy[hrac2]++;
    }
    //Specialny
    else if(R1 + R2 == 12 && hrac1 < hrac2 && hrac1 != -1 && hrac2 != -1){
        tmp = hrac1;
        hrac1 = hrac2;
        hrac2 = tmp;
        printf("[%d,%d] [%d] [%d,%d] [%d] S\n", round,cislohraca, hrac2, R1, R2, hrac1);
        navstevy[hrac1]++;
        navstevy[hrac2]++;
    }
    //Teleport alebo eliminacia, normalny tah
    else{
        if(hrac1 + max(R1,R2) == hrac2){
            hrac2 = -1;
            eliminacia = 1;
        }
        tmp = hrac1;
        hrac1 += max(R1,R2);
        for(i = 0; i < t; i++){
            if(hrac1 == tunely[i][0]){
                if(tunely[i][1] == hrac2){
                    hrac2 = -1;
                    eliminacia=1;
                }
                hrac1 = tunely[i][1];
                teleport = 1;
                break;
            }
        }
        if(teleport && eliminacia){
            printf("[%d,%d] [%d] [%d,%d] [%d] T E\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else if(teleport){
            printf("[%d,%d] [%d] [%d,%d] [%d] T\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else if(eliminacia){
            printf("[%d,%d] [%d] [%d,%d] [%d] E\n", round,cislohraca, tmp, R1, R2, hrac1);
        }else{
            printf("[%d,%d] [%d] [%d,%d] [%d]\n", round,cislohraca, tmp, R1, R2, hrac1);
        }
        eliminacia = 0;
        teleport = 0;
        if(hrac1 != -1){
            navstevy[hrac1]++;
        }

    }
    if(hrac1 >= n){
        printf("WINNER: %d\nVISITS:", cislohraca);
        for(i = 0; i < n; i++){
            printf(" %d",navstevy[i]);
        }
        return 0;
    }
    navrat[0] = hrac1;
    navrat[1] = hrac2;
    return 1;
}



int main() {
    //2D Pole s indexami tunelov
    int tunely[Nmax][2] = {0};
    //Pole s navstevnostami
    int navstevy[Nmax] = {0};
    //Premenna s na genereovanie cisel, t pocet tunelov, n dlzka trate, pomocne i,j pre for loopy
    int s = 0, t = 0, n = 0, i = 0, j = 0;
    //Premenne pre poziciu hracov
    int hrac1 = -1, hrac2 = -1;
    scanf("%d %d %d", &s, &t, &n);
    // Podmienky s > 0, n je v intervale <Nmin,Nmax>, t*2 musi byt <= n/2
    if(s <= 0 || n > Nmax || n < Nmin || (t*2 > n/2)){
        return 1;
    }
    //Zavolame funkciu na seed
    srnd(s);
    //Nacitame tunely pomocou for loopu
    for(i = 0; i < t; i++){
        scanf("%d %d", &tunely[i][0], &tunely[i][1]);
    }
    
    for(i = 0; i < t; i++){
        for(j = 0; j < t; j++){
            //Vchod a vychod tunela nesmie byt na prvom indexe (0) a poslednom policku drahy (n-1)
            //Vchod a vychod tunela nesmu byt mimo zavodnej drahy, to su zaporne indexy alebo indexy vacsie ako platny rozsah drahy
            if(tunely[i][0] <= 0 || tunely[i][0] >= n-1 || tunely[i][1] <= 0 || tunely[i][1] >= n-1){
                return 2;
            }
            //Kazdy tunel musi mat unikatny vchod
            if(tunely[i][0] == tunely[j][0] && i != j){
                return 2;
            }
            //Vychod tunela nesmie byt zaroven vchodom do ineho tunela
            if(tunely[i][1] == tunely[j][0] && i != j){
                return 2;
            }
            //Vchod a vychod z tunela nesmu mat rovnaky index
            if(tunely[i][0] == tunely[i][1]){
                return 2;
            }
        }
    }

    //Vyprintovanie tunelov podla zadania
    printf("TUNNELS:");
    for(j = 0; j < n; j++){
        for(i = 0;i < t; i++){
            if(j == tunely[i][0]){
                printf(" [%d,%d]", tunely[i][0], tunely[i][1]);
                break;
            }
        }
    }
    printf("\n");
    
    //While loop pre samotny zavod, trva dokym ani jeden z hracov neprejde cez celu trat
    int round = 1;
    
    while(1){
        if(tah( hrac1, hrac2, round, navstevy, t, tunely, n, 1) == 0){
            break;
        };
        hrac1 = navrat[0];
        hrac2 = navrat[1];
        round++;
        if(tah(hrac2, hrac1, round, navstevy, t, tunely, n, 2) == 0){
            break;
        }
        hrac2 = navrat[0];
        hrac1 = navrat[1];
        round++;
    }
    return 0;
}
