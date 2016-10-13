/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utility_functions.h
 * Author: abhilash-mdh
 *
 * Created on October 13, 2016, 5:53 PM
 */

#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

float max_of(float a1, float a2);
float min_of(float a1, float a2);
float body_jobs(real_time_taskset *task, float t);
float interference_of_task_with_CI(real_time_taskset *task, float t);
float interference_of_task_without_CI(real_time_taskset *task, float t);
void sort(float diff[100], int max);
float sum_largest(float diff[100], int m);
float blocking_from_m_tasks(real_time_taskset *task, int m);
float blocking_from_m_tasks_afterstart(real_time_taskset *task, int m);
float NP_blocking_from_m_tasks(real_time_taskset *task, int m);
float calc_no_of_preemptions(real_time_taskset *taskset, real_time_taskset *current_task, float t);


#endif /* UTILITY_FUNCTIONS_H */

