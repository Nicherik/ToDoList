//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_ITASKSTORAGE_HPP
#define TODOLIST_ITASKSTORAGE_HPP



#include <vector>
#include <Task.hpp>
class iTaskStorage {

public:

    [[nodiscard]] virtual vector<Task> loadTask() const = 0;
    virtual void saveTasks(const vector<Task>& tasks) const = 0;
    [[nodiscard]] virtual bool clearStorage() const = 0;
    [[nodiscard]] virtual bool deleteTaskById(int id) const=0;


};
#endif //TODOLIST_ITASKSTORAGE_HPP