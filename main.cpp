#include "eauth/eauth.cpp"
#include <algorithm>
#include <iostream>
#include <string>
void menu()
{
    system("cls");
    eauth auth;
    auth.init();
    std::string str(auth.ApplicationName);
    SetConsoleTitle(str.c_str());
    int s;
    std::cout << "1/ Login - 2/ Register" << "\n";
    std::cout << "-> ";
    std::cin >> s;
    if (s == 1)
    {
        system("cls");
        std::string username;
        std::string password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;
        if (auth.signin(username, password))
        {
            system("cls");
            std::cout << auth.LoggedMessage << "\n";
            std::cout << "Username: " << auth.NAME;
            std::cout << "Rank: " << auth.RANK;
            std::cout << "Create Date: " << auth.CREATEDATE;
            std::cout << "Expire Date: " << auth.EXPIREDATE;
            std::cout << "Hwid: " << auth.HWID();
            //std::cout << "Variable: " + auth.grabVariable("your variable id here") << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3500));
            menu();
        }
        else
        {
            menu();
        }
    }
    else if (s == 2)
    {
        system("cls");
        std::string username;
        std::string password;
        std::string invite;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;
        std::cout << "License Key: ";
        std::cin >> invite;
        if (auth.signup(username, password, invite))
        {
            system("cls");
            std::cout << auth.RegisteredMessage << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(3500));
            menu();
        }
        else
        {
            menu();
        }
    }
    else
    {
        system("cls");
        std::cout << "Wrong input!" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3500));
        menu();
    }
}
int main()
{
    menu();
	return EXIT_SUCCESS;
}