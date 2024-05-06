#ifndef _PROJECTS_PROJECT1_ROBOT_H__
#define _PROJECTS_PROJECT1_ROBOT_H__

/**
 * A Structure representing robot
 */
struct robot {
    const char* name;
    int row;
    int col;
    int required_payload;
    int current_payload; 
    int targ_col;
    int targ_row;
};

void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload);
// function for moving the robot
void movingRobot(struct robot* _robot, int targ_row, int targ_col);
#endif