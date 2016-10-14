/* 
 * File:   main.cpp
 * Author: atl05
 *
 * Created on den 3 juni 2015, 16:08
 */

#include <cstdlib>
#include<iostream>
#include<stdio.h>
#include "Tasks.h"
#include "Task_Gen.h"
#include "fps_schedulers.h"
#include"edf_schedulers.h"
#include "fps_experiments.h"
#include<math.h>
#include <fstream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    int MAX_NO_OF_TASKS=32;
    int MAX_TASKSETS_PER_SIMULATION=100;
    int number_of_tasks=6;
    int MAX_PERIOD=490;
    int MIN_PERIOD=10;
    float MIN_NO_PROC=(float)4.0000;
    float MAX_NO_PROC=(float)24.0000;
    
    float DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
    if(DEADLINE_FRACTION>1)
        DEADLINE_FRACTION=1;
    float npr_percentage=0.1;
       
    float NO_OF_PROCESSORS=(float)4.0000;   
    
    int MAX_TIME=10000;
    float MIN_UTIL=1.0000;
    float MAX_UTIL=NO_OF_PROCESSORS/2+0.01;
    
    varying_tasks(MAX_NO_OF_TASKS, number_of_tasks, MIN_UTIL, MAX_UTIL, MAX_TASKSETS_PER_SIMULATION, MIN_PERIOD, MAX_PERIOD, npr_percentage, NO_OF_PROCESSORS, MAX_TIME);
    varying_processors(MAX_NO_OF_TASKS, MIN_UTIL, MAX_UTIL, MAX_TASKSETS_PER_SIMULATION, MIN_PERIOD, MAX_PERIOD, npr_percentage, MIN_NO_PROC, MAX_NO_PROC, MAX_TIME);


}
