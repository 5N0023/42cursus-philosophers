
# Philosophers

## Introduction

The Philosophers project is a practical approach to understanding the complexities of concurrent programming, specifically through the lens of the dining philosophers problem. This simulation aims to explore synchronization issues, deadlock avoidance, and resource management among philosophers who need to share resources (forks) to complete their tasks (eating, thinking, sleeping).

## Mandatory

### Project Description

The core of the project involves creating a simulation where each philosopher is represented by a thread. Philosophers must share forks (mutexes) placed between them to eat, and they alternate between eating, thinking, and sleeping. The challenge is to implement a solution that avoids deadlocks and ensures no philosopher starves.

### Implementation Details

-   **Threads:** Each philosopher is represented by a thread.
-   **Mutexes:** Forks are represented by mutexes to manage access.
-   **Philosopher States:** Manage and display states - eating, thinking, sleeping.
-   **Deadlock Avoidance and Starvation Prevention:** Implement strategies to prevent deadlocks and ensure fairness.

### Running the Simulation

1.  **Compilation:**


```bash
gcc -pthread philosophers.c -o philosophers
```

2.  **Execution:**

```bash
./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

## Bonus

### Enhanced Features with Processes

For the bonus part, the project evolves by integrating processes within threads. This approach simulates each philosopher not only as a thread but also encapsulates each thread in its own process. This method aims to delve deeper into the synchronization between processes and threads, showcasing a hybrid model of concurrency.

### Implementation Details

-   **Processes:** Each philosopher is now additionally encapsulated within a process, providing an extra layer of separation and potentially a different synchronization challenge.
-   **Threads within Processes:** Maintain the use of threads within these processes to represent individual philosophers, exploring the dynamics between threaded and process-based concurrency.

### Running the Enhanced Simulation

The compilation might remain similar, but execution may require additional parameters or considerations due to the introduction of processes.

### Challenges and Opportunities

-   **Complex Synchronization:** Managing synchronization across both threads and processes introduces new challenges and learning opportunities.
-   **Performance Analysis:** Investigate the impact of using processes in addition to threads on the performance and efficiency of the simulation.
-   **Resource Management:** Explore advanced resource management strategies suitable for a hybrid concurrency model.
