#ifndef MULTIWIN_H
#define MULTIWIN_H
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QCalendarWidget>

#include <string>
#include <vector>
#include <sstream>
#include <string>

#include "product.h"
#include "mydatastore.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "comparators.h"

class Multiwin : public QWidget
{
	Q_OBJECT
public:
	Multiwin(MyDataStore* ds);
	~Multiwin();
public slots:
	void updateCart();	// Update Cart window
	QString getUserBalance();	// Returns the user's current balance
	void updateItem();	// Update Item list window
	void updateReview();	// Update Review window
	void resetReviewWin();	// Reset addReview window fields

	// Main Window buttons
	void changeUserButtonClicked();
	void orSearchClicked();	// Search with OR implementation
	void andSearchClicked();	// Search with AND implementation
	void itemSelected(int);	// Display the selected item's reviews
	void userChanged(int);	// Change the selected user
	void alphaSortClicked();	// Sort the items alphabatically
	void ratingSortClicked();	// Sort the items based on avg. rating
	void addCartClicked();	// Add selected item to user's cart
	void viewCartClicked();	//Show multiwin
	void reviewSortClicked();	// Sort reviews based on dates
	void addReviewClicked();	// Display Review win
	void recClicked();
	void saveClicked();	// Save win pop_up
	void quit();	// Quit

	// ViewCart buttons
	void removeClicked();	// Remove selected item from the user's cart
	void buyCartClicked();	// Buy items on the user's cart
	void close();	// Hide CartView window

	// AddReview buttons
	void reviewAddClicked();	// Add a new review for the selected item
	void reviewCancelClicked();	// Close AddReview window

	// LogIn buttons
	void loginButtonClicked();
	void newUserButtonClicked();

	// Rec buttons
	void doneButtonClicked();
	
private:
	MyDataStore* dat;
	std::vector<Product*> hits;
	std::string currUser;
	bool alphaChecked;
	std::vector<Review*> rev;

	// Main Layout
	// Buttons
	QPushButton* orButton;
	QPushButton* andButton;
	QRadioButton* alphaSortButton;
	QRadioButton* ratingSortButton;
	QPushButton* reviewSortButton;
	QPushButton* addReviewButton;
	QPushButton* addCartButton;
	QPushButton* viewCartButton;
	QPushButton* saveButton;
	QPushButton* quitButton;
	QPushButton* makeRecButton;
	// Others
	QLineEdit* searchBox;
	QLabel* currUserLabel;
	QPushButton* changeUserButton;
	QListWidget* itemList;
	QListWidget* reviewList;

	// ViewCart Layout
	QVBoxLayout* cartItemDisplay;
	QVBoxLayout* cartButtonDisplay;
	QWidget* viewCartWin;
	QListWidget* cartItemList;
	QPushButton* removeButton;
	QPushButton* buyCartButton;
	QPushButton* closeButton;
	QLabel* userBalance;

	// AddReview Layout
	QWidget* addReviewWin;
	QHBoxLayout* ratingDisplay;
	QRadioButton* one;
	QRadioButton* two;
	QRadioButton* three;
	QRadioButton* four;
	QRadioButton* five;
	QCalendarWidget* calendar;
	QLineEdit* review;
	QHBoxLayout* buttonDisplay;
	QPushButton* addButton;
	QPushButton* cancelButton;

	// Log-In Layout
	QWidget* loginWin;
	QHBoxLayout* loginButtonLayout;
	QLineEdit* userNameText;
	QLineEdit* passwordText;
	QPushButton* loginButton;
	QPushButton* logQuitButton;
	QPushButton* newUserButton;

	// Rec Layout
	QWidget* recWin;
	QListWidget* recList;
	QPushButton* doneButton;
	
	// Comparators
	AlphaStrComp comp1;
	RatingComp comp2;
	ReviewComp comp3;
	SuggestionComp comp4;
};
#endif