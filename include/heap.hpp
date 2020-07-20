#include<vector>
#include<iostream>

template<typename T>
class Heap {

    struct Node {
        T element;
        long double weight;
    };

   std::vector<Node> nodes2;
    long parentIndex(long childIndex) {
        if (childIndex == 0) {
            return -1;
        }
        return (childIndex - 1) / 2;
    }

    long leftChildIndex(long parentIndex) {
        long childIndex = 2 * parentIndex + 1;
        if (childIndex >= this->nodes2.size()) {
            return -1;
        }
        return childIndex;
    }

    long rightChildIndex(long parentIndex) {
        long childIndex = 2 * parentIndex + 2;
        if (childIndex >= this->nodes2.size()) {
            return -1;
        }
        return childIndex;
    }

    long double getWeight(long index) {
        return (this->nodes2[index]).weight;
    }

    void swap(long index1, long index2) {
        Node element = std::move(this->nodes2[index1]);
        this->nodes2[index1] = std::move(this->nodes2[index2]);
        this->nodes2[index2] = std::move(element);
    }

   public:
   /*
   Heap(long double (*weightFunction)(T), vector<T>& initialElements) {
       this->weightFunction = weightFunction;
       for (auto&& element : initialElements) {
           this->add(element);
       }
   }
   */

   T pop() {
       // Check for no elements! Otherwise get a seg fault
       long lastIndex = this->nodes2.size() - 1;
       if (nodes2.size() > 1) {
           this->swap(0, lastIndex);
       }
       T result2 = std::move((this->nodes2[lastIndex]).element); // [lastIndex].element;
       this->nodes2.pop_back();

       long parentIndex = 0;
        long leftChildIndex = this->leftChildIndex(parentIndex);
        long rightChildIndex = this->rightChildIndex(parentIndex);
        while((leftChildIndex >= 0 || rightChildIndex >= 0) ) {
            long childIndex = 0;
            if (leftChildIndex < 0) {
                childIndex = rightChildIndex;
            } else if (rightChildIndex < 0) {
                childIndex = leftChildIndex;
            } else if (getWeight(leftChildIndex) < getWeight(rightChildIndex)) {
                childIndex = rightChildIndex;
            } else {
                childIndex = leftChildIndex;
            }
            if (getWeight(childIndex) <= getWeight(parentIndex)) {
                break;
            }
            this->swap(childIndex, parentIndex);
            parentIndex = childIndex;
            leftChildIndex = this->leftChildIndex(parentIndex);
            rightChildIndex = this->rightChildIndex(parentIndex);
            // cout << parentIndex << " " << leftChildIndex <<  "inf loop!" << endl;
        }

        // re-adjust the heap
       return result2;
   }

    // TODO: T=unique_ptr won't work here I think
    // Maybe pass by R value reference?
    // Could also have addCopy
    // or pass by regulare reference and do the move itself
   void add(T element, double weight) {
       nodes2.push_back(Node{element, weight});
        long childIndex = nodes2.size() - 1;
        long parentIndex = this->parentIndex(childIndex);
        while (parentIndex >= 0 and (this->nodes2[parentIndex]).weight < (this->nodes2[childIndex]).weight ) {
            this->swap(parentIndex, childIndex);
            childIndex = parentIndex;
            parentIndex = this->parentIndex(childIndex);
        }
   }
};


