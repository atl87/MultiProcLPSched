#include"Tasks.h"
#include<iostream>
#include<cmath>
#include<iostream>
#include<fstream>
#include<time.h>
#include "fps_tests.h"
#include<stdlib.h>
using namespace std;

float max_of(float a1, float a2)
{
    if(a1 > a2)
        return a1;
    else
        return a2;
}

float min_of(float a1, float a2)
{
    if(a1 < a2)
        return a1;
    else
        return a2;
}

float body_jobs(real_time_taskset *task, float t)
{
    return (float) floor(max_of(t - task->comp_time, 0)/task->period);
}
float interference_of_task_with_CI(real_time_taskset *task, float t)
{
            
    float body= body_jobs(task, t) * task->comp_time;
    
    float head= ((float)((int)max_of(t-task->comp_time,0) % (int)task->period)) - (task->period - task->RT);
    
    head=max_of(head,0);
    
    head=min_of(head, task->comp_time);
    
    return (head+body);
}
float interference_of_task_without_CI(real_time_taskset *task, float t)
{
    float head=((float) floor(t/task->period))* task->comp_time;
    
    float tail= min_of((float)((int)t % (int)task->period), task->comp_time);
    
    return (head+tail);
}

void sort(float diff[100], int max)
{   
    for(int i=0;i<max;i++)
    {
        for(int j=i+1;j < max; j++)
        {
            if(diff[i]<diff[j])
            {
                float dummy=diff[i];
                diff[i]=diff[j];
                diff[j]=dummy;
            }  
        }
    }   
}

float sum_largest(float diff[100], int m)
{
    float sum=0.00;
    
    for(int i=0;i<m;i++)
        sum+=diff[i];
    
    return sum;
}

float blocking_from_m_tasks(real_time_taskset *task, int m)
{
    real_time_taskset * help=task->next_task;
    float blocking[100];
    int max=0;
    float sum=0;
    for(int i=0;i<100;i++)
        blocking[i]=(float)0.0000;
    
    while(help)
    {
        blocking[max++]=help->largest_NPR;
        help=help->next_task;
    }
    blocking[max++]=task->largest_NPR; 
    for(int i=0;i<max;i++)
    {
        for(int j=i+1;j < max; j++)
        {
            if(blocking[i]<blocking[j])
            {
                float dummy=blocking[i];
                blocking[i]=blocking[j];
                blocking[j]=dummy;
            } 
        }
    }
    
    for(int i=0;i<m;i++)
        sum+=blocking[i];
    
    return sum;
}

float blocking_from_m_tasks_afterstart(real_time_taskset *task, int m)
{
    real_time_taskset * help=task->next_task;
    float blocking[100];
    int max=0;
    float sum=0;
    for(int i=0;i<100;i++)
        blocking[i]=(float)0.0000*0;
    
    while(help)
    {
        blocking[max++]=help->largest_NPR;
        help=help->next_task;
    }

    for(int i=0;i<max;i++)
    {
        for(int j=i+1;j < max; j++)
        {
            if(blocking[i]<blocking[j])
            {
                float dummy=blocking[i];
                blocking[i]=blocking[j];
                blocking[j]=dummy;
            }
            
        }
    }
    
    for(int i=0;i<m;i++)
        sum+=blocking[i];
    
    return sum;
}

float NP_blocking_from_m_tasks(real_time_taskset *task, int m)
{
    real_time_taskset * help=task->next_task;
    float blocking[100];
    int max=0;
    float sum=0;
    for(int i=0;i<100;i++)
        blocking[i]=(float)0.0000;
    
    while(help)
    {
        blocking[max++]=help->comp_time;
        help=help->next_task;
    }
    blocking[max++]=task->comp_time;
    for(int i=0;i<max;i++)
    {
        for(int j=i+1;j < max; j++)
        {
            if(blocking[i]<blocking[j])
            {
                float dummy=blocking[i];
                blocking[i]=blocking[j];
                blocking[j]=dummy;
            }            
        }
    }    
    for(int i=0;i<m;i++)
        sum+=blocking[i];
    
    return sum;
}

float calc_no_of_preemptions(real_time_taskset *taskset, real_time_taskset *current_task, float t)
{
    real_time_taskset *high_priority=taskset;
    float no_of_releases=(float)0.000;
    
    while(high_priority!=current_task)
    {
        no_of_releases+= ceil(t/high_priority->period);

        high_priority=high_priority->next_task;
    }

    if(no_of_releases < current_task->no_of_pp)
        return no_of_releases;
    else
        return current_task->no_of_pp;
}