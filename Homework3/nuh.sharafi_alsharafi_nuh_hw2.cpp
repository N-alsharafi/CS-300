#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <sstream>
//#include "AvlTree.h"
#include "AVLSearchTree.h"

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




int main()
{
    // AVLSearchTree<string, WordItem*> tree("NOTFOUND", NULL);
    //
    // vector<DocumentItem> docs;
    // docs.push_back(DocumentItem("doc1", 1));
    // tree.insert("Istanbul", new WordItem("Istanbul", docs));
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




    //get the first round of inputs
     int inputFiles = 0;

     cout << "Enter the number of input files: ";
     cin >> inputFiles;
     vector<string> filenames;
    AVLSearchTree<string, WordItem*> tree("NOTFOUND", NULL);


     for (int i=1; i<=inputFiles; i++)
     {
         string filename;
         cout << "Enter " << i << ". file name: ";
         cin >> filename;


         //open file and read word by word
         ifstream file;
         file.open(filename.c_str());

         //check if file is open successfully
         while(file.fail())
         {
             cout << "File not found. Try again." << endl;
             cout << "Enter " << i << ". file name: ";
             cin >> filename;
             file.open(filename.c_str());
         }

         //add filename to vector
         filenames.push_back(filename);

         string word;
         while(file >> word)
         {
             makeValid(word);
             //check if word is empty or a number
             if(!(isdigit(word[0]) || word.length() <= 1))
             {
                 //cout << word << endl;
                 //check if word is in the tree
                 WordItem* item = tree.find(word);
                 if(item == NULL)
                 {
                     //item not found, create new item and add to tree
                     vector<DocumentItem> docs;
                     docs.push_back(DocumentItem(filename, 1));
                     tree.insert(word, new WordItem(word, docs));
                 }
                 else
                 {
                     //item found, update the item
                     //check if the document is already in the list
                     bool found = false;
                    for(int j=0; j<item->docList.size(); j++)
                    {
                        if(item->docList[j].documentName == filename)
                        {
                            item->docList[j].count++;
                            found = true;
                            j = item->docList.size();
                        }
                    }

                     //if not found, add the document to the list
                    if(!found)
                    {
                        item->docList.push_back(DocumentItem(filename, 1));
                    }
                 }

             }

         }
     }
     //I think the tree is built at this point
    //Sort filenames
    insertionSort(filenames);

     //get second round of inputs
     string query;
     cout << "Enter queried words in one line: ";
     cin.ignore();
     getline(cin, query);



     while(query != "ENDOFINPUT")
     {
         //check if the input starts with REMOVE
        if(query.substr(0, 6) == "REMOVE")
        {
            //remove the word from the tree
            string word = query.substr(7, query.length()-7);
            makeValid(word);
            tree.remove(word);
            cout << word << " has been REMOVED." << endl;
            //clear cin
            query="";
            continue;
        }

         //else run the query and print results
         //make a list of all the queried words
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


         //get next query
         cout << "Enter queried words in one line: ";
         getline(cin, query);
    }

    return 0;
}
