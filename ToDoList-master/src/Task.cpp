//
// Created by nikit on 8/5/2025.
//

#include "Task.hpp"
#include "Functions.hpp"
#include <iostream>
#include <optional>
#include <utility>
using namespace std;

int Task::next_id = 0;

 optional<string> Task::getDescription() const {

     return description;
}



optional<int> checkPriority(const optional<int> _pr) {
    if(_pr.has_value()) {
        if (_pr.value() <= 0 || _pr.value() > 5) throw invalid_argument("Incorrect priority!");
        return _pr.value();
    }
    return {};
}

const optional<chrono::system_clock::time_point>& Task::getDeadline() const {
    return deadline;
}



const optional<set<string>>& Task ::getTags() const {
    return tags;
}





Task::Task(string _title, optional <Status> _status, optional < string> _description,
           optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
           optional < set<string> > _tags)
        :id(++next_id), title(std::move(_title)), status(_status),
         description(std::move(_description)), priority(checkPriority(_priority)),
         deadline(_deadline), tags(std::move(_tags)) {}

Task::Task(int _id, string _title, optional <Status> _status, optional < string> _description,
           optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline,
           optional < set<string> >  _tags):id(_id), title(std::move(_title)),
                                            status(_status),
                                            description(std::move(_description)), priority(checkPriority(_priority)),
                                            deadline(_deadline), tags(std::move(_tags))
{}

bool Task::operator<(const Task& task) const
{
    if (!tags.has_value() && !task.tags.has_value()) return false;
    if (!tags.has_value()) return true;
    if (!task.tags.has_value()) return false;
    return tags.value() < task.tags.value();
}

int Task::getId() const {
    return id;
}

const string& Task::getTitle() const{
    return title;
}

optional<Status> Task::getStatus() const
{
     return status;
}

void Task::changeStatus(Status st) {
    status = st;
}

void Task::toString() const {

     cout<<"\n------------*"<<id<<"*------------";
    cout << "\nTitle: " << title ;

    if (status.has_value()) {
        if (status.value() == Status::Done)
            cout << "\nStatus: done";
        else
            cout << "\nStatus: pending" ;
    }
    if (description.has_value())
        cout << "\nDescription: "<<description.value();
    if (priority.has_value() && priority.value()!=0)
        cout << "\nPriority: "<<priority.value();

    if (deadline.has_value())
        cout << "\nDeadline: " << deadlineToString(deadline.value());


    if (tags.has_value()) {
        cout << "\nTags: ";
        for (const string& tg : tags.value())
            cout << tg << " ";
    }
    cout << endl << endl;
}

void Task::resetNextId()
{
    Task::next_id = 0;
}

optional<int> Task::getPriority() const
{
    return priority;
}

void Task::changeDescription(const string& new_description)
{
    description = new_description;
}

void Task::changeDeadline(const chrono::time_point<chrono::system_clock>& new_deadline)
{
    deadline = new_deadline;
}

void Task::changeTitle(const string& new_title)
{
    title = new_title;
}



void Task::changePriority(const int new_priority)
{
    priority = new_priority;
}

void Task::changeTags(const set<string>& new_tags)
{
    tags = new_tags;
}

void Task::setId(const int _id)
{
    id = _id;

}

void Task::setTitle(const string&_title)
{
    title = _title;
}

void Task::setStatus(const Status st)
{
    status = st;
}

void Task::setDescription(const string& dsc)
{
    description = dsc;
}

void Task::setPriority(int pr)
{
    priority = pr;
}

void Task::setDeadline(const chrono::time_point<chrono::system_clock>& d)
{
    deadline = d;
}

void Task::setTags(const set<string>& tg)
{
    tags = tg;
}
