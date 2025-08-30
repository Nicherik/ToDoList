//
// Created by nikit on 8/5/2025.
//
#include "catch_amalgamated.hpp"
#include "TaskList.hpp"
#include "Task.hpp"
#include "TestVars.hpp"





TEST_CASE("ТЕСТ ДОБАВЛЕНИЕ ЗАДАЧИ") {

    TaskList tasks;
    Task task1(title, status, description, priority, deadline, tags);
    Task task2(title, status, description, priority, deadline, tags);
    Task task3(title, status, description, priority, deadline, tags);

    tasks.addTask(task1);
    tasks.addTask(task2);
    tasks.addTask(task3);

    REQUIRE(tasks.getTasksAmount() == 3);

    Task::resetNextId();

}

TEST_CASE("ТЕСТ УДАЛЕНИЯ ЗАДАЧИ") {

    TaskList tasks;
    Task task1(title, status, description, priority, deadline, tags);
    Task task2(title, status, description, priority, deadline, tags);
    Task task3(title, status, description, priority, deadline, tags);

    tasks.addTask(task1);
    tasks.addTask(task2);
    tasks.addTask(task3);

    tasks.removeTask(3);
    REQUIRE(tasks.getTasksAmount()==2);

    Task::resetNextId();

}

TEST_CASE("ТЕСТ ПОЛУЧЕНИЯ ЗАДАЧИ") {

    TaskList tasks;
    Task task1(title, status, description, priority, deadline, tags);
    Task task2(title, status, description, priority, deadline, tags);
    Task task3(title, status, description, priority, deadline, tags);

    tasks.addTask(task1);
    tasks.addTask(task2);
    tasks.addTask(task3);

    REQUIRE(tasks.getTask(1).getId() == 1);
    REQUIRE(tasks.getTask(2).getId() == 2);
    REQUIRE(tasks.getTask(3).getId() == 3);

    Task::resetNextId();
}

TEST_CASE("ТЕСТ УДАЛЕНИЕ НЕСУЩЕСТВУЮЩЕГО ID") {

    TaskList tasks;
    Task task1(title, status, description, priority, deadline, tags);
    Task task2(title, status, description, priority, deadline, tags);
    Task task3(title, status, description, priority, deadline, tags);

    tasks.addTask(task1);
    tasks.addTask(task2);
    tasks.addTask(task3);

    REQUIRE_THROWS_AS(tasks.removeTask(4), runtime_error);

    Task::resetNextId();

}