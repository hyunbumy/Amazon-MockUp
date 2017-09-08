#ifndef CLOTHING_H
#define CLOTHING_H
#include "product.h"

class Clothing : public Product
{
public:
	Clothing(const std::string category, const std::string name, 
		double price, int qty, std::string size, std::string brand);
	~Clothing();
	std::set<std::string> keywords() const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
	std::string getSize() const;
	std::string getBrand() const;
	void setSize(std::string size);
	void setBrand(std::string brand);
private:
	std::string size_;
	std::string brand_;
};
#endif