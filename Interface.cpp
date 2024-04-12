#include "TransactionClass.cpp"
using namespace std;
#define TRIES 3

class interFace
{
private:
    int associatedLibrary;
    Library assocLibrary; // The Library being linked
    vector<Library> Libraries;
    vector<User> allUsers; // The users associated with the library
public:
    interFace(vector<Library> &x)
    {
        Libraries.resize(x.size());
        Libraries.assign(x.begin(), x.end());
    }

    interFace() {} // Default constructor

    string dashLineprint() { return "-------------------------------------"; }

    void Login(void)
    { // Starts the login sequence to any of the library branches
        int tries = TRIES;
        bool failedLoginAttempts = true;
        int Libraryindex = 0;
        while (tries)
        {
            cout << "You have " << tries << " tries left" << endl;
            tries--;
            cout << "Enter the user name: ";
            string key1;
            fflush(stdin);
            getline(cin, key1);
            cout << "Enter the password: ";
            string key2s;
            cin >> key2s;
            bool loginCheck = false;

            bool hasAlpha = false; // Checks if user inputs a string as only integers are accepted. This is a type of error handling
            for (auto ch : key2s){
                if (isalpha(ch)){
                    hasAlpha = true;
                    break;
                }
            }
            if (!hasAlpha){ 
                int key2 = stoi(key2s);
            }
            if (!hasAlpha){ 
                int key2 = stoi(key2s);
                for (Libraryindex = 0; Libraryindex < NOLIBRARIES; Libraryindex++)
                {
                    if (Libraries[Libraryindex].Login(key1, key2))
                    { // checking if the login info is correct for any library
                        loginCheck = true;
                        failedLoginAttempts = false;
                        break;
                    }
                }
            }
            if (!loginCheck)
            { // the login info arent correct for any library
                cout << "Invalid credentials." << endl
                     << dashLineprint() << endl;
            }
            else
                break;
        }
        if (tries == 0 && failedLoginAttempts)
        { // user used all the tries
            cout << "You failed all login attempts, closing program." << endl;
            exit(1); // Exits the program
        }
        else
        {
            associatedLibrary = Libraryindex;
            cout << "You have successfully logged into library: " << associatedLibrary + 1 << endl
                 << dashLineprint() << endl; // Greeting the librarian
            cout << "Welcome Mr. " << Libraries[associatedLibrary].getLibrarianName() << endl
                 << dashLineprint() << endl;                      // getting the librarian name from the associated Library
            Libraries[associatedLibrary].readDataBase();          // reading the database
            assocLibrary = Library(Libraries[associatedLibrary]); // Linking the assocLibrary with the logged-into library.
        }
    }

    char userOptions(){
        fflush(stdin);
        cout << "Choose the option you want to do right now: " << endl;
        cout << "1- add a book" << endl
             << "2- remove a book" << endl
             << "3- start a transaction" << endl
             << "4- search for a book" << endl
             << "5- print the entire library" << endl
             << "6- other CMD commands" << endl
             << "7- Configure User" << endl
             << "8- Add a new User" << endl
             << "Or press any other value to EXIT the program" << endl;
        char choice;
        cout << "Enter your choice: "; // THE MAIN MENU INTERFACE FOR THE PROGRAM
        cin >> choice;
        return choice;
    }

    int mainMenu()
    {
        char choice = userOptions();
        fflush(stdin);
        cout << dashLineprint() << endl;
        if (choice == '1')
            addBook(); // Manually add a book
        else if (choice == '2')
            removeBook(); // Manually remove a book
        else if (choice == '3')
            startTransaction(); // Starting a Transacation
        else if (choice == '4')
            searchForBook(); // Searching for a book
        else if (choice == '5')
            printLibrary(); // printing the books in library
        else if (choice == '6')
            otherCMDcommands(); // shortcut comands
        else if (choice == '7')
            userCommands(); // configuring the user
        else if (choice == '8')
            addUser(); // adding a new user to the database
        else
        {
            writeToCSV(assocLibrary.getLibraryData());
            writeToCSV(allUsers, assocLibrary.getLibraryName());
            return 0;
        }
        return (int)choice;
    }

    void welcomeScreen()
    { // at the very start of the program
        Database obj;
        allUsers = obj.readUsers();
        cout << "Welcome to the library management system. First, you need to login to your library." << endl
             << dashLineprint() << endl;
        Login();
    }

    books getBookData()
    {
        string name, author, genre;
        int copies, year;
        cout << "Enter the Book Name: "; // getting name
        getline(cin, name);
        cout << "Enter the Book Author: "; // getting author
        getline(cin, author);
        cout << "Enter the Book Genre: "; // getting genre
        getline(cin, genre);
        cout << "Enter the number of copies: "; // getting copies
        cin >> copies;
        cout << "Enter the year of publication: "; // getting year
        cin >> year;
        return books(name, author, genre, year, copies);
    }

    void addBook()
    { // system-user interaction
        books searchBook = getBookData();
        int bookIndex = searchForBook(searchBook);
        if (bookIndex == -1)
        {
            assocLibrary.bookADD(searchBook); // the library added it's 1st copy of the book
            cout << "Book: " << searchBook.getBookName() << " successfully added" << endl
                 << dashLineprint() << endl;
        }
        else
        {
            assocLibrary.bookADD(searchBook, bookIndex); // the library added more copies
            cout << "Book copies for: " << searchBook.getBookName() << " successfully added" << endl
                 << dashLineprint() << endl;
        }
    }

    void addBook(string name, string author, string genre, int year, int copies)
    { // overloaded function, function is called when the system in itself adds a new book (system-system interaction)
        books temp(name, author, genre, year, copies);
        int bookIndex = searchForBook(temp);
        if (bookIndex == -1)
        {
            assocLibrary.bookADD(temp); // the library added it's 1st copy of the book
            cout << "Book: " << name << " successfully added" << endl
                 << dashLineprint() << endl;
        }
        else
        {
            assocLibrary.bookADD(temp, bookIndex); // the library added more copies
            cout << "Book copies for: " << name << " successfully added" << endl
                 << dashLineprint() << endl;
        }
    }

    int removeBook()
    {
        string name;
        cout << "Enter the name of the book you want to remove: ";
        getline(cin, name);
        int bookIndex = searchForBook(name);
        if (bookIndex == -1)
        {
            cout << "Book: " << name << " Doesn't Exist" << endl;
            return -1;
        }
        else
        {
            assocLibrary.bookREMOVE(bookIndex); // the book was in the library and got removed
            cout << "Book: " << name << " successfully removed" << endl
                 << dashLineprint() << endl;
            return 1;
        }
    }

    int removeBook(string name)
    { // overloaded function
        int bookIndex = searchForBook(name);
        if (bookIndex == -1)
        {
            cout << "Book: " << name << " Doesn't Exist" << endl;
            return -1; // book is'nt even here
        }
        else
        {
            assocLibrary.bookREMOVE(bookIndex); // the book was in the library and got removed
            cout << "Book: " << name << " successfully removed" << endl
                 << dashLineprint() << endl;
            return 1;
        }
    }

    void borrowBook()
    {
        string bookname;
        cout << "Enter the borrowed book name: " << endl;
        getline(cin, bookname);
        string userName;
        string userPassword; // getting user info
        cout << "Enter the borrowing user's name: " << endl;
        getline(cin, userName);
        cout << "Enter the borrowing user's password: " << endl;
        getline(cin, userPassword);
        borrowBookUtil(userName,userPassword,bookname);
    }

    void borrowBookUtil(string userName,string userPassword, string bookname){
        Transaction objTrans;
        LateFeeObserver* observer=new LateFeeObserver(); // Creating an observer and attaching it to the transaction
        objTrans.Register_Observers(observer);
        transactions.push_back(objTrans); // pushing back an object to the vector

        int indexUser = authentication(allUsers, userName, userPassword);
        Book_Search *searchOBJ = Book_Search::instancePtr;
        int indexBook = searchOBJ->searchByName(assocLibrary.getLibraryData(), bookname);
        
        bool flag = 1;
        vector<string> userBooks = allUsers[indexUser].BorrowedBooks();
        for (int i =0;i < userBooks.size();i++){
            if (userBooks[i] == bookname){
                flag = 0;
                break;
            }
        }

        bool UserCanBorrow = eligibilityToBorrow(indexUser, indexBook) && flag;
        if (UserCanBorrow)
        {
            transactions[transactions.size() - 1].setTransaction(1); // Change here line 236 was duplicated
            transactions[transactions.size() - 1].setBorrowedBook(assocLibrary.getBookFromLibrary(indexBook));
            transactions[transactions.size() - 1].setBorrowingUser(allUsers[indexUser]); // Borrow complete
            assocLibrary.removeCopies(indexBook, 1);                                     // update library
            Book_Search *searchOBJ = Book_Search::instancePtr;
            string genre = searchOBJ->getGenre(assocLibrary.getLibraryData(),bookname);
            allUsers[indexUser].borrowBook(bookname,genre);      // Update User DATA
            allUsers[indexUser].updateBorrowed();                                        // Update USER DATA
        }
        else
        {
            transactions.pop_back();
            cout << "The Book you tried to borrow is unavailable or there is no user matching the given credentials or the user's membership doesn't comply or you have already borrowed the same book, please try again with a correct book name/ user data (Case Sensitive)" << endl
                 << dashLineprint() << endl;
        }
    }

    void returnBook()
    {
        string nameBook, nameUser;
        cout << "Enter the name of the book: " << endl;
        getline(cin, nameBook);
        cout << "Enter the name of the user: " << endl;
        getline(cin, nameUser);
        Book_Search *searchOBJ = Book_Search::instancePtr;
        int key1 = searchOBJ->searchByName(assocLibrary.getLibraryData(), nameBook);
        if (key1 == -1)
        {
            cout << "The book doesn't exist" << endl
                 << dashLineprint() << endl; // Validation for the existence of the book
        }
        int sz = transactions.size(), i = 0;
        bool check = false;
        for (i; i < sz; i++)
        {
            if (transactions[i].searchTransaction(nameBook, nameUser))
            { // Searching for the correct transaction to return
                check = true;
                break;
            }
        }
        if (check)
        {
            int x = authentication(allUsers, transactions[i].getBorrowingUser().getName(), transactions[i].getBorrowingUser().getPassword());
            Book_Search *searchOBJ = Book_Search::instancePtr;
            int index = searchOBJ->searchByName(assocLibrary.getLibraryData(), transactions[i].getBorrowedBook());
            assocLibrary.addCopies(index, 1);                                         // UPDATING BOOK DATA
            allUsers[x].returnaBook(transactions[i].getBorrowedBook().getBookName()); // UPDATING USER DATA
            transactions[i].setTransaction(2);                                        // Returning and checking for penalties
            transactions.erase(transactions.begin() + i);                             // RESOLVING the transaction
        }
        else{
            cout << "This transaction doesn't Exist" << endl
                 << dashLineprint() << endl;
        }
    }

    void startTransaction()
    {
        cout << "Which transaction would you like to do: " << endl
             << "1-Borrow a book" << endl
             << "2-Return a book" << endl;
        char choice;
        cin >> choice;
        fflush(stdin);
        while (choice != '1' && choice != '2')
        { // verification
            cout << "Invalid input. Please choose from 1 or 2 only" << endl;
            cin >> choice;
            fflush(stdin);
        }
        cout << dashLineprint() << endl;
        if (choice == '1')
        {
            borrowBook();
        }
        else
        {
            returnBook();
        }
    }

    bool eligibilityToBorrow(int indexUser, int indexBook)
    {
        bool bookExists = indexBook != -1 && assocLibrary.getBookAvailability(indexBook);
        bool userExists = indexUser != -1;
        bool userCanBorrow = allUsers[indexUser].getMaxBorrow() > allUsers[indexUser].BorrowedBooks().size();
        return bookExists && userExists && userCanBorrow;
    }

    int searchForBook(books k)
    {
        Book_Search *searchOBJ = Book_Search::instancePtr;                // creating a singleton object for the search
        return searchOBJ->searchByName(assocLibrary.getLibraryData(), k); // searching for the book by its entity
    }
    int searchForBook()
    {
        Book_Search *searchOBJ = Book_Search::instancePtr; // creating a singleton object for the search
        int searchValue = searchOBJ->searchByName(assocLibrary.getLibraryData());
        if (searchValue != -1)
        {
            cout << "Here are your chosen book's data: " << endl;
            assocLibrary.getLibraryData()[searchValue].print();
            cout << dashLineprint() << endl;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    int searchForBook(string name)
    {
        Book_Search *searchOBJ = Book_Search::instancePtr;
        return searchOBJ->searchByName(assocLibrary.getLibraryData(), name); // Try to find best matches for the given book name
    }

    void printLibrary()
    {
        assocLibrary.printLibrary();
    }

    string getUserCMDInput()
    {
        cout << "Either one of the following commands are used to shortcut execution of functions: " << endl
             << "++Book Name,Book Author,Genre,year,noCopies [Adds a book to the library no spaces in the csv line after the \',\']" << endl;
        cout << "Book name += noCopies [Adds a number of copies to the specified book], [error if book doesn't exist]" << endl;
        cout << "Book name -= noCopies [Withdraws a number of copies of the specified book from the library], [error if book doesn't exist]" << endl;
        cout << "--Book name [Removes a book from the library]" << endl;
        string commandLine;
        getline(cin, commandLine);
        standardizeCommandLineCSV(commandLine);
        return commandLine;
    }

    void addBookCMD(string &commandLine)
    {
        int validationofCSV = 0;
        int length = commandLine.length();
        for (int i = 2; i < length; i++)
        {
            if (commandLine[i] == ',')
                validationofCSV++;
        }
        if (validationofCSV != 4)
        {
            cout << "Invalid number of commas in the csv, please follow the guidelinns" << endl
                 << dashLineprint() << endl; // validating the csv command line
            otherCMDcommands();
        }
        else
        {
            commandLine.erase(commandLine.begin(), commandLine.begin() + 2); // reseting the string
            standardizeCommandLineCSV(commandLine);                          // Putting the line in csv format
            string name, author, genre, yearr, copies;
            char *newCommandLine = new char[commandLine.length() + 1];
            strcpy(newCommandLine, commandLine.c_str());
            char *token = strtok(newCommandLine, ","); // tokenizing the string to start taking book attributes
            for (int i = 0; i < 5; i++)
            {
                if (i == 0)
                    name = string(token);
                else if (i == 1)
                    author = string(token);
                else if (i == 2)
                    genre = string(token);
                else if (i == 3)
                    yearr = string(token);
                else if (i == 4)
                    copies = string(token);
                if (i < 4)
                    token = strtok(NULL, ",");
            }
            try
            { // Exception Handling
                int year = stoi(yearr);
                int copiesInt = stoi(copies);
                addBook(name, author, genre, year, copiesInt);
            }
            catch (...)
            {
                cout << "Input error, the command line has invalid values for the year and copies of the book" << endl
                     << dashLineprint() << endl;
            }
        }
    }

    char* getNewCommandLine(string& commandLine){
        char *newCommandLine = new char[commandLine.length() + 1];
        strcpy(newCommandLine, commandLine.c_str());
        return newCommandLine;
    }

    void otherCMDcommands()
    { // SHORTCUTS SOME OF THE INTERFACE FUNCTIONS WITH OPERATORS
        string commandLine = getUserCMDInput();
        if ((commandLine[0] == '+' && commandLine[1] != '+') || (commandLine[0] == '-' && commandLine[1] != '-'))
        { // verification
            cout << "Error in the beginning of the input, please follow the input guidelines" << endl
                 << dashLineprint() << endl;
            otherCMDcommands();
        }
        else if (commandLine[0] == '+' && commandLine[1] == '+')
        { // adding a book to library
            addBookCMD(commandLine);
        }
        else if (commandLine[0] == '-' && commandLine[1] == '-')
        {
            commandLine.erase(commandLine.begin(), commandLine.begin() + 2); // RESETTING THE STRING
            removeBook(commandLine);
        }
        else
        {
            int decision = 0;
            int sz1 = commandLine.length();
            for (int i = 0; i < sz1; i++)
            {
                if (commandLine[i] == '+' && commandLine[i + 1] == '='){
                    decision = 1;
                    break; // deciding which operations are to be used
                }
                else if (commandLine[i] == '-' && commandLine[i + 1] == '-'){
                    decision = 2;
                    break;
                }
            }
            if (decision == 1)
            {
                char *newCommandLine = getNewCommandLine(commandLine);
                char *token = strtok(newCommandLine, "+=");
                string bookName(token);
                for (int i = bookName.size()-1;i >=0;i--)
                {
                    if (bookName[i] == ' ') bookName.erase(bookName.begin() + i);
                    else break;
                }
                int indexLibrary = searchForBook(bookName);
                token = strtok(NULL, "+="); // splits the string into 2 parts (book and copies)
                string copiesSTR = string(token);
                while (copiesSTR[0] == ' ')
                {
                    copiesSTR.erase(copiesSTR.begin());
                }
                try
                {
                    int copies = stoi(copiesSTR); // if the second part isnt a number and gives an error
                    assocLibrary.addCopies(indexLibrary, copies);
                }
                catch (...)
                {
                    cout << "Value after += isn't a number, please enter a new command line following the guidelines" << endl
                         << dashLineprint() << endl;
                    otherCMDcommands(); // RECALL THE OTHERCMDCOMMANDS FUNCTION
                }
            }
            else if (decision == 2)
            {
                char *newCommandLine = getNewCommandLine(commandLine);
                char *token = strtok(newCommandLine, "-=");
                string bookName(token);
                for (int i = bookName.size()-1;i >=0;i--)
                {
                    if (bookName[i] == ' ') bookName.erase(bookName.begin() + i);
                    else break;
                }
                int indexLibrary = searchForBook(bookName);
                token = strtok(NULL, "-=");
                string copiesSTR = string(token);
                while (copiesSTR[0] == ' ')
                {
                    copiesSTR.erase(copiesSTR.begin());
                }
                try
                {
                    int copies = stoi(copiesSTR);
                    assocLibrary.removeCopies(indexLibrary, copies);
                }
                catch (...)
                {
                    cout << "Value after -= isn't a number, please enter a new command line following the guidelines" << endl
                         << dashLineprint() << endl;
                }
            }
        }
    }
    void standardizeCommandLineCSV(string &line)
    {
        while (line[0] == ' ')
        {
            line.erase(line.begin());
        }
        int sz = line.length();
        for (int i = 0; i < sz; i++)
        {
            if (line[i] == ',')
            {
                i++;
                while (line[i] == ' ')
                {
                    line.erase(line.begin() + i);
                }
            }
        }
    }

    char userChoice(int index){
        cout << "Welcome " << allUsers[index].getName() << endl;
        cout << dashLineprint() << endl;
        cout << "1- Change membership." << endl
             << "2- Show book Recommendations." << endl
             << "3- Get current borrowed books." << endl
             << "4- Search by Genre." << endl;
        char choice;
        cout << "Enter your choice: ";
        cin >> choice;
        return choice;
    }

    void changeMembership(int index){
        string newMembership;
        cout << "Congrats you're one of the lucky ones that can get any of the memberships for free!" << endl;
        cout << "Your current membership is " << allUsers[index].getMembership() << endl; // getting the current membership from the vector
        cout << "Please Enter the desired membership: " << endl;
        cout << "1- Premium" << endl;
        cout << "2- Diamond" << endl;
        cout << "Or enter 0 to cancel." << endl;
        int choice2 = 1;
        while (choice2)
        {
            cin >> choice2;
            if (choice2 == 1)
            { // chose premium
                allUsers[index].changeMembership("Premium");
                return;
            }
            else if (choice2 == 2)
            { // chose diamond
                allUsers[index].changeMembership("Diamond");
                return;
            }
            else
                cout << "Please enter a valid response." << endl; // verification
        }
    }

    void userCommands()
    {
        string username, password;
        cout << "Enter name: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
        int index = authentication(allUsers,username,password);
        if (index == -1)
        { // wrong information
            cout << "User not found. Please try again";
            return;
        }
        int choice = userChoice(index);
        fflush(stdin);
        if (choice == '1')
        {
            changeMembership(index);
        }
        else if (choice == '2')
        { // Book recommendation
            Book_Search *searchOBJ = Book_Search::instancePtr;
            string bookName = searchOBJ->searchByRecommendation(assocLibrary.getLibraryData(), assocLibrary.getGenresofLibrary(), allUsers[index].getTags());
            if (bookName == "DNE") cout << endl << dashLineprint() << endl;
            else borrowBookUtil(username,password,bookName);
        }
        else if (choice == '3')
        { // view borrowed books
            vector<string> userBooks = allUsers[index].BorrowedBooks();
            if (userBooks.size() == 0)
            { // checking the size of the vector
                cout << "User has no borrowed books" << endl << dashLineprint() << endl;
            }
            else
            {
                cout << "The user's borrowed books are: ";
                for (int i = 0; i < userBooks.size(); i++)
                {
                    cout << userBooks[i] << (i < userBooks.size() - 1 ? ", " : " "); // printint borrowed books
                }
                cout << endl;
            }
            cout << dashLineprint() << endl; // printing -----
        }
        else if (choice == '4')
        { // search by genre
            Book_Search *searchOBJ = Book_Search::instancePtr;
            searchOBJ->searchByGenre(assocLibrary.getLibraryData(), assocLibrary.getGenresofLibrary());
            cout << endl << dashLineprint() << endl;
        }
        else
            return;
    }

    void showMembershipDetails(){
        cout << "Why don't you take a look at our memberships?" << endl;
        cout << "(Current) Normal Membership (only 1 book can be borrowed): Free" << endl
        << "Premium Membership (3 books can be borrowed at the same time): 9.99$" << endl
        << "Diamond Membership (5 books can be borrowed at the same time): 29.99$" << endl;
        cout << "Since you're just starting out, you have the eligibility for our PRO MAX 2130 X9 ULTRA GIGA offer" << endl;
        cout << "which is basically getting any of the above memberships for free." << endl;
    }

    void addUser(){
        string firstName;
        vector<string> details;
        cout <<"Hello and welcome!"<<endl;
        string lastName;
        cout << "To Sign up, Please enter your first and last name: ";
        cin >> firstName >> lastName;
        string fullName = firstName+' '+lastName;
        details.push_back(fullName);
        string password;
        cout << "Now enter your password (try to create a strong password): ";
        cin >> password;
        details.push_back(password);
        details.push_back("");
        showMembershipDetails();
        while(true){
            cout << "Write 1 for no membership, 2 for Premium, 3 for Diamond" << endl;
            int choice;
            cin >> choice;
            if(choice==1)details.push_back("Normal");
            else if(choice==2)details.push_back("Premium");
            else if(choice==3)details.push_back("Diamond");
            else {
                cout << "Please Enter a valid number" << endl;
                continue;
            }
            break;
        }
        details.push_back("False");
        details.push_back("");
        details.push_back("");
        cout << "Please Enter your member type: "<<endl;
        int memberTypeInt;
        while(true){
            cout << "Write 1 for normal user, 2 for student user and 3 for staff user: ";
            cin >> memberTypeInt;
            if(memberTypeInt==1)details.push_back("normalUser");
            else if(memberTypeInt)details.push_back("studentUser");
            else if(memberTypeInt)details.push_back("staffUser");
            else {
                cout << "Please enter a valid number";
                continue;
            }
            break;
        } 
        createUser(details,allUsers);
        cout << "Thank you for signing up.Why don't try borrowing a book now!"<<endl;
        return;

    }

    void writeToCSV(vector<books> booksVec)
    {
        ofstream outFile;
        string libraryname = assocLibrary.getLibraryName();
        if (libraryname == "Library1")
        {
            outFile.open("Library1.txt");
        }
        else if (libraryname == "Library2")
        {
            outFile.open("Library2.txt");
        }
        else if (libraryname == "Library3")
        {
            outFile.open("Library3.txt");
        }

        outFile << assocLibrary.getLibrarianName() << "," << assocLibrary.getPassword() << endl;

        for (int i = 0; i < booksVec.size(); i++)
        {
            outFile << booksVec[i].getBookName() << "," << booksVec[i].getAuthor() << "," << booksVec[i].getGenre() << "," << booksVec[i].getYear() << "," << booksVec[i].getnoCopies() << endl;
        }

        outFile.close(); // closing the file
    }
    void writeToCSV(vector<User> usersVec, string libraryname)
    {
        ofstream outFile;
        outFile.open("users.csv");
        // writing the column headers
        outFile << "Name,password,ID,membership,Borrowed,BorrowedBooks,Tags,MemberType" << endl;

        // writing each user in the vector to the file
        for (int i = 1; i < usersVec.size(); i++)
        {
            outFile << usersVec[i].getName() << "," << usersVec[i].getPassword() << "," << usersVec[i].getID() << "," << usersVec[i].getMembership() 
            << "," << (usersVec[i].getBorrowed() ? "True" : "False") << "," << usersVec[i].printBorrowedBooks() << "," << usersVec[i].printTags() << "," << usersVec[i].getMemberType() << endl;        
        }
        outFile.close(); // closing the file
    }
};

// in line 330 added exeption handling
