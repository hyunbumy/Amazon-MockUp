#include <sstream>
#include "clothing.h"
#include "util.h"

Clothing::Clothing(const std::string category, const std::string name, 
	double price, int qty, std::string size, std::string brand) :
	Product(category, name, price, qty),
	size_(size),
	brand_(brand)
{

}

Clothing::~Clothing()
{

}

std::set<std::string> Clothing::keywords() const
{
	std::set<std::string> temp = parseStringToWords(brand_);
	std::set<std::string> key = parseStringToWords(getName());
	std::set<std::string>::iterator it;
	for (it = temp.begin(); it != temp.end(); ++it)
		key.insert(*it);
	return key;
}

std::string Clothing::displayString() const
{
	std::stringstream ss;
	ss << getName() << std::endl;
	ss << "Size: " << size_ << " Brand: " << brand_ << std::endl;
	ss << getPrice() << " " << getQty() << " left.";
	return ss.str();
}

void Clothing::dump(std::ostream& os) const
{
	Product::dump(os);
	os << size_ << std::endl;
	os << brand_ << std::endl;
}

std::string Clothing::getSize() const
{
	return size_;
}

std::string Clothing::getBrand() const
{
	return brand_;
}

void Clothing::setSize(std::string size)
{
	size_ = size;
}

void Clothing::setBrand(std::string brand)
{
	brand_ = brand;
}