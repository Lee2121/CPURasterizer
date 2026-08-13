#pragma once

template <typename T, size_t N>
class TFixedArray
{

public:

	constexpr size_t Size() const { return N; }

	T& operator[](size_t Index) { return Data[Index]; }
	const T& operator[](size_t Index) const { return Data[Index]; }

	bool Contains(const T& Elem) const
	{
		for (const T& CurrElem : Data)
		{
			if (CurrElem == Elem)
			{
				return true;
			}
		}
		return false;
	}

private:

	T Data[N];

public:

	struct Iterator
	{
		using Pointer		= T*;
		using Reference		= T&;
		using ValueType		= T;

		Iterator(Pointer InPtr) : Ptr(InPtr) {}

		// Dereference operator
		Reference operator*() const
		{
			return *Ptr;
		}

		Pointer operator->()
		{
			return Ptr;
		}

		// ++iterator
		Iterator operator++()
		{
			++Ptr;
			return *this;
		}

		// iterator++
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		friend bool operator==(const Iterator& A, const Iterator& B)
		{
			return A.Ptr == B.Ptr;
		}

		friend bool operator!=(const Iterator& A, const Iterator& B)
		{
			return A.Ptr != B.Ptr;
		}

	private:
		Pointer Ptr;
	};

	Iterator begin() { return &Data[0]; }
	Iterator end() { return &Data[Size()]; }
};