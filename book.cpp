#include <sstream>
#include "book.h"
#include "util.h"

Book::Book(const std::string category, const std::string name, 
	double price, int qty, std::string isbn, std::string author) :
	Product(category, name, price, qty),
	isbn_(isbn),
	author_(author)
{

}

Book::~Book()
{

}

std::set<std::string> Book::keywords() const
{
	std::set<std::string> temp = parseStringToWords(author_);
	std::set<std::string> key = parseStringToWords(getName());
	std::set<std::string>::iterator it;
	for (it = temp.begin(); it != temp.end(); ++it)
		key.insert(*it);
	key.insert(isbn_);
	return key;
}

std::string Book::displayString() const
{
	std::stringstream ss;
	ss << getName() << std::endl;
	ss << "Author: " << author_ << " ISBN: " << isbn_ << std::endl;
	ss << getPrice() << " " << getQty() << " left.";
	return ss.str();
}

void Book::dump(std::ostream& os) const
{
	Product::dump(os);
	os << isbn_ << std::endl;
	os << author_ << std::endl;
}

std::string Book::getIsbn() const
{
	return isbn_;
}

std::string Book::getAuthor() const
{
	return author_;
}
void Book::setIsbn(std::string isbn)
{
	isbn_ = isbn;
}

void Book::setAuthor(std::string author)
{
	author_ = author;
}