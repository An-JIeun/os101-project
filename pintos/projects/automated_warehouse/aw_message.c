#include "projects/automated_warehouse/aw_message.h"

void setMailbox(int idx, int bit, int cmd, int row, int col, int cur_payload, int req_payload){
        struct message new_msg;
        new_msg.cmd = cmd;
        new_msg.col = col;
        new_msg.row = row;
        new_msg.current_payload = cur_payload;
        new_msg.required_payload = req_payload;
        boxes_from_robots[idx].dirtyBit = bit;
        boxes_from_robots[idx].msg = new_msg;
        boxes_from_robots[idx].idx = idx;
        
}