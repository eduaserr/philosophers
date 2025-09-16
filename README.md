# philosophers
# philosophers
<div>
    <img align="left" alt="philosophers" src="https://github.com/eduaserr/42-project-badges/blob/main/badges/philosopherse.png" width="75px"><img align="right" alt="philosophers" src="https://github.com/eduaserr/42-project-badges/blob/main/badges/philosophersn.png" width="75px">
    <h1 align="center"> ~PHILOSOPHERS~ <br><img alt="success 100/100" src="https://img.shields.io/badge/100%2F100-green?style=plastic&logoColor=green&label=success"></h1>
</div>
<p align="center">Multithreading simulation solving the dining philosophers problem.<br>Focuses on preventing deadlock, starvation, and data races through proper mutex management.</p>

### PROJECT STRUCTURE
```
philosophers/
├── Makefile
├── main.c
├── bash.sh
├── hellgrind.sh
├── valgrind.supp
├── inc/
│   └── philo.h
├── lib/
│   ├── ft_atoi.c
│   ├── ft_isdigit.c
│   └── ft_strcmp.c
└── src/
    ├── free/
    │   └── free.c
    ├── init/
    │   ├── init_parser.c
    │   └── init_table.c
    ├── simulation/
    │   ├── actions.c
    │   ├── routine_utils.c
    │   ├── routines.c
    │   └── time.c
    └── utils/
        ├── utils_error.c
        ├── utils_print.c
        └── utils.c
```

### Module Organization:

📁 **Main files**
- [main.c](./main.c) - Main function and simulation control
- [philo.h](./inc/philo.h) - Header with all structures and definitions

📁 **[lib/](./lib)**
- Basic auxiliary functions (atoi, isdigit, strcmp)

📁 **[src/free/](./src/free)**
- Memory and resource management

📁 **[src/init/](./src/init)**
- Table and philosophers initialization, argument validation

📁 **[src/simulation/](./src/simulation)**
- Core logic: philosopher routines, actions (eat, sleep, think) and time control

📁 **[src/utils/](./src/utils)**
- Utility functions for errors, safe printing and mutex utilities

### DATA STRUCTURES

```c
typedef struct s_philo {
    int                id;          // Philosopher ID
    int                meals;       // Meals completed
    long               last_meal;   // Last meal timestamp
    pthread_t          thread;      // Philosopher thread
    pthread_mutex_t    *l_fork;     // Left fork
    pthread_mutex_t    *r_fork;     // Right fork
    t_table            *table;      // Table reference
} t_philo;

typedef struct s_table {
    int                n_ph;            // Number of philosophers
    long               time_to_die;     // Time to die
    long               time_to_eat;     // Time eating
    long               time_to_sleep;   // Time sleeping
    int                n_meals;         // Target number of meals
    long               start_time;      // Simulation start time
    int                someone_died;    // Death flag
    pthread_mutex_t    *forks;         // Fork array
    pthread_mutex_t    print_mutex;    // Print mutex
    pthread_mutex_t    meal_mutex;     // Meal mutex
    pthread_mutex_t    death_mutex;    // Death mutex
    t_philo            *philos;        // Philosophers array
} t_table;
```

### MAIN FUNCTIONS

**🔧 Initialization**
- `check_args()` - Input argument validation
- `init_table()` - Table and philosophers initialization

**⚙️ Simulation**
- `ph_routine()` - Main philosopher routine
- `dh_routine()` - "Reaper" routine that controls deaths
- `eat()`, `think()`, `ft_sleep()` - Philosopher actions

**🛡️ Concurrency Control**
- `check_death()` - Checks if a philosopher should die
- `check_someone_died()` - Checks global death state
- `print_msg()` - Thread-safe message printing

**⏰ Time Management**
- `get_time()` - Gets current timestamp in milliseconds
- `get_timestamp()` - Calculates relative time from start

### COMPILATION AND USAGE

```bash
make
./philo [num_philos] [time_to_die] [time_to_eat] [time_to_sleep] [num_meals]
```

**Example:**
```bash
./philo 5 800 200 200 7
# 5 philosophers, 800ms to die, 200ms eating, 200ms sleeping, 7 meals each
```

### IMPLEMENTED FEATURES

- ✅ **Deadlock prevention**: Specific order for taking forks
- ✅ **Starvation control**: Monitoring time since last meal
- ✅ **Thread safety**: Mutex usage to protect shared data
- ✅ **Memory management**: Proper resource cleanup
- ✅ **Synchronization**: Coordination between philosopher and monitor threads
- ✅ **Thread-safe logging**: Ordered event printing
- ✅ **Death detection**: Continuous monitoring of philosopher status
- ✅ **Clean termination**: Proper termination of all threads

### DEADLOCK PREVENTION ALGORITHM

```c
// Even ID philosophers take left fork first
// Odd ID philosophers take right fork first
if (ph->id % 2 == 0) {
    pthread_mutex_lock(ph->l_fork);
    pthread_mutex_lock(ph->r_fork);
} else {
    pthread_mutex_lock(ph->r_fork);
    pthread_mutex_lock(ph->l_fork);
}
```

### TESTING

Includes scripts for testing and debugging:
- `bash.sh` - Automated testing scripts
- `hellgrind.sh` - Race condition detection with Helgrind
- `valgrind.supp` - False positive suppression in Valgrind

### STATUS MESSAGES

- `[timestamp] [id] has taken a fork`
- `[timestamp] [id] is eating`
- `[timestamp] [id] is sleeping`
- `[timestamp] [id] is thinking`
- `[timestamp] [id] died`

---
*Proyecto desarrollado como parte del curriculum de 42.*