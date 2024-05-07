#ifndef _PROJECTS_PROJECT1_ROBOT_H__
#define _PROJECTS_PROJECT1_ROBOT_H__

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
/**
 * A Structure representing robot
 */
struct robot {
    const char* name;
    int idx;
    int row;
    int col;
    int required_payload;
    int current_payload; 
    int targ_col;
    int targ_row;
    int moving;
    //struct semaphore *sema; 
};

void setRobot(struct robot* _robot, const char* name,int idx, int row, int col, int required_payload, int current_payload);
// function for moving the robot

#endif