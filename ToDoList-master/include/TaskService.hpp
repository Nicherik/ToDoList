//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_TASKSERVICE_HPP
#define TODOLIST_TASKSERVICE_HPP



#include "ITaskStorage.hpp"
#include "Task.hpp"
#include "TaskList.hpp"
#include <optional>
#include <memory>
#include "SortType.hpp"
#include "GetType.hpp"
class TaskService {


    iTaskStorage& storage;
    unique_ptr<TaskList> lst=make_unique<TaskList>();

public:
    explicit TaskService(iTaskStorage& _storage);
    [[nodiscard]] bool addTask(string _title, optional <Status> _status, optional < string> _description,
                 optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
                 optional < set<string> > _tags);
    [[nodiscard]] bool addTask(int id, string _title, optional <Status> _status, optional < string> _description,
                               optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
                               optional < set<string> > _tags);

    [[nodiscard]] bool updateTask(int task_id, optional<string> new_title, optional <Status> new_status, optional < string> new_description,
                    optional <int> new_priority,
                    optional < chrono::time_point<chrono::system_clock> > new_deadline, optional < set<string> > new_tags);

    [[nodiscard]] bool removeTask(int id);

    [[nodiscard]] bool removeAllTasks();

    const Task& getTaskById(int id);

    TaskList& getAllTasks();

    [[nodiscard]] bool sortBy(const SortType& _type);

    vector<Task> getBy(GetType type1 , const string &parameter);
    vector<Task> getBy(Status status);
    vector<Task> getBy(int priority);
    vector<Task> getBy(chrono::time_point<chrono::system_clock> deadline);

    [[nodiscard]] bool loadTask();

    [[nodiscard]] bool saveTasks();

    void allTasksToString();

    int getTasKAmount();

    [[nodiscard]] bool deleteTaskFromFile(int id);




};
#endif //TODOLIST_TASKSERVICE_HPP
