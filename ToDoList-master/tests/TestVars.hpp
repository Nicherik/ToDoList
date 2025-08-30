//
// Created by nikit on 8/8/2025.
//

#ifndef TODOLIST_TESTVARS_HPP
#define TODOLIST_TESTVARS_HPP

#include <string>
#include <chrono>
#include <vector>


inline string file_name = string(PROJECT_SOURCE_DIR) + "/tests/testData.json";

inline string title = "Test task";
inline optional<Status> status = Status::Pending;
inline optional<string> description = "Some text";
inline optional<int> priority = 1;
inline optional<chrono::system_clock::time_point> deadline=chrono::system_clock::now();
inline optional<set<string>> tags = optional<set<string>>{{"tag1", "tag2"}};
#endif //TODOLIST_TESTVARS_HPP
