#ifndef Q5SOLUTION_HPP_
#define Q5SOLUTION_HPP_


#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


////////////////////////////////////////////////////////////////////////////////

//Problem 2

template<class T>
class NTN {
public:
    NTN ()
    : value(), children()
    {}
    
    NTN (const NTN<T>& n)
    : value(n.value), children(n.children.ibegin(),n.children.iend())
    {}
    
    NTN (T value, ics::ArraySet<NTN> values)
    : value(value), children(values.ibegin(),values.iend())
    {}
    
    //Needed to compile ArraySet<NTN>
    bool operator == (const NTN<T>& rhs) const {
        if (this == &rhs)
            return true;
        else
            return false;
    }
    
    //Needed to compile ArraySet<NTN>
    friend std::ostream& operator << (std::ostream& outs, const NTN<T>& s) {
        outs << "unprintable";
        return outs;
    }
    
    T                  value;
    ics::ArraySet<NTN> children;
};


//Helper
std::string int_to_str(int i) {
    std::ostringstream answer;
    answer << i;
    return answer.str();
}


//Recursive method to prompt user for node values to build NTN tree
NTN<int> build_NTN_tree(std::string indentation, std::string where) {
    NTN<int> n;
    n.value = ics::prompt_int(indentation + "Enter value for " + where);
    int childs = ics::prompt_int(indentation + "Enter # of children for " + where + "(0 for leaf)");
    for (int i=1; i<=childs; ++i)
        n.children.insert( build_NTN_tree(indentation+"  ", "child " + int_to_str(i) + " of " + int_to_str(n.value)));
    return n;
}

//Recursive method to build NTN tree
//The array has integers (in order) that the user enters in the above build_NTN_tree
NTN<int> build_NTN_tree(int values[], int& at) {
    NTN<int> n;
    n.value = values[at++];
    int childs = values[at++];
    for (int i=1; i<=childs; ++i)
        n.children.insert( build_NTN_tree(values,at) );
    return n;
}


//Print ntn, its n children indenter, their n children indented....
void print_NTN_tree(const NTN<int>& ntn, std::string indent) {
    std::cout << indent << ntn.value  << std:: endl;
    for (NTN<int> n : ntn.children)
        print_NTN_tree(n, indent+"  ");
}



//Write this function
template<class T>
T maximum(const NTN<T>& ntn) {
    if (ntn.children.size() == 0) {
        return ntn.value;
    } else {
        T max = ntn.value;
        for (NTN<T> &node : ntn.children) {
            T tmp = maximum(node);
            if (tmp > max) {
                max = tmp;
            }
        }
        return max;
    }
}



////////////////////////////////////////////////////////////////////////////////

//Problem 3

class DTN {
public:
    DTN () :
    is_word(false), word_to_here(""), children()
    {}
    DTN (bool iw, std::string wth) :
    is_word(iw), word_to_here(wth), children()
    {}
    
    //Needed to compile ArraySet<DTN>
    bool operator != (const DTN& rhs) const {
        if (this != &rhs)
            return true;
        else
            return false;
    }
    
    //Needed to compile ArraySet<DTN>
    bool operator == (const DTN& rhs) const {
        if (this == &rhs)
            return true;
        else
            return false;
    }
    
    //Needed to compile ArraySet<DTN> but not used
    friend std::ostream& operator << (std::ostream& outs, const DTN& s) {
        outs << "unprintable";
        return outs;
    }
    
    bool                     is_word;
    std::string              word_to_here;
    ics::ArrayMap<char,DTN>  children;
};


//Add a word correctly into a Digital Tree (of strings)
void add_a_word (DTN& dtn, std::string prefix, std::string postfix) {
    if (postfix.size() == 0) {
        dtn.is_word = true;
        return;
    } else {
        char first = postfix[0];
        if (!dtn.children.has_key(first))
            dtn.children[first] = DTN(false,prefix+first);
        return add_a_word(dtn.children[first],prefix+first,postfix.substr(1));
    }
}


//Print dtn, its n children indenter, their n children indented....
void print_DTN_tree(const DTN& dtn, std::string indent) {
    std::cout << indent << dtn.word_to_here  << (dtn.is_word? "*" : "") << std:: endl;
    for (auto n : dtn.children)
        print_DTN_tree(n.second, indent+"  ");
}


bool is_a_word (const DTN& dtn, std::string remaining_letters) {
    if (remaining_letters.empty())
        return dtn.is_word;          //all letters in tree; is it a word?
    else if (dtn.children.has_key(remaining_letters[0]) == false)
        return false;                 //some letters not in truee: it isn't a word
    else
        return is_a_word(dtn.children[remaining_letters[0]], //check the next letter
                         remaining_letters.substr(1));
}


void add_a_word (DTN& dtn, std::string word) {
    add_a_word(dtn,"",word);
}



//Write this function
std::string longest_word (const DTN& dtn) {
    std::string longest;
    if (dtn.children.size() == 0 && dtn.is_word) {
        longest = dtn.word_to_here;
    } else {
        for (auto &kv : dtn.children) {
            std::string tmp = longest_word(kv.second);
            if (tmp.length() > longest.length()) {
                longest = tmp;
            }
        }
    }
    return longest;
}


#endif /* Q5SOLUTION_HPP_ */
