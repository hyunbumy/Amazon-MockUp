#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <map>

/**
 * Implements User functionality and information storage
 *  You should not need to derive anything from User at this time
 */
class User {
public:
    User();
    User(std::string name, double balance, int type, unsigned long long hash);
    virtual ~User();

    double getBalance() const;
    std::string getName() const;
    unsigned long long getHash() const;
    void deductAmount(double amt);
    virtual void dump(std::ostream& os);
    //void setBasicSim(std::string, double sim);

private:
    std::string name_;
    double balance_;
    int type_;
    unsigned long long passHash_;
    //std::map<std::string, double> basicSim;
};
#endif
