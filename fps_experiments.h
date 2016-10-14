/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fps_experiments.h
 * Author: abhilash
 *
 * Created on October 14, 2016, 8:32 AM
 */

#ifndef FPS_EXPERIMENTS_H
#define FPS_EXPERIMENTS_H

void varying_tasks(int MAX_NO_OF_TASKS, int number_of_tasks, float MIN_UTIL, float MAX_UTIL, int MAX_TASKSETS_PER_SIMULATION, int MIN_PERIOD, int MAX_PERIOD, float npr_percentage, float NO_OF_PROCESSORS, int MAX_TIME);
void varying_processors(int number_of_tasks, float MIN_UTIL, float MAX_UTIL, int MAX_TASKSETS_PER_SIMULATION, int MIN_PERIOD, int MAX_PERIOD, float npr_percentage, float MIN_NO_PROC, float MAX_NO_PROC, int MAX_TIME);

#endif /* FPS_EXPERIMENTS_H */

