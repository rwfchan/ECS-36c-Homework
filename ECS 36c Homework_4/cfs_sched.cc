#include <algorithm>
#include <iostream>
#include <fstream>

#include "llrb_multimap.h"

class Schedule {
 public:
  int extractFile(const std::string &fileName);
  bool TaskListIsEmpty();
  void SearchForUnlaunchTask();
  void CheckOwnershipForProcessRescource();
  void CheckCurrentTaskIsNull();
  void ProcessCurTask();
  void PrintAndIncrementGT();

 private:
  struct Task {
    const char identifier;
    const unsigned int startTime;
    unsigned int duration;
    unsigned int vruntime;
  };

  friend std::ostream& operator <<(std::ostream &os, Schedule &s);

  bool IsCurTaskComplete();

  // Citation for ComparePtrToTask() :
  // https://stackoverflow.com/questions/7446743/sorting-vector-of-pointers/7446778
  static bool ComparePtrToTask(Task *lhs, Task *rhs) {
    return lhs->identifier < rhs->identifier;
  }

  unsigned int min_vruntime = 0;
  unsigned int global_ticks = 0;
  unsigned int tasks_processing = 0;
  LLRB_multimap<unsigned int, Task*> timeline;
  std::vector<Task*> task_list;
  Task *current_task = nullptr;
};


// Extracts file contents and creates Task objects with each line.
// Returns -1 if file cannot be open
// Returns 0 if file successfully read
int Schedule::extractFile(const std::string &fileName) {
  std::ifstream myfile(fileName);
  if (myfile.fail()) {
    std::cerr << "Error: cannot open file " << fileName << std::endl;
    return -1;
  }

  while (!myfile.eof()) {
    char identifier;
    unsigned int startTime, duration;

    myfile >> identifier >> startTime >> duration;

    // Invalid input format or EOF
    if (myfile.fail() || myfile.eof()) {
      std::sort(task_list.begin(), task_list.end(), ComparePtrToTask);
      myfile.close();
      return 0;
    }

    Task *object = new Task{identifier, startTime, duration};
    task_list.push_back(object);
  }

  return 0;
}

bool Schedule::TaskListIsEmpty() {
  return task_list.empty();
}

// Checks if there are tasks that should be launched at
// the current tick value. If so, insert to timeline.
void Schedule::SearchForUnlaunchTask() {
  for (auto &t : task_list) {
    if (t->startTime == global_ticks) {
      t->vruntime = min_vruntime;
      timeline.Insert(t->vruntime, t);
      tasks_processing++;
    }
  }
}

// "Check if the currently running task should let the
// processing resource to the next task" (from HW4 instructions)
void Schedule::CheckOwnershipForProcessRescource() {
  if (current_task) {
    if (current_task->vruntime > min_vruntime) {
      timeline.Insert(current_task->vruntime, current_task);
      current_task = nullptr;
    }
  }
}

// "There is no more current task, get the next
// schedulable task from the timeline." (from HW4 instructions)
void Schedule::CheckCurrentTaskIsNull() {
  if (!current_task) {
    if (!timeline.Size()) return;
    current_task = timeline.Get(timeline.Min());
    timeline.Remove(timeline.Min());
    if (!timeline.Size())
      min_vruntime = 0;
    else
      min_vruntime = timeline.Get(timeline.Min())->vruntime;
  }
}

void Schedule::ProcessCurTask() {
  if (!current_task) return;
  current_task->duration--;
  current_task->vruntime++;
}

// Displays scheduling status and updates the current tick
void Schedule::PrintAndIncrementGT() {
  std::cout << global_ticks << " [" << tasks_processing << "]: ";
  if (!current_task) {
    std::cout << '_';
  } else {
    std::cout << current_task->identifier;
    if (IsCurTaskComplete()) {
      std::cout << '*';
    }
  }
  std::cout << std::endl;
  global_ticks++;
}

bool Schedule::IsCurTaskComplete() {
  if (!current_task->duration) {
    for (auto itr = task_list.begin(); itr != task_list.end(); itr++) {
      if (*itr == current_task) {
        task_list.erase(itr);
        delete current_task;
        current_task = nullptr;
        tasks_processing--;
        return true;
      }
    }
  }
  return false;
}

std::ostream& operator <<(std::ostream &os, Schedule &s) {
  for (auto &t : s.task_list) {
    os << t->identifier << ' ' << t->startTime << ' ' << t->duration;
  }
  return os;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <task_file.dat>" << std::endl;
    return 1;
  }

  Schedule schedule;

  if (schedule.extractFile(argv[1]) == -1)
    return 1;

  while (!schedule.TaskListIsEmpty()) {
    schedule.SearchForUnlaunchTask();
    schedule.CheckOwnershipForProcessRescource();
    schedule.CheckCurrentTaskIsNull();
    schedule.ProcessCurTask();
    schedule.PrintAndIncrementGT();
  }

  return 0;
}
