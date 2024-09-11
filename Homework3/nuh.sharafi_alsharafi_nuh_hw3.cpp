#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <sstream>
//#include "AvlTree.h"
//#include "AVLSearchTree.h"
#include "BST.h"
#include "Hash.h"
#include <chrono>

using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    DocumentItem()
        : documentName(), count(0) {}

    DocumentItem(const string& name, int n)
        : documentName(name), count(n) {}
};

struct WordItem {
    string word;
    vector<DocumentItem> docList;
    //TODO: might have to create 2 different versions of this

    WordItem()
        : word(), docList() {}

    WordItem(const string& theWord, vector<DocumentItem> docs)
        :word(theWord), docList(docs){}
};



//function to check if valid word (not number or punctuation)
 void makeValid(string& word)
 {
     /* Function would make the word lowercase and remove any punctuation and numbers */
     for (int i=0; i<word.length(); i++)
     {
         if (ispunct(word[i]) || isdigit(word[i]))
         {
             word.erase(i, 1);
         }
         else
         {
             word[i] = tolower(word[i]);
         }
     }
 }

 void insertionSort(vector<string>& filenames)
 {
     for(int i=1; i<filenames.size(); i++)
     {
         string temp = filenames[i];
         int j = i;
         while(j>0 && filenames[j-1] > temp)
         {
             filenames[j] = filenames[j-1];
             j--;
         }
         filenames[j] = temp;
     }
 }

 bool findFile(const vector<DocumentItem>& docs, string filename)
 {
     for(int i=0; i<docs.size(); i++)
     {
         if(docs[i].documentName == filename)
         {
             return true;
         }
     }
     return false;
 }

 int findFileIndex(const vector<DocumentItem>& docs, string filename)
 {
     for(int i=0; i<docs.size(); i++)
     {
         if(docs[i].documentName == filename)
         {
             return i;
         }
     }
     return -1;
 }

void processFile(ifstream& file, vector<string>& words)
{
    /* Function reads reads every word in a file delimited by anything that isn't alphabetic and adds
     * it to the vector of words */

     char c;
     string word;
    while(file.get(c))
    {
        if(isalpha(c))
        {
            word += c;
        }
        else
        {
            if(word.length() > 0)
            {
                makeValid(word);
                words.push_back(word);
                word.clear();
            }
        }
    }

     if(word.length() > 0)
     {
         makeValid(word);
         words.push_back(word);
        word.clear();
     }
}




int main()
{
    // BST<string, WordItem*> tree("NOTFOUND", NULL);
    //
    // BST<string, DocumentItem*> docs;
    // docs.insert("doc1", new DocumentItem("doc1", 1));
    // tree.insert("Istanbul", new WordItem("Istanbul", docs));
    // tree.insert("Izmir", new WordItem("Izmir", docs));
    //
    //
    // if(tree.find("Izmir") == NULL)
    // {
    //     cout << "Not found..." << endl;
    // }
    // else
    // {
    //     cout << tree.find("Izmir")->word << endl;
    // }
    // tree.remove("Izmir");

    // //repeat the same test for the hash table
    // WordItem* notFound = new WordItem("NOTFOUND", docs);
    // Hash<string, WordItem*> hashTable("NOTFOUND", notFound, 50);
    // hashTable.insert("Istanbul", new WordItem("Istanbul", docs));
    // hashTable.insert("Izmir", new WordItem("Izmir", docs));
    //
    // if(hashTable.find("Izmir") == NULL)
    // {
    //     cout << "Not found..." << endl;
    // }
    // else
    // {
    //     cout << hashTable.find("Izmir")->word << endl;
    // }

     //Start of real program
     //get the first round of inputs
     int inputFiles = 0;

     cout << "Enter the number of input files: ";
     cin >> inputFiles;
     vector<string> filenames;
     BST<string, WordItem*> tree("NOTFOUND", NULL);
     Hash<string, WordItem*> hashTable("NOTFOUND", NULL, 50);

     for(int i=1; i<=inputFiles;i++)
     {
         string filename;
         cout << "Enter " << i << ". file name: ";
         cin >> filename;

         //open file and process into a vector of words
         ifstream file;
         //file.open(filename.c_str());

         //check if the file is opened successfully
         while(file.fail())
         {
             cout << "File not found. Try again." << endl;
             cout << "Enter " << i << ". file name: ";
             cin >> filename;
             file.open(filename.c_str());
         }

         //add file to vector
         filenames.push_back(filename);
     }


     for (int i=1; i<=inputFiles; i++)
     {
         // string filename;
         // cout << "Enter " << i << ". file name: ";
         // cin >> filename;
         //
         // //open file and process into a vector of words
         // ifstream file;
         // file.open(filename.c_str());
         //
         // //check if the file is opened successfully
         // while(file.fail())
         // {
         //     cout << "File not found. Try again." << endl;
         //     cout << "Enter " << i << ". file name: ";
         //     cin >> filename;
         //     file.open(filename.c_str());
         // }
         //
         // //add file to vector
         // filenames.push_back(filename);
         string filename = filenames[i-1];
         ifstream file;
         file.open(filename.c_str());

         //process file into a list of words delimited by anything that isn't alphabetic
         vector<string> words;
         processFile(file, words);

         for (int i=0;i<words.size();i++)
         {
             //cout << words[i] << endl;
             //check if word is in tree
             WordItem* itemBST = tree.find(words[i]);
             WordItem* itemHT = hashTable.find(words[i]);
             if(itemBST == NULL)
             {
                 //item not found, create new item and add to tree
                 vector<DocumentItem> docs;
                 docs.push_back(DocumentItem(filename, 1));
                 tree.insert(words[i], new WordItem(words[i], docs));
             }
             else
             {
                 //item found, update the item
                 //check if the document is already in the list
                 bool found = false;
                 for(int j=0; j<itemBST->docList.size(); j++)
                 {
                     if(itemBST->docList[j].documentName == filename)
                     {
                         itemBST->docList[j].count++;
                         found = true;
                         j = itemBST->docList.size();
                     }
                 }

                 //if not found, add the document to the list
                 if(!found)
                 {
                     itemBST->docList.push_back(DocumentItem(filename, 1));
                 }
             }

             //do the same for the hash table
             if(itemHT == NULL)
             {
                 //item not found, create new item and add to tree
                 vector<DocumentItem> docs;
                 docs.push_back(DocumentItem(filename, 1));
                 hashTable.insert(words[i], new WordItem(words[i], docs));
             }
             else
             {
                 //item found, update the item
                 //check if the document is already in the list
                 bool found = false;
                 for(int j=0; j<itemHT->docList.size(); j++)
                 {
                     if(itemHT->docList[j].documentName == filename)
                     {
                         itemHT->docList[j].count++;
                         found = true;
                         j = itemHT->docList.size();
                     }
                 }

                 //if not found, add the document to the list
                 if(!found)
                 {
                     itemHT->docList.push_back(DocumentItem(filename, 1));
                 }
             }
         }
     }
     //I think it is safe to assume both trees are compiled now
     hashTable.tableProperties();
    //sort the filenames
     insertionSort(filenames);


     //get second round of inputs
     string query;
     cout << "Enter queried words in one line: ";
     cin.ignore();
     getline(cin, query);


    //convert non-alphabetic characters to spaces
     for(int j=0; j<query.length(); j++)
     {
         if(!isalpha(query[j]))
         {
             query[j] = ' ';
         }
     }


     //cout << "query: " << query << endl;


     vector<string> words;
     for(int j = 0; j < query.length(); j++)
     {
         string word;
         while(j < query.length() && query[j] != ' ')
         {
             //cout << j;
             //cout << query[j] << endl;
             word += query[j];
             j++;
         }
         makeValid(word);
         words.push_back(word);
     }






    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments(with BST);
    }

     //qeury all the words
     vector<WordItem*> results;
     for(int j=0; j<words.size(); j++)
     {
         WordItem* item = tree.find(words[j]);
         if(item != NULL)
         {
             results.push_back(item);
         }
     }

     //check that all the queries are valid
     if(results.size() != words.size())
     {
         cout << "No document contains the given query." << endl;
     }
     else
     {
         //for every document, check if all the words are in the document
         for(int j=0; j<filenames.size();j++)
         {
             for(int k=0; k<results.size(); k++)
             {
                 if(!findFile(results[k]->docList, filenames[j]))
                 {
                     k = results.size();
                 }
                 if(k == results.size()-1)
                 {
                     cout << "In Document " << filenames[j] << ", ";
                     for (int l=0; l<results.size(); l++)
                     {
                         cout << results[l]->word << " found " << results[l]->docList[findFileIndex(results[l]->docList, filenames[j])].count << " times, ";
                     }
                     cout << endl;
                 }
             }
         }
     }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() -
    start);
    //cout << "\nTime: " << BSTTime.count() / k << "\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        // QueryDocuments (with hashtable);
    }

     //qeury all the words
     vector<WordItem*> results2;
     for(int j=0; j<words.size(); j++)
     {
         WordItem* item = hashTable.find(words[j]);
         if(item != NULL)
         {
             results2.push_back(item);
         }
     }

     //check that all the queries are valid
     if(results2.size() != words.size())
     {
         cout << "No document contains the given query." << endl;
     }
     else
     {
         //for every document, check if all the words are in the document
         for(int j=0; j<filenames.size();j++)
         {
             for(int k=0; k<results2.size(); k++)
             {
                 if(!findFile(results2[k]->docList, filenames[j]))
                 {
                     k = results2.size();
                 }
                 if(k == results2.size()-1)
                 {
                     cout << "In Document " << filenames[j] << ", ";
                     for (int l=0; l<results2.size(); l++)
                     {
                         cout << results2[l]->word << " found " << results2[l]->docList[findFileIndex(results2[l]->docList, filenames[j])].count << " times, ";
                     }
                     cout << endl;
                 }
             }
         }
     }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
     cout << "\nTime: " << BSTTime.count() / k << "";
     cout << "\nTime: " << HTTime.count() / k << "\n";
     cout << "Speed up: " << (double)BSTTime.count() / HTTime.count() << endl;

    return 0;
}
