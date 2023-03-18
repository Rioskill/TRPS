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

    using callback_t = std::function<void(const T&)>;
    using find_callback_t = std::function<bool(const T&)>;

    Node *head, *tail;
public:
    CircularList();
    ~CircularList();

    void forEach(callback_t) const;

    const Node *find(const T &value) const;
    const Node *find(find_callback_t) const;

    Node *find(const T &value);
    Node *find(find_callback_t);

    Node *getHead();
    Node *getTail();
    const Node *getHead() const;
    const Node *getTail() const;

    void push(const T &value);push_back

    friend std::ostream &operator<<(std::ostream &out, CircularList &list) {
        out << '[';
        for (auto node = list.getHead(); node != list.getTail(); node=node->next) {
            out << node->value << ", ";
        }
        out << list.getTail()->value << ']';
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
void CircularList<T>::forEach(callback_t callback) const {
    for (auto node = this->getHead(); node != this->getTail(); node=node->next) {
        callback(node->value);
    }
    callback(this->getTail()->value);
}

template <typename T>
const typename CircularList<T>::Node *CircularList<T>::find(find_callback_t callback) const {
    for (auto node = this->getHead(); node != this->getTail(); node=node->next) {
        if(callback(node->value)) {
            return node;
        }
    }

    if(callback(this->getTail()->value)) {
        return this->getTail();;
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
