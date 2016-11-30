/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fps_tests.h
 * Author: abhilash-mdh
 *
 * Created on October 13, 2016, 5:50 PM
 */

#ifndef FPS_TESTS_H
#define FPS_TESTS_H

int quick_test_RTA_FPS(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);
int bertogna_chirinei_lipari(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);
int p_fps_test_guan(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);
int lazy_lp_fps_test_linkbased(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);
int lazy_lp_fps_rta(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);
int eager_lp_fps_rta(real_time_taskset *taskset, float number_of_proc, int print_log, int print_result);


#endif /* FPS_TESTS_H */

