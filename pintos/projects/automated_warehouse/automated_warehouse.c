#include <stdio.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

#include "projects/automated_warehouse/aw_manager.h"
#include "projects/automated_warehouse/aw_message.h"
#include "projects/automated_warehouse/robot.h"
#include "projects/automated_warehouse/aw_thread.h"

struct robot* robots;

// test code for central control node thread
void *test_cnt(){
        while(1){
                print_map(robots, 4);
                thread_sleep(1000);
                block_thread();
        }
}

// test code for robot thread

void *test_thread(void* aux){
        int idx = *((int *)aux);
        
        printf("thread index : %d\n",idx);
        int test = 0;
        while(1){
                printf("\nthread %d : %d\n", idx, test++);
                
                thread_sleep(idx * 1000);
                printf("\nsleep end\n");
        }
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
                case 'C':
                        _robot->targ_row = 5;
                        _robot->targ_col = 2;

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
int payload_loc_mapper(int p_num){
        int *colrow;
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
        boxes_from_central_control_node->dirtyBit = 0;


        // create threads list
        tid_t* threads = malloc(sizeof(tid_t) * robot_num+1);
       
        
        // modified code >> parse the locations where robots get cargos and transfer to
        char* input_s;
        char* next_tok;
        input_s = strtok_r(argv[2],":", &next_tok);
        int robot_index = 0; // index increamented in while loop
        
         // name of robot, e.g. "R1"
        while(input_s != NULL){

                while(robot_index < robot_num){
                        printf("\nnow index : %d\n",robot_index+1);
                        char * robot_name = malloc(10);
                        char *parsed_data = input_s;
                        int req_payload = atoi(&parsed_data[0]);
                        char targ_p = &parsed_data[1];
                        int *thread_idx = malloc(sizeof(int));
                        
                        idxs[robot_index+1] = robot_index+1;
                        *thread_idx = idxs[robot_index + 1];

                        snprintf(robot_name, 10, "R%d\n",robot_index+1);
                        threads[robot_index+1] = thread_create(robot_name, 0, &test_thread, (void *)thread_idx); 
                        setRobot(&robots[robot_index+1],robot_index+1, 5,6, robot_name, req_payload, 0);
                        targ_point(targ_p, &robots[robot_index+1]);
                        
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
         threads[0] = thread_create("CNT", 0, &test_cnt, NULL);


        // example of create thread
        //tid_t* threads = malloc(sizeof(tid_t) * 4);
        //int idxs[4] = {1, 2, 3, 4};
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

    for(int i=0;i < size;i++){
        struct messsage_box item;
        item = boxes_from_central_control_node[i];
        if (i!=0){
            for (int j=0 ;j < i;j++ ){
                struct messsage_box comp;
                comp = boxes_from_central_control_node[j];
                if (item.msg.row == comp.msg.row){
                    item.msg.cmd = 0;
                }
            }
        }
        
    }
    
    }
}

// function for moving the robot
void movingRobot(struct robot* _robot, int targ_row, int targ_col){
    const int TopRow = 0;
    const int BottomRow = 5;
    const int LeftCol = 0;
    const int RightCol = 6;

    int mailboxNumber = _robot->idx;
    int direction;

    while (_robot->col != targ_col || _robot->row != targ_row){
        direction = rand() % 4; 
        switch(direction){
            case 0: // up
                if (_robot->row != targ_row || _robot->row > TopRow+1){
                    int curr = _robot->row;
                    int updated = curr--;
                    setMailbox(mailboxNumber, 0, 1, updated, _robot->col,_robot->current_payload, _robot->required_payload);
                    thread_sleep(500);

                    if (&boxes_from_central_control_node[mailboxNumber].msg == 1){
                        _robot->row = updated;
                        int now_loc = payload_loc_mapper(updated,_robot->col);
                        if (now_loc == _robot->required_payload){
                            setRobot(_robot, updated, mailboxNumber+1,_robot->col, _robot->name, _robot->required_payload, _robot->required_payload);
                        }
                        else{
                            setRobot(_robot, updated,mailboxNumber+1,_robot->col, _robot->name, _robot->required_payload, _robot->current_payload);
                        }
                    }
                    else if (&boxes_from_central_control_node[mailboxNumber].msg == 0){
                        block_thread();
                    }
                    
                }
                break;
            case 1: // down
                if (_robot->row != targ_row || _robot->row < BottomRow-1){
                    int curr = _robot->row;
                    int updated = curr++;
                    setMailbox(mailboxNumber, 0, 1, updated, _robot->col,_robot->current_payload, _robot->required_payload);
                    _robot->row = updated;
                }
                break;
            case 2: // left
                if (_robot->col != targ_col || _robot->col > LeftCol+1){
                    int curr = _robot->col;
                    int updated = curr--;
                    setMailbox(mailboxNumber, 0, 1, _robot->row, updated,_robot->current_payload, _robot->required_payload);
                    _robot->col = updated;
                }
                break;
            case 3: // right
                if (_robot->col != targ_col || _robot->col < RightCol-1){
                    int curr = _robot->col;
                    int updated = curr++;
                    setMailbox(mailboxNumber, 0, 1, _robot->row, updated,_robot->current_payload, _robot->required_payload);
                    _robot->col = updated;
                }
                break;

        }
    }


}
