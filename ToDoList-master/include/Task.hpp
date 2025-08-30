//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_TASK_HPP
#define TODOLIST_TASK_HPP



#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <optional>

using namespace std;

enum class Status { Pending, Done };

class Task {

    static  int next_id;
    int id{};
    string  title;
    optional<Status> status;
    optional<string> description;
    optional<int> priority;
    optional<chrono::time_point<chrono::system_clock> > deadline;
    optional<set<string> > tags;

public:

    Task() = default;
    Task(string _title, optional <Status> _status, optional < string> _description,
         optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline, optional < set<string> > _tags);
    Task(int id, string _title, optional <Status> _status, optional < string> _description,
         optional <int> _priority, optional < chrono::time_point<chrono::system_clock> > _deadline, optional < set<string> > _tags);

    bool operator <(const Task& task) const;
    [[nodiscard]] int getId() const;
    [[nodiscard]] const string& getTitle() const;
    [[nodiscard]] optional<Status> getStatus() const;
    [[nodiscard]] optional<string> getDescription() const;
    void changeStatus(Status);
    void toString() const;
    static void resetNextId();
    [[nodiscard]] optional<int> getPriority() const;
    [[nodiscard]]const optional<set<string>>& getTags() const ;
    [[nodiscard]] const optional<chrono::system_clock::time_point>& getDeadline() const ;
    void changeDescription(const string &new_description);
    void changeDeadline(const chrono::time_point<chrono::system_clock>& new_deadline);
    void changeTitle(const string& new_title);
    void changePriority(int  new_priority);
    void changeTags(const set<string>&  new_tags);
    void setId(int ) ;
    void setTitle(const string&) ;
    void setStatus(Status) ;
    void setDescription(const string&) ;
    void setPriority(int) ;
    void setDeadline(const chrono::time_point<chrono::system_clock>&);
    void setTags(const set<string>&) ;







};

#endif //TODOLIST_TASK_HPP