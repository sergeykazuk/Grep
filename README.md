# MyGrep

`MyGrep` is a multithreaded case sensitive file searching tool implemented in C++. It recursively searches through files in a directory for a specified pattern. The project utilizes thread pools for efficient file processing and supports **only UTF-8 encoded files** (like standard grep). Support for other encodings may be added as a configurable policy in the future.

## Features

- Multithreaded case sensitive file searching using a thread pool.
- Recursive directory traversal.
- Handles UTF8 and UTF16 encoded files.

## Prerequisites

- C++17
- CMake 3.10 or higher

## Building

1. Create a build directory and navigate to it:

    ```bash
    mkdir build
    cd build
    ```

2. Configure the project with CMake:

    ```bash
    cmake ..
    ```

3. Build the project:

    ```bash
    cmake --build .
    ```
Unit tests will not be built by default. If you want to build them either change ENABLE_TESTS flag to ON, or path it on CMake call as follow:
    
```bash
cmake -DENABLE_TESTS=ON ..
```


## Usage

To use `MyGrep`, run the executable with the following syntax:

```bash
./my_grep.exe [-c] PATTERN DIRECTORY
```
- `-c`, `--color`: Enable colored output.
- `-h`, `--help`: Show usage help.
- PATTERN: The string pattern you want to search for.
- DIRECTORY: The path to the directory or file where the search will be performed.

Example:
```bash
./my_grep.exe "APPCOMPAT" C:\trac_msi.log
```

Colored output example:
```bash
./my_grep.exe -c "APP" C:\trac_msi.log
```

Color output note:

- Colored output is implemented using ANSI escape sequences.
- It should work in most Unix-like terminals and in modern Windows terminals such as Windows Terminal and recent PowerShell sessions with ANSI support enabled.
- It may not render correctly in older Windows consoles or environments that do not interpret ANSI color codes.

Note:

search results for 
```bash
./my_grep.exe "APP" C:\trac_msi.log
```

and 
```bash
./my_grep.exe "app" C:\trac_msi.log
```
will not be the same.

## Implementation Details: Thread Pool and Synchronization abstractions

This project demonstrates how to implement a thread pool in C++ using a `std::vector<std::thread>`. The thread pool is responsible for managing a fixed number of worker threads that process tasks from a shared queue. This approach is more efficient than spawning a new thread for each task, as it avoids the overhead of frequent thread creation and destruction, and allows for better resource management and scalability.

### Why Use a Thread Pool?

- **Performance**: Creating and destroying threads for every task is expensive. A thread pool reuses threads, reducing overhead and improving throughput.
- **Resource Management**: Limits the number of concurrent threads, preventing resource exhaustion and improving system stability.
- **Task Scheduling**: Enables efficient scheduling and batching of tasks, which is especially useful for I/O-bound or CPU-bound workloads.


### Synchronization Mechanisms

The project showcases several synchronization techniques for signaling task completion and coordinating threads:

- **Mutex (`std::mutex`)**: Ensures exclusive access to shared resources, such as the task queue.
- **Condition Variable (`std::condition_variable`)**: Allows threads to wait for specific conditions (e.g., new tasks available) and to be notified when those conditions are met.
- **Future/Promise (`std::future`, `std::promise`)**: Provides a way to retrieve results from asynchronous tasks and, like it is done in this case, to signal when a task is completed.
- **Atomics (`std::atomic`)**: Enables lock-free, thread-safe operations on shared variables, useful for counters, flags, and simple state management without the overhead of mutexes.

These mechanisms are used together to safely manage the lifecycle of tasks and threads within the pool.

### Standard Thread Pool Implementations

While this project provides a custom thread pool implementation for educational purposes, production code often relies on well-tested libraries. Some standard and widely used thread pool implementations include:

- **Boost.Thread**: [Boost::asio::thread_pool](https://www.boost.org/doc/libs/release/doc/html/boost_asio/reference/thread_pool.html) provides a robust, cross-platform thread pool.
- **QtConcurrent**: [QtConcurrent::run](https://doc.qt.io/qt-6/qtconcurrentrun.html) and [QThreadPool](https://doc.qt.io/qt-6/qthreadpool.html) offer high-level APIs for parallel execution in Qt applications.
- **Folly**: [folly::ThreadPoolExecutor](https://github.com/facebook/folly) from Facebook's Folly library is a high-performance thread pool for advanced use cases.

These libraries provide additional features, optimizations, and integration with other concurrency primitives, making them suitable for production environments. 

---

## Known Issues/Limitations

**No UTF-16 Support**: This tool does not work with UTF-16 encoded text files. Only UTF-8 encoded files are supported.
