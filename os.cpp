#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// Node structure for the process queue
struct Process {
    string name;
    int size;
    int id;
    int rt;     // remaining time / burst time
    int pr;     // priority
    Process* next;

    Process() : next(nullptr) {}
};

class ProcessScheduler {
private:
    Process* start;

    // Helper: swap data between two nodes (without changing links)
    void swapData(Process* a, Process* b) {
        swap(a->name, b->name);
        swap(a->size, b->size);
        swap(a->id,   b->id);
        swap(a->rt,   b->rt);
        swap(a->pr,   b->pr);
    }

    
    void bubbleSort(int n, bool (*compare)(Process*, Process*)) {
        if (n <= 1) return;

        for (int i = 0; i < n - 1; i++) {
            Process* current = start;
            for (int j = 0; j < n - i - 1; j++) {
                Process* nextNode = current->next;
                if (compare(current, nextNode)) {
                    swapData(current, nextNode);
                }
                current = current->next;
            }
        }
    }

public:
    ProcessScheduler() : start(nullptr) {}

    ~ProcessScheduler() {
        Process* current = start;
        while (current != nullptr) {
            Process* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insertProcess(int id, const string& name, int size, int rt, int pr) {
        Process* newProcess = new Process();
        newProcess->name = name;
        newProcess->size = size;
        newProcess->id   = id;
        newProcess->rt   = rt;
        newProcess->pr   = pr;

        if (start == nullptr) {
            start = newProcess;
        } else {
            Process* temp = start;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newProcess;
        }
    }

    void priorityScheduling(int n) {
        // Higher priority = lower number 
        auto cmp = [](Process* a, Process* b) {
            return a->pr > b->pr;  
        };
        bubbleSort(n, cmp);
    }

    void sjf(int n) {
        // Shortest Job First → sort by burst time (rt)
        auto cmp = [](Process* a, Process* b) {
            return a->rt > b->rt;
        };
        bubbleSort(n, cmp);
    }

    void fcfs(int n) {
        // First Come First Serve → sort by arrival/order (id)
        auto cmp = [](Process* a, Process* b) {
            return a->id > b->id;
        };
        bubbleSort(n, cmp);
    }

    void display(int n) const {
        cout << "\n" << string(40, '=') << endl;
        cout << "     ORDER OF PROCESSES\n";
        cout << string(40, '=') << endl;

        Process* temp = start;
        int count = 1;
        while (temp != nullptr && count <= n) {
            cout << count << ". " << left << setw(20) << temp->name
                 << " | ID: " << temp->id
                 << " | Priority: " << temp->pr
                 << " | Burst: " << temp->rt << " ms" << endl;
            temp = temp->next;
            count++;
        }
        cout << string(40, '=') << "\n\n";
    }
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    ProcessScheduler scheduler;

    cout << "\nEnter details for " << n << " processes:\n\n";
    for (int i = 1; i <= n; i++) {
        string name;
        int size, rt, pr;

        cout << "Process " << i << ":\n";
        cout << "  Name             : "; cin >> name;
        cout << "  Size (KB)        : "; cin >> size;
        cout << "  Burst time (ms)  : "; cin >> rt;
        cout << "  Priority (1-" << n << ") : "; cin >> pr;
        cout << "----------------------------------------\n";

        scheduler.insertProcess(i, name, size, rt, pr);
    }

    scheduler.display(n);

    while (true) {
        cout << "\nCPU SCHEDULING MENU\n";
        cout << "1. Priority Scheduling\n";
        cout << "2. Shortest Job First (SJF)\n";
        cout << "3. First Come First Serve (FCFS)\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 4) {
            cout << "Exiting...\n";
            break;
        }

        switch (choice) {
            case 1:
                scheduler.priorityScheduling(n);
                cout << "\nAfter PRIORITY Scheduling :\n";
                scheduler.display(n);
                break;

            case 2:
                scheduler.sjf(n);
                cout << "\nAfter SJF Scheduling:\n";
                scheduler.display(n);
                break;

            case 3:
                scheduler.fcfs(n);
                cout << "\nAfter FCFS Scheduling:\n";
                scheduler.display(n);
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}