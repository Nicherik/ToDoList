
// Created by nikit on 8/5/2025.
//
#include "catch_amalgamated.hpp"
#include "TaskService.hpp"
#include "JsonTaskStorage.hpp"
#include <optional>
#include "Task.hpp"
#include "SortType.hpp"
#include <fstream>
#include "TestVars.hpp"




TEST_CASE("ТЕСТ МЕТОДА addTask"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res=ts.addTask(title, status, description, priority, deadline, tags);
    REQUIRE(res);
    REQUIRE(ts.getTasKAmount()==1);
    REQUIRE(ts.getTaskById(1).getTitle()==title);

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА updateTask"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res=ts.addTask(title, status, description, priority, deadline, tags);
    REQUIRE(res);
    bool update=ts.updateTask(1, "llll",Status::Done, "",
                  2, deadline, {});
    REQUIRE(update);

    REQUIRE(ts.getTaskById(1).getTitle()=="llll");
    if(ts.getTaskById(1).getStatus().has_value())
        REQUIRE(ts.getTaskById(1).getStatus()==Status::Done);
    if(ts.getTaskById(1).getPriority())
        REQUIRE(ts.getTaskById(1).getPriority()==2);

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА deleteTask"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res=ts.addTask(title, status, description, priority, deadline, tags);
    REQUIRE(res);
    bool delete_task=ts.removeTask(1);
    REQUIRE(delete_task);
    REQUIRE(ts.getTasKAmount()==0);
    REQUIRE_THROWS(ts.getTaskById(1));

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА getTaskById"){

    time_t time1;
    time_t time2 = 0;
    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res=ts.addTask(title, status, description, priority, deadline, tags);
    REQUIRE(res);
    Task tsk=ts.getTaskById(1);

    REQUIRE(tsk.getId()==1);
    REQUIRE(tsk.getTitle()==title);
    if(tsk.getPriority().has_value())
        REQUIRE(tsk.getPriority()==priority);
    if(tsk.getStatus().has_value())
        REQUIRE(tsk.getStatus()==status);
    time1=chrono::system_clock::to_time_t(deadline.value());
    if(tsk.getDeadline().has_value())
        time2=chrono::system_clock::to_time_t(tsk.getDeadline().value());
    REQUIRE(difftime(time1,time2)==0);
    if(tsk.getDescription().has_value())
        REQUIRE(tsk.getDescription()==description);
    if(tsk.getTags().has_value())
        REQUIRE(tsk.getTags()==tags);

    Task::resetNextId();

}


TEST_CASE("ТЕСТ МЕТОДА getAllTasks"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res1=ts.addTask(title, status, description, priority, deadline, tags);
    bool res2=ts.addTask("title", status, "description", 1, deadline,
                         set<string >{"#tags", "#SUN"});
    REQUIRE(res1);
    REQUIRE(res2);

    REQUIRE(ts.getAllTasks().getAllTasks().size()==2);

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА sortBy"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res=ts.addTask(title, status, description, priority, deadline, tags);
    REQUIRE(res);
    bool deadlineAsc=ts.sortBy(SortType::deadlineAsc);
    REQUIRE(deadlineAsc);
    bool deadlineDesc=ts.sortBy(SortType::deadlineDesc);
    REQUIRE(deadlineDesc);

    bool priorityAsc=ts.sortBy(SortType::priorityAsc);
    REQUIRE(priorityAsc);
    bool priorityDesc=ts.sortBy(SortType::priorityDesc);
    REQUIRE(priorityDesc);

    bool statusAsc=ts.sortBy(SortType::statusAsc);
    REQUIRE(statusAsc);
    bool statusDesc=ts.sortBy(SortType::statusDesc);
    REQUIRE(statusDesc);

    bool _tagsAsc=ts.sortBy(SortType::tagsAsc);
    REQUIRE(_tagsAsc);

    bool _tagsDesc=ts.sortBy(SortType::tagsDesc);
    REQUIRE(_tagsDesc);

    bool titleAsc=ts.sortBy(SortType::titleAsc);
    REQUIRE(titleAsc);
    bool titleDesc=ts.sortBy(SortType::titleDesc);
    REQUIRE(titleDesc);

    Task::resetNextId();

}

TEST_CASE("ТЕСТ МЕТОДА getBy"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res1=ts.addTask(title, status, description, priority, deadline, tags);
    bool res2=ts.addTask("title", status, "description", 1, deadline,
               set<string >{"#tags", "#SUN"});
    REQUIRE(res1);
    REQUIRE(res2);

    string tag;
    for(const Task & task:ts.getBy(priority.value()))
        REQUIRE(task.getPriority()==priority);
   for(const Task & task:ts.getBy(GetType::title, title))
       REQUIRE(task.getTitle()==title);
   for(const Task & task:ts.getBy(GetType::tags, tag))
       REQUIRE(task.getTags().value()==tags);
   for(const Task & task:ts.getBy(deadline.value()))
       REQUIRE(task.getDeadline()==deadline);
   for(const Task & task:ts.getBy(status.value()))
       REQUIRE(task.getStatus()==status);

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА saveTasks"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res1=ts.addTask(title, status, description, priority, deadline, tags);
    bool res2=ts.addTask("title", status, "description", 1, deadline,
                         set<string >{"#tags", "#SUN"});
    REQUIRE(res1);
    REQUIRE(res2);

    ofstream file(js.getFileName());
    bool result=ts.saveTasks();
    REQUIRE(result);
    REQUIRE(!file.tellp());

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА removeAllTasks"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res1=ts.addTask(title, status, description, priority, deadline, tags);
    bool res2=ts.addTask("title", status, "description", 1, deadline,
                         set<string >{"#tags", "#SUN"});
    REQUIRE(res1);
    REQUIRE(res2);

    bool res=ts.removeAllTasks();
    REQUIRE(res);
    REQUIRE(!ts.getTasKAmount());

    Task::resetNextId();
}

TEST_CASE("ТЕСТ МЕТОДА loadTask"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    ifstream file(js.getFileName());
    bool result=ts.loadTask();
    REQUIRE(result);
    REQUIRE(ts.getTasKAmount());

    Task::resetNextId();

}

TEST_CASE("ТЕСТ МЕТОДА deleteTaskFromFile"){

    JsonTaskStorage js(file_name);
    TaskService ts(js);

    bool res1=ts.addTask(title, status, description, priority, deadline, tags);
    bool res2=ts.addTask("title", status, "description", 1, deadline,
                         set<string >{"#tags", "#SUN"});
    REQUIRE(res1);
    REQUIRE(res2);

    bool res3=ts.deleteTaskFromFile(1);
    bool res4=ts.deleteTaskFromFile(2);
    REQUIRE(res3);
    REQUIRE(res4);

    Task::resetNextId();

}

