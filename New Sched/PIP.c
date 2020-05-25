#include <stdio.h>
#include <stdlib.h>
#include "Scheduler.h"
#include <time.h>
#define Max_period 10000


int min(int a, int b){

    if (a<b)
        return a;

    else 
        return b;
}


int min_period(int * set, int rows){

    int i, period;

    period = Max_period;


    for (i=0; i<rows; i++){

        if(set[i]<period){

            period = set[i];
        }
    }

    return period;

}

void gen_cri_sen(int * set, int rows, int sem, int cri_sen[rows][sem]){


    int i, j, pos, zeros, periodo, posx, posy, num_r;

    srand(time(NULL));

    for(i=0;i<rows;i++){

        for(j=0;j<sem;j++){

            cri_sen[i][j]= -1;
        }
    }
    
    int num_rd = ((rows/2)-1)*sem+1;


    zeros = rand() % (num_rd);

    
    if (zeros>0){

        pos = rand() % (sem);


        cri_sen[0][pos]= 0;

    
        for(i=0;i<(zeros-1);i++){
            
            posx =rand() % (sem);
            posy =rand() % (rows);

            
            if(cri_sen[posy][posx]>=0)
                i--;
            
            else
                cri_sen[posy][posx] = 0;
            
        }
    
    }
    
    periodo = min_period(set, rows);
    

    num_r =  periodo/sem;

    
    for(i=0;i<rows;i++)
        for(j=0;j<sem;j++)
            //pos = rand() % num_r;
            if ((cri_sen[i][j])<0){
                if(num_r==0)
                    cri_sen[i][j]=0;
                else
                    cri_sen[i][j] = rand() % num_r;
            }
}


int* gen_ceil(int rows, int columns, int cri_sen[rows][columns]){

    int i, j, index=0;

    int* ceil = malloc(sizeof(int)*columns);
    int* aux = malloc(sizeof(int)*rows);


    for(i=0; i<columns; i++)
        ceil[i]= rows-1;


    for(i=0; i<columns; i++){

        for(j=0; j<rows; j++){

            if(cri_sen[j][i]>0){
                ceil[i] = j;
                break;
            }
        }
    }

    return ceil;

}


int * calc_b(int * set, int n_tasks, int n_sem){
    
    int i = 0, k = 0, j, D_max;
    int *Bl, *Bs, *C_sem, *B;
    int cri_sec[n_tasks][n_sem];

    srand(time(NULL));

    B = malloc(sizeof(int)*n_tasks);
    Bl = malloc(sizeof(int)*n_tasks);
    Bs = malloc(sizeof(int)*n_tasks);

    C_sem = (int *)malloc(n_sem * sizeof(int));

    
    gen_cri_sen(set, n_tasks, n_sem, cri_sec);

    printf("\n   PRIORITY INHERITANCE PROTOCOL\n\n");

    printf("\n Critical Sections Matrix:\n  ");

    for(k=0;k<n_sem;k++)
        printf(" S%d", k+1);
    printf("\n");

    for(i=0;i<n_tasks;i++){
        printf("T%d ", i+1);
        for(k=0;k<n_sem;k++)
            printf(" %d ", cri_sec[i][k]);

        printf("\n");
    }

    printf("\n");


    C_sem = gen_ceil(n_tasks, n_sem, cri_sec);

    printf("\nSemaphores Ceiling: ");

    printf("\n");

    for(i=0;i<n_sem;i++)
        printf("S%d ", i+1);

    printf("\n");

    for(i=0;i<n_sem;i++)
        printf("P%d ", (C_sem[i]+1));

    printf("\n\n");


    for (i=0;i<(n_tasks-1);i++){
        
        Bl[i] = 0;

        for (j=i+1;j<n_tasks;j++){

            D_max = 0;

            for (k=0; k<n_sem; k++){

                if((C_sem[k]<=i) && (cri_sec[j][k]>D_max))
                    D_max = cri_sec[j][k];
            }

            if (D_max>0)
                Bl[i] = Bl[i] + D_max -1;
        }

        Bs[i] = 0;

        for (k=0; k<n_sem; k++){

            D_max = 0;

            for (j=i+1;j<n_tasks;j++){

                if((C_sem[k]<=i) && (cri_sec[j][k]>D_max))
                    D_max = cri_sec[j][k];                
            }

            if (D_max>0)
                Bs[i] = Bs[i] + D_max - 1;
        }

        B[i] = min(Bl[i], Bs[i]);
    }

    B[n_tasks-1] = 0;

    return B;
}