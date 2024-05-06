#include "projects/automated_warehouse/robot.h"

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

// function for moving the robot
void movingRobot(struct robot* _robot, int targ_row, int targ_col){
    const int TopRow = 0;
    const int BottomRow = 5;
    const int LeftCol = 0;
    const int RightRow = 6;
    int direction = rand() % 4; // 0 : up, 1 :right, 2: down, 3: left

    while (_robot->col != targ_col || _robot->row != targ_row){
        switch(direction){
            case 0: // up
                if (_robot->row != targ_row || _robot-> row != TopRow){
                    int curr = _robot->row;
                    int updated = curr--;
                    _robot->row = updated;
                }
                break;
            case 1: // down
                if (_robot->row != targ_row || _robot-> row != BottomRow){
                    int curr = _robot->row;
                    int updated = curr++;
                    _robot->row = updated;
                }
        }
    }


}