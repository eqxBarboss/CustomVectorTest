#include <stdlib.h>
#include <stdexcept>

#define DEFAULT_CAPACITY 10

template <class T>
class CustomVector
{
public:
	CustomVector();
	CustomVector(const CustomVector<T>& customVector);
	CustomVector(int elementsCount);
	~CustomVector();

	CustomVector<T>& operator=(const CustomVector<T>& customVector);
	const T& operator[] (const int index) const;
	T& operator[] (const int index);
	const T* Data() const;
	T* Data();

	int Count() const;
	int Capacity() const;

	void Clear();
	void PushBack(const T& element);
	void PopBack();
	void Insert(const int index, const T& element);
	void Erase(const int index);
	void Resize(const int elementsCount);

private:
	T* items;
	int count;
	int capacity;

	void IncreaseCapacity(int newCapacity = -1);
	void DecreaseCapacity(int newCapacity = -1);
};

template <class T>
CustomVector<T>::CustomVector()
	: items(nullptr),
	count(0),
	capacity(0)
{

}

template <class T>
CustomVector<T>::CustomVector(const CustomVector<T>& customVector)
	: items(nullptr),
	count(customVector.count),
	capacity(customVector.capacity)
{
	if (capacity) {
		items = static_cast<T*>(malloc(sizeof(T) * capacity));
		if (items == nullptr) throw new std::bad_alloc();
		memcpy(items, customVector.items, sizeof(T) * capacity);
	}
}

template <class T>
CustomVector<T>::CustomVector(int elementsCount)
	: items(nullptr),
	count(elementsCount),
	capacity(elementsCount)
{
	if (elementsCount <= 0) throw std::invalid_argument("Received invalid elements count.");

	items = static_cast<T*>(malloc(sizeof(T) * capacity));
	if (items == nullptr) throw new std::bad_alloc();
	for (int i = 0; i < count; new (items + i++) T());
}

template <class T>
CustomVector<T>::~CustomVector()
{
	Clear();
}

template <class T>
CustomVector<T>& CustomVector<T>::operator=(const CustomVector<T>& customVector)
{
	if (customVector.capacity) {
		for (int i = 0; i < count; items[i++].~T());
		count = 0;
		T* newItems = static_cast<T*>(realloc(items, sizeof(T) * customVector.capacity));
		if (newItems == nullptr) throw new std::bad_alloc();
		items = newItems;
		count = customVector.count;
		capacity = customVector.capacity;
		memcpy(items, customVector.items, sizeof(T) * capacity);
	}
	else {
		Clear();
	}
}

template <class T>
const T& CustomVector<T>::operator[] (const int index) const
{
	if ((count == 0) || (index > count - 1) || (index < 0)) throw new std::invalid_argument("Received invalid index.");
	return items[index];
}

template <class T>
T& CustomVector<T>::operator[] (const int index)
{
	if ((count == 0) || (index > count - 1) || (index < 0)) throw new std::invalid_argument("Received invalid index.");
	return items[index];
}

template <class T>
T* CustomVector<T>::Data()
{
	return items;
}

template <class T>
const T* CustomVector<T>::Data() const
{
	return items;
}

template <class T>
int CustomVector<T>::Count() const
{
	return count;
}

template <class T>
int CustomVector<T>::Capacity() const
{
	return capacity;
}

template <class T>
void CustomVector<T>::Clear()
{
	for (int i = 0; i < count; items[i++].~T());
	free(items);
	items = nullptr;
	count = 0;
	capacity = 0;
}

template <class T>
void CustomVector<T>::PushBack(const T& element)
{
	if (count == capacity) IncreaseCapacity();
	new (items + count) T(element);
	count++;
}

template <class T>
void CustomVector<T>::PopBack()
{
	Erase(count - 1);
}

// Index must be in [0; count].
template <class T>
void CustomVector<T>::Insert(const int index, const T& item)
{
	if (count == index) return PushBack(item);

	if ((count == 0) || (index > count - 1) || (index < 0)) throw new std::invalid_argument("Received invalid index.");

	if (count == capacity) IncreaseCapacity();

	memmove(items + index + 1, items + index, (count - index) * sizeof(T));
	new (items + index) T(item);
	count++;
}

template <class T>
void CustomVector<T>::Erase(const int index)
{
	if ((count == 0) || (index > count - 1) || (index < 0)) throw new std::invalid_argument("Received invalid index.");

	items[index].~T();
	memmove(items + index, items + index + 1, (count - (index + 1)) * sizeof(T));
	count--;

	if ((count < capacity / 2) && (capacity > DEFAULT_CAPACITY)) DecreaseCapacity();
}

template <class T>
void CustomVector<T>::Resize(const int elementsCount)
{
	if (elementsCount <= 0) throw std::invalid_argument("Received invalid elements count.");

	if (elementsCount == count) return;

	if (elementsCount > count) {
		if (elementsCount > capacity) IncreaseCapacity(elementsCount);
		for (int i = count; i < elementsCount; i++) new (items + i) T();
		count = elementsCount;
	}

	if (elementsCount < count) {
		for (int i = elementsCount; i < count; i++) items[i].~T();
		count = elementsCount;
		if ((elementsCount < capacity / 2) && (capacity > DEFAULT_CAPACITY)) DecreaseCapacity();
	}
}

template <class T>
void CustomVector<T>::IncreaseCapacity(int newCapacity)
{
	if (newCapacity == -1)
		newCapacity = ((capacity << 1) <= DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : (capacity << 1);
	T* newItems = static_cast<T*>(realloc(items, sizeof(T) * newCapacity));
	if (newItems == nullptr) throw new std::bad_alloc();

	items = newItems;
	capacity = newCapacity;
}

template <class T>
void CustomVector<T>::DecreaseCapacity(int newCapacity)
{
	if (newCapacity == -1)
		newCapacity = ((capacity >> 1) <= DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : (capacity >> 1);
	capacity = newCapacity;

	T* newItems = static_cast<T*>(realloc(items, sizeof(T) * newCapacity));
	if (newItems == nullptr) throw new std::bad_alloc();
	items = newItems;
}
