# WatchDog

A C implementation of a watchdog process that monitors a client application and revives it if it terminates unexpectedly.

## Overview

The WatchDog project provides a mechanism to ensure that a critical client process remains active. It does this by monitoring the client process and restarting it if it stops running. This is particularly useful for applications where uptime is crucial.

## Features

- **Process Monitoring**: Keeps track of a client process and ensures it is running.
- **Automatic Restart**: Revives the client process if it terminates unexpectedly.
- **Heartbeat Mechanism**: Uses signals to check the health of both client and watchdog processes.
- **Configurable Intervals**: Allows customization of check intervals and thresholds.
- **Inter-Process Communication**: Utilizes signals and semaphores for synchronization between processes.
- **Thread Support**: Employs multithreading to handle concurrent tasks.

## How It Works

1. **Initialization**: The client process initializes the watchdog mechanism by calling the `MMI` function, providing the check interval, threshold, and its arguments.

2. **Watchdog Process Creation**: If a watchdog process is not already running, the client forks a new process that executes the watchdog code.

3. **Heartbeat Exchange**: The client and watchdog processes send periodic heartbeat signals (`SIGUSR1`) to each other to confirm they are alive.

4. **Monitoring Loop**: Both processes increment a counter each time they send a heartbeat. If a process fails to receive a heartbeat within the specified interval, it assumes the other has terminated.

5. **Revival Mechanism**: Upon detecting that the other process has stopped responding, a process will attempt to revive it using the stored arguments.

6. **Termination**: When the client process is ready to terminate, it calls the `DNR` function to gracefully shut down both the client and watchdog processes.
   
![Watch Dog](https://github.com/user-attachments/assets/f7f4b585-1072-4214-9253-44745a6fc14e)

## Usage
### Integrate WatchDog into Your Client Application
1. **Include the WatchDog Header**
```c\n   #include \"watch_dog_private.h\"\n   ```

2. **Initialize the WatchDog in Your `main` Function**
```c
int main(int argc, char *argv[])
{
    // Start the watchdog monitoring
    MMI(interval_in_seconds, repetitions, argv);
    // Your application logic here
    // Before exiting, stop the watchdog
    DNR();
    return 0;
}

```

- **`interval_in_seconds`**: The time interval for heartbeat checks.
- **`repetitions`**: The number of missed heartbeats allowed before attempting a revival.
  
### Terminating the WatchDog
- Call the `DNR()` function before your application exits to gracefully shut down the watchdog process.

