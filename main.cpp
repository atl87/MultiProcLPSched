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
    
    int MAX_NO_OF_TASKS=30;
    int MAX_TASKSETS_PER_SIMULATION=100;
    int number_of_tasks=6;
    int MAX_PERIOD=490;
    int MIN_PERIOD=10;
    float DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
    if(DEADLINE_FRACTION>1)
        DEADLINE_FRACTION=1;
    float npr_percentage=0.1;
    
    float NO_OF_PROCESSORS=(float)4.0000;   
    int MAX_TIME=10000;
    
    varying_tasks(MAX_NO_OF_TASKS, number_of_tasks, MAX_TASKSETS_PER_SIMULATION, MIN_PERIOD, MAX_PERIOD, DEADLINE_FRACTION, npr_percentage, NO_OF_PROCESSORS, MAX_TIME);

}
