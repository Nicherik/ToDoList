//
// Created by nikit on 8/5/2025.
//

#include "JsonTaskStorage.hpp"
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <utility>
#include"Functions.hpp"
#include"Task.hpp"







void JsonTaskStorage::saveTasks(const vector<Task>& tsk) const
{
    ofstream file(_fileName);
    if (!file) {
        throw runtime_error("Could not open the file");

    }


    nlohmann::json j_array = nlohmann::json::array();
    for (const Task& task : tsk) {
        nlohmann::json j;


        j["id"] = task.getId();
        j["title"] = task.getTitle();
        if(task.getStatus().has_value())
            j["status"] = static_cast<int>(task.getStatus().value());
        if(task.getDescription().has_value())
            j["description"] = task.getDescription().value();
        if(task.getPriority().has_value())
            j["priority"] = task.getPriority().value();
        if(task.getDeadline().has_value())
            j["deadline"] = deadlineToString(task.getDeadline().value());
        if(task.getTags().has_value())
            j["tags"] = task.getTags().value();
        j_array.push_back(j);


    }
    file << setw(4) << j_array << endl;



}

vector<Task> JsonTaskStorage::loadTask() const
{
    ifstream file(_fileName);
    if (!file.is_open()) {
        throw runtime_error("Could not open the file");

    }
    vector<Task> vtsk;
    nlohmann::json j_array=nlohmann::json::array();
    file >> j_array;


    file.seekg(0, ios::beg);
    if (file.peek() != ifstream::traits_type::eof()) {
        for (const nlohmann::json& j : j_array) {

            Task task;
            try {
                task.setId(j["id"]);
                task.setTitle(j["title"]);
                if(j.contains("status"))
                    task.setStatus(j["status"]);
                if(j.contains("description"))
                    task.setDescription(j["description"]);
                if(j.contains("priority"))
                    task.setPriority(j["priority"]);
                if(j.contains("deadline"))
                    task.setDeadline(stringToDeadline(j["deadline"].get<string>()));
                if(j.contains("tags"))
                    task.setTags(j["tags"]);
                vtsk.push_back(task);
            }
            catch (const exception& e) {
                cerr<<e.what();
                throw runtime_error("Parsing is not valid!");
            }
        }
    }
    else
    {
        throw runtime_error("Nothing to load!");
    }
    return vtsk;

}

bool JsonTaskStorage::clearStorage() const
{

    ofstream file(_fileName, ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Could not open the file");

    }
    try {
        file.flush();
    }
    catch (const exception&e){
        cout<<"\nSomething went wrong";
        return false;
    }
    return true;
}

const string& JsonTaskStorage::getFileName() const
{
    return _fileName;
}


JsonTaskStorage ::JsonTaskStorage(string   file_name) noexcept:_fileName(std::move(file_name)){}

bool JsonTaskStorage::deleteTaskById(int id) const{
    ofstream file(_fileName, ios::app);
    if (!file.is_open()) {
        throw runtime_error("Could not open the file");
    }
    nlohmann::json  js;
    int c=1;
    try {
        vector<Task> tsk = loadTask();
        for (const Task &task: tsk) {
            if (task.getId() == id) {
                tsk.erase(tsk.begin() + c);
                break;
            }
            c++;
        }
        saveTasks(tsk);
    }
    catch (const exception&e){
        cerr<<e.what();
        return false;
    }
    return true;
}


