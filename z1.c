//
//  Samuel_Klement.c
//  
//
//  Created by Samuel Klement on 25/02/2022.
//

#include <stdio.h>
int main(){
    double m;
    double T;
    double H;
    double g;
    double vsoft;
    double dt;
    scanf("%lf %lf %lf %lf %lf %lf", &m, &T, &H, &g, &vsoft, &dt);
    printf("m=%.3lf\nT=%.3lf\nH=%.3lf\ng=%.3lf\nvsoft=%.3lf\ndt=%.3lf\n", m, T, H, g, vsoft, dt);
    double A = T/m;
    double df = ((A-g)*H) / A;
    double de = H - df;
    printf("de=%.3lf\n", de);
    int s = 0;
    double h = H;
    double t = 0.0;
    double v = 0.0;
    double a;
    int total = 0, max = 0;
    int temp;
    
    
    while( h > 0 ){
        if( h <= de && v < vsoft){
            s = 1;
            a=A-g;
            temp++;
            total++;
        }else{
            if(temp > max){
                max = temp;
            }
            temp=0;
            s = 0;
            a=-g;
        }
        if(temp > max){
            max = temp;
        }
        printf("s=%d h=%08.3lf t=%.3lf v=%.3lf\n", s, h, t, v);

        
        h = h + v*dt + a*(dt*dt)/2;
        v = v + a*dt;
        t+=dt;
    }
    printf("---Landed---\n");
    printf("h    =%08.3lf\n", h);
    printf("t    =%8.3lf\n", t);
    printf("v    =%8.3lf\n", v);
    printf("total=%4d\n",total);
    printf("max  =%4d\n",max);
    return 0;
}
