#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    size_t currentDay,
    size_t currentAssignment,
    const AvailabilityMatrix& a,
    const size_t d,
    const size_t m,
    std::vector<int>& shiftsWorked,
    DailySchedule& sched
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // Initially, set all worker's shifts worked as 0
    std::vector<int> shiftsWorked(avail[0].size());

    // Initialize sched
    sched = DailySchedule(avail.size(), std::vector<Worker_T>(dailyNeed, INVALID_ID));

    return scheduleHelper(0, 0, avail, dailyNeed, maxShifts, shiftsWorked, sched);

}

bool scheduleHelper(
    size_t currentDay,
    size_t currentAssignment,
    const AvailabilityMatrix& a,
    const size_t d,
    const size_t m,
    std::vector<int>& shiftsWorked,
    DailySchedule& sched
){
    // If finished state and valid
    if(currentDay == a.size() && currentAssignment == 0){
        return true;
    }

    // Iterate through the workers
    for(size_t i=0; i<a[0].size(); i++){
        // Check for valid state: if the worker can work today,
        // has not been assigned to work this day yet,
        // and didn't exceed their max shifts worked
        bool workerNotAssigned = std::find(sched[currentDay].begin(), sched[currentDay].end(), i) == sched[currentDay].end();
        if(a[currentDay][i]==1 && shiftsWorked[i]<m && workerNotAssigned){
            // Update state
            shiftsWorked[i] = shiftsWorked[i] + 1;
            sched[currentDay][currentAssignment] = i;
            bool goToNextDay = false;
            if(currentAssignment==(d-1)){
                currentDay++;
                currentAssignment = 0;
                goToNextDay = true;
            } else {
                currentAssignment++;
            }
            // Recursive call with current state
            bool status = scheduleHelper(currentDay, currentAssignment, a, d, m, shiftsWorked, sched);
            if(status){ // Recursive call returned true
                return true;
            }
            // If recursive call returned false, backtrack
            shiftsWorked[i] = shiftsWorked[i] - 1;
            sched[currentDay][currentAssignment] = INVALID_ID;
            if(goToNextDay){
                currentDay--;
                currentAssignment = (d-1);
            } else {
                currentAssignment--;
            }
        }
    }

    // No viable solution found
    return false;
}
