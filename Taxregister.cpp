#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

// The task is to implement class CTaxRegister, which implements a simple tax
// revenue database. The database is very simplified. We assume the database holds
// information about all citizens from the moment they born until they die. Each
// citizen is identified by his/her account. The accounts are strings and are
// unique in the database. Second, citizens may be positively identified by their
// names and addresses. We assume the pair (name, address) is unique in the
// database. That is, there may exist many citizens with the same name
// (e.g. "John Smith"), there may be many citizens registered with the same address
// (e.g. "Main Street 10"), however, there may be at most one "John Smith"
// registered on "Main Street 10".

//----------------------------------------------------------------------------------------------------------------------


// Class for person.  A person has a name, address, account, income and expense.
// Our database, or CTaxRegister if you will, will store Person objects.  And allow
// adding, updating fields, removing objects.

class Person{

public:

  // Getters for the public fields of a Person.

    string getName() const{
        return name;
    }

    string getAddress() const{
        return addr;
    }

    string getAccount() const{
        return acct;
    }

    int getIncome() const{
        return income;
    }

    int getExpense() const{
        return expense;
    }

// Constructor used for creating an object of type Person.  Takes as parameters
// three strings ( name, address, account)

    Person( string n , string a , string ac ){
        name = n;
        addr = a;
        acct = ac;
        income = 0;
        expense = 0;
    }

// Constructor used for creating and object of type Person that takes parameters
// two strings ( name , address)

    Person( string n , string a ){
        name = n;
        addr = a;
        income = 0;
        expense = 0;
    }

// Constructor that takes only one string ( account ) as a paramter.

    Person(string ac){
        acct = ac;
    }

// Setters used for adding/adding to fields of a Person.

    void AddAccount( string x ){
        acct = x;
    }

    void AddIncome( int i ){
        income += i;
    }

    void AddExpense( int e ){
        expense += e;
    }

    void AddName( string n ){
        name = n;
    }

    void AddAddr( string adr ){
        addr = adr;
    }

private:

    string name;
    string addr;
    string acct;
    int income;
    int expense;

};

//----------------------------------------------------------------------------------------------------------------------

// Iterator class that returns a boolean "true" if end of register was reached and
// returns false if end was not reached. Class can iterate or advance through the record
// and retrieve fields(name, address, account) of an object (Person) at current iterator position.

class CIterator{

public:

// Constructor creates and instance and copies passed container and size.

    CIterator( vector < Person > g ){
        p = g;
        size = p.size();
    }

// Determines if at end of regitster by checking index against size.

    bool AtEnd ( void ) const{
        if( counter == size ){
          return true;
        }
        else{
          return false;
        }
    }

// Accesses next index by incrementing counter.

    void Next( void ){
        counter++;
    }

// Getters of fields at current index.

    string Name( void ) const{
        return p.at( counter ).getName();
    }

    string Addr( void ) const{
        return p.at( counter ).getAddress();
    }

    string Account( void ) const{
        return p.at( counter ).getAccount();
    }

private:

    int counter = 0;
    int size;
    vector < Person > p;

};

//----------------------------------------------------------------------------------------------------------------------

// This is where most of the magic happens.  It is the tax revenue database.


class CTaxRegister{

public:

// Two important public fields are these vecotrs which store Persons.  One storing by account,
// the other storing by name and address (alphabetical order).

    vector < Person > PersonDB;
    vector < Person > PersonDBname;

// Comparator functions for binary insertion sort that allows for O(nlogn) complexity in the worst case.
// The first allowing for insertion of Person into by account, the second by name and address.
// The comparators take as parameters two person objects and return the "smaller" one.  Will be unsigned
// with std::lower_bound.

    static bool compPerson( const Person & b , const Person & c ){
        return b.getAccount() < c.getAccount();
    }

    static bool compName( const Person & b , const Person & c ){
        if( b.getName() != c.getName() ){
            return b.getName() < c.getName();
        }
        else{
            return b.getAddress() < c.getAddress();
        }
    }

// Function that allows the printing of the database.  This is not required although it is useful
// for testing/debugging.

    void PrintDB(){
        cout << "Database by account: ";

        for ( auto &i : PersonDB ){
            cout << "NAME = " << i.getName() << " | ADDY = " << i.getAddress() <<
                 " | ID = " << i.getAccount() << " | Income = " << i.getIncome() <<
                 " | Expense = " << i.getExpense() << endl;
        }

        cout << "==================" << endl;
        cout << "Database by name: ";

        for (auto &i : PersonDBname){
            cout << "NAME = " << i.getName() << " | ADDY = " << i.getAddress() <<
                 " | ID = " << i.getAccount() << " | Income = " << i.getIncome() <<
                 " | Expense = " << i.getExpense() << endl;
        }
    }

// Birth will add/insert a new object into the database using a binary insertion sort.
// The method takes as parameters name address and account - which are fields of a Person.

    bool Birth ( const string & name , const string & addr , const string & account ){

// New instance of a person.

        Person b( name , addr , account );

// The lower_bound function uses binary search and comparator function to find the lowest possible index to insert
// a new Person object.  The comparator is neccesary to show the computer how to compare type Person. Because there are
// two identical databses that are accessed in different ways (one by account, the other by name and account) there address
// two lower_bound functions.

            auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

            auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

// Important checks to determine if a record already exists in the database.  If identical record is found
// the iterator will not reach the end and will return false.  Else, it will insert at correct position.

            if( ( lo != PersonDB.end() && lo -> getAccount() == b.getAccount() ) ||
                ( low != PersonDBname.end() && low -> getName() == b.getName() &&
                  low -> getAddress() == b.getAddress() ) ){
                    return false;
                  }
            else{
                PersonDB.insert( lo , b );
                PersonDBname.insert( low , b );
                return true;
            }
        }

// Method that will remove a record from database by metaphorically "killing" a person.
// Very much like a hitman, the method Death needs the name and address of its target.

    bool Death ( const string & name , const string & addr ){

// Instance of a person used by the binary insertion.  Because the two vectors PersonDB and PersonDBname
// should be identical, anything we erase from one should be erased from the other.

        Person b( name , addr );

            auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

            if( ( low != PersonDBname.end() && low -> getName() == b.getName() &&
            low->getAddress() == b.getAddress() ) ){
                b.AddAccount( low -> getAccount() );
                PersonDBname.erase( low );

                auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

                if( lo != PersonDB.end() && lo -> getAccount() == b.getAccount() ){
                  PersonDB.erase( lo );
                }
                return true;
            }
            else{
                return false;
            }
    }

// Updates the income of a record in the database.  The method takes the account and the ammount.
// Once again we're using lower_bound to make sure we're accessing correct index.

    bool Income( const string & account , int amount ){
        Person b( account );

            auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

            if( lo != PersonDB.end() && lo -> getAccount() == b.getAccount() ){

// Update the income by the given ammount.

                lo -> AddIncome( amount );
                b.AddName( lo -> getName() );
                b.AddAddr( lo -> getAddress() );

                auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b, compName );

                if( ( low != PersonDBname.end() && low -> getName() == b.getName()
                && low -> getAddress() == b.getAddress() ) ){
                    low -> AddIncome( amount );
                }
                return true;
            }
            else{
                return false;
            }
    }
// This form of the income method will be doing the same thing as the previous however it takes name and address instead
// of account.
    bool Income( const string & name , const string & addr , int amount ){
        Person b( name , addr );

            auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

            if( ( low != PersonDBname.end() && low -> getName() == b.getName()
            && low -> getAddress() == b.getAddress() ) ){
                b.AddAccount(low -> getAccount() );
                low->AddIncome( amount );

                auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

                if( lo != PersonDB.end() && lo -> getAccount() == b.getAccount()  ){
                    lo -> AddIncome( amount );
                }
                return true;
            }
            else{
              return false;
            }
    }

// Operates the same as the income method, however, the expense field is updated. Once again the function
// takes account and amount.

    bool Expense( const string & account , int amount ){
        Person b( account );

            auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

            if( lo != PersonDB.end() && lo -> getAccount() == b.getAccount() ){
                lo -> AddExpense( amount );
                b.AddName( lo -> getName() );
                b.AddAddr( lo -> getAddress() );

                auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

                if( ( low != PersonDBname.end() && low -> getName() == b.getName()
                && low -> getAddress() == b.getAddress() ) ){
                    low -> AddExpense( amount );
                    return true;
                }
                else{
                  return false;
                }
            }
            else{
              return false;
            }
    }

// Same as the previous method except this version takes address, name and amount.

    bool Expense( const string & name , const string & addr , int amount ){
        Person b( name , addr );

            auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

            if( ( low != PersonDBname.end() && low -> getName() == b.getName() &&
            low -> getAddress() == b.getAddress() ) ){
                b.AddAccount( low -> getAccount() );
                low -> AddExpense( amount );

                auto lo = lower_bound( PersonDB.begin() , PersonDB.end() , b , compPerson );

                if( lo != PersonDB.end() && lo -> getAccount() == b.getAccount() ){
                    lo -> AddExpense( amount );
                    return true;
                }
                else{
                  return false;
                }
            }
            else{
              return false;
            }
    }

// This method takes name, address, account, sumIncome, and sumExpense.  It goes through the database and
// and locates a record by name and adress.  It will ouput the account, sumIncome, and sumExpense.

    bool Audit( const string & name , const string & addr , string & account ,
      int & sumIncome , int & sumExpense ) const{
        Person b( name , addr );

            auto low = lower_bound( PersonDBname.begin() , PersonDBname.end() , b , compName );

            if( ( low != PersonDBname.end() && low -> getName() == b.getName() &&
            low -> getAddress() == b.getAddress() ) ){

// Sets the output parameters to the found record's fields

                b.AddAccount( low -> getAccount());
                account = b.getAccount();
                sumIncome = low -> getIncome();
                sumExpense = low -> getExpense();
                return true;
            }
            else{
              return false;
            }
    }

// CIterator creates and returns a CIterator instance of the current database.

    CIterator ListByName ( void ) const{
        CIterator it( PersonDBname );
        return it;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    string acct;
    int    sumIncome, sumExpense;
    CTaxRegister b1;
    assert ( b1 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b1 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( b1 . Birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
    assert ( b1 . Birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );
    assert ( b1 . Income ( "Xuj5#94", 1000 ) );
    assert ( b1 . Income ( "634oddT", 2000 ) );
    assert ( b1 . Income ( "123/456/789", 3000 ) );
    assert ( b1 . Income ( "634oddT", 4000 ) );
    assert ( b1 . Income ( "Peter Hacker", "Main Street 17", 2000 ) );
    assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 ) );
    assert ( b1 . Expense ( "John Smith", "Main Street 17", 500 ) );
    assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 1000 ) );
    assert ( b1 . Expense ( "Xuj5#94", 1300 ) );
    assert ( b1 . Audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "123/456/789" );
    assert ( sumIncome == 3000 );
    assert ( sumExpense == 0 );
    assert ( b1 . Audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 1000 );
    assert ( sumExpense == 4300 );
    assert ( b1 . Audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "634oddT" );
    assert ( sumIncome == 8000 );
    assert ( sumExpense == 0 );
    assert ( b1 . Audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Z343rwZ" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 500 );
    CIterator it = b1 . ListByName ();
    assert ( ! it . AtEnd ()
             && it . Name () == "Jane Hacker"
             && it . Addr () == "Main Street 17"
             && it . Account () == "Xuj5#94" );
    it . Next ();
    assert ( ! it . AtEnd ()
             && it . Name () == "John Smith"
             && it . Addr () == "Main Street 17"
             && it . Account () == "Z343rwZ" );
    it . Next ();
    assert ( ! it . AtEnd ()
             && it . Name () == "John Smith"
             && it . Addr () == "Oak Road 23"
             && it . Account () == "123/456/789" );
    it . Next ();
    assert ( ! it . AtEnd ()
             && it . Name () == "Peter Hacker"
             && it . Addr () == "Main Street 17"
             && it . Account () == "634oddT" );
    it . Next ();
    assert ( it . AtEnd () );

    assert ( b1 . Death ( "John Smith", "Main Street 17" ) );

    CTaxRegister b2;
    assert ( b2 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b2 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( !b2 . Income ( "634oddT", 4000 ) );
    assert ( !b2 . Expense ( "John Smith", "Main Street 18", 500 ) );
    assert ( !b2 . Audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
    assert ( !b2 . Death ( "Peter Nowak", "5-th Avenue" ) );
    assert ( !b2 . Birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
    assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2 . Death ( "Jane Hacker", "Main Street 17" ) );
    assert ( b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2 . Audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 0 );
    assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

    return 0;
}
#endif /* __PROGTEST__ */
