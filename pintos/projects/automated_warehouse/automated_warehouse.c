#include <stdio.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

#include "projects/automated_warehouse/aw_manager.h"

struct robot* robots;

// test code for central control node thread
void test_cnt(){
        while(1){
                print_map(robots, 4);
                thread_sleep(1000);
                block_thread();
        }
}

// test code for robot thread
void test_thread(void* aux){
        int idx = *((int *)aux);
        int test = 0;
        while(1){
                printf("thread %d : %d\n", idx, test++);
                printf("fuxxxx");
                thread_sleep(idx * 1000);
        }
}

// entry point of simulator
void run_automated_warehouse(char **argv)
{
        init_automated_warehouse(argv); // do not remove this

        printf("implement automated warehouse!\n");

        // modified code >> parsing input arguments
        int robot_num = argv[1];
        printf("Number of Robots : %d",robot_num);

        // define central robot
        robots = malloc(sizeof(struct robot) * robot_num+1);
        setRobot(&robots[0], 5,6, "RO", 0, 0); // central robot

        // define thread index list
        int idxs[robot_num+1];
        idxs[0] = 0;

        // create threads list
        tid_t* threads = malloc(sizeof(tid_t) * robot_num+1);
        threads[0] = thread_create("CNT", 0, &test_cnt, NULL);
        
        // modified code >> parse the locations where robots get cargos and transfer to
        char* input_s =  strtok(argv[2], ":");
        int robot_index = 0; // index increamented in while loop
        char * robot_name; // name of robot, e.g. "R1"
        while(input_s != NULL){

                char *parsed_data = input_s;
                int req_payload = atoi(parsed_data[0]);
                idxs[robot_index+1] = robot_index+1;
                
                sprintf(robot_name, "R%d",robot_index+1);
                threads[robot_index+1] = thread_create(robot_name, 0, &test_thread, &idxs[robot_index+1]);
                setRobot(&robots[robot_index+1], 5,6, robot_name, req_payload, 0);
                input_s = strtok(NULL,":");
                robot_index++;
        }


        // example of create thread
        //tid_t* threads = malloc(sizeof(tid_t) * 4);
        //int idxs[4] = {1, 2, 3, 4};
        //threads[0] = thread_create("CNT", 0, &test_cnt, NULL);
        //threads[1] = thread_create("R1", 0, &test_thread, &idxs[1]);
        //threads[2] = thread_create("R2", 0, &test_thread, &idxs[2]);
        //threads[3] = thread_create("R3", 0, &test_thread, &idxs[3]);

        // if you want, you can use main thread as a central control node
        
}
