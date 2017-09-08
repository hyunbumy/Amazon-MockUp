#include <sstream>
#include "movie.h"
#include "util.h"

Movie::Movie(const std::string category, const std::string name, 
	double price, int qty, std::string genre, std::string rating) :
	Product(category, name, price, qty),
	genre_(genre),
	rating_(rating)
{

}

Movie::~Movie()
{

}

std::set<std::string> Movie::keywords() const
{
	std::set<std::string> temp = parseStringToWords(genre_);
	std::set<std::string> key = parseStringToWords(getName());
	std::set<std::string>::iterator it;
	for (it = temp.begin(); it != temp.end(); ++it)
		key.insert(*it);
	return key;
}

std::string Movie::displayString() const
{
	std::stringstream ss;
	ss << getName() << std::endl;
	ss << "Genre: " << genre_ << " Rating: " << rating_ << std::endl;
	ss << getPrice() << " " << getQty() << " left.";
	return ss.str();
}

void Movie::dump(std::ostream& os) const
{
	Product::dump(os);
	os << genre_ << std::endl;
	os << rating_ << std::endl;
}

std::string Movie::getGenre() const
{
	return genre_;
}

std::string Movie::getRating() const
{
	return rating_;
}

void Movie::setGenre(std::string genre)
{
	genre_ = genre;
}

void Movie::setRating(std::string rating)
{
	rating_ = rating;
}