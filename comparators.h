#include <vector>
#include <string>
#include "mydatastore.h"
#include "review.h"

struct AlphaStrComp
{
	bool operator()(Product* lhs, Product* rhs) 
    {
    	return lhs->getName() < rhs->getName(); 
    }
};

struct RatingComp
{
	bool operator()(Product* lhs, Product* rhs)
	{
		std::vector<Review*> t_left = lhs->getReview();
		std::vector<Review*> t_right = rhs->getReview();
		double leftAvgRating = 0.0;
		double rightAvgRating = 0.0;
		for (unsigned int i = 0;i<t_left.size(); i++)
			leftAvgRating += t_left[i]->rating;
		for (unsigned int i = 0;i<t_right.size(); i++)
			rightAvgRating += t_right[i]->rating;
		if (t_left.size() != 0)
			leftAvgRating /= t_left.size();
		if (t_right.size() != 0)
			rightAvgRating /= t_right.size();

		return leftAvgRating > rightAvgRating;
	}
};
	
struct ReviewComp
{
	bool operator()(Review* lhs, Review* rhs)
	{
		return lhs->date > rhs->date;
	}
};

struct SuggestionComp
{
	bool operator()(std::pair<std::string, double> lhs, std::pair<std::string, double> rhs)
	{
		if (lhs.second == rhs.second)
			return lhs.first < rhs.first;
		return lhs.second > rhs.second;
	}
};