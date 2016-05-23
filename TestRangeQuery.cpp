#include <iostream>
#include <string>
#include "AvlTree.h"
#include <fstream>
#include <sstream>
using namespace std;

template<class TreeType>
void ConstructTree(TreeType &a_tree, const string db_filename);

// Sample main for program testTrees
int main(int argc, char **argv)
{
    if (argc != 4) {
    cout << "Usage: " << argv[0] << " <databasefilename> <string> <string2>" << endl;
    return 0;
    }
    string db_filename(argv[1]);
    string str1(argv[2]);
    string str2(argv[3]);
    cout << "Input file is " << db_filename << " ";
    cout << "String 1 is " << str1 << "   and string 2 is " << str2 << endl;

    AvlTree<SequenceMap> a_tree;
    ConstructTree(a_tree, db_filename);

    SequenceMap map_a(str1, "");
    SequenceMap map_b(str2, "");
    a_tree.TestRangeQuery(map_a, map_b);

    return 0;
}

template<class TreeType>
void ConstructTree(TreeType &a_tree, const string db_filename) {
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
