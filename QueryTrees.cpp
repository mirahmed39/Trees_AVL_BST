#include "AvlTree.h"
#include "BinarySearchTree.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template <typename TreeType>
void TestQueryTree(TreeType &a_tree, const string &db_filename);

// Sample main for program queryTrees
int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <databasefilename> <tree-type>" << endl;
        return 0;
    }
    string db_filename(argv[1]);
    string param_tree(argv[2]);
    cout << "Input filename is " << db_filename << endl;
    if (param_tree == "BST") {
        cout << "I will run the BST code" << endl;
        // Insert code for testing a BST tree.
        BinarySearchTree<SequenceMap> a_tree;
        TestQueryTree(a_tree, db_filename);
        string sequence;
        int temp_counter = 0;
        cout << "Please enter recognition sequence: ";
        cin >> sequence;
        SequenceMap map(sequence, "");
        if(a_tree.contains(map, temp_counter))
            a_tree.printElement(map);
        else
            cout << "Tree does not contain the sequence\n";
    } else if (param_tree == "AVL") {
        cout << "I will run the AVL code" << endl;
        AvlTree<SequenceMap> a_tree;
        TestQueryTree(a_tree, db_filename);
        string sequence;
        int temp_counter = 0;
        cout << "Please enter recognition sequence: ";
        cin >> sequence;
        SequenceMap map(sequence, "");
        if(a_tree.contains(map, temp_counter))
            a_tree.printElement(map);
        else
            cout << "Tree does not contain the sequence\n";
    } else {
        cout << "Unknown tree type " << param_tree << " (User should provide BST, or AVL)" << endl;
    }
    return 0;
}

template <typename TreeType>
void TestQueryTree(TreeType &a_tree, const string &db_filename) {
    ifstream input_file(db_filename);
    if (!input_file.good())
        cerr << ".... error condition ...." << endl;
    string line;
    while (getline(input_file, line)) {
        if (line[line.length()-1] != '/' && line[line.length()-2] != '/')
            continue;
        stringstream ss;
        ss << line;
        vector<string> sequences;
        string word;
        while (getline(ss, word,'/'))
           sequences.push_back(word);
        for (unsigned int i = 1; i < sequences.size(); i++){
            SequenceMap a_map(sequences[i], sequences[0]);
            a_tree.insert(a_map);
        }
    }
    input_file.close();
}
