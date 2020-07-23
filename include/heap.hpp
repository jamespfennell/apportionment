#include <iostream>
#include <vector>

template <typename T, typename W> class Heap {

  struct Node {
    T element;
    W weight;
  };

  std::vector<Node> nodes;

  long parentIndex(long childIndex) const {
    if (childIndex == 0) {
      return -1;
    }
    return (childIndex - 1) / 2;
  }

  long leftChildIndex(long parentIndex) const {
    long childIndex = 2 * parentIndex + 1;
    if (childIndex >= nodes.size()) {
      return -1;
    }
    return childIndex;
  }

  long rightChildIndex(long parentIndex) const {
    long childIndex = 2 * parentIndex + 2;
    if (childIndex >= nodes.size()) {
      return -1;
    }
    return childIndex;
  }

  bool lessThan(long index1, long index2) const {
    return nodes[index1].weight <= nodes[index2].weight;
  }

  void swap(long index1, long index2) {
    Node element = std::move(nodes[index1]);
    nodes[index1] = std::move(nodes[index2]);
    nodes[index2] = std::move(element);
  }

public:
  T pop() {
    // Check for no elements! Otherwise get a seg fault
    long lastIndex = nodes.size() - 1;
    if (nodes.size() > 1) {
      swap(0, lastIndex);
    }
    T result = std::move((nodes[lastIndex]).element); // [lastIndex].element;
    this->nodes.pop_back();

    long parentIndex = 0;
    long leftChildIndex = this->leftChildIndex(parentIndex);
    long rightChildIndex = this->rightChildIndex(parentIndex);
    while ((leftChildIndex >= 0 || rightChildIndex >= 0)) {
      long childIndex = 0;
      if (leftChildIndex < 0) {
        childIndex = rightChildIndex;
      } else if (rightChildIndex < 0) {
        childIndex = leftChildIndex;
      } else if (lessThan(leftChildIndex, rightChildIndex)) {
        childIndex = rightChildIndex;
      } else {
        childIndex = leftChildIndex;
      }
      if (lessThan(childIndex, parentIndex)) {
        break;
      }
      swap(childIndex, parentIndex);
      parentIndex = childIndex;
      leftChildIndex = this->leftChildIndex(parentIndex);
      rightChildIndex = this->rightChildIndex(parentIndex);
    }
    return result;
  }

  // TODO: T=unique_ptr won't work here I think
  // Maybe pass by R value reference?
  // Could also have addCopy
  // or pass by regulare reference and do the move itself
  void add(T element, W weight) {
    nodes.push_back(Node{element, weight});
    long childIndex = nodes.size() - 1;
    long parentIndex = this->parentIndex(childIndex);
    while (parentIndex >= 0 and lessThan(parentIndex, childIndex)) {
      swap(parentIndex, childIndex);
      childIndex = parentIndex;
      parentIndex = this->parentIndex(childIndex);
    }
  }
};
