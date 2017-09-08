#include <QApplication>
#include <QGridLayout>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "multiwin.h"
#include "msort.h"
#include "hash.h"
#include "mydatastore.h"

Multiwin::Multiwin(MyDataStore* ds) : QWidget(NULL), dat(ds)
{
  // Main Window
  QGridLayout* mainLayout = new QGridLayout;
  setWindowTitle("Amazon");

  // Section 1
  QLabel* searchLabel = new QLabel("Type in keywords");
  mainLayout->addWidget(searchLabel, 0, 0, 1, 5);
  QLabel* userLabel = new QLabel("Users");
  mainLayout->addWidget(userLabel, 0, 5, 1, 2);

  // Section 2
  searchBox = new QLineEdit();
  mainLayout->addWidget(searchBox, 1, 0, 2, 5);

  orButton = new QPushButton("OR");
  mainLayout->addWidget(orButton, 1, 5, 1, 1);
  QObject::connect(orButton, SIGNAL(clicked()), 
  	this, SLOT(orSearchClicked()));

  andButton = new QPushButton("AND");
  mainLayout->addWidget(andButton, 2, 5, 1, 1);
  QObject::connect(andButton, SIGNAL(clicked()), 
  	this, SLOT(andSearchClicked()));

  QLabel* currLabel = new QLabel("Current User: ");
  mainLayout->addWidget(currLabel, 1, 6, 1, 1);
  currUserLabel = new QLabel("");
  mainLayout->addWidget(currUserLabel, 1, 7, 1, 1);

  changeUserButton = new QPushButton("Change User");
  mainLayout->addWidget(changeUserButton, 2, 6, 1, 1);
  QObject::connect(changeUserButton, SIGNAL(clicked()),
    this, SLOT(changeUserButtonClicked()));

  // Section 3
  QLabel* itemLabel = new QLabel("ITEMS");
  mainLayout->addWidget(itemLabel, 3, 2, 1, 1);
  QLabel* reviewLabel = new QLabel("REVIEWS");
  mainLayout->addWidget(reviewLabel, 3, 6, 1, 1);

  // Section 4
  itemList = new QListWidget();
  mainLayout->addWidget(itemList, 4, 0, 5, 5);
  QObject::connect(itemList, SIGNAL(currentRowChanged(int)),
  	this, SLOT(itemSelected(int)));

  reviewList = new QListWidget();
  mainLayout->addWidget(reviewList, 4, 5, 5, 3);

  // Section 5
  QLabel* sortLabel = new QLabel("Sort :");
  mainLayout->addWidget(sortLabel, 9, 0, 2, 2);

  alphaChecked = true;
  alphaSortButton = new QRadioButton("Alphabatical");
  mainLayout->addWidget(alphaSortButton, 9, 2, 1, 3);
  alphaSortButton->setChecked(true);
  QObject::connect(alphaSortButton, SIGNAL(clicked()),
  	this, SLOT(alphaSortClicked()));

  ratingSortButton = new QRadioButton("Avg. Rating");
  mainLayout->addWidget(ratingSortButton, 10, 2, 1, 3);
  QObject::connect(ratingSortButton, SIGNAL(clicked()),
  	this, SLOT(ratingSortClicked()));

  reviewSortButton = new QPushButton("Sort Review");
  mainLayout->addWidget(reviewSortButton, 9, 6, 1, 2);
  QObject::connect(reviewSortButton, SIGNAL(clicked()),
  	this, SLOT(reviewSortClicked()));

  addReviewButton = new QPushButton("Add Review");
  mainLayout->addWidget(addReviewButton, 10, 6, 1, 2);
  QObject::connect(addReviewButton, SIGNAL(clicked()),
    this, SLOT(addReviewClicked()));

  QLabel* empty = new QLabel(" ");
  mainLayout->addWidget(empty, 11, 0, 1, 1);

  // Section 6
  addCartButton = new QPushButton("Add Cart");
  mainLayout->addWidget(addCartButton, 15, 1, 1, 3);
  QObject::connect(addCartButton, SIGNAL(clicked()),
  	this, SLOT(addCartClicked()));

  viewCartButton = new QPushButton("View Cart");
  mainLayout->addWidget(viewCartButton, 16, 1, 1, 3);
  QObject::connect(viewCartButton, SIGNAL(clicked()), 
  	this, SLOT(viewCartClicked()));

  makeRecButton = new QPushButton("Product Recommendation");
  mainLayout->addWidget(makeRecButton, 15, 4, 1, 3);
  QObject::connect(makeRecButton, SIGNAL(clicked()),
    this, SLOT(recClicked()));

  saveButton = new QPushButton("SAVE");
  mainLayout->addWidget(saveButton, 15, 7, 1, 1);
  QObject::connect(saveButton, SIGNAL(clicked()),
  	this, SLOT(saveClicked()));

  quitButton = new QPushButton("QUIT");
  mainLayout->addWidget(quitButton, 16, 7, 1, 1);
  QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

  setLayout(mainLayout);

  // Cart View
  QHBoxLayout* viewCartLayout = new QHBoxLayout;
  viewCartWin = new QWidget;
  viewCartWin->setWindowTitle("Cart View");
  
  cartItemDisplay = new QVBoxLayout();
  viewCartLayout->addLayout(cartItemDisplay);
  cartButtonDisplay = new QVBoxLayout();
  viewCartLayout->addLayout(cartButtonDisplay);

  cartItemList = new QListWidget();
  cartItemDisplay->addWidget(cartItemList);

  userBalance = new QLabel(getUserBalance());
  cartButtonDisplay->addWidget(userBalance);

  removeButton = new QPushButton("Remove");
  cartButtonDisplay->addWidget(removeButton);
  QObject::connect(removeButton, SIGNAL(clicked()),
  	this, SLOT(removeClicked()));
  buyCartButton = new QPushButton("Buy Cart");
  cartButtonDisplay->addWidget(buyCartButton);
  QObject::connect(buyCartButton, SIGNAL(clicked()),
  	this, SLOT(buyCartClicked()));

  closeButton = new QPushButton("Close");
  cartButtonDisplay->addWidget(closeButton);
  QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  viewCartWin->setLayout(viewCartLayout);

  // Add Review View
  QVBoxLayout* addReviewLayout = new QVBoxLayout();
  addReviewWin = new QWidget;

  ratingDisplay = new QHBoxLayout();
  addReviewLayout->addLayout(ratingDisplay);

  QLabel* ratingLabel = new QLabel("Rating: ");
  ratingDisplay->addWidget(ratingLabel);

  one = new QRadioButton("1");
  ratingDisplay->addWidget(one);
  one->setChecked(1);
  two = new QRadioButton("2");
  ratingDisplay->addWidget(two);
  three = new QRadioButton("3");
  ratingDisplay->addWidget(three);
  four = new QRadioButton("4");
  ratingDisplay->addWidget(four);
  five = new QRadioButton("5");
  ratingDisplay->addWidget(five);

  calendar = new QCalendarWidget();
  addReviewLayout->addWidget(calendar);
  calendar->setDateRange(QDate(2016, 1, 1), QDate(2030, 12, 31));

  QLabel* reviewTextLabel = new QLabel("Type your review");
  addReviewLayout->addWidget(reviewTextLabel);

  review = new QLineEdit();
  addReviewLayout->addWidget(review);

  buttonDisplay = new QHBoxLayout();
  addReviewLayout->addLayout(buttonDisplay);

  addButton = new QPushButton("ADD");
  buttonDisplay->addWidget(addButton);
  QObject::connect(addButton, SIGNAL(clicked()),
    this, SLOT(reviewAddClicked()));

  cancelButton = new QPushButton("Cancel");
  buttonDisplay->addWidget(cancelButton);
  QObject::connect(cancelButton, SIGNAL(clicked()),
    this, SLOT(reviewCancelClicked()));

  addReviewWin->setLayout(addReviewLayout);

  // Log-In View
  QVBoxLayout* loginLayout = new QVBoxLayout;
  loginWin = new QWidget;
  loginWin->setWindowTitle("Log-In");

  QLabel* userNameLabel = new QLabel("Username : ");
  loginLayout->addWidget(userNameLabel);
  userNameText = new QLineEdit();
  loginLayout->addWidget(userNameText);

  QLabel* passwordLabel = new QLabel("Password : ");
  loginLayout->addWidget(passwordLabel);
  passwordText = new QLineEdit();
  loginLayout->addWidget(passwordText);

  loginButtonLayout = new QHBoxLayout;
  loginButton = new QPushButton("Login");
  loginButtonLayout->addWidget(loginButton);
  QObject::connect(loginButton, SIGNAL(clicked()),
    this, SLOT(loginButtonClicked()));

  logQuitButton = new QPushButton("Quit");
  loginButtonLayout->addWidget(logQuitButton);
  QObject::connect(logQuitButton, SIGNAL(clicked()),
    this, SLOT(quit()));

  newUserButton = new  QPushButton("Create New User");
  loginButtonLayout->addWidget(newUserButton);
  QObject::connect(newUserButton, SIGNAL(clicked()),
    this, SLOT(newUserButtonClicked()));

  loginLayout->addLayout(loginButtonLayout);
  loginWin->setLayout(loginLayout);
  loginWin->show();

  // Recommendation View
  QVBoxLayout* recLayout = new QVBoxLayout;
  recWin = new QWidget;
  recWin->setWindowTitle("Product Recommendation");

  recList = new QListWidget();
  recLayout->addWidget(recList);

  doneButton = new QPushButton("DONE");
  recLayout->addWidget(doneButton);
  QObject::connect(doneButton, SIGNAL(clicked()),
    this, SLOT(doneButtonClicked()));

  recWin->setLayout(recLayout);

  //****** Debug
  /*std::map< std::string, std::map<std::string, double> > sim_ = dat->getSim();
  std::map<std::string, std::map<std::string,double> >::iterator it;
  std::map<std::string, double>::iterator it2;
  for (it=sim_.begin(); it!=sim_.end(); ++it)
  {
    std::cout << it->first << std::endl;
    for (it2 = (it->second).begin(); it2!=(it->second).end(); ++it2)
      std::cout << it2->first << " " << it2->second << std::endl;
    std::cout << std::endl;
  }
  for (unsigned int i = 0; i<dat->getUserList().size(); i++)
  {
    std::vector<std::pair<std::string, double> > temp;
    temp = dat->makeSuggestion(dat->getUserList()[i]);
    mergeSort(temp, comp4);
    for (unsigned int j = 0; j < temp.size(); j++)
    {
      std::cout << temp[j].first << " " << temp[j].second << std::endl;
    }
    std::cout << std::endl;
  }*/
  
}
Multiwin::~Multiwin()
{
	delete dat;
}

// Main Window Functions
void Multiwin::changeUserButtonClicked()
{
  this->hide();
  loginWin->show();
}
void Multiwin::viewCartClicked()
{
  viewCartWin->show();
}
void Multiwin::orSearchClicked()
{
	if (searchBox->text().isEmpty())
		return;
	std::stringstream temp;
	temp.str(searchBox->text().toStdString());
	std::string term;
	std::vector<std::string> terms;
	while(temp >> term)
	{
		term = convToLower(term);
		terms.push_back(term);
	}
	hits = dat->search(terms, 1);
	searchBox->setText("");
	updateItem();
}
void Multiwin::andSearchClicked()
{
	if (searchBox->text().isEmpty())
		return;
	std::stringstream temp;
	temp.str(searchBox->text().toStdString());
	std::string term;
	std::vector<std::string> terms;
	while(temp >> term)
	{
		term = convToLower(term);
		terms.push_back(term);
	}
	hits = dat->search(terms, 0);
	searchBox->setText("");
	updateItem();
}
void Multiwin::itemSelected(int itemIndex)
{
	if (itemIndex == -1)
		rev.clear();
	else
		rev = hits[itemIndex]->getReview();
	updateReview();
}
void Multiwin::userChanged(int usrIndex)
{
	if (usrIndex == -1)
		return;
	currUser = (dat->getUserList())[usrIndex];
	updateCart();
}
void Multiwin::addCartClicked()
{
	if (itemList->currentRow() == -1)
		return;
	dat->addCart(hits, currUser, itemList->currentRow()+1);
	updateCart();
}
void Multiwin::saveClicked()
{
	std::string filename = (QFileDialog::getSaveFileName(this, 
		tr("Save File"))).toStdString();
	if (filename == "")
		return;
	std::ofstream ofile(filename.c_str());
    dat->dump(ofile);
    ofile.close();
}
void Multiwin::alphaSortClicked()
{
  alphaChecked = true;
  updateItem();
}
void Multiwin::ratingSortClicked()
{
  alphaChecked = false;
  updateItem();
}
void Multiwin::reviewSortClicked()
{
  mergeSort(rev, comp3);
  updateReview();
}
void Multiwin::addReviewClicked()
{
  if (itemList->currentRow() != -1)
  {
    addReviewWin->show();
    setEnabled(0);
    viewCartWin->setEnabled(0);
  }
}
void Multiwin::recClicked()
{
  recWin->show();
  setEnabled(0);
  viewCartWin->setEnabled(0);
  dat->updateBasicSim();

  // Populate the list with product recommendation
  recList->clear();
  std::vector<std::pair<std::string, double> > recom = dat->makeSuggestion(currUser);
  mergeSort(recom, comp4);
  for (unsigned int i = 0; i<recom.size(); i++)
  {
    std::stringstream ss;
    ss << "Rating : " << std::setprecision(3) << recom[i].second <<"\n";
    ss << "Product : " << recom[i].first;
    std::string output = ss.str();
    recList->addItem(QString::fromStdString(output));
  }

  // Dump to output file
  std::ofstream outfile("rec.txt");
  outfile << currUser << std::endl;
  for (unsigned int i = 0; i<recom.size(); i++)
  {
    outfile << std::setprecision(3) << recom[i].second <<" ";
    outfile << recom[i].first << std::endl;
  }
  outfile.close();
}
void Multiwin::quit()
{
  QApplication::exit();
}

// View Cart Functions
void Multiwin::buyCartClicked()
{
  dat->buyCart(currUser);
  updateCart();
  updateItem();
}
void Multiwin::removeClicked()
{
	dat->removeCart(currUser, cartItemList->currentRow());
	updateCart();
}
void Multiwin::close()
{
  viewCartWin->hide();
}

// Add Review Functions
void Multiwin::reviewAddClicked()
{
  int r = -1;
  std::string date;
  std::string outputText;
  if (one->isChecked())
    r = 1;
  else if (two->isChecked())
    r = 2;
  else if (three->isChecked())
    r = 3;
  else if (four->isChecked())
    r = 4;
  else if (five->isChecked())
    r = 5;
  if (r == -1 || review->text().isEmpty())
    return;
  date = ((calendar->selectedDate()).toString("yyyy-MM-dd")).toStdString();
  outputText = review->text().toStdString();
  Review* r_ptr = new Review(hits[itemList->currentRow()]->getName(), r,
    currUser, date, outputText);
  dat->addReview(r_ptr);
  resetReviewWin();
  rev = hits[itemList->currentRow()]->getReview();
  updateReview();
}
void Multiwin::reviewCancelClicked()
{
  resetReviewWin();
}
void Multiwin::resetReviewWin()
{
  addReviewWin->hide();
  one->setChecked(1);
  review->setText("");
  viewCartWin->setEnabled(1);
  setEnabled(1);
}

// LogIn Fuctions
void Multiwin::loginButtonClicked()
{
  std::string userName = userNameText->text().toStdString();
  std::string password = passwordText->text().toStdString();
  bool userFound = false;
  int userIndex = -1;
  bool passwordMatch = false;
  for (unsigned int i = 0; i<dat->getUserList().size(); i++)
  {
    if (userName == (dat->getUserList())[i])
    {
      userIndex = i;
      userFound = true;
      break;
    }
  }
  if (userFound)
  {
    unsigned long long hash = passToHash(password);
    if (hash == ((dat->getUserList(1))[userIndex])->getHash())
      passwordMatch = true;
  }

  if (userFound && passwordMatch)
  {
    currUser = userName;
    currUserLabel->setText(QString::fromStdString(currUser));
    this->show();
    loginWin->hide();
  }
  else
  {
    QMessageBox errorMsg(this);
    errorMsg.setText("Incorrect username or password");
    errorMsg.exec();
  }

  userNameText->clear();
  passwordText->clear();
}
void Multiwin::newUserButtonClicked()
{
  if (userNameText->text().isEmpty() || passwordText->text().isEmpty())
  {
    QMessageBox emptyLogin(this);
    emptyLogin.setText("Username and Password cannot be empty");
    emptyLogin.exec();
  }
  else
  {
    std::string userName = userNameText->text().toStdString();
    std::string password = passwordText->text().toStdString();
    bool match = false;
    for (unsigned int i = 0; i<dat->getUserList().size(); i++)
    {
      if (userName == (dat->getUserList())[i])
      {
        match = true;
        break;
      }
    }
    // If the user already exists
    if (match)
    {
      QMessageBox existingUser(this);
      existingUser.setText("User already exists");
      existingUser.exec();
    }
    // Check password length
    else if (password.length() > 8)
    {
      QMessageBox invalidPass(this);
      invalidPass.setText("Invalid Password");
      invalidPass.exec();
    }
    // Add the new user to the database
    else
    {
      dat->addUser(new User(userName, 100.00, 0, passToHash(password)));
      currUser = userName;
      currUserLabel->setText(QString::fromStdString(currUser));
      this->show();
      loginWin->hide();
    }
  }
  userNameText->clear();
  passwordText->clear();
}

// Rec View Fuctions
void Multiwin::doneButtonClicked()
{
  setEnabled(1);
  viewCartWin->setEnabled(1);
  recWin->hide();
}

// Misc.
void Multiwin::updateCart()
{
  cartItemList->clear();
  userBalance->setText(getUserBalance());
  std::queue<Product*> temp = dat->getCartItem(currUser);
  while (!temp.empty())
    {
      Product* curr = temp.front();
      std::stringstream ss;
      ss << curr->getName() << "\n";
      ss << "Price: " << curr->getPrice() << ", ";
      ss << "Qty: " << curr->getQty() << std::endl;
      std::string output = ss.str();
      cartItemList->addItem(QString::fromStdString(output));
      temp.pop();
    }
}
void Multiwin::updateItem()
{
  itemList->clear();
  if (alphaChecked)
    mergeSort(hits, comp1);
  else
    mergeSort(hits, comp2);
  for (unsigned int i = 0; i<hits.size(); i++)
  {
    std::stringstream ss;
    ss << hits[i]->getName() << "\n";
    ss << "Price: " << hits[i]->getPrice() << ", ";
    ss << "Qty: " << hits[i]->getQty() << std::endl;
    std::string output = ss.str();
    itemList->addItem(QString::fromStdString(output));
  }
}
void Multiwin::updateReview()
{
  reviewList->clear();
  for (unsigned int i = 0; i<rev.size(); i++)
  {
    std::stringstream ss;
    ss << rev[i]->prodName << "\n";
    ss << "Rating: " << rev[i]->rating << "/ ";
    ss << rev[i]->username << "/ " << rev[i]->date << std::endl;
    ss << rev[i]->reviewText << std::endl;
    std::string output = ss.str();
    reviewList->addItem(QString::fromStdString(output));
  }
}
QString Multiwin::getUserBalance()
{
  double res = 0;
  std::vector<User*> temp = dat->getUserList(1);
  for (unsigned int i =0; i<temp.size(); i++)
  {
    if (currUser == temp[i]->getName())
    {
      res = temp[i]->getBalance();
      break;
    }
  }
  QString qstr = "Balance: " + QString::number(res, 'f', 2);
  return qstr;
}