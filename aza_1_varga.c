//
//  main.c
//  pomoc
//
//  Created by Jozef Varga on 10.4.18.
//  Copyright © 2018 Jozef Varga. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define POCET_D 12
int velkost_x, velkost_y, vyber, vloz,  xz , yz;
char ukoncenie;

void vypis(char **mapa, int ****pole, int x, int y, int bod){
    int dx1, dy1, dx2, dy2;
    dx1 = bod / 1000 / POCET_D - 5;
    dy1 = bod / 1000 % POCET_D - 5;
    dx2 = bod % 1000 / POCET_D - 5;
    dy2 = bod % 1000 % POCET_D - 5;
    mapa[x][y] = 'o';
    x = x + (-1)*dx2;
    y = y + (-1)*dy2;
    if(xz != x || yz != y)vypis(mapa, pole, x, y, pole[x][y][dx1 + 5][dy1 + 5]);
    else mapa[xz][yz] = 'o';
}

void vloz_do_radu(int *rad, int x, int y, int dx, int dy){
    rad[vloz] = (x * velkost_y + y)*1000 + ((dx+5) * POCET_D + (dy+5));
    vloz++;
}

int vyber_z_radu(int *rad){
    int x = rad[vyber];
    vyber++;
    return x;
}

void podmienka_vkladania(char **mapa, int *rad, int ****pole, int xp, int yp, int x, int y, int dx1, int dy1, int dx2, int dy2, int xk, int yk){
    if ((dx2)*(dx2) + (dy2)*(dy2) <= 25 && x >= 0 && x < velkost_x && y >= 0 && y < velkost_y && pole[x][y][dx2+5][dy2+5] == -1 ) {
        pole[x][y][dx2 + 5][dy2 + 5] = ((dx1+5) * POCET_D + (dy1+5)) * 1000 + ((dx2+5) * POCET_D + (dy2+5));
        vloz_do_radu(rad, x, y, dx2, dy2);
        if(x == xk && y == yk && ukoncenie == 0){
            ukoncenie = 1;
            vypis(mapa, pole, x, y, ((dx1+5) * POCET_D + (dy1+5)) * 1000 + ((dx2+5) * POCET_D + (dy2+5)));
        }
    }
}

void prejdi_mapu(char **mapa, int ****pole, int xk, int yk){
    int *rad = (int *)malloc(velkost_x*velkost_y*POCET_D*POCET_D * sizeof(int));;
    vloz_do_radu(rad, xz, yz, 0, 0);
    ukoncenie = 0;
    while(vyber != vloz && ukoncenie == 0){
        int x, y, dx, dy;
        int pomoc = vyber_z_radu(rad);
        x = (pomoc / 1000) / velkost_y;
        y = (pomoc / 1000) % velkost_y;
        dx = ((pomoc % 1000) / POCET_D) - 5;
        dy = ((pomoc % 1000) % POCET_D) - 5;
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx - 1), (y + dy - 1), dx, dy, (dx - 1), (dy - 1), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx - 1), (y + dy), dx, dy, (dx - 1), (dy), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx - 1), (y + dy + 1), dx, dy, (dx - 1), (dy + 1), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx), (y + dy - 1), dx, dy, (dx), (dy - 1), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx), (y + dy), dx, dy, (dx), (dy), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx), (y + dy + 1), dx, dy, (dx), (dy + 1), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx + 1), (y + dy - 1), dx, dy, (dx + 1), (dy - 1), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx + 1), (y + dy), dx, dy, (dx + 1), (dy), xk, yk);
        podmienka_vkladania(mapa, rad, pole, x, y, (x + dx + 1), (y + dy + 1), dx, dy, (dx + 1), (dy + 1), xk, yk);
    }
    if(ukoncenie == 0)printf("\nNo solution");
    free(rad);
}

int main(int argc, const char * argv[]) {
    int pocet_podvstupov, podvstup;
    char enter;
    scanf("%d",&pocet_podvstupov);
    for(podvstup = 0; podvstup<pocet_podvstupov ; podvstup++){
        int xk = 0, yk = 0, i, j, k, l;
        vyber = 0;
        vloz = 0;
        scanf("%d %d", &velkost_x, &velkost_y);
        scanf("%d %d", &xz, &yz);
        scanf("%d %d", &xk, &yk);
        int ****pole = (int ****)malloc(velkost_x * sizeof(int ***));
        char **mapa = (char **)malloc(velkost_x * sizeof(char *));
        for(i = 0; i<velkost_x; i++){
            mapa[i] = (char *)malloc(velkost_y * sizeof(char));
            pole[i] = (int ***)malloc(velkost_y * sizeof(int **));
            scanf("%c", &enter);
            for(j = 0; j<velkost_y; j++) {
                pole[i][j] = (int **)malloc(POCET_D * sizeof(int *));
                scanf("%c", &mapa[i][j]);
                if(mapa[i][j] == '.') mapa[i][j] = ' ';
                for (k = 0; k < POCET_D; k++) {
                    pole[i][j][k] = (int *)malloc(POCET_D * sizeof(int));
                    for (l = 0; l < POCET_D; l++) {
                        if(mapa[i][j] != 'X')pole[i][j][k][l] = -1;
                    }
                }
            }
        }
        prejdi_mapu(mapa, pole, xk, yk);
        if(ukoncenie != 0){
            printf("\n");
            for(i = 0; i<velkost_x; i++) {
                for (j = 0; j < velkost_y; j++) {
                    printf("%c", mapa[i][j]);
                }
                printf("\n");
            }
        }
        free(pole);
        printf("--------------------");
    }
    return 0;
}