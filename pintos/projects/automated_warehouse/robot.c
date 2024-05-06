#include "projects/automated_warehouse/robot.h"
#include "projects/automated_warehouse/aw_message.h"
/**
 * A function setting up robot structure
 */
void setRobot(struct robot* _robot, const char* name, int idx, int row, int col, int required_payload, int current_payload){
    _robot->name = name;
    _robot->idx = idx;
    _robot->row = row;
    _robot->col = col;
    _robot->required_payload = required_payload;
    _robot->current_payload = current_payload;
}