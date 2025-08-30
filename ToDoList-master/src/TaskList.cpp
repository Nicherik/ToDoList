//
// Created by nikit on 8/5/2025.
//

#include "TaskList.hpp"
#include "Task.hpp"
#include <stdexcept>
#include <algorithm>
#include <optional>
using namespace std;



void TaskList::addTask(Task &tsk) {


    vtask.push_back(tsk);

}

void TaskList::addAllTasks(const vector<Task>& _vtsk) {

    vtask = _vtsk;

}


void TaskList::removeTask(int _id) {

    auto iter = find_if(vtask.begin(), vtask.end(), [&_id](const Task& task) {return task.getId() == _id; });

    if(iter==vtask.end())
        throw runtime_error("Task not found");
    vtask.erase(iter);


}

const Task& TaskList::getTask(int _id) const{

    auto iter= find_if(vtask.begin(), vtask.end(), [&_id](const Task& task) {return task.getId() == _id; });

    if (iter != vtask.end())
        return *iter;
    else
        throw runtime_error("Task not found");

}

const vector<Task>& TaskList::getAllTasks() const {

    return vtask;

}

int TaskList::getTasksAmount() const
{
    return vtask.size();
}


void TaskList::updateTask(int _id, optional<string> _title,
                          optional <Status> _status, optional < string> _description,
                          optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
                          optional < set<string> > _tags)
{
    vector<Task>::iterator iter;
    iter=find_if(vtask.begin(), vtask.end(), [&_id](const Task& tsk) {return tsk.getId() == _id; });
    if (_title.has_value())
        iter->changeTitle(_title.value());
    if (_status.has_value())
        iter->changeStatus(_status.value());
    if (_description.has_value())
        iter->changeDescription(_description.value());
    if (_priority.has_value())
        iter->changePriority(_priority.value());
    if (_deadline.has_value())
        iter->changeDeadline(_deadline.value());
    if (_tags.has_value())
        iter->changeTags(_tags.value());

}

vector<Task> TaskList::getTaskListByStatus(Status st)
{
    vector<Task> vtaskFilter;
    for (const auto& Task : vtask) {
        if (Task.getStatus() == st)
            vtaskFilter.push_back(Task);


    }
    return vtaskFilter;
}

vector<Task> TaskList::getTaskListByTitle(const string& _title)
{
    vector<Task> vtaskFilter;
    for (const auto& task : vtask) {
      //  string tmp_title=task.getTitle();
      //  string tmp_arg_title=_title;
      //  tmp_title.erase(remove_if(tmp_title.begin(), tmp_title.end(),
      //                            [](unsigned char c){return isspace(c);}), tmp_title.end());
      //  cout<<tmp_title<<endl;
//
      //  tmp_arg_title.erase(remove_if(tmp_arg_title.begin(), tmp_arg_title.end(),
      //                            [](unsigned char c){return isspace(c);}), tmp_arg_title.end());
      //  cout<<tmp_arg_title<<endl;
        if ( _title== task.getTitle())
            vtaskFilter.push_back(task);
    }
    TaskList::sortById(vtaskFilter);
    return vtaskFilter;
}



vector<Task> TaskList::getTaskListByPriority(int priority)
{
    vector<Task> vtaskFilter;
    for (const auto& Task : vtask) {

        if (Task.getPriority() == priority)
            vtaskFilter.push_back(Task);


    }
    return vtaskFilter;

}

vector<Task> TaskList::getTaskListByTag(const string &_tag)
{
    set<Task> vTaskFilter;

    for (const Task& task : vtask) {

        if(task.getTags().has_value()) {
            auto iter = find_if(task.getTags()->begin(), task.getTags()->end(),
                                                 [&_tag](const string &tag) { return tag == _tag; });

            if (iter!= task.getTags().value().end())
                vTaskFilter.insert(task);
        }
    }
    vector<Task> rvtask(vTaskFilter.begin(), vTaskFilter.end());

    return rvtask;


}

vector<Task> TaskList::getTaskListByDeadline(const chrono::time_point<chrono::system_clock> &_deadline)
{
    vector<Task> vTaskFilter;
    for (const Task& task : vtask) {


        if (task.getDeadline() == _deadline)
            vTaskFilter.push_back(task);
    }
    return vTaskFilter;
}



void TaskList::sortByDeadlineAscending()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getDeadline() < next_task.getDeadline(); });

}

void TaskList::sortByDeadlineDescending()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getDeadline() > next_task.getDeadline(); });

}



void TaskList::sortByTitleAscending()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getTitle() < next_task.getTitle(); });

}

void TaskList::sortByTitleDescending()
{
    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getTitle() > next_task.getTitle(); });
}

void TaskList::sortByStatusAsc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getStatus() < next_task.getStatus(); });

}

void TaskList::sortByStatusDesc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getStatus() > next_task.getStatus(); });

}

void TaskList::sortByPriorityAsc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getPriority() < next_task.getPriority(); });

}

void TaskList::sortByPriorityDesc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task) {return task.getPriority() > next_task.getPriority(); });


}

void TaskList::sortByAmountTagsAsc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task)
         {return (task.getTags().has_value()?task.getTags()->size():0) <
                 (next_task.getTags().has_value()?next_task.getTags()->size():0); });
}

void TaskList::sortByAmountTagsDesc()
{

    sort(vtask.begin(), vtask.end(),
         [](const Task& task, const Task& next_task)
         {return (task.getTags().has_value()?task.getTags()->size():0) >
                 (next_task.getTags().has_value()?next_task.getTags()->size():0); });
}

void TaskList::removeAllTasks() {

    vtask.clear();

}

void TaskList::sortById(vector<Task> & tsk) {

    std::sort(tsk.begin(), tsk.end(), []
    (const Task &tsk, const Task & nxt_tsk){return tsk.getId()<nxt_tsk.getId();});

}




