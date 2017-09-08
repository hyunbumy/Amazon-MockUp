#include <sstream>
#include <iomanip>
#include "product.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category)
{

}

Product::~Product()
{
    for (unsigned int i =0; i<prodReview_.size(); i++)
        delete prodReview_[i];
}


double Product::getPrice() const
{
    return price_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    (void) searchTerms;
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
}


vector<Review*> Product::getReview() const
{
    return prodReview_;
}

void Product::setReview(Review* r)
{
    prodReview_.push_back(r);
}

void Product::reviewDump(ostream& os) const
{
    for (unsigned int i = 0; i<prodReview_.size(); i++)
    {
        Review* temp = prodReview_[i];
        os << temp->prodName << endl;
        os << temp->rating << " " << temp->username << " " << temp->date;
        os << " " << temp->reviewText << endl;
    }
}