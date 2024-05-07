#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"
#include "projects/automated_warehouse/automated_warehouse.h"
#include "projects/automated_warehouse/aw_manager.h"
#include "projects/automated_warehouse/aw_message.h"
#include "projects/automated_warehouse/robot.h"
#include "projects/automated_warehouse/aw_thread.h"
struct condition value_updated;
struct robot *robots;
struct messsage_box *boxes_from_central_control_node;
struct messsage_box *boxes_from_robots;
struct list * blocked_threads;
void *test_cnt(void* aux){
        lock_acquire(&filesys_lock);
        // Critical section
        lock_release(&filesys_lock);
        printf("RELEASED\n");
        struct robot* robots = (struct robot*)aux;
        int robot_length = sizeof(robots)/sizeof(struct robot);
        while(1){
                print_map(robots, robot_length);
                centralControl();
        }
}
int random(void) {

    int ticks = timer_ticks();

    return ticks % 37;
}
// test code for robot thread

void *test_thread(void* aux){
        lock_acquire(&filesys_lock);
        printf("==locked==\n");
     
        lock_release(&filesys_lock);
        printf("==unLocked==\n");
        
        struct robot* robot = (struct robot*)aux;
        int ind;
        ind = robot->idx;
        printf("thread index : %d\n",ind);
        movingRobot(robot);
        
}

// mathh target ending point
void targ_point(char point, struct robot* _robot){
        switch(point){
                case 'A' :
                        _robot->targ_row = 0;
                        _robot->targ_col = 2;
                        break;
                case 'B':
                        _robot->targ_row = 2;
                        _robot->targ_col = 0;
                        break;
                case 'C':
                        _robot->targ_row = 5;
                        _robot->targ_col = 2;
                        break;

        }
}

// payload loc mapper
int payload_loc_mapper(int row, int col){
        int load;
        if (row == 1 && col == 1){
                load = 1;            
        }
        if (row == 1 && col == 3){
                load = 2;
        }
        if (row == 1 && col == 4){
                load = 3;
        }
        if (row == 1 && col== 5){
               load = 4; 
        }
        if (row == 4 && col == 1){
                load = 5;
        }
        if (row == 4 && col == 3){
                load = 6;
        }
        if (row == 4 && col == 4){
                load = 7;
        }
        return load;
}
int * payload_loc_mapper_r(int p_num){
        int *colrow = malloc(sizeof(int) * 2);;
        switch(p_num){
                case 1 :
                        colrow[0] = 1;
                        colrow[1] = 1;
                    break;
                case 2:
                        colrow[0] = 1;
                        colrow[1] = 3;
                    break;
                case 3:
                        colrow[0] = 1;
                        colrow[1] = 4;
                    break;
                case 4:
                        colrow[0] = 1;
                        colrow[1] = 5;
                    break;
                case 5:
                        colrow[0] = 4;
                        colrow[1] = 1;
                    break;
                case 6:
                        colrow[0] = 4;
                        colrow[1] = 3;
                    break;
                case 7:
                        colrow[0] = 4;
                        colrow[1] = 4;
                    break;
        }
        return colrow;
}


// entry point of simulator
void run_automated_warehouse(char **argv)
{
        init_automated_warehouse(argv); // do not remove this
        //list_init(blocked_threads);
        lock_init(&filesys_lock);
        cond_init(&value_updated);
        //init_list(&blocked_threads);
        //lock_acquire(&filesys_lock);
        //lock_release(&filesys_lock);
        printf("implement automated warehouse!\n");

        // modified code >> parsing input arguments
        int robot_num = atoi(argv[1]);
        printf("Number of Robots : %d\n",robot_num);

        // define central robot
        robots = malloc(sizeof(struct robot) * robot_num+1);
        setRobot(&robots[0], 5,6, "RO",0, 0, 0); // central robot

        // define thread index list
        int idxs[robot_num+1];
        idxs[0] = 0;
        
        // create threads list
        tid_t* threads = malloc(sizeof(tid_t) * robot_num+1);
        boxes_from_central_control_node = malloc(sizeof(struct messsage_box) * robot_num+1);
        boxes_from_robots= malloc(sizeof(struct messsage_box) * robot_num+1);
        
        //struct robot* robots;
        // modified code >> parse the locations where robots get cargos and transfer to
        char* input_s;
        char* next_tok;
        input_s = strtok_r(argv[2],":", &next_tok);
        int robot_index = 0; // index increamented in while loop
        threads[0] = thread_create("CNT", 0, &test_cnt, &robots);
         // name of robot, e.g. "R1"
        while(input_s != NULL){

                while(robot_index < robot_num){
                        printf("\nnow index : %d\n",robot_index+1);
                        char * robot_name = malloc(10);
                        char *parsed_data = input_s;
                        int req_payload = atoi(&parsed_data[0]);
                        char targ_p = parsed_data[1];
                        int *thread_idx = malloc(sizeof(int));
                        
                        idxs[robot_index+1] = robot_index+1;
                        *thread_idx = idxs[robot_index + 1];

                        snprintf(robot_name, 10, "R%d\n",robot_index+1);
                     
                        setRobot(&robots[robot_index+1],robot_name,robot_index+1, 5,6, req_payload, 0);
                        printf("TARGET POINT : %c | REQ PAYLOAD : %d\n", targ_p, req_payload);
                        targ_point(targ_p, &robots[robot_index+1]);
                        threads[robot_index+1] = thread_create(robot_name, 0, &test_thread, &robots[robot_index+1]); 
                        
                        // mesage box setting
                        boxes_from_robots[robot_index].dirtyBit = 0;
                        boxes_from_robots[robot_index].msg.cmd = 0;
                        boxes_from_robots[robot_index].msg.row = 5;
                        boxes_from_robots[robot_index].msg.col = 6;
                        boxes_from_robots[robot_index].msg.current_payload = 0;
                        boxes_from_robots[robot_index].msg.required_payload = req_payload;
                        // input string parsing
                        input_s = strtok_r(NULL, ":", &next_tok);
                        robot_index++;
                }

        }
         


        // example of create thread
        //tid_t* threads = malloc(sizeof(tid_t) * 4);
        //int idxs[4] = {1, 2, 3, 4}code >> parsing input arguments
        //int robot_num;
        //threads[0] = thread_create("CNT", 0, &test_cnt, NULL);
        //threads[1] = thread_create("R1", 0, &test_thread, &idxs[1]);
        //threads[2] = thread_create("R2", 0, &test_thread, &idxs[2]);
        //threads[3] = thread_create("R3", 0, &test_thread, &idxs[3]);

        // if you want, you can use main thread as a central control node
        
}



//function for cetal robot
void centralControl(){
    
    while(1){
    boxes_from_central_control_node = boxes_from_robots;
    int size = sizeof(boxes_from_robots);
    printf("\n<< initial robot >>\n");
    //unblock_threads();
    while (true) {
        struct thread *current_thread = thread_current();
         //unblock_threads();
        if (current_thread == NULL) {
                printf("No thread is currently running. Starting a new thread...\n");
                unblock_threads();
        }
        }
    }
    }

    
    
  
    


// function for moving the robot
void movingRobot(struct robot* _robot){
    const int TopRow = 0;
    const int BottomRow = 5;
    const int LeftCol = 0;
    const int RightCol = 6;
    int targ_row = _robot->targ_row;
    int targ_col = _robot->targ_col;

    int mailboxNumber = _robot->idx;
    int direction;
    printf("Now Thread %d | Target Row : %d | Target Column : %d\n", mailboxNumber, targ_row, targ_col );
    lock_acquire(&filesys_lock);
    

    while (true){
        if (_robot->col == targ_col && _robot->row == targ_row){
                if (_robot->current_payload == _robot->required_payload){
                        printf("\n\n=== TASK DONE ===\n\n");
                        break;
                }
        }
        if (_robot->moving == 0){
                printf("THREAD BLOCK >> \n");
                block_thread();
                thread_yield();
                unblock_threads();
        }
       
        direction = random()%4;
        printf("\n>>> Now Thread %d\n", mailboxNumber );
        printf("now => %d\n", direction); 
        //printf("now State : row # %d col # %d\n\n", _robot->row,_robot->col);
        
        switch(direction){
            case 0: // up
                if (_robot->row != targ_row || _robot->row > TopRow+1){
                    int curr = _robot->row;
                    int updated = curr-1;
                    if (updated > 0 && updated < 5){
                    setMailbox(mailboxNumber, 0, 1, updated, _robot->col,_robot->current_payload, _robot->required_payload);
            
                    printf("now cmd : %d\n",boxes_from_central_control_node[mailboxNumber].msg.cmd);
                    if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                    printf("LOCKED\n");
                    lock_acquire(&filesys_lock);
                    while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                        cond_wait(&value_updated, &filesys_lock);
                        }
                        lock_release(&filesys_lock);}
                    while(1){
                    
                        if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 1){
                                printf("into loop\n");
                                _robot->row = updated;
                                int now_loc = payload_loc_mapper(updated,_robot->col);
                                printf("now State : row # %d col # %d\n\n", _robot->row,_robot->col);
                                if (now_loc == _robot->required_payload && _robot->current_payload != _robot->required_payload){
                                        printf("==== GET PAYLOAD ====\n");
                                        _robot->current_payload = _robot->required_payload;
                                        _robot->moving=0;
                                       break;
                        
                                }
                                
                                boxes_from_central_control_node[mailboxNumber].msg.cmd = 0;
                                break;
                                //block_thread();
                        }
                        else if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                                
                                //block_thread();
                                lock_acquire(&filesys_lock);
                                while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                                        cond_wait(&value_updated, &filesys_lock);
                                }
                                printf("unblocked\n");
                                //unblock_threads();
                                lock_release(&filesys_lock);
                                
                        } 
                    }}
                    else{
                        break;
                    }
                }
                break;
            case 1: // down
                if (_robot->row != targ_row || _robot->row < BottomRow-1){
                    int curr = _robot->row;
                    int updated = curr+1;
                    if (updated > 0 && updated < 5){
                    setMailbox(mailboxNumber, 0, 1, updated, _robot->col,_robot->current_payload, _robot->required_payload);
                    printf("set up\n");
                    printf("now cmd : %d\n",boxes_from_central_control_node[mailboxNumber].msg.cmd);
                    if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                    printf("LOCKED\n");
                    lock_acquire(&filesys_lock);
                    while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                        cond_wait(&value_updated, &filesys_lock);
                        }
                        lock_release(&filesys_lock);}
                    while(1){
                        
                        if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 1){
                                printf("into loop\n");
                                _robot->row = updated;
                                int now_loc = payload_loc_mapper(updated,_robot->col);
                                printf("now State : row # %d col # %d\n\n", _robot->row,_robot->col);
                                if (now_loc == _robot->required_payload && _robot->current_payload != _robot->required_payload){
                                        printf("==== GET PAYLOAD ====\n");
                                        _robot->current_payload = _robot->required_payload;
                                        _robot->moving=0;
                                        break;
                        
                                }
                               
                                
                                boxes_from_central_control_node[mailboxNumber].msg.cmd = 0;
                                break;
                        }
                        else if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                                //block_thread();
                                lock_acquire(&filesys_lock);
                                while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                                        cond_wait(&value_updated, &filesys_lock);
                                }
                                printf("unblocked\n");
                                //unblock_threads();
                                lock_release(&filesys_lock);
                                
                        } 
                    } }
                    else{
                       break;
                    }
                    
                }
                break;
            case 2: // left
                if (_robot->col != targ_col || _robot->col > LeftCol+1){
                    int curr = _robot->col;
                    int updated = curr-1;
                    if (updated > 0 && updated < 6){
                    setMailbox(mailboxNumber, 0, 1, _robot->row, updated,_robot->current_payload, _robot->required_payload);
                    printf("set up\n");
                    printf("now cmd : %d\n",boxes_from_central_control_node[mailboxNumber].msg.cmd);
                    if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                    printf("LOCKED\n");
                    lock_acquire(&filesys_lock);
                    while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                        cond_wait(&value_updated, &filesys_lock);
                        }
                        lock_release(&filesys_lock);}
                    while(1){
                       
                        if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 1){
                                printf("into loop\n");
                                _robot->col = updated;
                                int now_loc = payload_loc_mapper(_robot->row,updated);
                                printf("now State : row # %d col # %d\n\n", _robot->row,_robot->col);
                
                                if (now_loc == _robot->required_payload && _robot->current_payload != _robot->required_payload){
                                        printf("==== GET PAYLOAD ====\n");
                                        _robot->current_payload = _robot->required_payload;
                                        _robot->moving=0;
                                        break;
                                }
                                
                              
                                boxes_from_central_control_node[mailboxNumber].msg.cmd = 0;
                                break;
                        }
                        else if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                                printf("==be blocked==\n");
                             
                                //block_thread();
                                lock_acquire(&filesys_lock);
                                while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                                        cond_wait(&value_updated, &filesys_lock);
                                }
                                printf("unblocked\n");
                                //unblock_threads();
                                lock_release(&filesys_lock);
                                
                        } 
                    }}
                    else{
                        break;
                    }
                }
                break;
            case 3: // right
                if (_robot->col != targ_col || _robot->col < RightCol-1){
                    int curr = _robot->col;
                    int updated = curr+1;
                    if (updated > 0 && updated < 6){
                    setMailbox(mailboxNumber, 0, 1, _robot->row, updated,_robot->current_payload, _robot->required_payload);
                    printf("set up\n");
                    printf("now cmd : %d\n",boxes_from_central_control_node[mailboxNumber].msg.cmd);
                    if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                    printf("LOCKED\n");
                    lock_acquire(&filesys_lock);
                    while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                        cond_wait(&value_updated, &filesys_lock);
                        }
                        lock_release(&filesys_lock);}
                    while(1){
                        
                        if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 1){
                                printf("into loop\n");
                                _robot->col = updated;
                                int now_loc = payload_loc_mapper(_robot->row,updated);
                                printf("now State : row # %d col # %d\n\n", _robot->row,_robot->col);
                                if (now_loc == _robot->required_payload && _robot->current_payload != _robot->required_payload){
                                        printf("==== GET PAYLOAD ====\n");
                                        _robot->current_payload = _robot->required_payload;
                                        _robot->moving=0;
                                        break;
                        
                                }
                                
                                boxes_from_central_control_node[mailboxNumber].msg.cmd = 0;
                                
                                break;
                        }
                        else if (boxes_from_central_control_node[mailboxNumber].msg.cmd == 0){
                                printf("==be blocked==\n");
                               
                               //block_thread();
                                lock_acquire(&filesys_lock);
                                while(boxes_from_central_control_node[mailboxNumber].msg.cmd != 1){
                                        cond_wait(&value_updated, &filesys_lock);
                                }
                                printf("unblocked\n");
                                //unblock_threads();
                                lock_release(&filesys_lock);
                                
                    }
                }}
                else{
                        break;
                }
                break;

        }
    }


}}
