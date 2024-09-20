#include <iostream>  // For input/output operations
#include <vector>    // For dynamic arrays
#include <string>    // For string manipulation
#include <algorithm> // For sorting and searching algorithms
#include <fstream>   // For file input/output operations
#include <sstream>   // For string stream processing
#include <ctime>     // For time-related functions
#include <iomanip>   // For input/output manipulators
#include <chrono>    // For time-related utilities

// Use the standard namespace
using namespace std;

// Define the Task class to represent individual tasks
class Task
{
private:
    int id;
    string description;
    string category;
    int priority;
    time_t dueDate;
    bool completed;

public:
    // Constructor
    Task(int id, const string &desc, const string &cat, int prio, time_t due)
        : id(id), description(desc), category(cat), priority(prio), dueDate(due), completed(false) {}

    // Getter methods
    int getId() const { return id; }
    string getDescription() const { return description; }
    string getCategory() const { return category; }
    int getPriority() const { return priority; }
    time_t getDueDate() const { return dueDate; }
    bool isCompleted() const { return completed; }

    // Setter methods
    void setDescription(const string &desc) { description = desc; }
    void setCategory(const string &cat) { category = cat; }
    void setPriority(int prio) { priority = prio; }
    void setDueDate(time_t due) { dueDate = due; }
    void setCompleted(bool status) { completed = status; }

    // Display task details
    void display() const
    {
        tm *localTime = localtime(&dueDate);
        cout << "ID: " << id << ", Description: " << description
             << ", Category: " << category << ", Priority: " << priority
             << ", Due: " << put_time(localTime, "%Y-%m-%d %H:%M:%S")
             << ", Completed: " << (completed ? "Yes" : "No") << endl;
    }
};

// Define the TaskManager class to manage multiple tasks
class TaskManager
{
private:
    vector<Task> tasks;
    int nextId;
    string filename;

public:
    // Constructor
    TaskManager(const string &file) : nextId(1), filename(file)
    {
        loadTasks();
    }

    // Destructor
    ~TaskManager()
    {
        saveTasks();
    }

    // Add a new task
    void addTask(const string &desc, const string &cat, int prio, int hours)
    {
        time_t now = time(nullptr);
        time_t due = now + hours * 3600; // Convert hours to seconds
        tasks.push_back(Task(nextId++, desc, cat, prio, due));
        saveTasks();
    }

    // Edit an existing task
    void editTask(int id, const string &desc, const string &cat, int prio, int hours)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                task.setDescription(desc);
                task.setCategory(cat);
                task.setPriority(prio);
                time_t now = time(nullptr);
                time_t due = now + hours * 3600; // Convert hours to seconds
                task.setDueDate(due);
                saveTasks();
                break;
            }
        }
    }

    // Edit task description
    void editTaskDescription(int id, const string &desc)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                task.setDescription(desc);
                saveTasks();
                break;
            }
        }
    }

    // Edit task category
    void editTaskCategory(int id, const string &cat)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                task.setCategory(cat);
                saveTasks();
                break;
            }
        }
    }

    // Edit task priority
    void editTaskPriority(int id, int prio)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                task.setPriority(prio);
                saveTasks();
                break;
            }
        }
    }

    // Edit task due date
    void editTaskDueDate(int id, int hours)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                time_t now = time(nullptr);
                time_t due = now + hours * 3600; // Convert hours to seconds
                task.setDueDate(due);
                saveTasks();
                break;
            }
        }
    }

    // Delete a task
    void deleteTask(int id)
    {
        tasks.erase(remove_if(tasks.begin(), tasks.end(),
                              [id](const Task &t)
                              { return t.getId() == id; }),
                    tasks.end());
        saveTasks();
    }

    // Mark a task as complete
    void markAsComplete(int id)
    {
        for (auto &task : tasks)
        {
            if (task.getId() == id)
            {
                task.setCompleted(true);
                saveTasks();
                break;
            }
        }
    }

    // Display all tasks
    void displayAllTasks() const
    {
        if (tasks.empty())
        {
            cout << "No tasks to display." << endl;
        }
        else
        {
            for (const auto &task : tasks)
            {
                task.display();
            }
        }
    }

    // Save tasks to a file
    void saveTasks() const
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (const auto &task : tasks)
            {
                file << task.getId() << "|" << task.getDescription() << "|"
                     << task.getCategory() << "|" << task.getPriority() << "|"
                     << task.getDueDate() << "|"
                     << task.isCompleted() << "\n";
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file for saving tasks.\n";
        }
    }

    // Load tasks from a file
    void loadTasks()
    {
        ifstream file(filename);
        if (file.is_open())
        {
            tasks.clear();
            string line;
            while (getline(file, line))
            {
                istringstream iss(line);
                string token;
                vector<string> tokens;

                while (getline(iss, token, '|'))
                {
                    tokens.push_back(token);
                }

                if (tokens.size() == 6)
                {
                    int id = stoi(tokens[0]);
                    string description = tokens[1];
                    string category = tokens[2];
                    int priority = stoi(tokens[3]);
                    time_t dueDate = stoll(tokens[4]);
                    bool completed = (tokens[5] == "1");

                    Task task(id, description, category, priority, dueDate);
                    task.setCompleted(completed);
                    tasks.push_back(task);
                    if (id >= nextId)
                    {
                        nextId = id + 1;
                    }
                }
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file for loading tasks.\n";
        }
    }
};

// Display the main menu
void displayMenu()
{
    cout << "\n=== Task Management System ===\n"
         << "1. Add Task\n"
         << "2. Edit Task\n"
         << "3. Delete Task\n"
         << "4. Mark Task as Complete\n"
         << "5. Display All Tasks\n"
         << "0. Exit\n"
         << "Enter your choice: ";
}

// Main function
int main()
{
    string filename = "tasks.txt";
    TaskManager manager(filename);
    int choice;

    do
    {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1: // Add Task
        {
            string desc, cat;
            int prio, hours;

            cout << "Enter task description: ";
            getline(cin, desc);
            cout << "Enter task category: ";
            getline(cin, cat);
            cout << "Enter priority (1-5, or any other number for no priority): ";
            cin >> prio;
            if (prio < 1 || prio > 5)
            {
                prio = 0; // Set priority to 0 for "no priority"
                cout << "Priority set to 'no priority'.\n";
            }
            cin.ignore();
            cout << "Enter number of hours until due: ";
            cin >> hours;
            cin.ignore();

            manager.addTask(desc, cat, prio, hours);
            break;
        }
        case 2: // Edit Task
        {
            int id;
            cout << "Enter task ID to edit: ";
            cin >> id;
            cin.ignore();

            int editChoice;
            cout << "What would you like to edit?\n"
                 << "1. Description\n"
                 << "2. Category\n"
                 << "3. Priority\n"
                 << "4. Due Date\n"
                 << "5. All fields\n"
                 << "Enter your choice: ";
            cin >> editChoice;
            cin.ignore();

            string desc, cat;
            int prio, hours;

            switch (editChoice)
            {
            case 1:
                cout << "Enter new description: ";
                getline(cin, desc);
                manager.editTaskDescription(id, desc);
                break;
            case 2:
                cout << "Enter new category: ";
                getline(cin, cat);
                manager.editTaskCategory(id, cat);
                break;
            case 3:
                cout << "Enter new priority (1-5): ";
                cin >> prio;
                manager.editTaskPriority(id, prio);
                break;
            case 4:
                cout << "Enter new number of hours until due: ";
                cin >> hours;
                manager.editTaskDueDate(id, hours);
                break;
            case 5:
                cout << "Enter new description: ";
                getline(cin, desc);
                cout << "Enter new category: ";
                getline(cin, cat);
                cout << "Enter new priority (1-5): ";
                cin >> prio;
                cin.ignore();
                cout << "Enter new number of hours until due: ";
                cin >> hours;
                cin.ignore();
                manager.editTask(id, desc, cat, prio, hours);
                break;
            default:
                cout << "Invalid choice. No changes made.\n";
            }
            break;
        }
        case 3: // Delete Task
        {
            int id;
            cout << "Enter task ID to delete: ";
            cin >> id;
            manager.deleteTask(id);
            break;
        }
        case 4: // Mark Task as Complete
        {
            int id;
            cout << "Enter task ID to mark as complete: ";
            cin >> id;
            manager.markAsComplete(id);
            break;
        }
        case 5: // Display All Tasks
            manager.displayAllTasks();
            break;
        case 0: // Exit
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}