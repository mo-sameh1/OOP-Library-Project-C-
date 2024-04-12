#include <bits/stdc++.h>
#include <windows.h> //was going to be used to do Sleep() but we've decided to remove the Sleep
#include "Library.cpp"
using namespace std;

class Transaction;
class PenaltyObserver{ // For notifying the transaction that the deadline is exceeded and penalty now holds
public:
virtual void notify(Transaction* transaction)=0;
};

class Transaction
{
    int trans_type = 0;
    int time = 30; // Time until deadline
    bool late_fee = false;
    int fee = 15;
    clock_t begin; // starting the time counter
    clock_t end;   // ending the cound
    // clock_t is used to store the processor time in terms of the number of CPU cycles passed since the start of the process
    books borrowedBook;
    User borrowingUser;
    vector<PenaltyObserver*> Observers;

public:
    void setTransaction(int n) //function was named previously : getTransactionType, but it in fact would set the transaction, so we did a rename method
    { // setter function
        Start_transaction(n);
    }
    Transaction() { setTransType(2); } // default constructor

    void setTransType(int t)
    {
        trans_type = t;
    }
    int getTransType()
    {
        return trans_type;
    }
    void setLateFee(bool x)
    {
        late_fee = x;
    }
    bool getLateFee()
    {
        return late_fee;
    }
    void Stopwatch(string type)
    { // parametrizing method
        if (type == "begin")
        {
            Begin();
        }
        if (type == "end")
        {
            End();
        }
    }
    void Begin()
    {
        begin = clock();
    }
    clock_t getBegin()
    {
        return begin;
    }
    void End()
    {
        end = clock();
    }
    clock_t getEnd()
    {
        return end;
    }
    int timeTaken()
    {                                                    // we calculate the differnce in time from the start of borrowing till returning the book
        return (getEnd() - getBegin()) / CLOCKS_PER_SEC; // we use CLOCK_PER_SEC to convert the CPU cycles into Seconds. when we're done we check
    }                                                    // whether the user exceeded the time limit or not

    void setBorrowedBook(books k)
    {
        borrowedBook = k;
    }
    books getBorrowedBook()
    { // getter
        return borrowedBook;
    }

    void setBorrowingUser(User m)
    { // setter
        borrowingUser = m;
    }
    User getBorrowingUser()
    { // getter
        return borrowingUser;
    }

    bool searchTransaction(string name, string user)
    {
        if (name == borrowedBook.getBookName() && user == borrowingUser.getName())
            return true;
        else
            return false;
    }
    void BorrowBook()
    {
        Stopwatch("begin"); // starting the a stop watch when you borrow
        setTransType(1);
    }
    Transaction(int type)
    { // type 1 is borrow, type 2 is return
        if (type == 1)
        {
            BorrowBook();
        }
        else if (type == 2)
        {
            setTransType(2);
            returnBook(trans_type);
        }
    }

    bool penalty()
    { // a function to determine whether the user returned the book late or not
        int time_taken = timeTaken();
        if (time_taken > time)
            {
                for(auto observer: Observers){
                    observer->notify(this);
                }
            }
        if (getLateFee())
        {
            cout << "You have exceeded the deadline by " << time_taken - time << " seconds, you will have to pay a late fee of 15 pounds" << endl;
            return getLateFee();
        } // This is to see how much the time was exceeded by
        cout << "You have returned the book within the deadline, you will not have to pay a late fee" << endl;
        return getLateFee();
    }

    void returnBook(int i)
    { // the int is just to check if the user started the transaction by returning a book or borrowing one
        if (i == 1)
        {
            Stopwatch("end"); // record when the book was returned
        }
        else if (i == 2)
        {
        }
        penalty();
    }

    void Start_transaction(int n)
    { // where we start the transaction to then process the needed transaction
        if (n == 1)
        {
            BorrowBook();
        }
        else if (n == 2)
        {
            returnBook(getTransType());
        }
    }
    void Register_Observers(PenaltyObserver* observer){
        Observers.push_back(observer);
    }
};

class LateFeeObserver: public PenaltyObserver{
//observer to notify transaction that the deadline is reached
public:
void notify(Transaction* transaction){
    transaction->setLateFee(true);
    }
};

vector<Transaction> transactions;
