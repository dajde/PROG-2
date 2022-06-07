#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
//ak sme na windowse, pouzijeme iny parameter pre system()
#ifdef _WIN32
#define clear "cls"
#else
#define clear "clear"
#endif
//Funkcia na print riadku 1
void row1(){
  for(int i = 0; i < 21; i++){
    printf("-");
  }
  printf("\n");
}
//Funkcia na print riadku 2 -> na kazde stvrte policko dame |,
//pomedzi to na kazde druhe dame adekvatne zafarbene pismenko, zvysok su medzery
void row2(char guess[6][6], int line, char colors[6][6][20]){
  int index = 0;
  for(int i = 0; i < 21; i++){
    if(i%4 == 0){
      printf("|");
    }else if(i%2 == 0){
      printf("%s",colors[line][index]);
      printf("%c",guess[line][index++]);
      printf("\033[0m");
    }
    else{
      printf(" ");
    }
  }
  printf("\n");
}
//Print na tabulku, striedame riadok 1 a riadok 2
void table(char guess[6][6], char colors[6][6][20]){
  int line = 0;
  for(int i = 0; i < 13; i++){
    if(i%2 == 0){
      row1();
    }else{
      row2(guess,line++, colors);
    }
  }
  printf("\n\n\n");
}
//Funkcia na spravne priradenie farieb k pismenkam
void check(char* guess, char* word, char colors[6][20]){
  int wordflag[5] = {0};
  int guessflag[5] = {0};
  //Najprv prejdeme hracov tip a hadane slovo a porovnavame, ci sa niektore znaky rovnaju
  //a ak ano, dame na tu poziciu v poli colors ZELENU farbu a compare flag nastavime na tej pozicii na 1
  for(size_t i = 0; i < 5; i++){
    if(guess[i] == word[i]){
      strcpy(colors[i], "\033[0;32m");
      wordflag[i] = 1;
      guessflag[i] = 1;
    }
  }
  //Teraz porovname zvysne, este nezafarbene pismenka a pripadne im dame zltu farbu
  for (size_t i = 0; i < 5; i++) {
    if(guessflag[i]){
      continue;
    }
    for (size_t j = 0; j < 5; j++){
      if(wordflag[j]){
        continue;
      }
      if(guess[i] == word[j]){
        strcpy(colors[i],"\033[0;33m");
        guessflag[i] = 1;
        wordflag[j] = 1;
        break;
      }
    }
  }
}
int main(int argc, char* argv[]){
  //Vytvorenie premennych, words-databaza slov z ktorej sa losuje, word-vybrane slovo,
  //guess-hracove slova, colors-farby pre kazdy znak
  char *words[10] = {"WATER","APPLE","PARTY","LUNCH","TOWEL","PEACE","RIVER","SPEED","STONE","RATIO"};
  srand(time(0));
  char word[6];
  char guess[6][6];
  char colors[6][6][20];
  //Do c nacitavame input, flag je na osetrenie ked je slovo kratsie nez 5, i pomocna
  char c;
  int flag = 0;
  int i = 0;
  //Vyplnime zoznam slov od hraca medzerami a nastavime farbu na povodnu
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      guess[i][j] = ' ';
      strcpy(colors[i][j], "\033[0m");
    }
  }
  //Vylosujeme slovo, ktore hrac bude hadat
  strcpy(word,words[rand()%10]);
  //Vycistime okno, vyprintujeme prazdnu tabulku
  system(clear);
  table(guess, colors);
  //Ideme do loopu, v ktorom sa odohrava hra
  while(i < 6){
    //Nacitavanie inputu od hraca, ak je kratsie nez 5, dame flag do 1, zaroven prekonvertujeme pismenka do velkych
    for(int j = 0;j < 5; j++){
      c = getchar();
      if(c == '\n'){
        flag = 1;
        break;
      }
      guess[i][j] = toupper(c);
    }
    //Ak je zapnuty flag, vynulujeme povodne nedostatocne dlhe slovo
    if(flag){
      for(int j = 0; j < 5; j++){
        guess[i][j] = ' ';
      }
      //Nastavime flag do 0, vycistime okno, vyprintujeme tabulku v stave pred poslednym tahom a zaciname while od zaciatku
      flag = 0;
      system(clear);
      table(guess,colors);
      continue;
    }
    //Na koniec aktualneho slova dame \0 na ukoncenie slova
    guess[i][5] = '\0';
    //Vycistime stdin a okno terminalu
    fseek(stdin,0,SEEK_END);
    system(clear);
    //Priradime znakom farbicky a vyprintujeme tabulku, pripadne hlasku YOU WIN a ukoncime hru
    check(guess[i],word, colors[i]);
    table(guess,colors);
    if(strcmp(guess[i],word) == 0){
      printf("\n\nYOU WIN\n");
      return 0;
    }
    //Ideme na dalsie slovo
    i++;
  }
  printf("\n\nYOU LOST. The word was: %s\n",word);
  return 0;
}
