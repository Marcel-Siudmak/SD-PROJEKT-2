#include "pairing_heap.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
pairing_heap<T>::pairing_heap() : root(nullptr), _size(0) {}

template <typename T>
pairing_heap<T>::~pairing_heap() {
    // Recursively delete all nodes
    deleteTree(root);
    root = nullptr;
}

// Helper function: meld two heaps (returns root of merged heap)
template <typename T>
Node<T>* pairing_heap<T>::meld(Node<T>* heap1, Node<T>* heap2) {
    if (!heap1) return heap2;
    if (!heap2) return heap1;

    // Max heap: larger key becomes the root
    if (heap1->_key > heap2->_key) {
        // heap1 becomes parent, heap2 becomes its child
        heap2->sibling = heap1->child;
        heap1->child = heap2;
        return heap1;
    } else {
        // heap2 becomes parent, heap1 becomes its child
        heap1->sibling = heap2->child;
        heap2->child = heap1;
        return heap2;
    }
}

// Helper function: recursively delete all nodes in a tree
template <typename T>
void pairing_heap<T>::deleteTree(Node<T>* node) {
    if (!node) return;
    
    // Delete all children
    Node<T>* child = node->child;
    while (child) {
        Node<T>* nextChild = child->sibling;
        deleteTree(child);
        child = nextChild;
    }
    
    delete node;
}


template <typename T>
void pairing_heap<T>::insert(T value, int key) {
    Node<T>* newNode = new Node<T>(value, key);
    
    if (!root) {
        root = newNode;
    } else {
        root = meld(root, newNode);
    }
    
    _size++;
}

template <typename T>
T pairing_heap<T>::extract_max() {
    if (!root) {
        throw std::runtime_error("Cannot extract from empty heap");
    }
    T val = root->_value;
    Node<T>* oldRoot = root;
    // Get the first child
    Node<T>* firstChild = root->child;
    if (firstChild) {
        // If root has children, we need to merge them back and in process choose new root
        root = twoPassMerge(firstChild);
    } else {
        root = nullptr;
    }
    delete oldRoot;
    _size--;
    return val;
}


template <typename T>
Node<T>* pairing_heap<T>::twoPassMerge(Node<T>* firstSibling) {
    if (!firstSibling) return nullptr;
    if (!firstSibling->sibling) {
        firstSibling->sibling = nullptr;
        return firstSibling;
    }

    Node<T>* current = firstSibling;
    Node<T>* pairedStack = nullptr; 

    // Pass 1: Left-to-Right merge in pairs.
    // We push the merged pairs onto 'pairedStack' to reverse their order.
    while (current && current->sibling) {
        Node<T>* A = current;
        Node<T>* B = current->sibling;
        Node<T>* next = B->sibling;

        // Isolate the pair
        A->sibling = nullptr;
        B->sibling = nullptr;

        // Meld and push to our temporary stack
        Node<T>* merged = meld(A, B);
        merged->sibling = pairedStack; 
        pairedStack = merged;

        current = next;
    }

    // If there is an odd number of siblings, push the last one
    if (current) {
        current->sibling = pairedStack;
        pairedStack = current;
    }

    // Pass 2: Right-to-Left merge.
    // Popping from our stack automatically processes them Right-to-Left.
    Node<T>* result = pairedStack;
    pairedStack = pairedStack->sibling;
    result->sibling = nullptr;

    while (pairedStack) {
        Node<T>* next = pairedStack->sibling;
        pairedStack->sibling = nullptr;
        
        result = meld(pairedStack, result);
        
        pairedStack = next;
    }

    return result;
}

// Returns the value of root
template <typename T>
T pairing_heap<T>::peek() {
    if (!root) {
        throw std::runtime_error("Cannot peek at empty heap");
    }
    return root->_value;
}

// Helper function: find and disconnect a node from its parent
// Required for modify_key to properly update heap structure
template <typename T>
Node<T>* pairing_heap<T>::disconnectNode(Node<T>*& node, T value) {
    if (!node) return nullptr;
    
    // Check direct children
    Node<T>* child = node->child;
    Node<T>* prev = nullptr;
    
    while (child) {
        if (child->_value == value) {
            // Found it - remove from sibling chain
            if (prev) {
                prev->sibling = child->sibling;
            } else {
                node->child = child->sibling;
            }
            
            // Merge the disconnected node's children back into the heap
            if (child->child) {
                Node<T>* mergedChildren = twoPassMerge(child->child);
                root = meld(root, mergedChildren);
            }
            
            // Detach the found node completely
            child->sibling = nullptr;
            child->child = nullptr;
            
            return child;
        }
        prev = child;
        child = child->sibling;
    }
    
    // Recursively search in children's subtrees
    child = node->child;
    while (child) {
        Node<T>* found = disconnectNode(child, value);
        if (found) {
            return found;
        }
        child = child->sibling;
    }
    
    return nullptr;
}

template <typename T>
void pairing_heap<T>::modify_key(T value, int new_key) {
    if (!root) {
        throw std::runtime_error("Cannot modify key in empty heap");
    }

    if (root->_value == value) {
        int old_key = root->_key;
        root->_key = new_key;
        if(new_key < old_key) {
            // If the key decreased, we may need to move it down
            Node<T>* children = root->child;
            root->child = nullptr; // Detach children for merging
            if (children) {
                Node<T>* mergedChildren = twoPassMerge(children);
                root = meld(root, mergedChildren);
            }
        }
        else return;
    }

    Node<T>* node = disconnectNode(root, value);
    if (!node) {
        throw std::runtime_error("Value not found in heap");
    }
    node->_key = new_key; // Update the key
    root = meld(root, node);
}

template <typename T>
int pairing_heap<T>::return_size() {
    return _size;
}

template <typename T>
void pairing_heap<T>::display() {
    if(!root) {
        std::cout << "Heap is empty." << std::endl;
        return;
    }
    std::cout << "Heap contents (value: key):" << std::endl;
    // Lambda for recursive display with indentation to show structure
    std::function<void(Node<T>*, int)> displayHelper = [&](Node<T>* node, int level) {
        if (!node) return;
        std::cout << std::string(level * 2, ' ') << node->_value << ": " << node->_key << std::endl;
        displayHelper(node->child, level + 1);
        displayHelper(node->sibling, level);
    };
    displayHelper(root, 0);
}
template <typename T>
void pairing_heap<T>::clear() {
    deleteTree(root);
    root = nullptr;
    _size = 0;
}

// Explicit template instantiations
template class pairing_heap<int>;
template class pairing_heap<double>;
template class pairing_heap<std::string>;

