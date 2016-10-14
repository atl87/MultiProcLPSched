#include <cstdlib>
#include<iostream>
#include<stdio.h>
#include "Tasks.h"
#include "Task_Gen.h"
#include "fps_schedulers.h"
#include"edf_schedulers.h"
#include<math.h>
#include <fstream>

using namespace std;

void varying_tasks(int MAX_NO_OF_TASKS, int number_of_tasks, float MIN_UTIL, float MAX_UTIL, int MAX_TASKSETS_PER_SIMULATION, int MIN_PERIOD, int MAX_PERIOD, float npr_percentage, float NO_OF_PROCESSORS, int MAX_TIME)
{
    srand (time(NULL));
    fstream f_weighted, f_normal;
    f_weighted.open("./results/weighted_results.txt",ios::out);
    if(!f_weighted)
    {
        cout<<"\nError opening file: ./results/weighted_results.txt";
        exit(1);
    }
    f_normal.open("./results/normal_results.txt",ios::out);
    if(!f_normal)
    {
        cout<<"\nError opening file: ./results/normal_results.txt";
        exit(1);
    }
    
/******************************CONTROL VARIABLES******************************/    
    int print_basic=0;
    int print_log=0;
    int print_inter=0;
         
    int DM=1;
    int DC=2;
    float DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
    if(DEADLINE_FRACTION>1.000)
        DEADLINE_FRACTION=1.0000;
/******************************CONTROL VARIABLES******************************/
    
    int no_of_preemptions_fps=0;
    int no_of_preemptions_rds=0;
    int no_of_preemptions_ads=0;  
    int no_of_preemptions_fps_d_c=0;
    int no_of_preemptions_rds_d_c=0;
    int no_of_preemptions_ads_d_c=0;
    
    
    float fps=0.0000, rds=0.0000, ads=0.0000;
    float fps_d_c=0.0000, rds_d_c=0.0000, ads_d_c=0.0000;
    float util_sum=0.0000;   
 
    while(number_of_tasks<=MAX_NO_OF_TASKS){
        
        float cur_util=MIN_UTIL;
        float max_util=MAX_UTIL;
        
        fps=0.0000;
        rds=0.0000;
        ads=0.0000;      
        fps_d_c=0.0000;
        rds_d_c=0.0000;
        ads_d_c=0.0000;
        
        util_sum=0.0000;
        
        no_of_preemptions_fps=0;
        no_of_preemptions_rds=0;
        no_of_preemptions_ads=0;
        
        no_of_preemptions_fps_d_c=0;
        no_of_preemptions_rds_d_c=0;
        no_of_preemptions_ads_d_c=0;    
               
        cout<<"\nNumber of tasks: "<<number_of_tasks<<"\n";  

        while(cur_util<=max_util){
            if(print_basic)
                cout<<"\nUtilization: "<<cur_util<<"\n";
            
            int counter=1;
            while(counter<=MAX_TASKSETS_PER_SIMULATION)
            {
                DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
                if(DEADLINE_FRACTION>1.000)
                    DEADLINE_FRACTION=1.0000;
                int successful_parameter_generation=0;
                real_time_taskset *taskset=(real_time_taskset*) malloc(sizeof(real_time_taskset));
                if(print_log)
                    cout<<"\n\nCreating real-time tasks: ";
                create_tasks(number_of_tasks,taskset);  
                if(taskset==NULL)
                {
                    cout<<"\n*****************task set not created ! *****************";
                    exit(1);
                }
                else if(print_log)
                    cout<<"\nCreated task set :"<<counter<<"\n";
                
                while(!successful_parameter_generation)
                {                   
                    if(UUniFast(number_of_tasks,cur_util,taskset,DEADLINE_FRACTION, MAX_PERIOD, MIN_PERIOD)==1)
                    {
                        successful_parameter_generation=1; 
                        if(taskset==NULL)
                        {
                            cout<<"\n\n***************** parameters not generated ! *****************";
                            exit(1);
                        }
                        taskset=sort_task_set(taskset, DM);
                    }
                }      
/****************************************************************************************YOUR CODE GOES HERE****************************************************************************************/                              
                create_NPRS(taskset,NO_OF_PROCESSORS,npr_percentage);

                float taskset_util=return_utilization(taskset);
                int preemptions_fps=0, preemptions_rds=0, preemptions_ads=0, count=0, count1=0, count2, count3, count4, count5,count6;               
                int preemptions_fps_d_c=0, preemptions_rds_d_c=0, preemptions_ads_d_c=0, count7, count8, count9; 
                
                util_sum+=taskset_util;
                count=count_tasks(taskset);
                if(count!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks generated (expected: "<<number_of_tasks<<" Actual: "<<count<<" )\n\n";
                    exit(1);          
                }
        
                preemptions_fps=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps+=preemptions_fps;
                fps+=(taskset_util*preemptions_fps);
                
                count1=count_tasks(taskset);
                if(count1!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS (expected: "<<number_of_tasks<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds+=(taskset_util*preemptions_rds);
                no_of_preemptions_rds+=preemptions_rds;
                
                count2=count_tasks(taskset);
                if(count2!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads+=(taskset_util*preemptions_ads);
                no_of_preemptions_ads+=preemptions_ads;
                
                count3=count_tasks(taskset);
                if(count3!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS (expected: "<<count2<<" Actual: "<<count3<<" )\n\n";
                    exit(1);       
                }
                
                taskset=sort_task_set(taskset, DC);
                preemptions_fps_d_c=g_fps_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                no_of_preemptions_fps_d_c+=preemptions_fps_d_c;
                fps_d_c+=(taskset_util*preemptions_fps_d_c);
                
                count7=count_tasks(taskset);
                if(count7!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS_d_M (expected: "<<number_of_tasks<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds_d_c=rds_schedule_modified(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                rds_d_c+=(taskset_util*preemptions_rds_d_c);
                no_of_preemptions_rds_d_c+=preemptions_rds_d_c;
                
                count8=count_tasks(taskset);
                if(count8!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS_d_c (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads_d_c=ads_schedule(taskset, NO_OF_PROCESSORS, MAX_TIME, print_log);
                ads_d_c+=(taskset_util*preemptions_ads_d_c);
                no_of_preemptions_ads_d_c+=preemptions_ads_d_c;
                
                count9=count_tasks(taskset);
                if(count9!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS_d_c (expected: "<<count2<<" Actual: "<<count3<<" )\n\n";
                    exit(1);       
                }
                
/****************************************************************************************YOUR CODE GOES ABOVE****************************************************************************************/                
                delete_taskset(taskset,print_log); 
                counter++;
            }
            
            f_normal<<"\n"<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
            f_normal<<"\n"<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);
            
            if(print_inter)
            {
                cout<<"\nUtilization: "<<cur_util;
                cout<<"\nPreemptions P-FPS: "<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS : "<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS : "<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
                
                cout<<"\nPreemptions P-FPS_d_c: "<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS_d_c: "<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS_d_c: "<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);
            }
            
            cur_util=cur_util+0.1;
        }
        
        f_weighted<<(fps/util_sum)<<"\t"<<(rds/util_sum)<<"\t"<<(ads/util_sum)<<"\t"<<(fps_d_c/util_sum)<<"\t"<<(rds_d_c/util_sum)<<"\t"<<(ads_d_c/util_sum)<<"\n";
        f_normal<<"\n\n";
        
        cout<<"\n\nWeighted preemptions P-FPS: "<<(fps/util_sum)<<" Weighted preemptions RDS-FPS: "<<(rds/util_sum)<<" Weighted preemptions ADS-FPS: "<<(ads/util_sum);
        cout<<"\n\nWeighted preemptions P-FPS_d_c: "<<(fps_d_c/util_sum)<<" Weighted preemptions RDS-FPS_d_c: "<<(rds_d_c/util_sum)<<" Weighted preemptions ADS-FPS_d_c: "<<(ads_d_c/util_sum);        
        number_of_tasks+=2;
    }
    
    
    f_normal.close();
    f_weighted.close();
}

void varying_processors(int number_of_tasks, float MIN_UTIL, float MAX_UTIL, int MAX_TASKSETS_PER_SIMULATION, int MIN_PERIOD, int MAX_PERIOD, float npr_percentage, float MIN_NO_PROC, float MAX_NO_PROC, int MAX_TIME)
{

    srand (time(NULL));
    fstream f_weighted, f_normal;
    f_weighted.open("./results/weighted_results_proc.txt",ios::out);
    if(!f_weighted)
    {
        cout<<"\nError opening file: ./results/weighted_results_proc.txt";
        exit(1);
    }
    f_normal.open("./results/normal_results_proc.txt",ios::out);
    if(!f_normal)
    {
        cout<<"\nError opening file: ./results/normal_results_proc.txt";
        exit(1);
    }
    
/******************************CONTROL VARIABLES******************************/    
    int print_basic=0;
    int print_log=0;
    int print_inter=1;   
    int no_of_proc=MIN_NO_PROC;
    int DM=1;
    int DC=2;
    float DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
    if(DEADLINE_FRACTION>1.000)
        DEADLINE_FRACTION=1.0000;
/******************************CONTROL VARIABLES******************************/
    
    int no_of_preemptions_fps=0;
    int no_of_preemptions_rds=0;
    int no_of_preemptions_ads=0;
    
    int no_of_preemptions_fps_d_c=0;
    int no_of_preemptions_rds_d_c=0;
    int no_of_preemptions_ads_d_c=0;
    
    float fps=0.0000, rds=0.0000, ads=0.0000;
    float fps_d_c=0.0000, rds_d_c=0.0000, ads_d_c=0.0000;
    float util_sum=0.0000;
   

    while(no_of_proc<=MAX_NO_PROC){
        
        float cur_util=MIN_UTIL;
        float max_util=MAX_UTIL;
        
        fps=0.0000;
        rds=0.0000;
        ads=0.0000;
        
        fps_d_c=0.0000;
        rds_d_c=0.0000;
        ads_d_c=0.0000;      
        
        util_sum=0.0000;
        
        no_of_preemptions_fps=0;
        no_of_preemptions_rds=0;
        no_of_preemptions_ads=0;
        
        no_of_preemptions_fps_d_c=0;
        no_of_preemptions_rds_d_c=0;
        no_of_preemptions_ads_d_c=0;
      
        cout<<"\nNumber of processors: "<<no_of_proc<<"\n";  

        while(cur_util<=max_util){
            if(print_basic)
                cout<<"\nUtilization: "<<cur_util<<"\n";
            
            int counter=1;
            while(counter<=MAX_TASKSETS_PER_SIMULATION)
            {
                DEADLINE_FRACTION=((float) rand() / (RAND_MAX))+0.7;
                if(DEADLINE_FRACTION>1.0000)
                    DEADLINE_FRACTION=1.0000;
                
                int successful_parameter_generation=0;
                real_time_taskset *taskset=(real_time_taskset*) malloc(sizeof(real_time_taskset));
                if(print_log)
                    cout<<"\n\nCreating real-time tasks: ";
                create_tasks(number_of_tasks,taskset);  
                if(taskset==NULL)
                {
                    cout<<"\n*****************task set not created ! *****************";
                    exit(1);
                }
                else if(print_log)
                    cout<<"\nCreated task set :"<<counter<<"\n";
                
                while(!successful_parameter_generation)
                {                   
                    if(UUniFast(number_of_tasks,cur_util,taskset,DEADLINE_FRACTION, MAX_PERIOD, MIN_PERIOD)==1)
                    {
                        successful_parameter_generation=1; 
                        if(taskset==NULL)
                        {
                            cout<<"\n\n***************** parameters not generated ! *****************";
                            exit(1);
                        }
                        taskset=sort_task_set(taskset, DM);
                    }
                }      
/****************************************************************************************YOUR CODE GOES HERE****************************************************************************************/                              
                create_NPRS(taskset,no_of_proc,npr_percentage);

                float taskset_util=return_utilization(taskset);
                int preemptions_fps=0, preemptions_rds=0, preemptions_ads=0, count=0, count1=0, count2, count3, count4, count5,count6;               
                int preemptions_fps_d_c=0, preemptions_rds_d_c=0, preemptions_ads_d_c=0, count7=0, count8=0, count9;   
                
                util_sum+=taskset_util;
                count=count_tasks(taskset);
                if(count!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks generated (expected: "<<number_of_tasks<<" Actual: "<<count<<" )\n\n";
                    exit(1);
                           
                }
        
                preemptions_fps=g_fps_schedule(taskset, no_of_proc, MAX_TIME, print_log);
                no_of_preemptions_fps+=preemptions_fps;
                fps+=(taskset_util*preemptions_fps);
                
                count1=count_tasks(taskset);
                if(count1!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS (expected: "<<number_of_tasks<<" Actual: "<<count1<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds=rds_schedule_modified(taskset, no_of_proc, MAX_TIME, print_log);
                rds+=(taskset_util*preemptions_rds);
                no_of_preemptions_rds+=preemptions_rds;
                
                count2=count_tasks(taskset);
                if(count2!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS (expected: "<<count1<<" Actual: "<<count2<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads=ads_schedule(taskset, no_of_proc, MAX_TIME, print_log);
                ads+=(taskset_util*preemptions_ads);
                no_of_preemptions_ads+=preemptions_ads;
                
                count3=count_tasks(taskset);
                if(count3!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS (expected: "<<count2<<" Actual: "<<count3<<" )\n\n";
                    exit(1);       
                }
                
                
                taskset=sort_task_set(taskset, DC);
                preemptions_fps_d_c=g_fps_schedule(taskset, no_of_proc, MAX_TIME, print_log);
                no_of_preemptions_fps_d_c+=preemptions_fps_d_c;
                fps_d_c+=(taskset_util*preemptions_fps_d_c);
                
                count7=count_tasks(taskset);
                if(count7!=number_of_tasks)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after FPS (expected: "<<number_of_tasks<<" Actual: "<<count7<<" )\n\n";
                    exit(1);          
                }
                
                preemptions_rds_d_c=rds_schedule_modified(taskset, no_of_proc, MAX_TIME, print_log);
                rds_d_c+=(taskset_util*preemptions_rds_d_c);
                no_of_preemptions_rds_d_c+=preemptions_rds_d_c;
                
                count8=count_tasks(taskset);
                if(count8!=count1)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after RDS (expected: "<<count1<<" Actual: "<<count8<<" )\n\n";
                    exit(1);       
                }
                
                preemptions_ads_d_c=ads_schedule(taskset, no_of_proc, MAX_TIME, print_log);
                ads_d_c+=(taskset_util*preemptions_ads_d_c);
                no_of_preemptions_ads_d_c+=preemptions_ads_d_c;
                
                count9=count_tasks(taskset);
                if(count9!=count2)
                {
                    print_tasks(taskset);
                    cout<<"\n\n\tError in the number of tasks after ADS (expected: "<<count2<<" Actual: "<<count9<<" )\n\n";
                    exit(1);       
                }
                
                
/****************************************************************************************YOUR CODE GOES ABOVE****************************************************************************************/                
              
                delete_taskset(taskset,print_log); 
                counter++;
            }
            
            f_normal<<"\n"<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);
            f_normal<<"\t"<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION)<<"\t"<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);
              
            
            if(print_inter)
            {
                cout<<"\nUtilization: "<<cur_util;
                cout<<"\nPreemptions P-FPS: "<<(no_of_preemptions_fps/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS : "<<(no_of_preemptions_rds/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS : "<<(no_of_preemptions_ads/MAX_TASKSETS_PER_SIMULATION);

                cout<<"\nPreemptions P-FPS_d_c: "<<(no_of_preemptions_fps_d_c/MAX_TASKSETS_PER_SIMULATION)<<" Preemptions RDS-FPS_d_c: "<<(no_of_preemptions_rds_d_c/MAX_TASKSETS_PER_SIMULATION);
                cout<<" Preemptions ADS-FPS_d_c: "<<(no_of_preemptions_ads_d_c/MAX_TASKSETS_PER_SIMULATION);
            }
            
            cur_util=cur_util+0.1;
        }

        f_weighted<<(fps/util_sum)<<"\t"<<(rds/util_sum)<<"\t"<<(ads/util_sum)<<"\t"<<(fps_d_c/util_sum)<<"\t"<<(rds_d_c/util_sum)<<"\t"<<(ads_d_c/util_sum)<<"\n";
        f_normal<<"\n\n";
                
        cout<<"\n\nWeighted preemptions P-FPS: "<<(fps/util_sum)<<" Weighted preemptions RDS-FPS: "<<(rds/util_sum)<<" Weighted preemptions ADS-FPS: "<<(ads/util_sum);
        cout<<"\n\nWeighted preemptions P-FPS_d_c: "<<(fps_d_c/util_sum)<<" Weighted preemptions RDS-FPS_d_c: "<<(rds_d_c/util_sum)<<" Weighted preemptions ADS-FPS_d_c: "<<(ads_d_c/util_sum);
      
        no_of_proc+=2;
    }
    
}