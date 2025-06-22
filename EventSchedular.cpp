#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

// Structure to represent an event
struct Event {
    string name;
    int priority;  // Lower value indicates higher priority
    int time;      // Time of the event (in 24-hour format, e.g., 1430 for 2:30 PM)
    string description; // Description of the event

    // Overloading the < operator for priority queue comparison
    bool operator<(const Event& other) const {
        if (priority == other.priority) {
            return time > other.time; // Earlier time gets higher priority
        }
        return priority > other.priority;
    }
};

class EventScheduler {
private:
    priority_queue<Event> eventQueue;
    unordered_map<string, Event> eventMap; // For quick lookup and management of events

public:
    // Add an event to the scheduler
    void addEvent(const string& name, int priority, int time, const string& description) {
        if (eventMap.find(name) != eventMap.end()) {
            cout << "Event with this name already exists.\n";
            return;
        }

        Event newEvent{name, priority, time, description};
        eventQueue.push(newEvent);
        eventMap[name] = newEvent;
        cout << "Event '" << name << "' added successfully.\n";
    }

    // Display all scheduled events in priority order
    void displayEvents() {
        if (eventQueue.empty()) {
            cout << "No events scheduled.\n";
            return;
        }

        priority_queue<Event> tempQueue = eventQueue;
        cout << "Scheduled Events (Priority Order):\n";
        while (!tempQueue.empty()) {
            Event current = tempQueue.top();
            tempQueue.pop();
            cout << "- " << current.name << " at " << formatTime(current.time)
                 << " [Priority: " << current.priority << "]\n  Description: " << current.description << "\n";
        }
    }

    // Process the next event in the queue
    void processNextEvent() {
        if (eventQueue.empty()) {
            cout << "No events to process.\n";
            return;
        }

        Event nextEvent = eventQueue.top();
        eventQueue.pop();
        eventMap.erase(nextEvent.name);
        cout << "Processing event: " << nextEvent.name
             << " at " << formatTime(nextEvent.time) << "\n  Description: " << nextEvent.description << "\n";
    }

    // Remove an event by name
    void removeEvent(const string& name) {
        if (eventMap.find(name) == eventMap.end()) {
            cout << "Event not found.\n";
            return;
        }

        eventMap.erase(name);
        rebuildQueue();
        cout << "Event '" << name << "' removed successfully.\n";
    }

    // Save events to a file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Failed to open file for saving.\n";
            return;
        }

        priority_queue<Event> tempQueue = eventQueue;
        while (!tempQueue.empty()) {
            Event current = tempQueue.top();
            tempQueue.pop();
            outFile << current.name << "|" << current.priority << "|" << current.time << "|" << current.description << "\n";
        }
        outFile.close();
        cout << "Events saved to " << filename << " successfully.\n";
    }

    // Load events from a file
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "Failed to open file for loading.\n";
            return;
        }

        eventQueue = priority_queue<Event>(); // Clear existing events
        eventMap.clear();

        string line;
        while (getline(inFile, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);

            string name = line.substr(0, pos1);
            int priority = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int time = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            string description = line.substr(pos3 + 1);

            addEvent(name, priority, time, description);
        }
        inFile.close();
        cout << "Events loaded from " << filename << " successfully.\n";
    }

    // Format time from 24-hour integer to a readable format
    string formatTime(int time) {
        int hours = time / 100;
        int minutes = time % 100;
        string period = (hours >= 12) ? "PM" : "AM";
        if (hours > 12) hours -= 12;
        if (hours == 0) hours = 12;
        char formattedTime[6];
        sprintf(formattedTime, "%02d:%02d", hours, minutes);
        return string(formattedTime) + " " + period;
    }

private:
    // Rebuild the priority queue from the event map
    void rebuildQueue() {
        eventQueue = priority_queue<Event>();
        for (const auto& pair : eventMap) {
            eventQueue.push(pair.second);
        }
    }
};

int main() {
    EventScheduler scheduler;
    int choice;

    cout << "Event Scheduler\n";
    do {
        cout << "\nMenu:\n";
        cout << "1. Add Event\n";
        cout << "2. Display Events\n";
        cout << "3. Process Next Event\n";
        cout << "4. Remove Event\n";
        cout << "5. Save Events to File\n";
        cout << "6. Load Events from File\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, description;
                int priority, time;
                cout << "Enter event name: ";
                cin.ignore(); // Clear input buffer
                getline(cin, name);
                cout << "Enter priority (lower number = higher priority): ";
                cin >> priority;
                cout << "Enter time (in 24-hour format, e.g., 1430 for 2:30 PM): ";
                cin >> time;
                cin.ignore(); // Clear input buffer
                cout << "Enter event description: ";
                getline(cin, description);
                scheduler.addEvent(name, priority, time, description);
                break;
            }
            case 2:
                scheduler.displayEvents();
                break;
            case 3:
                scheduler.processNextEvent();
                break;
            case 4: {
                string name;
                cout << "Enter event name to remove: ";
                cin.ignore(); // Clear input buffer
                getline(cin, name);
                scheduler.removeEvent(name);
                break;
            }
            case 5: {
                string filename;
                cout << "Enter filename to save events: ";
                cin >> filename;
                scheduler.saveToFile(filename);
                break;
            }
            case 6: {
                string filename;
                cout << "Enter filename to load events: ";
                cin >> filename;
                scheduler.loadFromFile(filename);
                break;
            }
            case 7:
                cout << "Exiting Event Scheduler.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
