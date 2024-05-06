#include "projects/automated_warehouse/aw_message.h"

void setMailbox(int idx, int bit, int cmd, int row, int col, int cur_payload, int req_payload){
        boxes_from_robots[idx].dirtyBit = bit;
        boxes_from_robots[idx].msg.cmd = cmd;
        boxes_from_robots[idx].msg.row = row;
        boxes_from_robots[idx].msg.col = col;
        boxes_from_robots[idx].msg.current_payload = cur_payload;
        boxes_from_robots[idx].msg.required_payload = req_payload;
}