#ifndef FAILURE_DETECTOR_H
#define FAILURE_DETECTOR_H

#include<stdio.h>

#include"common.h"
#include"group.h"
#include"node.h"

// After 150ms without any sign of living from
// a node it is considered as dead.
// The time bellow is in µsec
#define TIME_DEATH 150000  

// The failure detector weak every 10ms to check the if
// we have received any signs of living from processes
// The time bellow is in nsec
#define WAKE_TIME 10000000

void init_P();
void* run_P();

#endif
