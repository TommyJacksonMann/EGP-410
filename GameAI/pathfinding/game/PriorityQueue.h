#pragma once
#include <queue>

/* This Code was taken from Josh Grazda's Github with his permission.*/

template<class T, class Container = std::vector<T>, class Compare = std::less<typename Container::value_type>>
class PriorityQueue : public std::priority_queue<T, Container, Compare>
{
public:
	typedef typename std::priority_queue<T, Container, Compare>::container_type::const_iterator const_iterator;

	const_iterator begin() const
	{
		auto first = this->c.cbegin();
		return first;
	}

	const_iterator end() const
	{
		auto end = this->c.cend();
		return end;
	}

	const_iterator contains(const T&value) const
	{
		auto first = this->c.cbegin();
		auto end = this->c.cend();
		while (first != end)
		{
			if (*first == value)
			{
				return first;
			}
			++first;
		}
		return end;
	}

	bool remove(const T& value)
	{
		auto it = std::find(this->c.begin(), this->c.end(), value);
		if (it != this->c.end())
		{
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		else
		{
			return false;
		}
	}

};

	//Compare cost stuct
	struct CompareCost : public std::binary_function<NodeRecord, NodeRecord, bool>
	{
		bool operator()(const NodeRecord lhs, const NodeRecord rhs) const
		{
			return lhs.mCostSoFar > rhs.mCostSoFar;
		}
	};

	//Compare estimated cost struct
	struct CompareEstimatedCost : public std::binary_function<NodeRecord, NodeRecord, bool>
	{
		bool operator()(const NodeRecord lhs, const NodeRecord rhs) const
		{
			return lhs.mEstimatedTotalCost > rhs.mEstimatedTotalCost;
		}
	};
