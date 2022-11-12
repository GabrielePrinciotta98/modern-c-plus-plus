#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <iostream>

namespace contact_list {

// TODO create implementation here!
bool add(storage& contacts, std::string_view name, number_t number)
{
    bool existingName{std::find(contacts.names.begin(), contacts.names.end(), name) != contacts.names.end()};
    if (existingName || name == "")
        return false;
        
    contacts.names.push_back(static_cast<std::string>(name));
    contacts.numbers.push_back(number);

    return true;
}

size_t size(const storage& contacts)
{
    return contacts.names.size();
}

number_t get_number_by_name(storage& contacts, std::string_view name)
{
    auto found = std::find(contacts.names.begin(), contacts.names.end(), name);
    if (found != contacts.names.end())
        return contacts.numbers.at(found - contacts.names.begin());

    return -1;    
}

std::string get_name_by_number(storage& contacts, number_t number)
{
    auto found = std::find(contacts.numbers.begin(), contacts.numbers.end(), number);
    if (found != contacts.numbers.end())
        return contacts.names.at(found - contacts.numbers.begin());

    return "";
}


std::string to_string(const storage& contacts)
{
    std::stringstream ss{};
    for (unsigned int i{0}; i<contacts.names.size(); ++i)
    {
         ss << contacts.names[i] << " - " << contacts.numbers[i] << '\n';
    }

    std::string result{ss.str()};
    return result;
}

bool remove(storage& contacts, std::string_view name)
{
    auto foundName = std::find(contacts.names.begin(), contacts.names.end(), name);
    auto numberToFind = get_number_by_name(contacts, name);
    auto foundNumber = std::find(contacts.numbers.begin(), contacts.numbers.end(), numberToFind);
    
    if (foundName != contacts.names.end() && foundNumber != contacts.numbers.end())
    {
        contacts.names.erase(foundName);
        contacts.numbers.erase(foundNumber);
        return true;
    }

    return false;

}

void sort(storage& contacts)
{
    std::vector<std::string> vecA(contacts.names);
    std::vector<number_t> sortedNumbers(contacts.numbers.size());

    std::sort(contacts.names.begin(), contacts.names.end());
    std::vector<int> pos;

    for (unsigned int i{0}; i<contacts.names.size(); ++i)
    {
        auto foundName = std::find(contacts.names.begin(), contacts.names.end(), vecA[i]);
        pos.push_back(foundName - contacts.names.begin()); 
    }

    for (unsigned int i{0}; i<contacts.names.size(); ++i)
    {
        sortedNumbers[pos[i]] = contacts.numbers[i];
    }

    contacts.numbers.clear();
    contacts.numbers = sortedNumbers;
    /*
    for (int i{0}; i<contacts.names.size(); ++i)
    {
         std::cout << contacts.names[i] << '\n';
    }

    for (int i{0}; i<contacts.names.size(); ++i)
    {
         std::cout << vecA[i] << '\n';
    }
    */



}

}