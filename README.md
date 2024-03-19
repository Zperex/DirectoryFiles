# File Management System

This project is a simple file management system implemented in C++. It allows users to perform various operations on files within a specified directory, including sorting files alphabetically and by size, searching for files, deleting files, and displaying file information.

## Features

- **Sorting Files**: Users can sort files alphabetically or by size using the Merge Sort and Quick Sort algorithms respectively.
- **Searching**: Users can search for files by filename using the Binary Search algorithm.
- **Deleting Files**: Users can delete files from both the linked list and the file system using Binary Search for efficient lookup.
- **Error Handling**: The program provides error messages for invalid input and file deletion failures.

## How it Works

- **Linked List Storage**: Files in the specified directory are stored in a singly linked list data structure. Each node in the linked list represents a file and contains information about the filename and filesize.
- **Sorting Algorithms**:
  - **Merge Sort**: Used to sort files alphabetically by filename.
  - **Quick Sort**: Used to sort files by size.
- **Searching Algorithm**:
  - **Binary Search**: Used to search for files by filename. Also utilized in the delete operation for efficient lookup.

## Usage

1. Clone the repository.
2. Compile the C++ code using a C++ compiler (e.g., g++).
3. Run the executable.
4. Follow the menu prompts to perform various operations on the files in the specified directory.

## Dependencies

- C++17 or later
- Filesystem library (part of the C++ Standard Library)
- Compiler that supports C++17 features

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

