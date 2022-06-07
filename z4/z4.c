#include <stdio.h>
#include "config.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
void print(WAREHOUSE sklad, int *cislo, int W, int t, int p, char *t_item_name, int max_price){
  int counter = 1;
  if(!W){
    for(int i = 0;i < sklad.n; i++){
      if(t && strcmp(sklad.items[i].name,t_item_name) != 0){
        continue;
      }
      if(p && sklad.items[i].price > max_price){
        continue;
      }
      printf("%d. %s %d : %s %.3lf %.3lf %d\n",*cislo, sklad.items[i].name, sklad.items[i].price, sklad.name, sklad.gps.lat, sklad.gps.lon, sklad.n);
      *cislo = *cislo + 1;
    }
  }else{
    printf("%s %.3lf %.3lf %d :\n", sklad.name, sklad.gps.lat, sklad.gps.lon, sklad.n);
    for(int i = 0; i < sklad.n; i++){
      if(t && strcmp(sklad.items[i].name,t_item_name) != 0){
        continue;
      }
      if(p && sklad.items[i].price > max_price){
        continue;
      }
      printf("%d. %s %d\n",counter, sklad.items[i].name, sklad.items[i].price);
      counter++;
    }
  }
}
int main(int argc, char *argv[]){
    int w=0, i=0, n=0, e=0, t=0, p=0, W=0, max_price,opt, skipflag = 0, size = sizeof(db)/sizeof(db[0]), cisloriadku = 1, distsklad;
    double lat = 0, lon = 0, vzd = 0, max = INT_MAX;
    char name[MAX_NAME],item_name[MAX_NAME],t_item_name[MAX_NAME], optstring[] = ":w:i:n:e:t:p:W";
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'w':
                strcpy(name,optarg);
                w = 1;
                break;
            case 'i':
                strcpy(item_name,optarg);
                i = 1;
                break;
            case 'n':
                if(!isdigit(optarg[0])){
                  return 4;
                }
                lat = atof(optarg);
                n = 1;
                break;
            case 'e':
                if(!isdigit(optarg[0])){
                  return 4;
                }
                lon = atof(optarg);
                e = 1;
                break;
            case 't':
                strcpy(t_item_name,optarg);
                t = 1;
                break;
            case 'p':
                max_price=atoi(optarg);
                p = 1;
                break;
            case 'W':
                W = 1;
                break;
            case '?':
              return 1;
            case ':':
              return 2;

        }
    }
    if((!e && n) || (e && !n)){
      return 3;
    }
    if(e && n){
      if(lon > 180 || lon < -180 || lat > 90 || lat < -90){
        return 4;
      }
    }
    GPS bod;
    bod.lat = lat;
    bod.lon = lon;

  for(int x=0;x < size; x++){
    if(w && strcmp(db[x].name, name) !=0){
      continue;
    }
    if(i){
      skipflag = 0;
      for (int j= 0; j < db[x].n; j++) {
        if(strcmp(db[x].items[j].name, item_name) == 0){
          skipflag = 1;
        }
      }
      if(!skipflag){
        continue;
      }
    }
    if(e&&n){
      vzd = distance(bod,db[x].gps);
      vzd = fabs(vzd);
      if(vzd < max){
        max = vzd;
        distsklad = x;
      }
      if(x!=size-1){
        continue;
      }else{
        print(db[distsklad],&cisloriadku,W,t,p,t_item_name,max_price);
        break;
      }
    }
    print(db[x],&cisloriadku,W,t,p,t_item_name,max_price);
  }
      return 0;
}
