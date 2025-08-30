//
// Created by nikit on 8/5/2025.
//
#include "Functions.hpp"
#include "JsonTaskStorage.hpp"
#include "Task.hpp"
#include "TaskList.hpp"
#include <filesystem>
#include "TestVars.hpp"
#include "catch_amalgamated.hpp"
#include <iostream>






TEST_CASE("ПРОВЕРКА НА СОЗДАНИЕ ФАЙЛА") {

    TaskList lst;
    JsonTaskStorage j(file_name);


    REQUIRE(std::filesystem::exists(j.getFileName()));

}



TEST_CASE("ПРОВЕРКА НА ВЫГРУЗКУ ИЗ ФАЙЛА И ОЧИСТКУ ФАЙЛА") {
    TaskList lst;

    Task task1( title, status, description, priority, deadline, tags);
    Task task2(title, status, description, priority, deadline, tags);
    Task task3( title, status, description, priority, deadline, tags);
    JsonTaskStorage j(file_name);
    lst.addTask(task1);
    lst.addTask(task2);
    lst.addTask(task3);

    vector<Task> vt;

    j.saveTasks(lst.getAllTasks());
    vt = j.loadTask();
    REQUIRE(!vt.empty());
    bool res=j.clearStorage();
    REQUIRE(res);

    REQUIRE(filesystem::file_size(file_name)==0);
    REQUIRE_THROWS(j.loadTask());

    Task::resetNextId();

}





TEST_CASE("ПРОВЕРКА ЭКВИВАЛЕНТНОСТИ ДАННЫХ") {
    TaskList lst;

    Task task1(title+"43rgrfg", status, description, priority, deadline, tags);
    Task task2(title+"egreg23434", status, description, priority, deadline, tags);
    Task task3(title+"435rgfref", status, description, priority, deadline, tags);
    JsonTaskStorage j(file_name);
    vector<Task> vt;
    lst.addTask(task1);
    lst.addTask(task2);
    lst.addTask(task3);
    j.saveTasks(lst.getAllTasks());
    vt=j.loadTask();
    for (int i = 1; i <= lst.getAllTasks().size(); i++) {

       REQUIRE(lst.getTask(i).getId() == vt[i-1].getId());
       REQUIRE(lst.getTask(i).getTitle() == vt[i-1].getTitle());
       if(lst.getTask(i).getStatus().has_value())
           REQUIRE(lst.getTask(i).getStatus() == vt[i-1].getStatus());
       if(lst.getTask(i).getDescription().has_value())
           REQUIRE(lst.getTask(i).getDescription() == vt[i-1].getDescription());
       if(lst.getTask(i).getPriority().has_value())
           REQUIRE(lst.getTask(i).getPriority() == vt[i-1].getPriority());
       if( lst.getTask(i).getDeadline().has_value())
           REQUIRE(deadlineToString( lst.getTask(i).getDeadline().value()) ==deadlineToString( vt[i-1].getDeadline().value()));
       if(lst.getTask(i).getTags().has_value())
           REQUIRE(lst.getTask(i).getTags() == vt[i-1].getTags());
    }

    Task::resetNextId();
}