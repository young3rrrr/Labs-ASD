#include <iostream>
using namespace std;

struct Node 
{
    int key;
    Node* next;

Node(int val){
    key = val;
    next = nullptr;
};

};

Node* head = nullptr;


void readRangeList (Node* &head) {
 if (head == nullptr) {
    return;
 }

    Node* head0 = nullptr;  Node* tail0 = nullptr;
    Node* head1 = nullptr;  Node* tail1 = nullptr;
    Node* head2 = nullptr;  Node* tail2 = nullptr;

    Node* curr = head;

    int rem = curr->key % 3;

    while (curr != nullptr) {
        Node* nextNode = curr->next;
        curr->next = nullptr;
        rem = curr->key % 3;

        if (rem == 0) {
            if (head0 == nullptr) {
                head0 = curr;
                tail0 = curr;
            } else {
                tail0->next = curr;
                tail0 = curr;
            }
        } else if (rem == 1) {
            if (head1 == nullptr) {
                head1 = curr;
                tail1 = curr;
            } else {
                tail1->next = curr;
                tail1 = curr;
            }
        } else {
            if (head2 == nullptr) {
                head2 = curr;
                tail2 = curr;
            } else {
                tail2->next = curr;
                tail2 = curr;
            }
        }

        curr = nextNode; 
    }

    if (head1 != nullptr) {
        tail1->next = head2;
    }
    if (head0 != nullptr) {
        if (head1 != nullptr) {
            tail0->next = head1;
        } else {
            tail0->next = head2;
        }
    } else {
        if (head1 != nullptr) {
            head = head1;
        } else {
            head = head2;
        }
    }
}


void createList(Node* &head) {
    int n; 
    cout << "Enter the number of elements: ";
    cin >> n;

    if (n == 0) {
        cout << "Number of elements must be greater than 0." << endl;
        return;
}

    Node* tail = nullptr;

    for (int i = 0; i < n; ++i) {
        int val;
        cout << "Enter element " << (i + 1) << ": ";
        cin >> val;

        Node* newNode = new Node(val);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

}

void printList(Node* head) {
    if (head == nullptr) {
        cout << "List is empty." << endl;
        return;
    }

    Node* curr = head;
    while (curr != nullptr) {
        cout << curr->key << "->";
        curr = curr->next;
    }
    cout << "nullptr" << endl;
}

void freeList(Node* &head) {
    Node* curr = head;
    while (curr != nullptr) {
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
}


int main() {
    createList(head);
    cout << "Original list: ";
    printList(head);

    readRangeList(head);
    cout << "Rearranged list: ";
    printList(head);

    freeList(head);
    return 0;
}