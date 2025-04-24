#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
int countBlanks(std::string str);
bool containsChar(std::string str, char c, int& index);

void wordleHelper(
    int position,
    std::string& in,
    std::string& floating,
    const std::set<std::string>& dict,
    std::set<std::string>& result);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> res;
    // Make sure it is not const
    std::string inCopy = in;
    std::string floatingCopy = floating;
    wordleHelper(0, inCopy, floatingCopy, dict, res);
    return res;
}

// Define any helper functions here
int countBlanks(std::string str){
  int count = 0;
  for(int i=0; i<str.size(); i++){ // Iterate through str
    if(str[i]=='-'){
      count++;
    }
  }
  return count;
}

bool containsChar(std::string str, char c, int& index){
  for(int i=0; i<str.size(); i++){ // Iterate through str
    if(str[i]==c){ // If the character exists in str
      index = i; // Update index
      return true;
    }
  }
  return false; // Character is not in str
}

void wordleHelper(int position, std::string& in,
  std::string& floating, const std::set<std::string>& dict,
  std::set<std::string>& result){
  // Count blanks
  int count = countBlanks(in);

  // ----BASE CASE----
  // Entire word is complete
  if(position==in.size()){
    // Check if word is in dictionary
    if(dict.find(in)!=dict.end()){
      result.insert(in);
    }
  }

  // CASE: there is already a letter placed
  else if(in[position]!='-'){
    wordleHelper(position+1, in, floating, dict, result);
  }

  // CASE: you can only use the floating letters
  else if(count==floating.size()){
    // Iterate through the floating letters
    for(int i=0; i<floating.size(); i++){
      // Make a copy of in and floating,
      // to make sure in and floating are preserved
      // for the next iteration
      std::string inCopy = in;
      std::string floatingCopy = floating;
      // Update inCopy
      inCopy[position]=floating[i];
      // Update floatingCopy
      floatingCopy.erase(i, 1);
      wordleHelper(position+1, inCopy, floatingCopy, dict, result);
    }
  }
  // CASE: you can use any letter
  else{
    // Iterate through every letter
    for(char c='a'; c<='z'; c++){
      // Make a copy of in and floating,
      // to make sure in and floating are preserved
      // for the next iteration
      std::string inCopy = in;
      std::string floatingCopy = floating;
      inCopy[position]=c;
      // Check if c is in floating
      int floatingIndex;
      bool hasC = containsChar(floatingCopy, c, floatingIndex);
      if(hasC){ // If c is in floating
        floatingCopy.erase(floatingIndex, 1);
      }
      wordleHelper(position+1, inCopy, floatingCopy, dict, result);
    }
  }
}