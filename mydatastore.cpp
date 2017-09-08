#include "mydatastore.h"
#include "util.h"
#include "heap.h"
#include <cmath>

MyDataStore::MyDataStore()
{

}

MyDataStore::~MyDataStore()
{
    // De-allocate things
    for (unsigned int i = 0; i<userList_.size(); i++)
        delete userList_[i];
    for (unsigned int i = 0; i<productList_.size(); i++)
        delete productList_[i];
    for (unsigned int i = 0; i<orphanReview_.size(); i++)
        delete orphanReview_[i];
}

/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p)
{
    productList_.push_back(p);

    // Map every keyword to a vector of Products that have such keyword
    std::set<std::string>::iterator it;
    std::set<std::string> temp = p->keywords();
    for (it = temp.begin(); it!=temp.end(); ++it)
        keys[(*it)].insert(p);
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u)
{
    userList_.push_back(u);
}

void MyDataStore::addReview(Review* r)
{
    bool found = false;
    for (unsigned int i = 0; i<productList_.size(); i++)
    {
        if (convToLower(r->prodName) == 
            convToLower(productList_[i]->getName()))
        {
            found = true;
            productList_[i]->setReview(r);
            break;
        }
    }
    if (!found)
        orphanReview_.push_back(r);
}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, 
    int type)
{
    std::vector<Product*> res;
    std::set<Product*> temp = keys[terms[0]];
    for (unsigned int i = 1; i<terms.size(); i++)
    {
        if (type == 0)
            temp = setIntersection(temp, keys[terms[i]]);
        else if (type == 1)
            temp = setUnion(temp, keys[terms[i]]);
    }
    std::set<Product*>::iterator it;
    for (it = temp.begin(); it != temp.end(); ++it)
        res.push_back((*it));
    return res;
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(std::ostream& ofile)
{
    ofile << "<products>" << std::endl;
    for (unsigned int i = 0; i<productList_.size(); i++)
        productList_[i]->dump(ofile);
    ofile << "</products>" << std::endl;
    ofile << "<users>" << std::endl;
    for (unsigned int i = 0; i<userList_.size(); i++)
        userList_[i]->dump(ofile);
    ofile << "</users>" << std::endl;
    ofile << "<reviews>" << std::endl;
    for (unsigned int i = 0; i<productList_.size(); i++)
        productList_[i]->reviewDump(ofile);
    for (unsigned int i = 0; i<orphanReview_.size(); i++)
    {
        Review temp = *(orphanReview_[i]);
        ofile << temp.prodName << std::endl;
        ofile << temp.rating << " " << temp.username << " " << temp.date;
        ofile << " " << temp.reviewText << std::endl;
    }
    ofile << "</reviews>" << std::endl;
}

void MyDataStore::addCart(std::vector<Product*> hits, 
    std::string usr, int index)
{
    bool found = false;
    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        if (usr == (userList_[i])->getName())
        {
            found = true;
            break;
        }
    }

    if (!found || index > int(hits.size()) || index < 0)
        std::cout << "Invalid request" << std::endl;
    else
        (cart[usr]).push(hits[index-1]);
}

void MyDataStore::viewCart(std::string usr)
{
    bool found = false;
    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        if (userList_[i]->getName() == usr)
        {
            found = true;
            break;
        }
    }
    if (!found)
        std::cout << "Invalid username" << std::endl;
    else
    {
        std::queue<Product*> temp = cart[usr];
        int index = 1;
        while (!temp.empty())
        {
            Product* curr = temp.front();
            std::cout << "Item " << index++ << std::endl;
            std::cout << curr->displayString() << std::endl;
            std::cout << std::endl;
            temp.pop();
        }
    }
}

void MyDataStore::buyCart(std::string usr)
{
    bool found = false;
    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        if (userList_[i]->getName() == usr)
        {
            found = true;
            break;
        }
    }
    if (!found)
        std::cout << "Invalid username" << std::endl;
    else
    {
        int userIndex = -1;
        for (unsigned int i = 0; i<userList_.size(); i++)
        {
            if (usr == userList_[i]->getName())
            {
                userIndex = i;
                break;
            }
        }
        if (userIndex == -1)
            return;
        int iteration = (cart[usr]).size();
        for (int i = 0; i<iteration; i++)
        {
            Product* curr = (cart[usr]).front();
            double pricediff = (userList_[userIndex]->getBalance())
             - (curr->getPrice());
            if (curr->getQty() > 0 && pricediff > 0)
            {
                userList_[userIndex]->deductAmount(curr->getPrice());
                curr->subtractQty(1);
            }
            else
                (cart[usr]).push(curr);
            cart[usr].pop();
        }
    }
}

std::vector<std::string> MyDataStore::getUserList() const
{
    std::vector<std::string> output;
    for (unsigned int i = 0; i<userList_.size();i++)
        output.push_back(userList_[i]->getName());
    return output;
}

std::queue<Product*> MyDataStore::getCartItem(std::string usr)
{
    return cart[usr];
}

void MyDataStore::removeCart(std::string usr, int itemIndex)
{
    if (itemIndex == -1)
        return;
    bool found = false;
    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        if (userList_[i]->getName() == usr)
        {
            found = true;
            break;
        }
    }
    if (!found)
        std::cout << "Invalid username" << std::endl;
    else
    {
        int userIndex = -1;
        for (unsigned int i = 0; i<userList_.size(); i++)
        {
            if (usr == userList_[i]->getName())
            {
                userIndex = i;
                break;
            }
        }
        if (userIndex == -1)
            return;
        std::queue<Product*> res = cart[usr];
        unsigned int list_size = res.size();
        for (unsigned int i = 0; i<list_size; i++)
        {
            if ((int) i == itemIndex)
                res.pop();
            else
            {
                res.push(res.front());
                res.pop();
            }
        }
        cart[usr] = res;
    }
}

std::vector<User*> MyDataStore::getUserList(int i) const
{
    (void) i;
    return userList_;
}

// Think about how to minimize the user connections (iterations)
void MyDataStore::updateBasicSim()
{
    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        std::map<std::string, double> newEntry;
        for (unsigned int j = 0; j<userList_.size(); j++)
        {
            if (i != j)
            {
                double unitBasicSim = calcBasicSim(userList_[i], userList_[j]);
                newEntry[userList_[j]->getName()] = unitBasicSim;
                basicSim_[userList_[i]->getName()] = newEntry;
            }
        }
    }
}

double MyDataStore::calcBasicSim(User* a, User* b)
{
    std::set<Product*> productA;
    std::set<Product*> productB;
    std::set<Product*> intersection;
    std::map<Product*, int> reviewRatingA;
    std::map<Product*, int> reviewRatingB;

    for (unsigned int i = 0; i<productList_.size(); i++)
    {
        int ratingA = hasRated(a->getName(), productList_[i]);
        int ratingB = hasRated(b->getName(), productList_[i]);
        if (ratingA != -1)
        {
            productA.insert(productList_[i]);
            reviewRatingA[productList_[i]] = ratingA;
        }
        if (ratingB != -1)
        {
            productB.insert(productList_[i]);
            reviewRatingB[productList_[i]] = ratingB;
        }
    }

    intersection = setIntersection(productA, productB);
    if (intersection.empty())
        return 1.0;
    std::set<Product*>::iterator it;
    double sim = 0;
    for (it = intersection.begin(); it!=intersection.end(); ++it)
        sim += abs((reviewRatingA[*it] - reviewRatingB[*it]))/4.0;
    return sim/intersection.size();
}

std::map<std::string, double> MyDataStore::calcRefineSim(std::string a)
{
    Heap<std::string> heap;
    std::map<std::string, double> output;
    std::map<std::string, double>::iterator it;
    output[a] = 0;
    heap.push(0, a);

    for (unsigned int i = 0; i<userList_.size(); i++)
    {
        std::string currName = userList_[i]->getName();
        if (currName != a)
        {
            output[currName] = 1;
            heap.push(basicSim_[a][currName], currName);
        }
    }
    while (!heap.empty())
    {
        std::string curr = heap.top();
        heap.pop();
        for (it=basicSim_[curr].begin(); it!=basicSim_[curr].end(); ++it)
        {
            if ((output[curr] + it->second) < output[it->first])
            {
                output[it->first] = output[curr] + it->second;
                heap.decreaseKey(output[it->first], it->first);
            }
        }
    }

    return output;
}

std::vector<std::pair<std::string, double> > MyDataStore::makeSuggestion(std::string currentUser)
{
    updateBasicSim();
    std::map<std::string, double> currSim = calcRefineSim(currentUser);
    std::vector<std::pair<std::string, double> > result;
    for (unsigned int i = 0; i<productList_.size(); i++)
    {
        if (hasRated(currentUser, productList_[i]) == -1)
        {
            double ratingSum = 0.0;
            double weightSum = 0.0;
            for (unsigned int j = 0; j < userList_.size(); j++)
            {
                std::string comp_user = userList_[j]->getName();
                int prod_rating = hasRated(comp_user, productList_[i]);
                if (prod_rating != -1)
                {
                    ratingSum += (1-currSim[comp_user])*prod_rating;
                    weightSum += (1-currSim[comp_user]);
                }
            }
            double final_rating;
            if (ratingSum == 0 || weightSum == 0)
                final_rating = 0;
            else
            {
                final_rating = ratingSum / weightSum;
            }
            result.push_back(std::make_pair(productList_[i]->getName(), final_rating));
        }
    }

    return result;
}

// Checks to see if the user has rated the product,
// return -1 with not, the rating if did
int MyDataStore::hasRated(std::string user, Product* currProd)
{
    std::vector<Review*> currReviews = currProd->getReview();
    for (unsigned int i = 0; i <currReviews.size(); i++)
    {
        if (currReviews[i]->username == user)
            return currReviews[i]->rating;
    }
    return -1;
}

std::map< std::string, std::map<std::string, double> > MyDataStore::getSim() const
{
    return basicSim_;
}