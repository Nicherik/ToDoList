//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_TASKLIST_HPP
#define TODOLIST_TASKLIST_HPP



#include <vector>
#include "Task.hpp"
#include <set>
#include <optional>


class TaskList {

    vector<Task> vtask;

public:

    void addTask(Task&);
    void removeTask(int id);
    void removeAllTasks();
    [[nodiscard]] const Task& getTask(int id) const;
    [[nodiscard]] const vector<Task>& getAllTasks() const;
    [[nodiscard]] int getTasksAmount() const;
    void updateTask(int id, optional<string> _title,
                    optional <Status> _status, optional < string> _description,
                    optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
                    optional < set<string> > _tags);

    vector<Task> getTaskListByStatus(Status);
    vector<Task> getTaskListByTitle(const string & _title);
    vector<Task> getTaskListByPriority(int priority);
    vector<Task> getTaskListByTag(const string& tag);
    vector<Task> getTaskListByDeadline(const chrono::time_point<chrono::system_clock> & _deadline);
    static void sortById(vector<Task>&);
    void sortByDeadlineAscending();
    void sortByDeadlineDescending();
    void sortByTitleAscending();
    void sortByTitleDescending();
    void sortByStatusAsc();
    void sortByStatusDesc();
    void sortByPriorityAsc();
    void sortByPriorityDesc();
    void sortByAmountTagsAsc();
    void sortByAmountTagsDesc();
    void addAllTasks(const vector<Task>& _vtsk);
};
#endif //TODOLIST_TASKLIST_HPP