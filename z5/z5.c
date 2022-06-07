#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "config.h"
//Funkcia na zratanie riadkov v subore
int countlines(char* where){
  FILE* file;
  char ch = 0;
  int whnum = 0;
  file = fopen(where,"r");
  //For ide dokym nenajde \n, potom zvysi pocet riadok a skonci ked sa dostane na EOF
  for(;ch != EOF;ch = getc(file)){
    if( ch == '\n'){
      whnum++;
    }
  }
  fclose(file);
  return whnum;
}
//Vyprintovanie tovarov v sklade danom parametrom
void printwh(WAREHOUSE wh, int a, int d){
  int pocet = 0, i = 0, j = 0;
  char cesta[MAX_NAME+20] = {0};
  FILE* file;
  ITEM temp;
  //Pozliepame dokopy cestu pre subor z ktoreho cerpame data o tovaroch
  strcat(cesta,ITEMS_FOLDER);
  strcat(cesta,PATH_SEPARATOR);
  strcat(cesta,wh.name);
  strcat(cesta,".txt");
  if(access(cesta, F_OK) == -1){
    fprintf(stderr, "FILE_ERROR %s.txt\n", wh.name);
    return;
  }
  pocet = countlines(cesta);
  if(pocet > wh.capacity){
    fprintf(stderr,"CAPACITY_ERROR %s.txt\n", wh.name);
    return;
  }
  file = fopen(cesta,"r");
  //Alokacia tolko itemov, kolko je riadkov v subore
  wh.items = malloc(pocet * sizeof(ITEM));
  //For ide od 0 po pocet itemov a uklada ich do zoznamu itemov
  for(i = 0; i < pocet; i++){
    if(fscanf(file,"%s %d", wh.items[i].name, &wh.items[i].price) != 2 ){
      fprintf(stderr,"FORMAT_ERROR %s.txt\n", wh.name);
      free(wh.items);
      return;
    }
  }
  fclose(file);
  //Bubble sort na usporiadanie itemov podla mena
  for(i = 0; i < pocet; i++){
    for(j = 0; j < pocet-1; j++){
      if(!a && !d){
        if(strcmp(wh.items[j].name,wh.items[j+1].name) > 0){
          temp = wh.items[j];
          wh.items[j] = wh.items[j+1];
          wh.items[j+1] = temp;
        }
      }
      if(a){
        if(wh.items[j].price > wh.items[j+1].price){
          temp = wh.items[j];
          wh.items[j] = wh.items[j+1];
          wh.items[j+1] = temp;
        }
      }
      if(d){
        if(wh.items[j].price < wh.items[j+1].price){
          temp = wh.items[j];
          wh.items[j] = wh.items[j+1];
          wh.items[j+1] = temp;
        }
      }
    }
  }
  //Vypis hlavicky
  printf("%s %.3lf %.3lf %d :\n", wh.name, wh.gps.lat, wh.gps.lon, wh.capacity);
  //Vyprintujeme zoradene tovary
  for(i = 0; i < pocet; i++){
    printf("%d. %s %d\n", i + 1, wh.items[i].name, wh.items[i].price);
  }
  free(wh.items);
}
int main(int argc, char *argv[]){
  FILE* whs;
  char name[MAX_NAME], optstring[] = ":w:n:e:ad";
  double lat = 0, lon = 0, vzd = 0, max = INT_MAX;
  int i = 0, num = 0, w = 0, e = 0, n = 0, a = 0, d = 0, opt = 0,distsklad = 0;
  WAREHOUSE *sklady;
  num = countlines(WAREHOUSE_DB_FILE);
  //Zoznamu skladov alokujeme num krat WAREHOUSE pamate
  sklady = malloc(sizeof(WAREHOUSE) * num);
  //Nacitavanie CMD argumentov
  while ((opt = getopt(argc, argv, optstring)) != -1) {
      switch (opt) {
          case 'w':
            strcpy(name,optarg);
            w = 1;
            break;
          case 'n':
            lat = atof(optarg);
            n = 1;
            break;
          case 'e':
            lon = atof(optarg);
            e = 1;
            break;
          case 'a':
            a = 1;
            break;
          case 'd':
            d = 1;
            break;
      }
  }
  GPS bod;
  bod.lat = lat;
  bod.lon = lon;
  //Otvorime subor so skladmi a nacitame ich VSETKY do zoznamu skladov a zaroven vyprintujeme
  whs = fopen(WAREHOUSE_DB_FILE,"r");
  for(i = 0; i < num; i++){
    fscanf(whs, "%s %lf %lf %d", sklady[i].name, &sklady[i].gps.lat, &sklady[i].gps.lon, &sklady[i].capacity);
    printf("%s %.3lf %.3lf %d\n", sklady[i].name, sklady[i].gps.lat, sklady[i].gps.lon, sklady[i].capacity);
  }
  fclose(whs);
  //For loop ide od 0 po pocet skladov, kontroluje podmienky a podla toho vyradi sklady ktore nevyhovuju prepinacom
  for(i = 0; i < num; i++){
    if(w && strcmp(sklady[i].name, name) != 0){
      continue;
    }
    if(e&&n){
      vzd = distance(bod,sklady[i].gps);
      vzd = fabs(vzd);
      if(vzd < max){
        max = vzd;
        distsklad = i;
      }
      if(i!=num-1){
        continue;
      }else{
        printwh(sklady[distsklad], a, d);
        break;
      }
    }
    printwh(sklady[i], a, d);
  }
  free(sklady);
  return 0;
}
