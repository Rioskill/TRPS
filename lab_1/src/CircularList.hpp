#pragma once

#include <iostream>
#include <functional>

template <typename T>
class CircularList {
private:
    struct Node {
        Node *next;
        T value;

        Node(const T &value, Node *next=nullptr);
    };

    using const_callback_t = std::function<void(const T&)>;
    using callback_t = std::function<void(T&)>;
    using node_callback_t = std::function<void(Node*)>;
    using find_callback_t = std::function<bool(const T&)>;

    Node *head, *tail;
public:
    CircularList();
    ~CircularList();

    void forEach(const_callback_t) const;
    void forEach(callback_t);

    void forEachNode(node_callback_t);

    const Node *find(const T &value) const;
    const Node *find(find_callback_t) const;

    Node *find(find_callback_t);
    Node *find(const T &value);

    const Node *find_prev(find_callback_t) const;
    const Node *find_prev(const T &value) const;

    Node *find_prev(find_callback_t);
    Node *find_prev(const T &value);

    Node *getHead();
    Node *getTail();
    const Node *getHead() const;
    const Node *getTail() const;

    void push(const T &value);
    void remove(const T &value);

    void bubble_sort();

    friend std::ostream &operator<<(std::ostream &out, CircularList &list) {
        out << '[';
        for (auto node = list.getHead(); node != list.getTail(); node=node->next) {
            out << node->value << ", ";
        }
        if (list.getTail()) {
            out << list.getTail()->value;
        }
        out << ']';
        return out;
    }
};

template <typename T>
CircularList<T>::Node::Node(const T &value, Node *next): value(value), next(next) {}

template <typename T>
CircularList<T>::CircularList(): head(nullptr), tail(nullptr) {}

template <typename T>
CircularList<T>::~CircularList() {
    auto node = this->getHead();
    while(node != this->getTail()) {
        auto next_node = node->next;
        delete node;
        node = next_node;
    }
    delete node;
}

template <typename T>
void CircularList<T>::forEachNode(node_callback_t callback) {
    for (auto node = this->getHead(); node != this->getTail(); node=node->next) {
        callback(node);
    }
    callback(this->getTail());
}

template <typename T>
void CircularList<T>::forEach(callback_t callback) {
    forEachNode([&callback](Node &node){
        callback(node->value);
    });
}

template <typename T>
void CircularList<T>::forEach(const_callback_t callback) const {
    forEach(static_cast<callback_t>(callback));
}

#include <iostream>

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::find_prev(find_callback_t callback) const {
    if (callback(this->getHead()->value)) {
        return this->getTail();
    }

    const Node *prev = this->getHead();

    for (auto node = this->getHead()->next; node != this->getTail(); prev=node, node=node->next) {
        if(callback(node->value)) {
            return prev;
        }
    }

    if(callback(this->getTail()->value)) {
        return prev;
    }

    return nullptr;
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::find_prev(find_callback_t callback) {
    return const_cast<Node*>(const_cast<const CircularList*>(this)->find_prev(callback));
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::find_prev(const T &value) const {
    return this->find_prev([&value](const T &node_value){
        return value == node_value;
    });
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::find_prev(const T &value) {
    return const_cast<Node*>(const_cast<const CircularList*>(this)->find_prev(value));
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::find(find_callback_t callback) const {
    for (auto node = this->getHead(); node != this->getTail(); node=node->next) {
        if(callback(node->value)) {
            return node;
        }
    }

    if(callback(this->getTail()->value)) {
        return this->getTail();
    }

    return nullptr;
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::find(find_callback_t callback) {
    return const_cast<Node*>(const_cast<const CircularList*>(this)->find(callback));
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::find(const T &value) const {
    return this->find([&value](const T &node_value){
        return value == node_value;
    });
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::find(const T &value) {
    return const_cast<Node*>(const_cast<const CircularList*>(this)->find(value));
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::getHead() {
    return head;
}

template <typename T>
typename CircularList<T>::Node *CircularList<T>::getTail() {
    return tail;
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::getHead() const {
    return head;
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::getTail() const {
    return tail;
}

template <typename T>
void CircularList<T>::push (const T &value) {
    if (!head) {
        this->head = new Node(value);
        this->head->next = this->tail;
        this->tail = this->head;
        return;
    }

    Node *new_tail = new Node(value, this->head);
    this->tail->next = new_tail;
    this->tail = new_tail;
}

template <typename T>
void CircularList<T>::remove (const T &value) {
    auto prev = find_prev(value);

    if (!prev) {
        return;
    }

    auto removed = prev->next;

    if (prev == removed) {
        delete removed;
        head = nullptr;
        tail = nullptr;
        return;
    }

    prev->next = removed->next;

    if (removed == head) {
        head = removed->next;
    } else if (removed == tail) {
        tail = prev;
    }

    delete removed;
}

template <typename T>
void CircularList<T>::bubble_sort() {
    forEachNode([this](Node *first_node){
        for (auto second_node = first_node->next; second_node != getTail(); second_node = second_node->next) {
            if (first_node->value < second_node->value) {
                std::swap(first_node->value, second_node->value);
            }
        }
        if (first_node->value < getTail()->value) {
            std::swap(first_node->value, getTail()->value);
        }
    });
}
