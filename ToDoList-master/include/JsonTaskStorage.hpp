//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_JSONTASKSTORAGE_HPP
#define TODOLIST_JSONTASKSTORAGE_HPP




#include "ITaskStorage.hpp"

#include <string>
#include "Task.hpp"
class JsonTaskStorage :public iTaskStorage {
protected:
    string _fileName;

public:
    JsonTaskStorage()=delete;
    explicit JsonTaskStorage(string file_name) noexcept;
    void saveTasks(const vector<Task>& tsk) const override;
    [[nodiscard]] vector<Task> loadTask() const override;
    [[nodiscard]] bool clearStorage() const override;
    [[nodiscard]] const string& getFileName() const;
    [[nodiscard]] bool deleteTaskById(int id) const override;



};
#endif //TODOLIST_JSONTASKSTORAGE_HPP