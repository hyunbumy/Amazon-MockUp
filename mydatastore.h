#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <map>
#include <queue>
#include "datastore.h"

/**
 * DataStore Interface needed for parsing and instantiating products and users
 *
 * A derived version of the DataStore can provide other services as well but
 * must support those below
 */
class MyDataStore : public DataStore
{
public:
    MyDataStore();
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
    * Adds a review
    */
    void addReview(Review* r);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    void addCart(std::vector<Product*> hits, std::string usr, int index);

    void viewCart(std::string usr);

    void buyCart(std::string usr);

    void removeCart(std::string usr, int itemIndex);

    std::vector<std::string> getUserList() const;   // Return names

    std::vector<User*> getUserList(int) const;  // Get the list of Users

    std::queue<Product*> getCartItem(std::string usr);

    void updateBasicSim();    // Calculate the basic similarities of all users

    std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);

    // For debuggin
    std::map< std::string, std::map<std::string, double> > getSim() const;
private:
    // Calculate the avg similarity between two users
    double calcBasicSim(User* a, User* b);
    // Calculate refined similarity given the username
    std::map<std::string, double> calcRefineSim(std::string);
    // Checks to see if the user has rated the product,
    // return -1 with not, the rating if did
    int hasRated(std::string, Product*);
    std::vector<User*> userList_;
    std::vector<Product*> productList_;
    std::vector<Review*> orphanReview_; // List of reviews with no product
    std::map<std::string,std::queue<Product*> > cart;
    std::map<std::string, std::set<Product*> > keys;
    std::map< std::string, std::map<std::string, double> > basicSim_;
};
#endif