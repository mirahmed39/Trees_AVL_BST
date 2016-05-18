#include "AvlTree.h"
#include "BinarySearchTree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;
// function protocols
template<class TreeType>
void ConstructTree( TreeType &a_tree, const string& db_filname );

template<class TreeType>
int CountSuccessfulQueries( const TreeType &a_tree, const string &db_filename );

template<class TreeType>
int CountFindRecursionCalls( const TreeType &a_tree, const string &db_filename );

template<class TreeType>
int CountRemoveEveryotherSequences( TreeType &a_tree, const string& db_filname );


// Sample main for program testTrees
int main(int argc, char **argv) {
    if (argc != 4) {
    cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename> <tree-type>" << endl;
    return 0;
    }
    string db_filename(argv[1]);
    string query_filename(argv[2]);
    string param_tree(argv[3]);
    //int recursion_calls;
    cout << "Input file is " << db_filename << ", and query file is " << query_filename << endl;
    if (param_tree == "BST") {
        cout << "I will run the BST code " << endl;

        BinarySearchTree<SequenceMap> a_tree;
        //1
        ConstructTree(a_tree, db_filename);
        int total_nodes = a_tree.countNodes(),
            succesful_queries = CountSuccessfulQueries(a_tree, query_filename),
            find_counter = CountFindRecursionCalls(a_tree, query_filename);
        double average_depth = double(a_tree.depthOfTree(0)) / total_nodes,
                ratio = average_depth / log2(total_nodes);

        //2
        cout << "Total Number of Nodes: " << total_nodes << endl;
        cout << "Average Depth of Tree: " << average_depth << endl;
        cout << "Ratio: " << ratio << endl;

        //3
        cout << "Successful Queries: " << succesful_queries << endl;
        cout << "Average Recursion Calls for Find: " << double(find_counter) / succesful_queries << endl;

        //4
        int delete_counter = CountRemoveEveryotherSequences(a_tree, query_filename);
        average_depth = double(a_tree.depthOfTree(0)) / a_tree.countNodes();
        ratio = average_depth / log2(a_tree.countNodes());
        cout << "Successful Number of Removes: " << total_nodes - a_tree.countNodes() << endl;

        //5
        cout << "Total Nodes after Deletion: " << a_tree.countNodes() << endl;
        cout << "Average depth after deletion: " << average_depth << endl;
        cout << "Average Ratio after deletion: " << ratio << endl;
        cout << "Average Recursion calls for Delete: " << double(delete_counter) / (total_nodes - a_tree.countNodes()) << endl;

    }
    else if (param_tree == "AVL") {
        cout << "I will run the AVL code " << endl;
        AvlTree<SequenceMap> a_tree;
        //1
        ConstructTree(a_tree, db_filename);
        int total_nodes = a_tree.countNodes(),
            succesful_queries = CountSuccessfulQueries(a_tree, query_filename),
            find_counter = CountFindRecursionCalls(a_tree, query_filename);
        double average_depth = double(a_tree.depthOfTree(0)) / total_nodes,
                ratio = average_depth / log2(total_nodes);

        //2
        cout << "Total Number of Nodes: " << total_nodes << endl;
        cout << "Average Depth of Tree: " << average_depth << endl;
        cout << "Ratio: " << ratio << endl;

        //3
        cout << "Successful Queries: " << succesful_queries << endl;
        cout << "Average Recursion Calls for Find: " << double(find_counter) / succesful_queries << endl;

        //4
        int delete_counter = CountRemoveEveryotherSequences(a_tree, query_filename);
        average_depth = double(a_tree.depthOfTree(0)) / a_tree.countNodes();
        ratio = average_depth / log2(a_tree.countNodes());
        cout << "Successful Number of Removes: " << total_nodes - a_tree.countNodes() << endl;

        //5
        cout << "Total Nodes after Deletion: " << a_tree.countNodes() << endl;
        cout << "Average depth after deletion: " << average_depth << endl;
        cout << "Average Ratio after deletion: " << ratio << endl;
        cout << "Average Recursion calls for Delete: " << double(delete_counter) / (total_nodes - a_tree.countNodes()) << endl;
    } else {
        cout << "Unknown tree type " << param_tree << " (User should provide BST, or AVL)" << endl;
    }

    return 0;
}



template<class TreeType>
void ConstructTree( TreeType &a_tree, const string& db_filename ) {
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

template<class TreeType>
int CountSuccessfulQueries( const TreeType &a_tree, const string &db_filename ) {
    ifstream myFile(db_filename);
    string word;
    int countSuccess = 0;
    // we dont need this, but we have to pass it in the function
    // just to keep the function prototype correct.
    int temp_counter = 0;
    while(myFile >> word) {
        SequenceMap map(word, ""); // we just need sequence to make contains work.
        bool success = a_tree.contains(map, temp_counter);
        if (success)
            countSuccess++;
    }
    myFile.close();
    return countSuccess;
}

template<class TreeType>
int CountFindRecursionCalls( const TreeType &a_tree, const string &db_filename ) {
    ifstream myFile(db_filename);
    string word;
    int find_counter = 0;
    while(myFile >> word) {
        SequenceMap map(word, ""); // we just need sequence to make contains work.
        bool success = a_tree.contains(map, find_counter);
    }
    myFile.close();
    return find_counter;
}

template<class TreeType>
int CountRemoveEveryotherSequences( TreeType &a_tree, const string& query_filename ) {
    ifstream myFile(query_filename);
    string word;
    int delete_counter = 0;
    // we dont need this, but we have to pass it in the function
    // just to keep the function prototype correct.
    int temp_counter = 0;
    int state = 1;
    while(getline(myFile, word)) {
        if(state%2 == 0) {
            SequenceMap map(word, "");
            if (a_tree.contains(map, temp_counter)){
                a_tree.remove(map, delete_counter);
            }
            state++; // the next iteration will skip this if statement
                    // since it becomes an odd integer.
        }
        else
            state++;
    }
    myFile.close();
    return delete_counter;
}



