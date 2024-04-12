#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class User
{
private:
    static int no_users;
    string name;
    string password;
    int ID;
    string membership;
    bool borrowed;          // if the user has a borrowed book or not
    int maxNoBorrowedBooks; // the max amount of book that the user can borrow, depending on the membership
    vector<string> borrowedBooks;
    vector<string> tags; // tags are what the user likes , so we can recommend
    string memberType;
public:
    User()
    {
        no_users++;
        ID = no_users;
    }
    User(string n, string p)
    { // overloaded constructor
        name = n;
        password = p;
        no_users++;
        ID = no_users;
        borrowed = false;
        membership = "Normal";
        memberType = "normalUser";
    }
    void changeMembership(string m)
    { // a function to change the membership
        membership = m;
        getMaxBorrow(); // chaging the Max borrow
    }
    string getMembership()
    { // getter function
        return membership;
    }
    virtual void borrowBook(string bookName,string genre)
    {   
        borrowedBooks.push_back(bookName); // pushing back the borrowed book into a vector of strings
        for(int i =0;i<tags.size();i++){
            if(tags[i]==genre)return;
        }
        tags.push_back(genre);
    }
    bool checkBorrowed()
    { // getter function
        return borrowed;
    }
    vector<string> BorrowedBooks()
    { // getter function to the borrowed books
        return borrowedBooks;
    }
    string printBorrowedBooks()
    {
        string str = "";
        for (int i = 0; i < borrowedBooks.size(); i++)
        {
            str += borrowedBooks[i];
            bool lastBookCheck = i < borrowedBooks.size() - 1;
            str += (lastBookCheck ? "-" : "");
        }
        if (borrowedBooks.size() == 0)
        {
            str = "-1";
        }
        return str;
    }
    string printTags()
    {
        string str = "";
        for (int i = 0; i < tags.size(); i++)
        {
            str += tags[i];
            bool lastTagCheck = i < tags.size() - 1;
            str += (lastTagCheck ? "-" : "");
        }
        if(tags.size()==0)
        {
            str="-1";
        }
        return str;
    }
    string getMemberType()
    {
        return memberType;
    }
    void returnaBook(string name)
    {                                  // function to return a book
        int sz = borrowedBooks.size(); // vector size
        int i = 0;
        bool check = false;
        for (i; i < sz; i++)
        { // checking if you have any borrowed books
            if (borrowedBooks[i] == name)
            {
                check = true; // yes you do
                break;
            }
        }
        if (check = true)
        {
            borrowedBooks.erase(borrowedBooks.begin() + i);
            updateBorrowed();
        }
    }
    string getName()
    { // getter
        return name;
    }
    string getPassword()
    { // getter
        return password;
    }
    int getID()
    {
        return ID;
    }
    void changeMemberType(string m){
        memberType=m;
    }
    bool getBorrowed()
    {
        return borrowed;
    }
    vector<string> getTags()
    { // getting the tags
        return tags;
    }
    int getMaxBorrow()
    {
        if (getMembership() == "Normal")
        { // normal only has 1 allowed borrows
            return maxNoBorrowedBooks = 1;
        }
        else if (getMembership() == "Premium")
        { // premium has 3 allowed borrows
            return maxNoBorrowedBooks = 3;
        }
        else if (getMembership() == "Diamond")
        { // diamond has 5 allowed borrows
            return maxNoBorrowedBooks = 5;
        }
        else
            return maxNoBorrowedBooks = 0;
    }
    void updateBorrowed()
    {
        if (borrowedBooks.size() > 0)
            borrowed = true;
        else
            borrowed = false;
    }
    friend void createUser(vector<string> row, vector<User> &allUsers);
    friend int authentication(vector<User> allUsers, string name, string password);
};

class StaffMember : public User
{ // a staff user, also a diamond user
private:
    int staffId;

public:
    StaffMember(){};
    StaffMember(string n, string p) : User(n, p)
    {
        changeMembership("Diamond");
        changeMemberType("staffMember");
    }
};

class StudentMember : public User
{ // a student user, can also be a premium user
private:
    string studentEmail;

public:
    StudentMember(){};
    StudentMember(string n, string p) : User(n, p)
    {
        changeMembership("Premium");
        changeMemberType("studentUser");
    }
};

enum userTypes{
    normalUser,
    staffUser,
    studentUser
};

class UserFactory{
public:
    User*build(userTypes userType){
        if(userType==0)return new User();
        else if(userType==1)return new StaffMember();
        else if(userType==2)return new StudentMember();
        else return nullptr;
    }
    User*build(userTypes userType,string n,string p){
        if(userType==0)return new User();
        else if(userType==1)return new StaffMember(n,p);
        else if(userType==2)return new StudentMember(n,p);
        else return nullptr;
    }
};

int User::no_users = 0;

vector<User> readUsers()
{ // vector of objects containing users
    vector<User> allUsers;
    string fname = "users.csv"; // file name

    vector<string> row;
    string line, word;



    fstream file(fname, ios::in); // accessing the file
    if (file.is_open())
    {
        while (getline(file, line))
        { // getting line
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
            {
                row.push_back(word); // getting the user names
            }

            createUser(row, allUsers);
        }
    }
    else
        cout << "Could not open the file\n"; // if file didnt open
    return allUsers;
}

void createUser(vector<string> row, vector<User> &allUsers)
{
    UserFactory*ufactory= new UserFactory();
    userTypes type;
    if(row[7]=="normalUser")type = normalUser;
    else if(row[7]=="studentUser")type= studentUser;
    else type = staffUser;
    User* u = ufactory->build(type);
    u->name = row[0];       // getting name from the database
    u->password = row[1];   // getting Password from the database
    u->changeMembership(row[3]); // getting membership from the database
    string word;
    if (row[4] == "True")
    { // checking if the user borrowed a book
        u->borrowed = true;
        stringstream str(row[5]);
        while (getline(str, word, '-'))
            u->borrowedBooks.push_back(word);
    }
    else
        u->borrowed = false;
    stringstream str2(row[6]);
    while (getline(str2, word, '-'))
        u->tags.push_back(word);
    u->memberType = row[7];
    allUsers.push_back(*u);
    delete u;
    delete ufactory;

}

bool checkUser(string name, string password, User user)
{
    if (name == user.getName() && password == user.getPassword())
        return true;
    return false;
}

int authentication(vector<User> allUsers, string name, string password)
{ // a function to ensure that the user enter correct info
    for (int i = 0; i < allUsers.size(); i++)
    {
        if (checkUser(name, password, allUsers[i]))
            return i;
        else if (name == allUsers[i].name)
            return -1;
    }
    return -1;
}


/*
Refactoring done:
    Composing methods
        Extract Variable
            - Moved the expression in lines 69 and 84 to a more self-explanatory variable
    Organizing Data
        Self Encapsulate Field
            - in line 139, I changed the getMaxBorrow function to get the membership string from the getter function rather than direct access to membership itself
    Simplifying Conditional Expressions
        Decompose Conditional
            - moved the conditional in the authentication function in line 293 to a seperate method called checkUser

Design Patterns done:
    Factory Design Pattern
        I've made implemented factory that makes users (normal user, staff user or a student user) as each of them has different membership and so
        different functionalities. 
*/
