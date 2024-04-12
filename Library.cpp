#include <bits/stdc++.h>
#include "Users.cpp"
using namespace std;

#define NOLIBRARIES 3 // Maybe edited later when other libraries are added.

class Genre
{
    string gen;
    int noBooks;

public:
    Genre() {} // Default constructor
    Genre(string z)
    { // Constructor with 1argument
        gen = z;
        noBooks = 0;
    }
    void increaseCount() { noBooks++; }
    void increaseCount(int num) { noBooks += num; } // overloaded function to increase count by a number that doesn' have to be 1
    void decreaseCount() { noBooks--; }
    void decreaseCount(int num) { noBooks -= num; } // overloaded function to decrease count by a number that doesn' have to be 1
    string getGenre() { return gen; }
    int getnoBooks() { return noBooks; } // Getter
    void setGenre(string l) { gen = l; } // Setter
    bool operator==(Genre x)
    { // Overloaded binary boolean operator == to check if 2 genres are the same.
        bool isSameGenre = !x.getGenre().compare(gen); //Simplofying conditionals
        if (isSameGenre)
            return true;
        else
            return false;
    }
};
/*
--------------------------------------------------------------------------
END OF GENRE CLASS
--------------------------------------------------------------------------
*/

class books : public Genre
{ // Books inherits from genre all of its attributes with public access specification.
    string nameBook;
    string author;
    int year;
    int noCopies;
    bool Available;

public:
    books() {} // Default constructor
    string getBookName()
    { // getter
        return nameBook;
    }
    int getYear()
    {
        return year;
    }
    string getAuthor()
    { // getter
        return author;
    }
    void addCopies(int x)
    {
        noCopies += x;
    }
    void removeCopies(int x)
    {
        noCopies -= x;
    }
    void setCopies(int x)
    {
        noCopies = x;
    }
    int getnoCopies() { return noCopies; } // getter
    books(string x, string y, string z, int m, int l) : Genre(z)
    { // Full constructor
        nameBook = x;
        author = y;
        year = m;
        noCopies = l;
        if (noCopies > 0)
            Available = true;
        else
            Available = false;
    }
    void print()
    {
        cout << "Book name: " << nameBook << ", Book Author: " << author << ", Book Genre: " << getGenre() << ", Publication Year: " << year << ", Availability: " << (Available ? "True" : "False") << endl; // Ternary operator used to check if a book is avialable or not
    }
    bool operator==(books l)
    { // Overloaded operator to check if 2 books are the same.
        bool isSameName = !nameBook.compare(l.nameBook);
        bool isSameAuthor = !author.compare(l.author);
        if ( isSameName&&isSameAuthor ) //extract variable
            return true;
        else
            return false;
    }
    void operator+=(int x)
    { // Overloaded operator to add x number of copies
        this->addCopies(x);
    }
    void operator-=(int x)
    { // Overloaded operator to take away x number of copies
        this->removeCopies(x);
    }
    bool checkAvailable()
    { // Getter for the boolean available variable
        return noCopies > 0 ? true : false;
    }
    void updateAvailable()
    { // Update the boolean available variable
        Available = noCopies > 0 ? true : false;
    }
};

/*
--------------------------------------------------------------------------
END OF BOOK CLASS
--------------------------------------------------------------------------
*/
class UserDAO {
public:
    vector<User> readUsersFromFile(const string& filename) {
        vector<User> allUsers;
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                vector<string> row;
                stringstream str(line);
                string word;
                while (getline(str, word, ',')) {
                    row.push_back(word);
                }
                createUser(row, allUsers);
            }
            file.close();
        }
        else {
            cout << "Could not open the file\n";
        }
        return allUsers;
    }
};

class BookDAO {
public:
    void readBooksFromFile(const string& libraryName, int& NoBooks, vector<books>& library, vector<Genre>& genres) {
        string copyname = libraryName + ".txt";
        ifstream inFile(copyname.c_str());
        if (inFile.is_open()) {
            string ignoreLine;
            getline(inFile, ignoreLine);
            string x, y, z, yearr;
            int m, l;
            while (inFile) {
                getline(inFile, x, ',');
                if (x[0] == '\n') {
                    x.erase(x.begin());
                }
                getline(inFile, y, ',');
                getline(inFile, z, ',');
                getline(inFile, yearr, ',');
                m = stoi(yearr, nullptr);
                inFile >> l;
                books temp(x, y, z, m, l);
                library.push_back(temp);
                NoBooks++;
                Genre tempgen(temp.getGenre());
                auto index = find(genres.begin(), genres.end(), tempgen);
                if (index == genres.end()) {
                    genres.push_back(tempgen);
                    genres[genres.size() - 1].increaseCount();
                }
                else {
                    (*index).increaseCount();
                }
            }
            inFile.close();
        }
        else {
            cout << "Could not open the file\n";
        }
    }
};

class Database {
private:
    UserDAO userDAO;
    BookDAO bookDAO;

public:
    vector<User> readUsers() {
        string filename = "users.csv";
        return userDAO.readUsersFromFile(filename);
    }

    void readBooks(string& Libraryname, int& NoBooks, vector<books>& library, vector<Genre>& genres) {
        bookDAO.readBooksFromFile(Libraryname, NoBooks, library, genres);
    }
};
/*
--------------------------------------------------------------------------
END OF DATABASE (Data Access Object)
--------------------------------------------------------------------------
*/

class Library
{
    string Libraryname;
    string Librarianname;
    int password;
    int NoBooks;
    int NoMembershipHolders;
    vector<books> library;
    vector<Genre> genres;

public:
    bool Login(string user, int num)
    { // Login function.
        if (!Librarianname.compare(user) && password == num)
            return true;
        else
            return false;
    }

    Library(string s, int n, string x)
    { // Library constructor
        Librarianname = s;
        password = n;
        Libraryname = x;
        NoBooks = 0;
        NoMembershipHolders = 0;
    }

    Library() {} // Default constructor

    Library(const Library &copy1)
    {
        Libraryname = copy1.Libraryname;
        Librarianname = copy1.Librarianname;
        password = copy1.password;
        NoBooks = copy1.NoBooks;
        NoMembershipHolders = copy1.NoMembershipHolders;
        library.assign(copy1.library.begin(),copy1.library.end());
        genres.assign(copy1.genres.begin(), copy1.genres.end());
    }//copy Constructor
    string getLibraryName() { return Libraryname; } // Getter function
    int getPassword() { return password; }
    string getLibrarianName() { return Librarianname; }   // Getter function
    vector<books> getLibraryData() { return library; }    // Getter function
    vector<Genre> getGenresofLibrary() { return genres; } // Getter function

    void sortLibraryByGenre()
    { // sorts the array by its genres alphabetically
        int sz = library.size();
        for (int i = 0; i < sz; i++)
        {
            for (int j = i + 1; j < sz; j++)
            {
                if (library[i].getGenre() > library[j].getGenre())
                    swap(library[i], library[j]);
                else if (library[i].getGenre() == library[j].getGenre() && library[i].getBookName() > library[j].getBookName())
                    swap(library[i], library[j]); // if 2 books have the same genre
                else if (library[i].getGenre() == library[j].getGenre() && library[i].getBookName() == library[j].getBookName() && library[i].getAuthor() > library[j].getAuthor())
                    swap(library[i], library[j]); // if 2 books have the same genre and name sort by author
            }
        }
    }
    void sortGenreArray()
    { // Sorts the genres array alphabetically
        int sz = genres.size();
        for (int i = 0; i < sz; i++)
        {
            for (int j = i + 1; j < sz; j++)
            {
                if (genres[i].getGenre() > genres[j].getGenre())
                    swap(genres[i], genres[j]);
            }
        }
    }

    void readDataBase()
    { // Function to initialize the library database
        Database obj;
        obj.readBooks(Libraryname,NoBooks,library,genres);
        NoBooks--;
        library.erase(library.end() - 1); // Avoid the repetetion in the last row.
        sortLibraryByGenre();             // sorts the library array
        sortGenreArray();
    }

    void printLibrary()
    {
        int sz = library.size();
        for (int i = 0; i < sz; i++)
        {
            library[i].print(); // calls the book printing function for each element
        }
    }

    void bookADD(books k)
    { // adds a book object to the library if it doesnt exist in the library
        library.push_back(k);
        Genre searchGen(k.getGenre());
        library[library.size() - 1].updateAvailable();
        auto index = find(genres.begin(), genres.end(), searchGen);
        if (index != genres.end())
        {
            (*index).increaseCount(k.getnoCopies());
        }
        else
        {
            genres.push_back(searchGen);
            genres[genres.size() - 1].increaseCount(k.getnoCopies());
        }
        sortLibraryByGenre();
    }

    void bookADD(books k, int index)
    { // adds a number of book copies to the index of the book if it is found in the library
        library[index].addCopies(k.getnoCopies());
        library[index].updateAvailable();
        Genre searchGen(k.getGenre());
        auto o = find(genres.begin(), genres.end(), searchGen);
        if (o != genres.end())
        {
            (*o).increaseCount(k.getnoCopies());
        }
        else
        {
            genres.push_back(searchGen);
            genres[genres.size() - 1].increaseCount(k.getnoCopies());
        }
    }

    void bookREMOVE(int index)
    { // removes a book from the library
        Genre searchGen(library[index].getGenre());
        auto o = find(genres.begin(), genres.end(), searchGen);
        if (o != genres.end())
        {
            (*o).decreaseCount(library[index].getnoCopies());
        }
        library.erase(library.begin() + index);
    }

    books getBookFromLibrary(int index)
    { // get a book from the library by knowing its index
        return library[index];
    }

    void removeCopies(int index, int num)
    {
        library[index] -= num; // take away a number of copies from a book knowing its index in the library.
        library[index].updateAvailable();
    }
    void addCopies(int index, int num)
    {
        library[index] += num; // Add a number of copies to a book knowing its index.
        library[index].updateAvailable();
    }
    bool getBookAvailability(int index)
    {
        return (library[index].checkAvailable()); // return the availability of a book knowing its index.
    }
};

/*
--------------------------------------------------------------------------
END OF LIBRARY CLASS
--------------------------------------------------------------------------
*/

class Book_Search
{ // class Book_Search implemented using design pattern singleton
private:
    static Book_Search *instance; // static pointer for singleton implementation
    Book_Search() {}              // Private constructor

public:
    static Book_Search *instancePtr;

    bool genreCheck(string genreName, vector<string> tags)
    { // Extract Method
        for (int k = 0; k < tags.size(); k++)
        {
            if (tags[k] == genreName)
            { // If genre name from library matches any of the genres the user has read check = true and automatically exits for loop
                return true;
            }
        }
        return false;
    }

    string searchByRecommendation(vector<books> bookNames, vector<Genre> Genres, vector<string> tags)
    {                                       // Searches based on tags which are the user's previous genres borrowed
        int j = 0;                          // j will be the variable used to access inside the vector
        vector<string> matchingBooks;       // This will be the vector of all the books that will be recommended to the user
        int numberOfGenres = Genres.size(); // Extract Variable
        for (int i = 0; i < numberOfGenres; i++)
        {                                             // We will loop over all genres in the library
            string genreName = Genres[i].getGenre();  // store genre name by getGenre function
            bool check = genreCheck(genreName, tags); // Extract Method

            if (check)
            {                                     // If the genre is read by user then check = true
                int add = Genres[i].getnoBooks(); //  Number of books in specific genre
                int kk = j;                       // store the current position in the vector
                for (j; j < kk + add; j++)
                { // Loop  over the entire number of books in the library specific to that tag
                    if (j >= bookNames.size())
                        break; // Incase the variable looping over the library exceeds its size it breaks
                    string bookName = bookNames[j].getBookName();
                    matchingBooks.push_back(bookName); // Add the name of the book to the vector
                }
            }
            else
                j += Genres[i].getnoBooks(); // If genre is not found then the number of books in that genre is irrelevant to us
        }
        if (matchingBooks.empty())
        { // If there are no books that could be recommended this then the vector is empty and no matching books were found.
            cout << "No matching books found." << endl;
            return "DNE";
        }
        else
        {
            cout << "Matching genres:" << endl;
            for (int i = 0; i < matchingBooks.size(); i++)
            { // Prints the possible recommended books
                cout << i + 1 << ". " << matchingBooks[i] << endl;
            }
            int bookIndex;
            cout << "\nEnter the index of the book you want to borrow: " << endl;
            cout << "Enter 0 to not borrow any book: ";
            cin >> bookIndex;
            if (bookIndex >= 1 && bookIndex <= matchingBooks.size()){
                return matchingBooks[bookIndex-1];
            }
            else return "DNE";
        }
    }
    // Extract Method combined

    string getGenre(vector<books> bookNames,string bookName){
        for (int i =0;i < bookNames.size();i++){
            if (bookNames[i].getBookName() == bookName){
                return bookNames[i].getGenre();
            }
        }
        return "";
    }


    void searchByGenre(vector<books> bookNames, vector<Genre> Genres)
    { // This searches by exact genre
        string searchQuery;
        cout << "Enter Genre Name: "; // User inputs Genre
        getline(cin, searchQuery);
        int j = 0;                    // j will be the variable used to access inside the vector
        vector<string> matchingBooks; // This will be the vector of all the books that will be recommended to the user
        for (int i = 0; i < Genres.size(); i++)
        { // Loop over all genres
            string genreName = Genres[i].getGenre();
            if (genreName.find(searchQuery) != string::npos)
            {                                     // If the two strings are similar this condition is met
                int add = Genres[i].getnoBooks(); //  Number of books in specific genre
                int kk = j;                       // store the current position in the vector
                for (j; j < kk + add; j++)
                { // Loop  over the entire number of books in the library specific to that tag
                    if (j >= bookNames.size())
                        break;
                    string bookName = bookNames[j].getBookName();
                    matchingBooks.push_back(bookName);
                }
            }
            else
                j += Genres[i].getnoBooks(); // If genre is not found then the number of books in that genre is irrelevant to us
        }
        if (matchingBooks.empty())
        {
            cout << "No matching books found." << endl;
            return;
        }
        else
        {
            cout << "Matching genres:" << endl; // Prints books in these genres
            for (int i = 0; i < matchingBooks.size(); i++)
            {
                cout << i + 1 << ". " << matchingBooks[i] << endl;
            }
        }
    }

    int searchByName(vector<books> bookNames, books query)
    { // This is linear search used by the program, therefore it is sure that the book exists
        for (int i = 0; i < bookNames.size(); i++)
        {
            if (bookNames[i] == query)
                return i;
        }
        return -1; // Returns -1 if the book is not found
    }
    int searchByName(vector<books> bookNames, string name)
    { // This is linear search used by the program, therefore it is sure that the name is always correct
        for (int i = 0; i < bookNames.size(); i++)
        {
            if (!bookNames[i].getBookName().compare(name))
                return i;
        }
        cout << "Book: " << name << " not found" << endl;
        return -1; // Returns -1 if the book is not found
    }



    int searchForGenre(vector<Genre> genresSearch, string name)
    { // This is linear such used by the program, therefore the genre exists
        for (int i = 0; i < genresSearch.size(); i++)
        {
            Genre query(name);
            if (genresSearch[i] == query)
                return i;
        }
        return -1; // Returns -1 if the genre is not found
    }

    int searchByName(vector<books> bookNames)
    { // User searches for book by its name
        string searchQuery;
        cout << "Enter Book Name: ";
        getline(cin, searchQuery); // User inputs book name.

        vector<string> matchingBooks;
        vector<int> indices;
        for (int i = 0; i < bookNames.size(); i++)
        { // Loops over all book names
            string bookName = bookNames[i].getBookName();
            if (bookName.find(searchQuery) != string::npos)
            { // If the book name from the library and the input are similar the book name is put inside the vector
                matchingBooks.push_back(bookName);
                indices.push_back(i);
            }
        }

        if (matchingBooks.empty())
        { // The vector is empty so there was no books found.
            cout << "No matching books found." << endl;
            return -1;
        }
        else
        { // If there is 1 or more books the user chooses which one.
            cout << "Matching books:" << endl;
            for (int i = 0; i < matchingBooks.size(); i++)
            {
                cout << i + 1 << ". " << matchingBooks[i] << endl;
            }
        }
        int choice;
        cout << "Enter a number to pick from the shown choices: ";
        cin >> choice;
        while (choice > matchingBooks.size())
        { // While loop if the user enters a wrong choice to make the user enter a correct choice.
            cout << "You have entered a number out of range please input another number: ";
            cin >> choice;
        }
        cout << "You have picked this book: " << matchingBooks[choice - 1] << endl; // Outputs picked book and returns it
        return indices[choice - 1];
    }
};

Book_Search *Book_Search::instance = new Book_Search(); // Static so has to be declared outside the class
Book_Search *Book_Search::instancePtr = Book_Search::instance;

/*
--------------------------------------------------------------------------
END OF LIBRARY SEARCH CLASS (Singleton)
--------------------------------------------------------------------------
*/