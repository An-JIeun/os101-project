#include "projects/automated_warehouse/robot.h"
#include "projects/automated_warehouse/aw_message.h"
/**
 * A function setting up robot structure
 */
void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload){
    _robot->name = name;
    _robot->row = row;
    _robot->col = col;
    _robot->required_payload = required_payload;
    _robot->current_payload = current_payload;
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
                    if (boxes_from_central_control_node[mailboxNumber].msg == 1){
                        _robot->row = updated;
                        if ()
                        setRobot(&robots[mailboxNumber+1],mailboxNumber+1,updated, _robot->col, _robot->name, _robot->required_payload, _robot->current_payload);
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