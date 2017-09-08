#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

    // Debug purposes
    std::vector< std::pair<double, T> > getHeap();

private:
    /// Add whatever helper functions you need below
	void trickleUp(int loc, PComparator c);
	void heapify(int index, PComparator c);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    for (unsigned int i = 0; i<store_.size(); i++)
    {
        if (store_[i].second == item)
            return;
    }
	std::pair<double, T> newNode = std::make_pair(priority, item);
	store_.push_back(newNode);
	keyToLocation_.insert(std::make_pair(newNode.second, store_.size()-1));
	trickleUp(store_.size()-1, c_);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(int loc, PComparator c)
{
	int parent = (loc-1)/m_;
	while (parent >= 0 && c(store_[loc].first, store_[parent].first))
	{
		std::pair<double, T> temp = store_[parent];
		store_[parent] = store_[loc];
		store_[loc] = temp;
		keyToLocation_[temp.second] = loc;
		keyToLocation_[store_[parent].second] = parent;
		loc = parent;
		parent = (loc-1)/m_;
	}
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    // Find the item
    typename std::unordered_map<T, size_t, Hasher, TComparator>::iterator it = keyToLocation_.find(item);
    // Update the priority
    store_[it->second].first = priority;
    // trickleup
    trickleUp(it->second, c_);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    // You complete
    return (store_[0]).second;
}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    typename std::unordered_map<T, size_t, Hasher, TComparator>::iterator it = keyToLocation_.find(store_[0].second);
    keyToLocation_.erase(it);
  	store_[0] = store_.back();
  	keyToLocation_[store_[0].second] = 0;
  	store_.pop_back();
  	if (!store_.empty())
  		heapify(0, c_);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(int index, PComparator c)
{
	// Check to see if it is a leaf node
	if (index >= int(store_.size())/m_)
	{
		if ((store_.size() >= m_))
			return;
		else if (index != 0 || store_.size() == 1)
			return;
	}
	int nextChild = m_*index + 1;
	int smallerChild = nextChild;
	int count = 1;
	while ((nextChild + 1 < int(store_.size())) && (count < m_))
	{
		nextChild++;
		if(c(store_[nextChild].first, store_[smallerChild].first))
			smallerChild = nextChild;
		count++;
	}
	if (c(store_[smallerChild].first, store_[index].first))
	{
		std::pair<double, T> temp = store_[smallerChild];
		store_[smallerChild] = store_[index];
		store_[index] = temp;
		keyToLocation_[temp.second] = index;
		keyToLocation_[store_[smallerChild].second] = smallerChild;
		heapify(smallerChild, c_);
	}
}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
typename std::vector< std::pair<double, T> > Heap<T,TComparator,PComparator,Hasher>::getHeap()
{
	return store_;
}

#endif

