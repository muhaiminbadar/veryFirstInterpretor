#pragma once

#include <memory>
#include <math.h> 

using namespace std;

template<class T>
class Node {
public:
    Node(T pass) : data(pass), next(nullptr), prev(nullptr) {}
    Node<T>* next;
    Node<T>* prev;
    T data;

    ~Node()
    {
        next = nullptr;
        prev = nullptr;
        data = NULL;
    }
};

template<class T>
class LList {
    Node<T>* head;
    Node<T>* tail;
    size_t node_count;
public:
    LList() : head(nullptr), tail(nullptr), node_count(0) {}
    LList(const LList<T>&) = delete; // no shallow copies
    LList<T>& operator=(const LList<T>& list) = delete; // no shallow copies

    // Doubly implementation
    void addNode(const T& data)
    {
        if(head == nullptr)
        {
            head = new Node<T>(data);
            tail = head;
        }
        else if(tail == head)
        {
            tail = new Node<T>(data);
            tail->prev = head;
            head->next = tail;
        }
        else
        {
            auto* temp = new Node<T>(data);
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        node_count ++;
    }
    void addFront(const T& data)
    {
        if(head == nullptr)
        {
            head = new Node<T>(data);
            tail = head;
        }
        else
        {
            auto* temp = new Node<T>(data);
            
            temp->next = head;
            head->prev = temp;

            head = temp;
        }
        node_count ++;
    }
    void deletePtr(Node<T>* ptr)
    {
        if(!node_count || ptr == nullptr)
            return;
        
        if(head == ptr)
            head = ptr->next;
        
        if(ptr->next != nullptr)
            ptr->next->prev = ptr->prev;
        
        if(ptr->prev != nullptr)
            ptr->prev->next = ptr->next;

        auto* i = head;
        if(i != nullptr)
        {
            for(; i->next != nullptr; i = i->next) {}
        }
        tail = i;
        node_count --;
    }
    Node<T>* getHead()
    {
        return head;
    }
    Node<T>* getTail()
    {
        return tail;
    }
    size_t size()
    {
        return node_count;
    }
    void PrintList()
    {
        cout << "[";
        for(auto i = head; i != nullptr; i = i->next)
        {
            cout << i->data;
            if(i->next != nullptr) { cout << ", "; }
        }
        cout << "]" << endl;
    }
    ~LList()
    {
        auto i = head;
        while(i != nullptr)
        {
            auto next = i->next;
            delete i;
            i = next;
        }
    }
};

template <typename T>
class LListStack {
public:
    std::shared_ptr<LList<T>> container;
    LListStack()
    {
        container = std::make_shared<LList<T>>();
    }
    LListStack(const LList<T>& other)  {
        container = other.container;
    }
    LListStack<T>& operator=(const LListStack<T>& other){
        container = other.container;
    }
    void push(const T& data)
    {
        container->addFront(data);
    }
    const T top()
    {
        return container->getHead()->data;
    }
    void pop()
    {
        container->deletePtr(container->getHead());
    }
    const size_t size() const
    {
        return container->size();
    }

};

template <typename T>
class ArrayStack {
    T* arr;
    size_t capacity, size;
public:
    ArrayStack(int maxSize)
    {
        arr = new T[maxSize];
        capacity = 0;
        size = maxSize;
    }
    void push(const T& data)
    {
        if(capacity >= size)
            return;

        arr[capacity] = data;
        capacity++;
    }
    T top()
    {
        return arr[capacity];
    }
    void pop()
    {
        if(capacity <= 0)
            return;

        capacity--;
    }
    ~ArrayStack()
    {
        delete[] arr;
    }

};
