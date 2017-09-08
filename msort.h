#include <iostream>
#include <vector>

template <class T, class Comparator>
void merge(std::vector<T>& list, std::vector<T>& output,
			int s1, int e1, int s2, int e2, Comparator comp)
{
	int ptr = s1;
	while ((s1 < e1 || s2 < e2) && ptr < e2)
	{
		if (s1 == e1)
		{
			output[ptr] = list[s2];
			s2++;
		}
		else if (s2 == e2)
		{
			output[ptr] = list[s1];
			s1++;
		}
		else if ((comp(list[s1], list[s2])))
		{
			output[ptr] = list[s1];
			s1++;
		}
		else if (!(comp(list[s1], list[s2])))
		{
			output[ptr] = list[s2];
			s2++;
		}
		ptr++;
	}
	//for (int i = 0; )
	//list = output;	// O(n)
}

template <class T, class Comparator>
void mergeSort_h(std::vector<T>& list, std::vector<T>& output, 
				int start, int end, Comparator comp)
{
	if ((end - start) < 2)
		return;
	int mid = (start+end)/2;
	// switch output & list so that it alternates everytime for merge
	mergeSort_h(output, list, start, mid, comp);
	mergeSort_h(output, list, mid, end, comp);
	merge(list, output, start, mid, mid, end, comp);
}

template <class T, class Comparator>
void mergeSort(std::vector<T>& myArray, Comparator comp)
{
	std::vector<T> temp(myArray);
	mergeSort_h(temp, myArray, 0, myArray.size(), comp);
}