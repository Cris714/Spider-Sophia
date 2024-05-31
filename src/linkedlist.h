#pragma once

template <typename T>
class List {
    private:
        struct Node {
            T data;
            Node* next;
            Node(T value) : data(value), next(nullptr) {}
        };

        Node* head;
        int size;

    public:
        List() : head(nullptr), size(0) {}

        ~List() {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }

        void add(T value) {
            Node* newNode = new Node(value);
            if (head == nullptr) {
                head = newNode;
            } else {
                Node* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
            size++;
        }

        void remove(T value) {
            if (head == nullptr) return;

            if (head->data == value) {
                Node* temp = head;
                head = head->next;
                delete temp;
                size--;
                return;
            }

            Node* temp = head;
            while (temp->next != nullptr && temp->next->data != value) {
                temp = temp->next;
            }

            if (temp->next != nullptr) {
                Node* nodeToDelete = temp->next;
                temp->next = temp->next->next;
                delete nodeToDelete;
                size--;
            }
        }

        T& operator[](int index) {
            if (index < 0 || index >= size) {
                throw std::out_of_range("Index out of range");
            }

            Node* temp = head;
            for (int i = 0; i < index; i++) {
                temp = temp->next;
            }

            return temp->data;
        }

        int getSize() const {
            return size;
        }
};