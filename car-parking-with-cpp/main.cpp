#include <iostream>
#include <fstream>
#include<conio.h>

using namespace std;

int car[50] = {0}, bike[100] = {0}, carNo = 0, bikeNo = 0;
void sync();
void cache();
void addVehicle();
void removeVehicle();
void check();

int main()
{
    int choice, flag = 1;
    cache();
    while (flag)
    {
        system("cls");
        cout << "|-----------------------------------------------------------------------------------------|" << endl;
        cout << "|                                  C A R   P A R K I N G                                  |" << endl;
        cout << "|-----------------------------------------------------------------------------------------|" << endl;
        cout << "|                                 1. Park Vehicle                                         |" << endl;
        cout << "|                                 2. Checkout                                             |" << endl;
        cout << "|                                 3. Check available lots                                 |" << endl;
        cout << "|                                 Anything to exit...                                     |" << endl;
        cout << "|-----------------------------------------------------------------------------------------|" << endl;
        cout << "royParkingSystem$ ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            system("cls");
            addVehicle();
            break;
        case 2:
            system("cls");
            removeVehicle();
            break;
        case 3:
            system("cls");
            check();
            break;
        default:
            flag = 0;
            break;
        }
    }
    return 0;
}

void sync()
{
    fstream DATA;
    DATA.open("data.db", ios::out);
    for (int i = 0; i < 50; i++)
    {
        DATA << car[i];
    }
    for (int i = 0; i < 100; i++)
    {
        DATA << bike[i];
    }
    DATA.close();
}

void cache()
{
    fstream DATA;
    DATA.open("data.db", ios::in);
    if (!DATA)
    {
        DATA.open("data.db", ios::out);
        for (int i = 0; i < 50; i++)
        {
            DATA << 0;
        }
        for (int i = 0; i < 100; i++)
        {
            DATA << 0;
        }
    }
    else
    {
        char x;
        for (int i = 0; i < 50; i++)
        {
            DATA >> x;
            car[i] = ((int)x) - 48;
            if (car[i] == 1)
                carNo++;
        }
        for (int i = 0; i < 100; i++)
        {
            DATA >> x;
            bike[i] = ((int)x) - 48;
            if (bike[i] == 1)
                bikeNo++;
        }
    }
    DATA.close();
}

void addVehicle()
{
    char choice;
    cout << "|-----------------------------------------------------------------------------------------|" << endl;
    cout << "|                                 1. Park a car                                           |" << endl;
    cout << "|                                 2. Part a bike                                          |" << endl;
    cout << "|                                 Anything to exit...                                     |" << endl;
    cout << "|-----------------------------------------------------------------------------------------|" << endl;
    cout << "royParkingSystem$ ";
    cin >> choice;
    system("cls");
    switch (choice)
    {
    case '1':
        if ((50 - carNo) > 0)
        {
            for (int i = 0; i < 50; i++)
            {
                if (car[i] == 0)
                {
                    car[i] = 1;
                    carNo++;
                    cout << "Lot number " << i << " is alloted for your parking\nPay 200RS while checking out\n";
                    break;
                }
            }
            sync();
        }
        else
        {
            cout << "No car parking spaces is left :(\n";
        }
        break;
    case '2':
        if ((100 - bikeNo) > 0)
        {
            for (int i = 0; i < 100; i++)
            {
                if (bike[i] == 0)
                {
                    bike[i] = 1;
                    bikeNo++;
                    cout << "Lot number " << i << " is alloted for your parking\nPay 50RS while checking out\n";
                    break;
                }
            }
            sync();
        }
        else
        {
            cout << "No bike parking spaces is left :(\n";
        }
        break;

    default:
        return;
    }
    cout << "Press any key... $ ";
    getch();
}

void removeVehicle()
{
    char choice;
    int lot;
    cout << "|-----------------------------------------------------------------------------------------|" << endl;
    cout << "|                                 1. Takeout a car                                        |" << endl;
    cout << "|                                 2. Takeout a bike                                       |" << endl;
    cout << "|                                 Anything to exit...                                     |" << endl;
    cout << "|-----------------------------------------------------------------------------------------|" << endl;
    cout << "royParkingSystem$ ";
    cin >> choice;
    system("cls");
    switch (choice)
    {
    case '1':
        cout << "Enter your lot number: ";
        cin >> lot;
        if (lot < 0 || lot >= 50)
        {
            cout << "Invalid lot number\n";
            goto end;
        }
        else if (car[lot] == 0)
        {
            cout << "Wrong lot number\n";
            goto end;
        }
        car[lot] = 0;
        carNo--;
        cout << "Successfully removed\n";
        sync();
        break;
    case '2':
        cout << "Enter your lot number: ";
        cin >> lot;
        if (lot < 0 || lot >= 100)
        {
            cout << "Invalid lot number\n";
            goto end;
        }
        else if (bike[lot] == 0)
        {
            cout << "Wrong lot number\n";
            goto end;
        }
        bike[lot] = 0;
        bikeNo--;
        cout << "Successfully removed\n";
        sync();
        break;
    default:
        return;
    }
end:
    cout << "Press any key... $ ";
    getch();
}

void check()
{
    cout << "Number of free car parking lots: " << (50 - carNo) << endl;
    cout << "Number of free bike parking lots: " << (100 - bikeNo) << endl;
    cout << "\nPress any key... $";
    getch();
}
