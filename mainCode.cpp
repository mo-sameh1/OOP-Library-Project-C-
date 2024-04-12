#include "Interface.cpp"

int main(){
    vector <Library> Libraries(NOLIBRARIES);
    for(int i = 0; i < NOLIBRARIES; i++){
        ifstream loginkey;
        int x;
        string l;
        if(i == 0){
                loginkey.open("Library1.txt");
                getline(loginkey, l, ',');
                loginkey>>x;
                Libraries[i] = Library(l,x, "Library1");//Store the 2 login parameters in library[0]
        }
        else if(i == 1){
                loginkey.open("Library2.txt");
                getline(loginkey, l, ',');
                loginkey>>x;
                Libraries[i] = Library(l,x, "Library2");//Store the 2 login parameters in library[1]
        }
        else if(i == 2){
                loginkey.open("Library3.txt");
                getline(loginkey, l, ',');
                loginkey>>x;
                Libraries[i] = Library(l,x, "Library3");//Store the 2 login parameters in library[2]
        }
    }
    interFace cmdInter(Libraries); //Initiates the interface
    cmdInter.welcomeScreen();
    while(cmdInter.mainMenu());//As long as the user inputs something between 1 and 7, the mainmenu will open.
 }