#ifndef __PROJECTS_AUTOMATED_WAREHOUSE_H__
#define __PROJECTS_AUTOMATED_WAREHOUSE_H__
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
extern struct robot* robots;
struct lock filesys_lock;
extern struct messsage_box* boxes_from_central_control_node;
extern struct messsage_box* boxes_from_robots;
void run_automated_warehouse(char **argv);

#endif 
