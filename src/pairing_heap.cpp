#include "pairing_heap.hpp"
#include <stdexcept>

template <typename T>
PairingHeap<T>::PairingHeap() : root(nullptr), _size(0) {}

template <typename T>
PairingHeap<T>::~PairingHeap() {
    // Recursively delete all nodes
    deleteTree(root);
    root = nullptr;
}

// Helper function: meld two heaps (returns root of merged heap)
template <typename T>
Node<T>* PairingHeap<T>::meld(Node<T>* heap1, Node<T>* heap2) {
    if (!heap1) return heap2;
    if (!heap2) return heap1;
    
    // Max heap: larger key becomes the root
    if (!(heap1 < heap2)) {
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

// Helper function: merge all siblings using the pairing strategy
template <typename T>
Node<T>* PairingHeap<T>::mergeSiblings(Node<T>* sibling) {
    if (!sibling) return nullptr;
    if (!sibling->sibling) return sibling;
    
    // First pass: pair up siblings
    Node<T>* merged = meld(sibling, sibling->sibling);
    sibling->sibling = nullptr;
    if (sibling->sibling) {
        sibling->sibling->sibling = nullptr;
    }
    
    Node<T>* next = sibling->sibling ? sibling->sibling->sibling : nullptr;
    
    // Continue with remaining siblings
    if (next) {
        Node<T>* mergedNext = mergeSiblings(next);
        return meld(merged, mergedNext);
    }
    
    return merged;
}

// Helper function: recursively delete all nodes in a tree
template <typename T>
void PairingHeap<T>::deleteTree(Node<T>* node) {
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

// Helper function: find a node with given value
template <typename T>
Node<T>* PairingHeap<T>::findNode(Node<T>* node, T value) {
    if (!node) return nullptr;
    
    if (node->value == value) return node;
    
    // Search in children
    Node<T>* child = node->child;
    while (child) {
        Node<T>* found = findNode(child, value);
        if (found) return found;
        child = child->sibling;
    }
    
    return nullptr;
}

template <typename T>
void PairingHeap<T>::insert(T value, int key) {
    Node<T>* newNode = new Node<T>(value, key);
    
    if (!root) {
        root = newNode;
    } else {
        root = meld(root, newNode);
    }
    
    _size++;
}

template <typename T>
void PairingHeap<T>::extract_max() {
    if (!root) {
        throw std::runtime_error("Cannot extract from empty heap");
    }
    
    Node<T>* oldRoot = root;
    
    // Get the first child
    Node<T>* firstChild = root->child;
    
    if (firstChild) {
        // Merge all children using pairing strategy
        // First, collect all siblings and reset them
        Node<T>* current = firstChild;
        while (current) {
            current->sibling = current->sibling;
            current = current->sibling;
        }
        
        // Merge siblings in pairs from left to right, then merge results
        root = mergePairs(firstChild);
    } else {
        root = nullptr;
    }
    
    delete oldRoot;
    _size--;
}

// Helper function: merge pairs of siblings
template <typename T>
Node<T>* PairingHeap<T>::mergePairs(Node<T>* head) {
    if (!head) return nullptr;
    if (!head->sibling) {
        head->sibling = nullptr;
        return head;
    }
    
    Node<T>* first = head;
    Node<T>* second = head->sibling;
    Node<T>* rest = second->sibling;
    
    first->sibling = nullptr;
    second->sibling = nullptr;
    
    Node<T>* merged = meld(first, second);
    
    if (rest) {
        return meld(merged, mergePairs(rest));
    }
    
    return merged;
}

template <typename T>
T PairingHeap<T>::peek() {
    if (!root) {
        throw std::runtime_error("Cannot peek at empty heap");
    }
    return root->value;
}

// Helper function: find and disconnect a node from its parent
template <typename T>
bool PairingHeap<T>::disconnectNode(Node<T>*& node, T value) {
    if (!node) return false;
    
    // Check children
    Node<T>* child = node->child;
    Node<T>* prev = nullptr;
    while (child) {
        if (child->_value == value) {
            if (prev) {
                prev->sibling = child->sibling;
            } else {
                node->child = child->sibling;
            }
            child->sibling = nullptr;
            return true;
        }
        prev = child;
        child = child->sibling;
    }
    
    // Recursively search in children's subtrees
    child = node->child;
    while (child) {
        if (disconnectNode(child, value)) {
            return true;
        }
        child = child->sibling;
    }
    
    return false;
}

template <typename T>
void PairingHeap<T>::modify_key(T value, int new_key) {
    Node<T>* node = findNode(root, value);
    
    if (!node) {
        throw std::runtime_error("Value not found in heap");
    }
    
    // Update the key
    node->_key = new_key;
    
    // If it's the root, no restructuring needed
    if (node == root) {
        return;
    }
    
    // For non-root nodes: cut and re-meld
    // First, save the node's children
    Node<T>* children = node->child;
    node->child = nullptr;
    node->sibling = nullptr;
    
    // Disconnect the node from its parent
    disconnectNode(node, value);
    
    // Merge the node's children
    if (children) {
        Node<T>* mergedChildren = mergePairs(children);
        Node<T>* newHeap = meld(node, mergedChildren);
        root = meld(root, newHeap);
    } else {
        root = meld(root, node);
    }
}

template <typename T>
int PairingHeap<T>::return_size() {
    return _size;
}

template <typename T>
void PairingHeap<T>::display() {
    if(!root) {
        std::cout << "Heap is empty." << std::endl;
        return;
    }
    std::cout << "Heap contents (value: key):" << std::endl;
    std::function<void(Node<T>*, int)> displayHelper = [&](Node<T>* node, int level) {
        if (!node) return;
        std::cout << std::string(level * 2, ' ') << node->_value << ": " << node->_key << std::endl;
        displayHelper(node->child, level + 1);
        displayHelper(node->sibling, level);
    };
    displayHelper(root, 0);
}

// Explicit template instantiations
template class PairingHeap<int>;
template class PairingHeap<double>;
template class PairingHeap<std::string>;
