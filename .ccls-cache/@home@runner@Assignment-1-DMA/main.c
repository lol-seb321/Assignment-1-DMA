/* COP 3502C Assignment 1
This program is written by: Sebastian Ferreira */

/*
In this program we will be using DMA, pointers, and array of pointers to solve a coding problem involving several monster trainers that want to capture a certain number of monsters in various regions. In this program we want to estimate the number of monsters the trainers could expect to capture in each region.
  */

// pre processor directives
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
#define NAMEMAX 51

// Structure declarations
typedef struct monster {
    char* name;
    char* element;
    int population;
} monster;

typedef struct region {
    char* name;
    int monster_cnt;
    int total_population;
    monster** monsters;
} region;

typedef struct itinerary {
    int region_cnt;
    region** regions;
    int captures;
} itinerary;

typedef struct trainer {
    char* name;
    itinerary* visits;
} trainer;

// Function prototypes
monster* makeMonster(char* name, char* element, int population);
monster** readMonsters(int* monsterCount);
monster* getMonster(monster** mons, char* name, int count);
region* getRegion(region** reg, char* name, int count);
region** readRegions(int* countRegions, monster** monsterList, int monsterCount);
trainer* loadTrainers(int* trainerCount, region** regionList, int countRegions);
void processInputs(monster** monsterList, int monsterCount, region** regionList,int regionCount, trainer* trainerList, int trainerCount);
void releaseMemory(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount);

//This function returns a dynamically allocated monster filled with the provided parameters
monster* makeMonster(char* name, char* element, int population) {
    monster* Mon = (monster*)malloc(sizeof(monster));
    Mon->population = population;
    Mon->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    Mon->element = (char*)malloc(sizeof(char) * (strlen(element) + 1));
    strcpy(Mon->element, element);
    strcpy(Mon->name, name);
    return Mon;
}

////This function returns an array of monster pointers
//where each monster pointer points to the dynamically allocated monsters with fill - up information from the
//provided inputs.It can use the makeMonster function in this process.This function also updates the passed
//variable reference pointed by monsterCount so that the caller to this function knows how many monsters are
//returned from the function.
monster** readMonsters(int* monsterCount) {
    monster** monsterArr = (monster**)malloc(sizeof(monster*) * (*monsterCount));

    char element[NAMEMAX], name[NAMEMAX];
    int population;

    for (int i = 0; i < (*monsterCount); i++) {
        scanf("%s", name);
        scanf("%s", element);
        scanf("%d", &population);
        monsterArr[i] = makeMonster(name, element, population);
    }
    return monsterArr;
}
//return monster pointer of matching name from total monsters
monster* getMonster(monster** mons, char* name, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(mons[i]->name, name) == 0)
            return mons[i];
    }
    return NULL;
}

//return region pointer of matching name from total monsters
region* getRegion(region** reg, char* name, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(reg[i]->name, name) == 0)
            return reg[i];
    }
    return NULL;
}

//This function returns an array of region pointers where each region pointer points to a dynamically allocated
//region, filled up with the information from the inputs, and the region’s monsters member points to an
//appropriate list of monsters from the monsterList passed to this function.This function also updates the passed
//variable reference pointed by countRegions(to inform the caller about this count).As the loadMonsters
//function has created all the monsters using dynamic memory allocation, you are getting this feature to use / reuse those monsters in this process
region** readRegions(int* countRegions, monster** monsterList, int monsterCount) {

    char name[50];

    region** regArr = (region**)malloc(sizeof(region*) * (*countRegions));
    for (int i = 0; i < (*countRegions); i++) {
        int totalPopulation = 0;
        scanf("%s", name);
        region* reg = (region*)malloc(sizeof(region));
        scanf("%d", &reg->monster_cnt);
        reg->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(reg->name, name);
        reg->monsters = (monster**)malloc(sizeof(monster*) * (reg->monster_cnt));
        for (int j = 0; j < reg->monster_cnt; j++) {
            scanf("%s", name);
            reg->monsters[j] = getMonster(monsterList, name, monsterCount);
            totalPopulation += reg->monsters[j]->population;
        }
        reg->total_population = totalPopulation;
        regArr[i] = reg;
    }
    return regArr;
}

//This function returns a dynamically allocated array of trainers, filled up with the information from the inputs,
//and the trainer’s visits field points to a dynamically allocated itinerary which is filled based on the passed
//regionList.This function also updates the passed variable reference pointed by trainerCount.As the
//loadRegions function has crated all the regions using dynamic memory allocation, you are getting this feature
//to use / re - use those regions in this process
trainer* loadTrainers(int* trainerCount, region** regionList, int countRegions) {
    char name[NAMEMAX];
    trainer* trainerList = (trainer*)malloc(sizeof(trainer) * (*trainerCount));
    for (int i = 0; i < (*trainerCount); i++) {
        scanf("%s", name);
        trainerList[i].name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(trainerList[i].name, name);
        itinerary* itin = (itinerary*)malloc(sizeof(itinerary));
        scanf("%d", &itin->captures);
        scanf("%d", &itin->region_cnt);
        itin->regions = (region**)malloc(sizeof(region*) * (itin->region_cnt));
        for (int j = 0; j < itin->region_cnt; j++) {
            scanf("%s", name);
            itin->regions[j] = getRegion(regionList, name, countRegions);
        }
        trainerList[i].visits = itin;
    }
    return trainerList;
}

//This function processes all the data and produce the output.
void processInputs(monster** monsterList, int monsterCount, region** regionList,int regionCount, trainer* trainerList, int trainerCount) {
  // Loops throgh each trainer.
    for (int i = 0; i < trainerCount; i++)
    {
        // Prints name of trainer.
        printf("%s\n", trainerList[i].name);
        itinerary* visits = trainerList[i].visits;
      
        // Loops through each region visited by trainer.
        for (int j = 0; j < visits->region_cnt; j++) 
        {
            // Prints name of the region visited
            printf("%s\n", visits->regions[j]->name);

            // Travereses through each monster in that region.
            for (int k = 0; k < visits->regions[j]->monster_cnt; k++)
            {
                // Calculates estimated num of monsters trainer can capture in the region they visited.
                // We do this by dividing the population of monster by total population of the region, then multiply that val by total captures 
                // the trainer is allowed to make.
                float num = (visits->regions[j]->monsters[k]->population) /(float)(visits->regions[j]->total_population);
                num *= visits->captures;
              
                // Round estimated num of monsters to nearest int.
                num = round(num);

                // If we have monsters captured we print monsters the trainer can capture in that region with the name of the monster.
                if (num > 0)
                    printf("%.f-%s\n", num, visits->regions[j]->monsters[k]->name);
            }
        }

        // If we get to the last trainer in trainer list. Do nothing if last, otherwise print a newline.
        if (i != trainerCount - 1)
            printf("\n");
    }
}

//This function takes all the
//dynamically allocated arraysand free - up all the memory.Y
void releaseMemory(monster** monsterList, int monsterCount, region** regionList, int regionCount, trainer* trainerList, int trainerCount) {
    for (int i = 0; i < monsterCount; i++) {
        free(monsterList[i]->name);
        free(monsterList[i]->element);
        free(monsterList[i]);
    }
    free(monsterList);
    for (int i = 0; i < regionCount; i++) {
        free(regionList[i]->name);
        free(regionList[i]->monsters);
        free(regionList[i]);
    }
    free(regionList);
    for (int i = 0; i < trainerCount; i++) {
        free(trainerList[i].name);
        free(trainerList[i].visits->regions);
        free(trainerList[i].visits);
    }
    free(trainerList);
}

// Driver function reads info about monster trainers, regions, and the monsters they wish to capture visiting that specified region. 
// It uses function readMonsters to allocate memory for an array of pointers of type monster. We fill up each monster with provided inputs from 
// the file and returns the entire 2D array.
// Next it uses function readRegions to allocate memory for the double pointer array where each pointer can point to a region. We fill up each region 
// with provided inputs from the file.  
// We then use the loadTrainers funciton to read the name of each trainer and the number of captures they expect and number of regions they are visiting
// from regionList.
// Process inputs function used to process all the data. Then loops through each trainer and prints their name. Loops through each region prints the number
// of monsters expected to be captured its a void function so nothing gets returned.
// Finally we release all dynamically allocated memory with the last function call. Free the memory allocated to each pointer in the arrays. Then frees the
// memory for the double pointer arrays themselves.
int main(void) {
    atexit(report_mem_leak); //for memory leak detector.
    int monsterCount = 0;
    int countRegions = 0;
    int countTrainer = 0;
    scanf("%d", &monsterCount);
    monster** monsterArr = readMonsters(&monsterCount);
    scanf("%d", &countRegions);
    region** regArr = readRegions(&countRegions, monsterArr, monsterCount);
    scanf("%d", &countTrainer);

    trainer* trinaerArr = loadTrainers(&countTrainer, regArr, countRegions);
    processInputs(monsterArr, monsterCount, regArr, countRegions, trinaerArr, countTrainer);
    releaseMemory(monsterArr, monsterCount, regArr, countRegions, trinaerArr, countTrainer);

    /*for (int i = 0;i < monsterCount;i++)
            printf("%s %s %d\n", monsterArr[i]->name, monsterArr[i]->element,
       monsterArr[i]->population);*/
       /*for (int i = 0;i < countRegions;i++)
               printf("%s %d", regArr[i]->name,regArr[i]->total_population);*/
}