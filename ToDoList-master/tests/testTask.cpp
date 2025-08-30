//
// Created by nikit on 8/5/2025.
//

#include "catch_amalgamated.hpp"
#include "Task.hpp"
#include "TestVars.hpp"
#include <chrono>
#include <ctime>


TEST_CASE("ТЕСТ АТРИБУТОВ") {

    time_t d1;
    time_t d2;
    Task task(title, status, description, priority, deadline, tags);
    REQUIRE(task.getId() == 1);
    REQUIRE(task.getTitle() == title);
    if(task.getStatus().has_value())
        REQUIRE(task.getStatus().value() == status.value());
    if(task.getPriority().has_value())
        REQUIRE(task.getPriority().value() == priority.value());
    if (task.getDeadline().has_value()) {
        d1 = chrono::system_clock::to_time_t(task.getDeadline().value());
        d2 = chrono::system_clock::to_time_t(deadline.value());
        REQUIRE(std::difftime(d1, d2) == 0);
    }
    if(task.getTags().has_value())
        REQUIRE(task.getTags().value() == tags.value());

    Task::resetNextId();
}

TEST_CASE("ТЕСТ ИЗМЕНЕНИЯ СТАТУСА") {

    Task task(title, status, description, priority, deadline, tags);
    if(task.getStatus().has_value())
        REQUIRE(task.getStatus() == status);

    Task::resetNextId();

}




