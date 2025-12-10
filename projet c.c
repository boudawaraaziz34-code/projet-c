#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
float simulation_croissant(float temp){
    temp=temp+(rand()%40)*0.1;
    return temp;

}
float simulation_decroissant(float temp){
    temp=temp-(rand()%40)*0.1;
    return temp; 
}
float simulation_aleatoire(float temp){
    if (temp<3){
        temp+=rand()%2;
    }else if(temp>35){
        temp-=rand()%3;
    }else{
       
    
    
    temp=temp+(rand()%8)-4;}
         
       return temp;
}
/*
int mesure(float min ,float max,time_t t){
    srand (t*100);
    float alea = (min + rand() % (int)(max-min));
    return(alea);
}*/
float seuil(char str[],int debut,int longueur){
    char sub[10];
    strncpy(sub ,str+debut ,longueur);
    sub[longueur]='\0';
    return atof(sub);
}




//  ===================== Programme Principale ===================  //
int main(){
    float seuil_min,seuil_max;
    int intervalle_mesure;
    char fmin[100],fmax[100],intervalle[100];


    //  ===== creation du fichier de configuration ===== //
    FILE * fichier;
    fichier=fopen("config.txt","w");
    fprintf(fichier,"seuil_min = 5\nseuil_max = 35\nintervalle = 500");
    fclose(fichier);


    // ===== lire le fichier de configuration =====//

    fichier=fopen("config.txt","r");
    fgets(fmin,100,fichier);
    fgets(fmax,100,fichier);
    fgets(intervalle,100,fichier);
    seuil_min=seuil(fmin,12,3);
    seuil_max=seuil(fmax,12,3);
    intervalle_mesure=(int)seuil(intervalle,13,4);
    fclose(fichier);



    int valeur_temp_initiale =20;

    float valeur_mesuree=valeur_temp_initiale; 
    int nb_alert3=0,nb_alert2=0,nb_alert1 =0;
    time_t current_time;


    // ===== choisir un type de simulation ===== //
    int type_simulation ;
    printf("choisir un type de simulation :\n1.simulation d un jour chaud \n2. simulation d un jour froid \n3.simulation d un jour aleatoire \n");
    scanf("%d",&type_simulation) ;
    // ===== creation ou intialisation de fichier val_act ===== //
    FILE *val_act;
    val_act=fopen("val_act.txt","w");
    float min =1000,max=-100 , moyenne=0;
    int nb_alert_succ=0,nb_mesures =24;
    int duree_alert=0;
   for (int i=0;i<nb_mesures; i++){
        current_time=time(NULL)+3599.5*i;
        srand(current_time);
         switch(type_simulation) {
            case 1:valeur_mesuree=simulation_croissant(valeur_mesuree);break;
            case 2:valeur_mesuree=simulation_decroissant(valeur_mesuree);break;
            case 3:valeur_mesuree=simulation_aleatoire(valeur_mesuree);break;
        }
    
        
        if (valeur_mesuree < min)  min = valeur_mesuree ;
        if (valeur_mesuree>max) max = valeur_mesuree ;
        moyenne += valeur_mesuree/nb_mesures;
        fprintf(val_act," Mesure %d : %.1f        efectuee a %s",i+1,valeur_mesuree,ctime(&current_time));
        if (valeur_mesuree>seuil_max+30 || valeur_mesuree<seuil_min-15){
            fprintf( val_act,"====>danger immédiat\n");
            nb_alert1++;  nb_alert_succ+=1;
        }else if (valeur_mesuree>seuil_max+15 || valeur_mesuree<seuil_min-7){
            fprintf(val_act,"====>  dépassement important\n");
            nb_alert2++;  nb_alert_succ+=1;
        }else if (valeur_mesuree>seuil_max || valeur_mesuree<seuil_min){
            fprintf(val_act,"====> dépassement léger des seuils\n");
            nb_alert3++;  nb_alert_succ+=1;
        }else {
            if(nb_alert_succ==1){
                fprintf(val_act ,"====> fausse alarm  , etat normale\n");}
            else if(nb_alert_succ>=2){
                duree_alert+=(nb_alert_succ-1)*intervalle_mesure;
                fprintf(val_act,"====> etat normale ");
                
            }
            nb_alert_succ=0;
            
        };
        
           
        Sleep(intervalle_mesure);
    }
    if (nb_alert_succ >= 1) {
    duree_alert += (nb_alert_succ - 1) * intervalle_mesure;
}

    fclose(val_act);





    // ==== generer un rapport journalier ===//

    FILE* rapport_journalier=fopen("rapport_journalier.txt","w");
    fprintf(rapport_journalier, "           Rapport journalier de temperature \n");
    fprintf(rapport_journalier,"la temperature moyenne est de l ordre de : %.2f °C\n",moyenne);
    fprintf(rapport_journalier, "la temperature minimale est : %.2f\n",min);
    fprintf(rapport_journalier, "la temperature maximale est : %.2f\n",max);
    fprintf(rapport_journalier,"la duree totale en etat dalert est : %d\n",duree_alert);
    fprintf(rapport_journalier,"le nombre dalertes de niveau 1 est : %d \n",nb_alert1);
    fprintf(rapport_journalier,"le nombre dalertes de niveau 2 est : %d \n",nb_alert2);
    fprintf(rapport_journalier,"le nombre dalertes de niveau 3 est : %d \n",nb_alert3);
    fclose(rapport_journalier);


    return 0 ;
}