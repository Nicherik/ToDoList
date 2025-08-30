//
// Created by nikit on 8/5/2025.
//

#ifndef TODOLIST_FUNCTIONS_HPP
#define TODOLIST_FUNCTIONS_HPP



#include<chrono>
#include <string>
#include <sstream>
#include "Task.hpp"
#include <algorithm>

using namespace std;

inline string deadlineToString(optional<chrono::time_point<chrono::system_clock> > tp){
    if(tp.has_value()) {
        time_t t = chrono::system_clock::to_time_t(tp.value());

        tm *_tm = localtime(&t);
        if (_tm == nullptr) {
            throw std::runtime_error("localtime() returned nullptr");
        }
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", _tm);
        return string(buffer);
    }
    return {};
}
inline chrono::time_point<chrono::system_clock> stringToDeadline(const string& dateStr){
    int day, month, year;
    char sep1, sep2;
    istringstream ss(dateStr);
    ss >> day >> sep1 >> month >> sep2 >> year;
    if (ss.fail() || sep1 != '-' || sep2 != '-') {
        throw std::runtime_error("Invalid date format");
    }

    tm tm = {};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;   // месяца с 0
    tm.tm_year = year - 1900; // годы с 1900

    time_t time = std::mktime(&tm);
    if (time == -1) {
        throw runtime_error("Failed to convert time");
    }
    return chrono::system_clock::from_time_t(time);
}

inline optional<chrono::time_point<chrono::system_clock> > stringToDeadline(const optional<string>& dateStr){
    if(!dateStr.has_value()) return {};
    int day, month, year;
    char sep1, sep2;
    std::istringstream ss(dateStr.value());
    ss >> day >> sep1 >> month >> sep2 >> year;
    if (ss.fail() || sep1 != '-' || sep2 != '-') {
        throw std::runtime_error("Invalid date format");
    }

    std::tm tm = {};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;   // месяца с 0
    tm.tm_year = year - 1900; // годы с 1900

    std::time_t time = std::mktime(&tm);
    if (time == -1) {
        throw std::runtime_error("Failed to convert time");
    }
    return std::chrono::system_clock::from_time_t(time);
}




inline optional<Status> stringToStatus(const optional<std::string>& statusStr) {
    if (!statusStr.has_value()) return {};

    string val = statusStr.value();
    transform(val.begin(), val.end(), val.begin(), ::tolower);
    if (val == "pending") return Status::Pending;
    if (val == "done") return Status::Done;

    return {};
}


inline optional<int> stringToPriority(const optional<string>& priorityStr){

    int res;
    if(!priorityStr.has_value()) return {};
    try {
        const string& pr = priorityStr.value();
        res= stoi(pr);
    }
    catch (const exception&e) {
        throw runtime_error("Invalid priority");
    }
    return res;

}

inline optional<set<string> > stringToTags(const optional<string>& tagsStr){

    if(!tagsStr.has_value()) return {};
    set<string> result_set;
    istringstream iss(tagsStr.value());
    string tag;
    while (iss>>tag)
        result_set.insert(tag);
    return result_set;

}



#endif //TODOLIST_FUNCTIONS_HPP