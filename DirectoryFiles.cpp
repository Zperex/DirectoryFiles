#include <iostream>
#include <filesystem>
#include <algorithm> // for std::transform
#include <vector>    // for std::vector

namespace fs = std::filesystem;
using namespace std;
// Define a simple linked list node
struct Node {
    fs::path filename;
    uintmax_t filesize; // using std::uintmax_t for file size
    Node* next;

    Node(const fs::path& name, std::uintmax_t size) : filename(name), filesize(size), next(nullptr) {}
};

// Function to convert a string to lowercase
    string toLower(const std::string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Function to merge two sorted linked lists
Node* merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    Node* result = nullptr;

    if (toLower(left->filename.string()) < toLower(right->filename.string())) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

// Function to perform merge sort on the linked list
Node* mergeSort(Node* head) {
    if (!head || !head->next) {
        return head;
    }

    // Split the linked list into two halves
    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* right = slow->next;
    slow->next = nullptr;

    // Recursively sort each half
    Node* leftSorted = mergeSort(head);
    Node* rightSorted = mergeSort(right);

    // Merge the sorted halves
    return merge(leftSorted, rightSorted);
}

// Function to partition the linked list for quicksort
Node* partition(Node* head, Node* end, Node*& newHead, Node*& newEnd) {
    Node* pivot = end;
    Node* prev = nullptr;
    Node* curr = head;
    Node* tail = pivot;

    while (curr != pivot) {
        if (curr->filesize <= pivot->filesize) {
            if (!newHead) {
                newHead = curr;
            }
            prev = curr;
            curr = curr->next;
        } else {
            if (prev) {
                prev->next = curr->next;
            }
            Node* temp = curr->next;
            curr->next = nullptr;
            tail->next = curr;
            tail = curr;
            curr = temp;
        }
    }

    if (!newHead) {
        newHead = pivot;
    }

    newEnd = tail;

    return pivot;
}
// Function to get the tail of the linked list
Node* getTail(Node* head) {
    while (head != nullptr && head->next != nullptr) {
        head = head->next;
    }
    return head;
}
// Function to implement quicksort on the linked list
Node* quickSort(Node* head, Node* end) {
    if (!head || head == end) {
        return head;
    }

    Node* newHead = nullptr;
    Node* newEnd = nullptr;

    Node* pivot = partition(head, end, newHead, newEnd);

    if (newHead != pivot) {
        Node* temp = newHead;
        while (temp->next != pivot) {
            temp = temp->next;
        }
        temp->next = nullptr;

        newHead = quickSort(newHead, temp);

        temp = getTail(newHead);
        temp->next = pivot;
    }

    pivot->next = quickSort(pivot->next, newEnd);

    return newHead;
}



// Function to display the linked list
void displayList(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        cout << "Filename: " << current->filename << ", Size: " << current->filesize << " bytes" << endl;
        current = current->next;
    }
}
// Function to search for a filename using binary search
Node* binarySearch(Node* head, const fs::path& target) {
    Node* current = head;

    while (current != nullptr) {
        if (toLower(current->filename.string()) == toLower(target.string())) {
            return current; // File found
        } else if (toLower(current->filename.string()) < toLower(target.string())) {
            current = current->next; // Search in the right half
        } else {
            break; // File not found (it should be in the left half)
        }
    }

    return nullptr;
}
// Function to delete a node from the linked list and file system
void deleteNode(Node*& head, const fs::path& target) {
    Node* current = head;
    Node* prev = nullptr;

    // Search for the node to delete
    while (current != nullptr && toLower(current->filename.string()) != toLower(target.string())) {
        prev = current;
        current = current->next;
    }

    // If the node is found, delete it
    if (current != nullptr) {
        // Unlink the node from the linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }

        // Delete the file from the file system
        try {
            fs::remove(current->filename);
            std::cout << "File deleted - Filename: " << current->filename << ", Size: " << current->filesize << " bytes" << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error deleting file: " << e.what() << std::endl;
        }

        // Free the memory allocated for the node
        delete current;
    } else {
        std::cout << "File not found. Deletion failed.\n";
    }
}
// Function to clean up allocated memory
void cleanupList(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

int main() {
    // Get the current directory
    fs::path currentDir = fs::current_path();

    // Initialize linked list
    Node* head = nullptr;
    Node* tail = nullptr;

    // Iterate through the files in the directory
    for (const auto& entry : fs::directory_iterator(currentDir)) {
        // Check if the entry is a regular file
        if (entry.is_regular_file()) {
            // Create a new node for the filename and filesize
            Node* newNode = new Node(entry.path().filename(), entry.file_size());

            // Add the node to the linked list
            if (head == nullptr) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

    int choice;
    do {
        // Display menu
        std::cout << "\nMenu:\n1. Sort files alphabetically\n2. Sort files by size\n3. Search for a filename\n4. Delete a file\n5. Exit\nEnter your choice: ";

        // Check if the input is a valid integer
        if (!(std::cin >> choice)) {
            // Clear the input buffer and ignore invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            // Perform merge sort on the linked list
            head = mergeSort(head);
            // Display the sorted linked list
            displayList(head);
        } else if (choice == 2) {
            // Perform quicksort on the linked list
            Node* end = getTail(head);
            head = quickSort(head, end);
            // Display the sorted linked list
            displayList(head);
        } else if (choice == 3) {
            // Search for a filename using binary search
            std::string filename;
            std::cout << "Enter the filename to search: ";
            std::cin >> filename;
            fs::path target(filename);
            Node* result = binarySearch(head, target);
            if (result) {
                std::cout << "File found - Filename: " << result->filename << ", Size: " << result->filesize << " bytes" << std::endl;
            } else {
                std::cout << "File not found.\n";
            }
        } else if (choice == 4) {
            // Delete a file from the linked list and file system
            std::string filename;
            std::cout << "Enter the filename to delete: ";
            std::cin >> filename;
            fs::path target(filename);
            deleteNode(head, target);
        } else if (choice == 5) {
            std::cout << "Exiting program.\n";
        } else {
            std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 5);

    // Clean up: free allocated memory
    cleanupList(head);

    return 0;
}
