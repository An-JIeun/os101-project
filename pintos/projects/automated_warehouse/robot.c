#include "projects/automated_warehouse/robot.h"
#include "projects/automated_warehouse/aw_message.h"

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
/**
 * A function setting up robot structure
 */
void setRobot(struct robot* _robot, const char* name, int idx, int row, int col, int required_payload, int current_payload){
    _robot->name = name;
    _robot->idx = idx;
    _robot->row = row;
    _robot->col = col;
    _robot->targ_row=0;
    _robot->targ_col=0;
    _robot->moving = 1;
   // sema_init(_robot, 0);
    _robot->required_payload = required_payload;
    _robot->current_payload = current_payload;
}