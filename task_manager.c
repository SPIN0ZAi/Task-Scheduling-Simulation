#include <stdio.h>


#define MAX_TASKS 500
#define MAX_DURATION 1000
#define MAX_PRIORITY 25

// Structure to represent a task
typedef struct {
    int ID;
    int duration;
    int priority;
    int dependencies[MAX_TASKS];
    int depen_num;
    int completed;
} Task;

// Structure to manage the entire scheduler
typedef struct {
    Task tasks[MAX_TASKS];
    int total_tasks;
    int total_time;
    int order[MAX_TASKS];
    int executed;
} TaskScheduler;

// Function declarations
void initializing(TaskScheduler *simulation);
void input_tasks(TaskScheduler *simulation);
void input_dependencies(TaskScheduler *simulation, int task_id);
void execution(TaskScheduler *simulation);
void show_status(TaskScheduler *simulation);
void results_report(TaskScheduler *simulation);

// Initialize the task scheduler
void initializeScheduler(TaskScheduler *simulation) {
    simulation-> total_tasks = 0;
    simulation-> total_time = 0;
    simulation-> executed = 0;

    for (int i = 0; i < MAX_TASKS; i++) {
        simulation->tasks[i].completed = 0;
        simulation->tasks[i].depen_num = 0;
    }
}

// Input tasks from the user
void input_tasks(TaskScheduler *simulation) {
    printf("Enter the total number of tasks: ");
    scanf("%d", &simulation->total_tasks);  // Read total number of tasks

    if (simulation->total_tasks > MAX_TASKS) {
        printf("ERROR - Too many tasks!\n");
        return;
    }

    // Input task details (duration, priority)
    for (int i = 0; i < simulation->total_tasks; i++) {
        printf("for task ID #%d:\n", i);
        do {
            printf("Enter task duration (in seconds, must be positive):\n ");
            scanf("%d", &simulation->tasks[i].duration);
        } while (simulation->tasks[i].duration <= 0);  // Ensure duration is positive

        printf("Enter task priority (lower values indicate higher priority):\n ");
        scanf("%d", &simulation->tasks[i].priority);

        simulation->tasks[i].ID = i;  // Assign task ID
        simulation->tasks[i].depen_num = 0;  // Initialize dependencies count
        simulation->tasks[i].completed = 0;  // Task is not completed initially

        input_dependencies(simulation, i);  // Get dependencies for the task
    }
}

// Input dependencies for a task
void input_dependencies(TaskScheduler *simulation, int task_id) {
    Task *task = &simulation->tasks[task_id];  // Access the task with the given task_id
    int dependency;

    printf("Enter dependencies for task %d (end it with -1):\n", task_id);

    // Reset dependencies before taking new input
    task->depen_num = 0;

    // Loop to get dependencies for the current task
    while(1) {
        scanf("%d", &dependency);  // Read the dependency task_id

        // End input if dependency is -1
        if (dependency == -1) {
            break;
        }

        // Check if the entered dependency is valid
        if (dependency < 0 || dependency >= task_id) {
            printf("Invalid dependency:( Task %d cannot depend on itself or on a future task.\n", task_id);
            return;  // Exit if invalid dependency is found
        }

        // Check for duplicate dependencies
        for (int i = 0; i < task->depen_num; i++) {
            if (task->dependencies[i] == dependency) {
                printf("ERROR - Task %d is already a dependency!\n", dependency);
                return;  // Exit if duplicate dependency is found
            }
        }

        // Add the valid dependency to the dependencies list
        task->dependencies[task->depen_num++] = dependency;
    }
}

// Execute task scheduling based on priority and dependencies
void execution(TaskScheduler *simulation) {
    int started [MAX_TASKS] = {0};  // Track which tasks have been started
    int time_taken = 0;
    int n = 0;  // To keep track of where to store the task ID in execution_order

    // Execute tasks in the order of their dependencies and priority
    for (int i = 0; i < simulation->total_tasks; i++) {
        // Find the task that can be executed next (no pending dependencies)
        for (int j = 0; j < simulation->total_tasks; j++) {
            Task *task = &simulation->tasks[j];

            // Check if the task is not started and its dependencies are completed
            if (!started[j]) {
                int can_execute = 1;
                for (int k = 0; k < task->depen_num; k++) {
                    if (!started[task->dependencies[k]]) {
                        can_execute = 0;  // Cannot execute if a dependency is not completed
                        break;
                    }
                }

                if (can_execute) {
                    printf("Task %d has started...\n", j);
                    started[j] = 1;
                    time_taken += task->duration;
                    task->completed = 1;  // Mark task as completed
                    simulation->order[n++] = j;  // Store task ID in execution order
                    printf("Task %d has been completed in %d seconds.\n", j, task->duration);
                }
            }
        }
    }
    printf("Task scheduling completed.\n");
    printf("Total time taken: %d seconds\n", time_taken);
    printf("Task scheduling completed.\n");
    printf("Total time taken: %d seconds\n", time_taken);
}

// Show the status of all tasks
void show_status(TaskScheduler *simulation) {
    printf("Status of all tasks:\n");
    for (int i = 0; i < simulation->total_tasks; i++) {
        Task *task = &simulation->tasks[i];
        printf("Task #%d - Duration: %d sec, Priority: %d, Status: %s, Dependencies: ",task->ID, task->duration, task->priority);
               if (task->completed) {
               printf("completed");
               } else {
               printf("pending");
               }
        if (task->depen_num == 0) {
            printf("none\n");
        } else {
            for (int j = 0; j < task->depen_num; j++) {
                printf("%d ", task->dependencies[j]);
            }
            printf("\n");
        }
    }
}

// Show the results report
void results_report(TaskScheduler *simulation) {
    printf("Task scheduling results report: \n");
    int total_time = 0;

    for (int i = 0; i < simulation->total_tasks; i++) {
        Task *task = &simulation->tasks[i];
        printf("Task #%d - Duration: %d sec, Priority: %d, Status: ", task->ID, task->duration, task->priority);
        if (task->completed) {
            printf("completed\n");
            total_time += task->duration;
        } else {
            printf("pending\n");
        }
    }

    printf("Total time: %d seconds.\n", total_time);
}
// Main menu
int main() {
    int choice;
    TaskScheduler scheduler;
    int mode;

    printf("Select mode (0 for Judge Mode, 1 for User Mode): ");
    scanf("%d", &mode);
    initializeScheduler(&scheduler);
    if (mode == 0) {
        // Judge mode: Skip menu and just process tasks and display results
        results_report(&scheduler);
    } else if (mode == 1) {
        // User mode: Display menu and interact with the user

        do {
            printf("\nMenu: \n");
            printf("1. Enter tasks\n");
            printf("2. Execute task scheduling\n");
            printf("3. Show the status of all tasks\n");
            printf("4. View results report\n");
            printf("5. Exit\n");
            printf("Choose an option: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    input_tasks(&scheduler);
                break;
                case 2:
                    execution(&scheduler);
                break;
                case 3:
                    show_status(&scheduler);
                break;
                case 4:
                    results_report(&scheduler);
                break;
                case 5:
                    printf("Exiting...\n");
                break;
                default:
                    printf("Invalid choice, please try again.\n");
                break;
            }
        } while (choice != 5);
        return 0;
    }
}


