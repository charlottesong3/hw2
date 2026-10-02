#include "mydatastore.h"
#include "util.h"
#include <iostream>

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for(vector<Product*>::iterator it = products_.begin();
        it != products_.end(); ++it) {
        delete *it;
    }

    for(map<string, User*>::iterator it = users_.begin();
        it != users_.end(); ++it) {
        delete it->second;
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);

    set<string> words = p->keywords();

    for(set<string>::iterator it = words.begin();
        it != words.end(); ++it) {
        string keyword = convToLower(*it);
        keywordIndex_[keyword].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    string username = convToLower(u->getName());
    users_[username] = u;
    carts_[username] = vector<Product*>();
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    vector<Product*> hits;

    if(terms.empty()) {
        return hits;
    }

    set<Product*> matches;

    if(type == 0) {
        // AND search begins with the matches for the first term.
        string firstTerm = convToLower(terms[0]);

        if(keywordIndex_.find(firstTerm) == keywordIndex_.end()) {
            return hits;
        }

        matches = keywordIndex_[firstTerm];

        for(size_t i = 1; i < terms.size(); i++) {
            string term = convToLower(terms[i]);

            if(keywordIndex_.find(term) == keywordIndex_.end()) {
                matches.clear();
                break;
            }

            set<Product*> termMatches = keywordIndex_[term];
            matches = setIntersection(matches, termMatches);
        }
    }
    else if(type == 1) {
        // OR search begins empty and adds matches for every term.
        for(size_t i = 0; i < terms.size(); i++) {
            string term = convToLower(terms[i]);

            if(keywordIndex_.find(term) != keywordIndex_.end()) {
                set<Product*> termMatches = keywordIndex_[term];
                matches = setUnion(matches, termMatches);
            }
        }
    }

    for(set<Product*>::iterator it = matches.begin();
        it != matches.end(); ++it) {
        hits.push_back(*it);
    }

    return hits;
}

bool MyDataStore::addToCart(const string& username, Product* product)
{
    string lowerUsername = convToLower(username);

    if(users_.find(lowerUsername) == users_.end() || product == NULL) {
        return false;
    }

    carts_[lowerUsername].push_back(product);
    return true;
}

bool MyDataStore::viewCart(const string& username)
{
    string lowerUsername = convToLower(username);

    if(users_.find(lowerUsername) == users_.end()) {
        return false;
    }

    vector<Product*>& cart = carts_[lowerUsername];

    for(size_t i = 0; i < cart.size(); i++) {
        cout << "Item " << i + 1 << endl;
        cout << cart[i]->displayString() << endl;
        cout << endl;
    }

    return true;
}

bool MyDataStore::buyCart(const string& username)
{
    string lowerUsername = convToLower(username);

    if(users_.find(lowerUsername) == users_.end()) {
        return false;
    }

    User* user = users_[lowerUsername];
    vector<Product*>& cart = carts_[lowerUsername];

    size_t i = 0;

    while(i < cart.size()) {
        Product* product = cart[i];

        if(product->getQty() > 0 &&
           user->getBalance() >= product->getPrice()) {
            user->deductAmount(product->getPrice());
            product->subtractQty(1);

            // Do not increment i after erasing. The next item shifts to i.
            cart.erase(cart.begin() + i);
        }
        else {
            ++i;
        }
    }

    return true;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;

    for(vector<Product*>::iterator it = products_.begin();
        it != products_.end(); ++it) {
        (*it)->dump(ofile);
    }

    ofile << "</products>" << endl;
    ofile << "<users>" << endl;

    for(map<string, User*>::iterator it = users_.begin();
        it != users_.end(); ++it) {
        it->second->dump(ofile);
    }

    ofile << "</users>" << endl;
}