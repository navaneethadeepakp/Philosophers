# Philosophers

A multithreaded simulation written in C that solves the classic Dining Philosophers problem. This project demonstrates advanced process synchronization, memory management, and deadlock prevention using POSIX threads and mutexes.

## Architecture & Concurrency Mechanics

This system models philosophers as independent threads that must share limited resources (forks) to eat, sleep, and think without starving.

*   **Thread Management:** Each philosopher operates as an independent POSIX thread initialized via `pthread_create`.
*   **Deadlock Prevention:** Forks are represented by individual mutexes. Circular wait deadlocks are structurally prevented by enforcing a strict locking hierarchy where philosophers always lock their lower-indexed fork before their higher-indexed fork.
*   **Race Condition Protection:** Shared states, meal tracking, and terminal outputs are strictly protected by dedicated simulation and write mutexes, entirely preventing data races and scrambled output.
*   **Asynchronous Supervisor:** A dedicated monitoring routine continuously evaluates the timestamp of each philosopher's last meal to instantly detect starvation (`time_to_die`) or verify if the global meal requirement has been met.
*   **Precise Thread Synchronization:** A global `start_flag` synchronized via a mutex ensures all threads begin their lifecycles simultaneously for accurate timekeeping.
*   **Microsecond Timing:** Implements a custom `smart_sleep` routine that yields the CPU while checking the simulation's active status in 500-microsecond intervals, allowing threads to exit immediately upon termination rather than waiting for long sleep cycles to finish.

## Build & Usage

The project is compiled using a strict Makefile enforcing `-Wall -Wextra -Werror -pthread` flags.

### Compilation

```bash
make
```

### Execution

```
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat (optional)]
```

### Example

```
./philo 5 800 200 200 7
```
(Runs a simulation with 5 philosophers who die if they don't eat within 800ms, take 200ms to eat, take 200ms to sleep, and the simulation ends when all have eaten at least 7 times).
