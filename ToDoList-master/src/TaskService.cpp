//
// Created by nikit on 8/5/2025.
//
#include "TaskService.hpp"
#include "Task.hpp"



#include <iostream>
#include <utility>

bool TaskService::addTask(string _title, optional<Status> _status,
                          optional<string> _description, optional<int> _priority,
                          optional<chrono::time_point<chrono::system_clock>> _deadline, optional<set<string>> _tags)
{
    try {


        Task task(std::move(_title), _status, std::move(_description), _priority, _deadline, std::move(_tags));

        lst->addTask(task);

    }
    catch(const exception& e) {
        return false;
    }
    return true;
}

bool TaskService::updateTask(int task_id, optional<string> new_title, optional<Status> new_status,
                             optional<string> new_description, optional<int> new_priority,
                             optional<chrono::time_point<chrono::system_clock>> new_deadline, optional<set<string>> new_tags)
{
    try {
        lst->updateTask(task_id, std::move(new_title), new_status, std::move(new_description), new_priority, new_deadline, std::move(new_tags));
    }
    catch (exception& e) {
        return false;
    }
    return true;
}

bool TaskService::removeTask(int id)
{
    try {

        lst->removeTask(id);

    }
    catch (exception& e) {
        return false;
    }
    return true;
}

const Task& TaskService::getTaskById(int id)
{
    return lst->getTask(id);

}

TaskList& TaskService::getAllTasks()
{

    return *lst;
}

bool TaskService::sortBy(const SortType& _type)
{
    try {
        switch (_type) {

            case SortType::titleAsc:
                lst->sortByTitleAscending();
                break;
            case SortType::titleDesc:
                lst->sortByTitleDescending();
                break;
            case SortType::statusAsc:
                lst->sortByStatusAsc();
                break;
            case SortType::statusDesc:
                lst->sortByStatusDesc();
                break;
            case SortType::priorityAsc:
                lst->sortByPriorityAsc();
                break;
            case SortType::priorityDesc:
                lst->sortByPriorityDesc();
                break;
            case SortType::deadlineAsc:
                lst->sortByDeadlineAscending();
                break;
            case SortType::deadlineDesc:
                lst->sortByDeadlineDescending();
                break;
            case SortType::tagsAsc:
                lst->sortByAmountTagsAsc();
                break;
            case SortType::tagsDesc:
                lst->sortByAmountTagsDesc();
                break;

        }
    }
    catch (const exception& e) {
        return false;
    }
    return true;
}

vector<Task> TaskService::getBy(GetType type1, const string &parameter)
{
    vector<Task> rvtask;
    try {
        switch (type1) {
            case GetType::title:
                        rvtask = lst->getTaskListByTitle(parameter);
                        TaskList::sortById(rvtask);
                        break;

            case GetType::tags:
                        rvtask = lst->getTaskListByTag(parameter);
                        TaskList::sortById(rvtask);
                        break;
        }
    }

    catch (const exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return rvtask;
}

vector<Task> TaskService::getBy(int priority)
{
    vector<Task> rvtask;
    try {
        rvtask=lst->getTaskListByPriority(priority);
        TaskList::sortById(rvtask);
    }

    catch (const exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return rvtask;
}

vector<Task> TaskService::getBy(chrono::time_point<chrono::system_clock> deadline)
{
    vector<Task> rvtask;
    try {
        rvtask=lst->getTaskListByDeadline(deadline);
        TaskList::sortById(rvtask);
    }

    catch (const exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return rvtask;
}

vector<Task> TaskService::getBy(Status status)
{
    vector<Task> rvtask;
    try {
        rvtask=lst->getTaskListByStatus(status);
        TaskList::sortById(rvtask);
    }

    catch (const exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return rvtask;
}





bool TaskService::loadTask()
{
    try {
        lst->addAllTasks(storage.loadTask());
    }
    catch (const exception& e) {
        cout<<e.what();
        return false;
    }
    return true;

}

bool TaskService::saveTasks()
{
    try {
        storage.saveTasks(lst->getAllTasks());

    }
    catch (const exception& e) {
        std::cerr << "Exception in saveTasks: " << e.what() << std::endl;
        return false;
    }
    return true;
}

TaskService::TaskService(iTaskStorage &_storage):storage(_storage) {}

int TaskService::getTasKAmount() {
    return lst->getTasksAmount();
}

bool TaskService::removeAllTasks() {
    try {
        lst->removeAllTasks();
    }
    catch (const exception& e){
        return false;
    }
    return true;
}

void TaskService::allTasksToString() {

    for(int i=0; i<lst->getTasksAmount(); i++)
        lst->getAllTasks()[i].toString();

}



bool TaskService::addTask(int id, string _title, optional<Status> _status, optional<string> _description,
                          optional<int> _priority, optional<chrono::time_point<chrono::system_clock>> _deadline,
                          optional<set<string>> _tags) {
    try {

        Task task(id, std::move(_title), _status, std::move(_description), _priority, _deadline, std::move(_tags));

        lst->addTask(task);

    }
    catch(const exception& e) {
        return false;
    }
    return true;
}

bool TaskService::deleteTaskFromFile(const int id) {
    if(storage.deleteTaskById(id))
        return true;
    return false;
}







