#include"Tasks.h"
#include<iostream>
#include<cmath>
#include<iostream>
#include<fstream>
#include<time.h>
#include "fps_tests.h"
#include "utility_functions.h"
#include<stdlib.h>
using namespace std;

int quick_test_RTA_FPS(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result)
{
    real_time_taskset *helper=taskset;
    int test_failed=0;
    float new_response_time=helper->comp_time;
    float response_time=helper->comp_time;
    float interference=0.00000;
    int ctr=number_of_proc;
    while(ctr>0)
    {
        helper->RT=helper->comp_time;            //m Highest priority task has response time = comp time
         if(helper->RT > helper->deadline)
            test_failed=1;
        
        if(print_log)
            cout<<"\n\t[Crude test]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        helper=helper->next_task;
        ctr--;
    }  
    while(helper)
    {
        do{
        real_time_taskset *higher_priority_task=taskset;
        response_time=new_response_time;
        interference=(float) 0.00000;
        
        while(higher_priority_task!=helper){
            interference= interference + higher_priority_task->comp_time + (float)ceil(response_time/higher_priority_task->period)*higher_priority_task->comp_time;    
            higher_priority_task=higher_priority_task->next_task;
        }
        new_response_time=helper->comp_time + (interference/number_of_proc);
        }while(response_time!=new_response_time && new_response_time <= helper->deadline);
        
        helper->RT=new_response_time;
        if(print_log)
            cout<<"\n\tCrude test: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        if(new_response_time > helper->deadline)
        {
            test_failed=1;
            if(print_log)
                cout<<"**";
        }
        
        helper=helper->next_task;
    }
    if(test_failed)
    {
        if(print_result)
            cout<<"\n\tTaskset NOT SCHEDULABLE by QUICK";
        return 0;
    }
    else
    {
        if(print_result)
            cout<<"\n\tTaskset IS SCHEDULABLE by QUICK !!!!!";
        return 1;
    }
}

int bertogna_chirinei_lipari(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result)
{
    real_time_taskset *helper=taskset;
    int test_failed=0;
    float new_response_time=helper->comp_time;
    float response_time=helper->comp_time;
    float interference=0.00000;
    float no_of_full_jobs=0, body=0,tail=0;
    int ctr=number_of_proc;
    
     while(ctr>0)
    {
        helper->RT=helper->comp_time;            //m Highest priority task has response time = comp time
        if(helper->RT > helper->deadline)
            test_failed=1;
        
        if(print_log)
            cout<<"\n\t[Bertogna-Chirinei-Lipari]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        helper=helper->next_task;
        ctr--;
    }
        
        while(helper)
        {
            do{
            real_time_taskset *higher_priority_task=taskset;
            response_time=new_response_time;
            interference=(float) 0.00000;   
            while(higher_priority_task!=helper)
            {

                no_of_full_jobs= (float) floor((response_time+higher_priority_task->deadline - higher_priority_task->comp_time)/higher_priority_task->period);
                body=no_of_full_jobs*higher_priority_task->comp_time;
                float last_part=(response_time + higher_priority_task->deadline - higher_priority_task->comp_time) - (no_of_full_jobs*higher_priority_task->period);
                if(higher_priority_task->comp_time < last_part)
                {
                    tail=higher_priority_task->comp_time;
                }
                else
                {
                    tail=last_part;
                }
                interference+=body + tail;
                higher_priority_task=higher_priority_task->next_task;
            }
            new_response_time=helper->comp_time + (interference/number_of_proc);
            //cout<<"\n\tResponse time of task "<<helper->task_no<<" for iteration "<<i<<" is : "<<new_response_time;
            }while(response_time!=new_response_time && new_response_time <= helper->deadline);

            helper->RT=new_response_time;
            if(print_log)
                cout<<"\n\t[Bertogna-Chirinei-Lipari] : "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 

            if(helper->RT > helper->deadline)
            {
                test_failed=1; 
                if(print_log)
                    cout<<"**";
            }  
            helper=helper->next_task;
        }   
    if(test_failed)
    {
        if(print_result)
            cout<<"\n\tTaskset NOT SCHEDULABLE by BCL";        
        return 0;
    }
    else
    {
        if(print_result)
            cout<<"\n\tTaskset IS SCHEDULABLE by BCL !!!!!";
        return 1;
    }
}

int p_fps_test_guan(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result)
{   
    real_time_taskset *helper=taskset;
    int test_failed=0;
    float response_time=(float)0.00, new_response_time=(float)0.00, interference=(float)0.00;
    
    float diff[100];
    for(int s=0;s<100;s++)
            diff[s]=(float)0.0000;
    
    int ctr=(int)number_of_proc;
    
    while(ctr>0)
    {
        helper->RT=helper->comp_time;            //m Highest priority task has response time = comp time
        helper->RT_guan=helper->RT;
        if(helper->RT > helper->deadline)
            test_failed=1;
        
        if(print_log)
            cout<<"\n\t[Guan etal.]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        helper=helper->next_task;
        ctr--;
    }
    while(helper)
    {
        new_response_time=helper->comp_time;

        do{
        real_time_taskset *higher_priority_task=taskset;
        response_time=new_response_time;
        interference=(float) 0.00000;
        int i=0;
        
        while(higher_priority_task!=helper){
            
            float no_carry_in=interference_of_task_without_CI(higher_priority_task,response_time);
            
            float interference2= response_time - helper->comp_time +1;
            if(interference2 < no_carry_in)
                no_carry_in=interference2;                     
            
            interference+=no_carry_in;
            
            float with_carry_in=interference_of_task_with_CI(higher_priority_task,response_time);
            
            if(interference2 < with_carry_in)
                with_carry_in=interference2;
            
            diff[i++]=with_carry_in - no_carry_in;
            
            higher_priority_task=higher_priority_task->next_task;
        }
        
        sort(diff,i-1);
        interference+=sum_largest(diff,(int)number_of_proc-1);
               
        new_response_time=helper->comp_time + (floor(interference/number_of_proc));
        }while(response_time!=new_response_time && new_response_time <= helper->deadline);
        
        helper->RT=new_response_time;
        helper->RT_guan=helper->RT;
        if(print_log)
            cout<<"\n\t[Guan etal.]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        if(new_response_time > helper->deadline)
        {
            test_failed=1;   
            if(print_log)
                cout<<"**";
        }
   
        helper=helper->next_task;
    }
      
    if(test_failed)
    {
        if(print_result)
            cout<<"\n\tTaskset NOT SCHEDULABLE by Guan et al.";
        return 0;
    }
    else
    {
         if(print_result)
             cout<<"\n\tTaskset IS SCHEDULABLE by Guan et al. !!!!!";
        return 1;
    }
              
}

int lazy_lp_fps_test_linkbased(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result)
{   
    real_time_taskset *helper=taskset;
    int test_failed=0;
    float response_time=(float)0.00, new_response_time=(float)0.00, interference=(float)0.00;
    
    float diff[100];
    for(int s=0;s<100;s++)
            diff[s]=(float)0.0000;
    
    int ctr=(int)number_of_proc;
    
    generate_inflated_taskset(taskset);
    
    while(ctr>0)
    {
        helper->RT=helper->comp_time;            //m Highest priority task has response time = comp time
        helper->RT_guan=helper->RT;
        if(helper->RT > helper->deadline)
            test_failed=1;
        
        if(print_log)
            cout<<"\n\t[Guan etal.]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        helper=helper->next_task;
        ctr--;
    }
    while(helper)
    {
        new_response_time=helper->comp_time;

        do{
        real_time_taskset *higher_priority_task=taskset;
        response_time=new_response_time;
        interference=(float) 0.00000;
        int i=0;
        
        while(higher_priority_task!=helper){
            
            float no_carry_in=interference_of_task_without_CI(higher_priority_task,response_time);
            
            float interference2= response_time - helper->comp_time +1;
            if(interference2 < no_carry_in)
                no_carry_in=interference2;                     
            
            interference+=no_carry_in;
            
            float with_carry_in=interference_of_task_with_CI(higher_priority_task,response_time);
            
            if(interference2 < with_carry_in)
                with_carry_in=interference2;
            
            diff[i++]=with_carry_in - no_carry_in;
            
            higher_priority_task=higher_priority_task->next_task;
        }
        
        sort(diff,i-1);
        interference+=sum_largest(diff,(int)number_of_proc-1);
               
        new_response_time=helper->comp_time + (floor(interference/number_of_proc));
        }while(response_time!=new_response_time && new_response_time <= helper->deadline);
        
        helper->RT=new_response_time;
        helper->RT_guan=helper->RT;
        if(print_log)
            cout<<"\n\t[Guan etal.]: "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        if(new_response_time > helper->deadline)
        {
            test_failed=1;   
            if(print_log)
                cout<<"**";
        }
   
        helper=helper->next_task;
    }
    
    restore_taskset(taskset);
    if(test_failed)
    {
        if(print_result)
            cout<<"\n\tTaskset NOT SCHEDULABLE by Link based et al.";
        return 0;
    }
    else
    {
         if(print_result)
             cout<<"\n\tTaskset IS SCHEDULABLE by Link based et al. !!!!!";
        return 1;
    }              
}

int eager_lp_fps_rta(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result)
{   
    real_time_taskset *helper=taskset;
    int test_failed=0;
    float new_workload=0.00*0, interference=0.00*0;
    float diff[100];
    
    while(helper)
    {
        float total_blocking_before_start=blocking_from_m_tasks(helper,number_of_proc);
        if(print_result)
            cout<<" Block: "<<total_blocking_before_start;

        float total_blocking_after_start=blocking_from_m_tasks_afterstart(helper,number_of_proc-1);
       
        for(int s=0;s<100;s++)
            diff[s]=(float)0.0000*0;
        
        float t=(float)1.0000;

        do{
        real_time_taskset *higher_priority_task=taskset;
        interference=(float) 0.0000*0;
        int i=0;
        
        while(higher_priority_task!=helper){
            
            float no_carry_in=interference_of_task_without_CI(higher_priority_task,t);
            
            float limit_on_interference= t - (helper->comp_time - helper->largest_NPR +1);
            
            if(limit_on_interference < no_carry_in && limit_on_interference> (float) 0)
                no_carry_in=limit_on_interference;
              
            interference+=no_carry_in;
            
            float with_carry_in=interference_of_task_with_CI(higher_priority_task,t);
            
            if(limit_on_interference < with_carry_in && limit_on_interference> (float) 0)
                with_carry_in=limit_on_interference;
            
            diff[i++]=with_carry_in - no_carry_in;
            
            higher_priority_task=higher_priority_task->next_task;
        }
        
        sort(diff,i-1);
        interference+=sum_largest(diff,(int)number_of_proc-1); 
                
        float no_of_preemptions=calc_no_of_preemptions(taskset, helper, t);
        
        new_workload=helper->comp_time - helper->largest_NPR + 1 + floor((total_blocking_before_start + (no_of_preemptions*total_blocking_after_start) + interference)/number_of_proc);
                
        t=t+(float)1.0000;        
        }while((new_workload > (t-(float)1.0000)) && new_workload <= helper->deadline);
        
        t=t-(float)1.0000; 
        float no_of_preemptions=calc_no_of_preemptions(taskset, helper, t);
        new_workload=total_blocking_before_start + (no_of_preemptions*total_blocking_after_start) + interference;
        
        helper->RT=helper->comp_time +(float)floor(new_workload/number_of_proc);
               
        if(interference < (float)0.0000)
            exit(1);
        helper->RT_our=helper->RT;
        
        if(print_log)
            cout<<"\n\t[G-LP-FPS with EPA]: Task "<<helper->task_no<<" : "<<helper->RT<<" (dl : "<<helper->deadline<<")"; 
        
        if(helper->RT > helper->deadline)
        {
            test_failed=1;   
            if(print_log)
                cout<<"**";
        }
   
        helper=helper->next_task;
    }
      
    if(test_failed)
    {
        if(print_result)
            cout<<"\n\tTaskset NOT schedulable by [G-LP-FPS with EPA]";
        return 0;
    }
    else
    {
         if(print_result)
             cout<<"\n\tTaskset is schedulable by [G-LP-FPS with EPA]";
        return 1;
    }              
}