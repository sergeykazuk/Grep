# MyGrep

`MyGrep` is a multithreaded case sensitive file searching tool implemented in C++. It recursively searches through files in a directory for a specified pattern. The project utilizes thread pools for efficient file processing and supports UTF8 and UTF16 encoded files.

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
./my_grep.exe PATTERN DIRECTORY
```
- PATTERN: The string pattern you want to search for.
- DIRECTORY: The path to the directory or file where the search will be performed.

Example:
```bash
./my_grep.exe "APPCOMPAT" C:\trac_msi.log
```

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


## Implementation Details

- File Encoding: The tool detects UTF-16 encoding with BOM (Byte Order Mark) and handles both little-endian and big-endian formats.
- Multithreading: Uses a thread pool to process multiple files concurrently for efficient searching.
- File Handling: Supports recursive directory traversal to search within all files in the specified directory.

## Known Issues

**Deprecated Coding Headers**: The implementation uses std::codecvt_utf16 for handling UTF-16 encoding, which is deprecated in **C++17** and removed in **C++20**. This may result in compatibility issues with newer C++ standards.