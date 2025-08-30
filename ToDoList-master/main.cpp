
#include <iostream>
#include <string>
#include <regex>
#include "TaskService.hpp"
#include "JsonTaskStorage.hpp"
#include "Functions.hpp"
#include "fstream"
#include <ctime>
#include <ranges>



using namespace std;
bool flag_update=false;
bool isValidFutureDate(const string& dateStr) {
    // Проверка формата с помощью regex dd-mm-yyyy
    regex date_regex(R"(^(\d{2})-(\d{2})-(\d{4})$)");
    smatch match;
    if (!regex_match(dateStr, match, date_regex)) {
        return false;
    }

    // Парсим день, месяц, год
    int day = stoi(match[1].str());
    int month = stoi(match[2].str());
    int year = stoi(match[3].str());

    // Проверяем корректность даты (в том числе високосный год)
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    // Високосный год
    bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    int maxDay = daysInMonth[month - 1];
    if (isLeap && month == 2) maxDay = 29;

    if (day < 1 || day > maxDay) return false;

    // Сравниваем с текущей датой
    time_t now = time(nullptr);
    tm currentDate = *localtime(&now);

    tm inputDate = {};
    inputDate.tm_mday = day;
    inputDate.tm_mon = month - 1; // месяцы с 0
    inputDate.tm_year = year - 1900; // с 1900

    // Обнулим часы, минуты, секунды, чтобы сравнивать только даты
    inputDate.tm_hour = 0;
    inputDate.tm_min = 0;
    inputDate.tm_sec = 0;
    currentDate.tm_hour = 0;
    currentDate.tm_min = 0;
    currentDate.tm_sec = 0;

    time_t inputTime = mktime(&inputDate);
    time_t currentTime = mktime(&currentDate);

    if (inputTime == -1) return false; // Некорректная дата
    if (difftime(inputTime, currentTime) <= 0) return false; // Дата не больше текущей

    return true;
}


void warningToSaveData(bool flag_saved_data, int amount_of_unsaved_tasks, string &ans, TaskService & task_service){

    if(!flag_saved_data || amount_of_unsaved_tasks) {
        cout << "\nDo you want to save data?(yes/no): ";
        getline(cin, ans);
        transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
        while (ans != "yes" && ans != "no") {
            cout << "\nInvalid input, try again:";
            getline(cin, ans);
            transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
        }
        if (ans == "yes") {
            bool res = task_service.saveTasks();
            cout << (res ? "\nData successfully saved!" : "Something went wrong, try again...");
        } else {
            cout << "\nAre you sure?(yes/no): ";
            getline(cin, ans);
            transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
            while (ans != "yes" && ans != "no") {
                cout << "\nInvalid input, try again:";
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
            }
            if (ans != "yes") {
                bool res = task_service.saveTasks();
                cout << (res ? "\nData successfully saved!" : "Something went wrong, try again...");
            }
        }
    }

}

bool isFileEmpty(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Cannot open file\n";
        return false;
    }


    streampos current_pos = file.tellg();


    file.seekg(0, ios::beg);

    int c = file.peek();


    file.seekg(current_pos);

    return c == ifstream::traits_type::eof();
}

optional<string> toOptional(const string& s) {
    if (s.empty() || s == "\t")
        return {};
    return s;
}



struct TaskInput {
    optional<string> title;
    optional<string> status;
    optional<string> description;
    optional<string> priority;
    optional<string> deadline;
    optional<string> tags;
};


TaskInput fillParameters() {
    TaskInput input;
    string tmp;
    cout << "\nEnter parameters of task or enter tab to skip parameter:";
    while (!input.title.has_value() && !flag_update) {
        cout << "\nTitle (required): ";
        getline(cin, tmp);
        if (tmp=="\t" || tmp.empty()){
            cout<<"Title is required fil, try again...";
            continue;
        }

        input.title= tmp;
    }
    if(flag_update){
        cout << "\nTitle (optional)";
        getline(cin, tmp);
        input.title= toOptional(tmp);
    }


    cout << "\nStatus (optional): ";
    getline(cin, tmp);

    while(tmp!="pending" && tmp!="done" &&tmp!="\t") {
        cout << "\nIncorrect status(pending/done), try again: ";
        getline(cin, tmp);
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    }
    input.status = toOptional(tmp);

    cout << "\nDescription (optional): ";
    getline(cin, tmp);
    input.description = toOptional(tmp);

    cout << "\nPriority (optional): ";
    getline(cin, tmp);
    while (tmp != "\t" && !tmp.empty()) {
        if (!all_of(tmp.begin(), tmp.end(), ::isdigit)) {
            cout << "\nIncorrect priority, try again: ";
            getline(cin, tmp);
            continue;
        }

        int val = stoi(tmp);
        if (val < 1 || val > 5) {
            cout << "\nIncorrect priority(from 1 to 5), try again: ";
            getline(cin, tmp);
            continue;
        }

        break;
    }
    input.priority = toOptional(tmp);

    cout << "\nDeadline (optional): ";
    getline(cin, tmp);

    while(!isValidFutureDate(tmp) && tmp!="\t") {
        cout << "\nIncorrect deadline(dd-mm-yyyy), try again: ";
        getline(cin, tmp);
    }
    input.deadline = toOptional(tmp);

    cout << "\nTags (optional): ";
    getline(cin, tmp);
    input.tags = toOptional(tmp);

    return input;
}

void menu(){
    cout<<"\nMenu:";
    cout<<"\n1. Add task"<<'\t'<<setw(25)<<"2. Update task";
    cout<<"\n3. Remove task"<<'\t'<<setw(30)<<"4. Remove all tasks";
    cout<<"\n5. Read all tasks"<<'\t'<<setw(19)<<"6. Read the task";
    cout<<"\n7. Sort task by parameter"<<setw(26)<<"8. Get task by parameter";
    cout<<"\n9. Save tasks"<<setw(29)<<"10. Delete task";
    cout<<"\n11.Exit";
    cout<<"\n---------------------------------------------------";
    cout<<"\nDanger zone: ";
    cout<<"\nType 12345 to preeminently delete all your data ";
    cout<<"\n---------------------------------------------------"<<endl;

}

string getFilePath() {
    string folder_path = string(PROJECT_SOURCE_DIR) + "/json/";;
    string file_path;
    string last_file_path = folder_path + "file_name.txt";


    fstream last_file_name(last_file_path, ios::in | ios::out | ios::binary);

    if (!last_file_name) {

        ofstream create_file(last_file_path, ios::binary);
        create_file.close();
        last_file_name.open(last_file_path, ios::in | ios::out | ios::binary);
        if(!last_file_name) {
            cout << "\nUnable to open last file!";
            return "";
        }
    }

    last_file_name.seekg(0, ios::end);
    if (last_file_name.tellg() == 0) {
        cout << "\nCreate or open file, type file name here (without path): ";
        getline(cin, file_path);

        fstream file(folder_path + file_path+".json", ios::in | ios::out | ios::binary);
        if (!file) {

            ofstream create_file(folder_path + file_path+".json", ios::binary);
            create_file.close();
        }
        file.close();


        last_file_name.seekp(0);
        last_file_name <<folder_path+ file_path+".json";
        last_file_name.close();
    } else {

        last_file_name.seekg(0);
        getline(last_file_name, folder_path);
        last_file_name.close();
        return folder_path;
    }


    return folder_path + file_path+".json";
}

int main(void) {


    int amount_of_unsaved_tasks=0;
    bool flag=true;
    bool flag_saved_data=false;


    string ans="yes";
    int number_of_command;
    int task_number;
    JsonTaskStorage js(getFilePath());
    TaskService task_service(js);

    if(!isFileEmpty(js.getFileName())) {
        bool res = task_service.loadTask();
        if (res) {
            cout << "\nData successfully loaded";
        }
        else
            cout << "\nError occurred due loading data";
    }
    int amount_of_saved_tasks=task_service.getTasKAmount();

    while(ans=="yes") {
        cout<<"\nAmount of save tasks: "<<amount_of_saved_tasks;
        cout<<"\nAmount of unsaved tasks: "<<amount_of_unsaved_tasks;
        menu();
        string cmd;
        while (true) {
            cout << "\nEnter number of command: ";
            getline(cin, cmd);
            size_t pos = 0;
            try {
                number_of_command = stoi(cmd, &pos);
            }
            catch (const exception &e) {
                cout << "\nInvalid command, try again...";
                continue;
            }


            if (pos != cmd.size()) {
                cout << "\nInvalid command, try again...";
                continue;
            }


            if ((number_of_command < 1 || number_of_command > 11 ) && number_of_command!=12345) {
                cout << "\nInvalid command number (1-10), try again.";
                continue;
            }

            break;
        }
        switch (number_of_command) {
            case 1: {
                flag_saved_data= false;
                TaskInput input = fillParameters();

                if(flag) {
                    bool res = task_service.addTask(amount_of_saved_tasks+1, input.title.value(),
                                                    stringToStatus(input.status),
                                                    input.description,
                                                    stringToPriority(input.priority),
                                                    stringToDeadline(input.deadline),
                                                    stringToTags(input.tags));

                    if (res) {
                        cout << "\nTask was successfully added"<<endl;
                        amount_of_unsaved_tasks++;
                    }
                    else
                        cout << "\nSomething went wrong:( Try to check your input...";

                }
                else
                {
                    bool res = task_service.addTask(input.title.value(), stringToStatus(input.status),
                                                    input.description,
                                                    stringToPriority(input.priority),
                                                    stringToDeadline(input.deadline),
                                                    stringToTags(input.tags));
                    if (res) {
                        cout << "\nTask was successfully added"<<endl;
                        amount_of_unsaved_tasks++;
                    }
                    else
                        cout << "\nSomething went wrong:( Try to check your input...";

                }
                break;
            }
            case 2: {
                flag_saved_data= false;
                flag_update= true;
                task_service.allTasksToString();
                cout << "\nEnter number of task you want to update: ";
                cin >> task_number;
                while (task_number > task_service.getTasKAmount()) {
                    cout << "\nIncorrect number of task, try again: ";
                    cin >> task_number;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                TaskInput input = fillParameters();
                bool res = task_service.updateTask(task_number, input.title, stringToStatus(input.status),
                                                   input.description,
                                                   stringToPriority(input.priority),
                                                   stringToDeadline(input.deadline), stringToTags(input.tags));
                if (res) {
                    cout << "\nTask was successfully updated:";
                    task_service.getTaskById(task_number).toString();
                } else
                    cout << "\nSomething went wrong:( Try to check your input...";
                flag_update= false;
                break;
            }
            case 3: {
                flag_saved_data= false;
                task_service.allTasksToString();
                cout << "\nEnter number of task to remove: ";
                cin >> task_number;
                if(cin.fail()){
                    cout<<"\nIncorrect input, try again...";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                if(task_number>amount_of_unsaved_tasks+amount_of_saved_tasks+amount_of_saved_tasks){
                    cout<<"\nIncorrect number of task, try again...";
                    continue;
                }
                if(amount_of_unsaved_tasks==0){
                    cout<<"\nNothing to remove";
                    continue;
                }
                cout << "\nYou will remove the task: ";
                task_service.getTaskById(task_number).toString();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n Are you sure?(yes/no):";
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                if (ans == "yes") {
                    bool res = task_service.removeTask(task_number);
                    if (res) {
                        cout << "\nTask was successfully removed";
                        amount_of_unsaved_tasks--;
                    }
                    else
                        cout << "\nSomething went wrong:( Try to check your input...";
                    break;
                } else
                    continue;


            }
            case 4: {

                flag_saved_data= false;
                cout << "\nAre you sure you to remove unsaved tasks?(yes/no):";
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                if (ans == "yes") {
                    bool res = task_service.removeAllTasks();
                    if (res) {
                        if(amount_of_unsaved_tasks!=0) {
                            cout << "\nTasks was successfully removed";
                            amount_of_unsaved_tasks = 0;
                        }
                        else{
                            cout<<"\nNothing to remove...";
                        }
                    } else
                        cout << "\nSomething went wrong:( Try to check your input...";
                    break;
                } else if(ans=="no") {
                    ans="yes";
                    continue;
                }
                else{
                    cout<<"\nIncorrect answer...";
                    ans="yes";
                    continue;
                }

            }

            case 5: {
                int num;
                cout<<"\n1. Show saved tasks";
                cout<<"\n2. Show unsaved tasks";
                cout<<"\n3. Show all tasks";
                cout<<"\nEnter what type of tasks to show: ";
                if(cin>>num) {
                    if (num == 1) {
                        if (amount_of_saved_tasks != 0) {
                            for (const Task &task: task_service.getAllTasks().getAllTasks() |
                                                   views::take(amount_of_saved_tasks ))
                                task.toString();
                        } else {
                            cout << "\nNothing to show...";
                        }
                    }
                     else if (num == 2) {
                        if (amount_of_unsaved_tasks != 0) {
                            for (const Task &task: task_service.getAllTasks().getAllTasks() |
                                                   views::drop(amount_of_saved_tasks ) |
                                                   views::take(amount_of_unsaved_tasks ))
                                task.toString();
                        }
                        else{
                            cout << "\nNothing to show...";
                        }

                    }
                    else if(num==3){
                        task_service.allTasksToString();
                    }
                     else {
                        cout << "\nInvalid number, try again"<<endl;
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        continue;
                    }
                } else{
                    cout<<"\nInvalid input, try again"<<endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                flag_saved_data= false;
                break;
            }
            case 6: {

                cout << "\nEnter a number of the task:";
                cin >> task_number;
                if(amount_of_unsaved_tasks+amount_of_saved_tasks>0)
                    task_service.getTaskById(task_number).toString();
                else
                    cout<<"\nNothing to show...";
                flag_saved_data= false;
                break;
            }
            case 7: {
                cout << "\nEnter parameter you want to sort by: ";
                string parameter;
                getline(cin,parameter);
                transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);

                bool res = false;
                if (parameter == "titleasc")
                    res = task_service.sortBy(SortType::titleAsc);
                else if (parameter == "titledesc")
                    res = task_service.sortBy(SortType::titleDesc);
                else if (parameter == "statusasc")
                    res = task_service.sortBy(SortType::statusAsc);
                else if (parameter == "statusdesc")
                    res = task_service.sortBy(SortType::statusDesc);
                else if (parameter == "priorityasc")
                    res = task_service.sortBy(SortType::priorityAsc);
                else if (parameter == "prioritydesc")
                    res = task_service.sortBy(SortType::priorityDesc);
                else if (parameter == "deadlineasc")
                    res = task_service.sortBy(SortType::deadlineAsc);
                else if (parameter == "deadlinedesc")
                    res = task_service.sortBy(SortType::deadlineDesc);
                else if (parameter == "tagsasc")
                    res = task_service.sortBy(SortType::tagsAsc);
                else if(parameter=="tagsdesc")
                    res = task_service.sortBy(SortType::tagsDesc);
                else{
                    cout<<"\nInvalid parameter(titleasc, titledesc, statusasc, statusdesc, priorityasc,"
                          " prioritydesc, deadlineasc, deadlinedesc, tagsasc, tagsdesc), try again...";
                }
                if (res)
                    task_service.allTasksToString();
                else
                    cout << "\nSomething went wrong:( Try to check your input...";
                flag_saved_data= false;
                break;
            }
            case 8: {

                cout << "\nEnter parameter you want to get by: ";
                string parameter;
                string parameter_name;
                string _deadline;
                int pr;
                getline(cin,parameter);
                while (parameter!="title" && parameter!="tags" && parameter!="status"
                       && parameter!="priority" && parameter!="deadline") {
                    cout << "\nIncorrect parameter name(title, tags, status, priority, deadline), try again:";
                    getline(cin, parameter);
                }
                    transform(parameter.begin(), parameter.end(), parameter.begin(), ::tolower);

                    if (parameter == "priority") {
                        cout << "\nEnter priority from 1 to 5:";
                        cin >> pr;

                    } else if (parameter == "deadline") {
                        cout << "\nEnter deadline (dd-mm-yyyy):";
                        getline(cin, _deadline);

                    } else if (parameter == "tags") {
                        cout << "\nEnter name of tag: ";
                        getline(cin, parameter_name);

                    }else if (parameter == "title") {
                        cout << "\nEnter name of title: ";
                        getline(cin, parameter_name);

                    }
                    else if (parameter == "status") {
                        cout << "\nEnter name of status(pending/done): ";
                        getline(cin, parameter_name);

                    }




                vector<Task> vres;
                if (parameter == "title")
                    vres = task_service.getBy(GetType::title, parameter_name);
                if (parameter == "tags")
                    vres = task_service.getBy(GetType::tags, parameter_name);
                if (parameter == "status" && parameter_name == "done")
                    vres = task_service.getBy(Status::Done);
                if (parameter == "status" && parameter_name == "pending")
                    vres = task_service.getBy(Status::Pending);
                if (parameter == "priority")
                    vres = task_service.getBy(pr);
                if (parameter == "deadline") {
                    if (isValidFutureDate(parameter_name))
                        vres = task_service.getBy(stringToDeadline(_deadline));
                    else
                        cout<<"\nInvalid date, try again...";
                }
                if(!vres.empty()) {
                    for (const Task &task: vres)
                        task.toString();
                } else{
                    cout<<"\nNothing to show...";
                }
                flag_saved_data= false;
                break;
            }
            case 9: {

                bool res = task_service.saveTasks();
                if (res) {
                    cout << "\nTasks was successfully saved";
                    amount_of_saved_tasks+=amount_of_unsaved_tasks;
                    amount_of_unsaved_tasks=0;
                    flag_saved_data= true;
                }
                else
                    cout << "\nSomething went wrong:( Try to check your input...";

                break;
            }

            case 10:{
                flag_saved_data= false;
                task_service.allTasksToString();
                cout << "\nEnter number of task to delete from file: ";
                cin >> task_number;
                if(cin.fail()){
                    cout<<"\nIncorrect input, try again...";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                if(task_number>amount_of_saved_tasks){
                    cout<<"\nIncorrect number of task, try again...";
                    continue;
                }
                cout << "\nYou will delete the task: ";
                task_service.getTaskById(task_number).toString();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n Are you sure?(yes/no):";
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                if (ans == "yes") {
                    bool res = task_service.deleteTaskFromFile(task_number);
                    if (res) {
                        cout << "\nTask was successfully deleted";
                        amount_of_saved_tasks--;
                    }
                    else
                        cout << "\nSomething went wrong:( Try to check your input...";
                    break;
                } else
                    continue;

            }
            case 11:{

                warningToSaveData(flag_saved_data, amount_of_unsaved_tasks, ans, task_service);
                cout<<"\nBye!";
                return 0;


            }
            case 12345: {
                cout << "\nDo you Do you want to preeminently delete all data?(yes/no): ";
                getline(cin, ans);
                transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                while (ans != "yes" && ans != "no") {
                    cout << "\nInvalid input, try again:";
                    getline(cin, ans);
                    transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                }
                if (ans == "yes") {
                    cout << "\nAre you sure?(yes/no): ";
                    getline(cin, ans);
                    transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                    while (ans != "yes" && ans != "no") {
                        cout << "\nInvalid input, try again:";
                        getline(cin, ans);
                        transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
                    }
                    if (ans == "yes") {
                        bool res = js.clearStorage();
                        cout << (res ? "\nSaved data successfully deleted!" : "Something went wrong, try again...");
                        res=task_service.removeAllTasks();
                        cout << (res ? "\nUnsaved data successfully deleted!" : "Something went wrong, try again...");
                        amount_of_saved_tasks=0;
                        amount_of_unsaved_tasks=0;
                    }

                }


                flag_saved_data= false;
                break;
            }
        }
        cout << "\nDo you want to proceed?(yes/no)";
        if (cin.peek() == '\n' || cin.peek() == EOF)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, ans);
        transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
        while (ans != "yes" && ans != "no") {
            cout << "\nInvalid input, try again:";
            getline(cin, ans);
            transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
        }
        flag= false;
    }
    warningToSaveData(flag_saved_data, amount_of_unsaved_tasks, ans, task_service);

    cout<<"\nBye!";
    return 0;
}
