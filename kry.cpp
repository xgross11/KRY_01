#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

void parse_parameters(int argc, char*argv[], int*a, int*b, string*version, string*filename_in, string*filename_out, string*input_text) {

    for (int i = 0; i < argc; i++) {

        if (strcmp(argv[i],"-a")==0) {
            *a = stoi(argv[i+1]);
            i++;
        } else if (strcmp(argv[i],"-b")==0) {
            *b = stoi(argv[i+1]);
            i++;
        } else if (strcmp(argv[i],"-e")==0) {
            *version = "e";
        } else if (strcmp(argv[i],"-d")==0) {
            *version = "d";
        } else if (strcmp(argv[i],"-c")==0) {
            *version = "c";
        } else if (strcmp(argv[i],"-f")==0) {
            *filename_in = argv[i+1];
            i++;
        } else if (strcmp(argv[i],"-o")==0) {
            *filename_out = argv[i+1];
            i++;
        } else {
            if(*filename_in == "" && i != 0) {
                *input_text = argv[i];
            }
        }

    }

}

string get_input_from_file(string filepath) {

    string content;

    ifstream file(filepath);
    if (file) { 
        ostringstream ss;
        ss << file.rdbuf();
        content = ss.str();
    }
    file.close();
    return content;

}

void write_output_to_file(string filepath, string content) {
    
    ofstream file;
    file.open (filepath);
    file << content;
    file.close();

}

int get_order_number(char symbol) {
    int order_number;
    order_number = int(symbol)-65;  // 65 is ASCII code of A
    if(order_number > 25){
        order_number = order_number-32; // ASCII code of a is 97
    }
    return order_number;
}

char get_letter_from_order_number(int number) {
    char letter;
    letter = char(number+65);  // 65 is ASCII code of A
    return letter;
}

string encrypt(int a, int b, string input_text){

    string encrypted_text;
    char encrypted_symbol;

    for (int i = 0; i < input_text.size(); i++){
        if(int(input_text[i]) != 32) {
            int number_value = get_order_number(input_text[i]);
            encrypted_symbol = get_letter_from_order_number(((a * number_value) + b) % 26);
            encrypted_text = encrypted_text + encrypted_symbol;
        } else {
            encrypted_text = encrypted_text + input_text[i];
        }
    }

    return encrypted_text;
}

int modInverse(int a){
    for (int i = 1; i < 26; i++){
        if ((a*i) % 26 == 1){
            return i;
        }
    }

    return 0;
            
}

string decrypt(int a, int b, string input_text){
    
    string decrypted_text;
    char decrypted_symbol;

    int mod_inverse = modInverse(a);

    for (int i = 0; i < input_text.size(); i++){
        if(int(input_text[i]) != 32) {

            int number_value = get_order_number(input_text[i]);
            decrypted_symbol = get_letter_from_order_number((((mod_inverse*(number_value - b)) % 26) + 26) % 26); //for number_value < b was negative results, +26%26 is for correction
            decrypted_text = decrypted_text + decrypted_symbol;

        } else {
            decrypted_text = decrypted_text + input_text[i];
        }
    }

    return decrypted_text;

}

string hack(string input_text){

    map<char,int> char_count;
    char t;
    char most_frequented, second_most_frequented, third_most_frequented = 'w'; // least frequented letter
    int a = -1, b = -1;
    vector<int> a_value = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    vector<pair<int,int>> ABvalues;

    for (int i=0; i < input_text.length(); i++){ 
        t = input_text[i];
        if(t != ' ') {  // save just letters, not spaces
            char_count[t]? char_count[t]++ : char_count[t] = 1;
        }
    }

    map<char, int>::iterator it = char_count.begin();

    // compute most frequented letters in input text
    while (it != char_count.end()) {

        if(it->second > char_count[third_most_frequented]) {  //if count of current letter is more than of the most frequented saved letter
            if(it->second > char_count[second_most_frequented]){
                if(it->second > char_count[most_frequented]){
                    third_most_frequented = second_most_frequented;
                    second_most_frequented = most_frequented; // current most frequented letter is now second most frequented most frequented
                    most_frequented = it->first;              // save letter as most frequented
                } else {
                    third_most_frequented = second_most_frequented;
                    second_most_frequented = it->first; 
                }

            } else {
                third_most_frequented = it->first;
            }
        }
        ++it;
    }

    map<char,int> top_letters_map;
    top_letters_map = {{'O', get_order_number('O')},    // most frequented letter in czech language
                       {'E', get_order_number('E')},    // second most frequented letter in czech language
                       {'N', get_order_number('N')},
                       {'I', get_order_number('I')},
                       {'A', get_order_number('A')}, 
                       {'T', get_order_number('T')},
                       {'V', get_order_number('V')},
                       {'R', get_order_number('R')},
                       {'U', get_order_number('U')},
                       {'S', get_order_number('S')},
                       {'K', get_order_number('K')},
                       {'L', get_order_number('L')},
                       {'M', get_order_number('M')},
                       {'Z', get_order_number('Z')}};

    int order_of_most_frequented = get_order_number(most_frequented);
    int order_of_second_most_frequented = get_order_number(second_most_frequented);
    int order_of_third_most_frequented = get_order_number(third_most_frequented);

    // gather all a-b combinations that fits equation for encrypting
    for (int i: a_value){ 
        for (int j=0; j < 26; j++) {
            map<char, int>::iterator it;
            for (it = top_letters_map.begin(); it != top_letters_map.end(); it++) {
                if ((i * it->second + j) % 26 == order_of_most_frequented ){
                    map<char, int>::iterator it2;
                    for (it2 = top_letters_map.begin(); it2 != top_letters_map.end(); it2++) {
                        if( (((i * it2->second) + j) % 26 == order_of_second_most_frequented)) {
                            map<char, int>::iterator it3;
                            for (it3 = top_letters_map.begin(); it3 != top_letters_map.end(); it3++) {
                                if( (((i * it3->second) + j) % 26 == order_of_third_most_frequented)) {
                                    ABvalues.push_back(make_pair(i,j));
                                }
                            }
                        }
                    }
                } 
            }
        }
    }

    //for each a-b key combination counts occurances of QWX-least frequented letters in czech
    map<int, int> QWXcount;
    for(int i = 0;i < ABvalues.size();i++) {
        QWXcount[i] = 0;
        string Q, W, X;
        Q = encrypt(ABvalues[i].first, ABvalues[i].second, "Q");
        W = encrypt(ABvalues[i].first, ABvalues[i].second, "W");
        X = encrypt(ABvalues[i].first, ABvalues[i].second, "X");

        for (int j = 0; j < input_text.length(); j++){ 
            t = input_text[j];
            if(t != ' ') {  // just letters, not spaces
                if(t == Q[0] or t == W[0] or t == X[0]){
                    QWXcount[i] = QWXcount[i]+1;
                }
            }
        }
    } 

    //choose index of combination of keys with least count of QWX
    unsigned currentMin = INT_MAX;
    unsigned index = 0;
    for(auto it = QWXcount.cbegin(); it != QWXcount.cend(); ++it ) {
        if (it ->second < currentMin) {
            currentMin = it->second;
            index = it->first;
        }
    }
    

    a = ABvalues[index].first;
    b = ABvalues[index].second;

    printf("a=%d,b=%d\n", a, b);

    return decrypt(a, b, input_text);

}

int main (int argc, char* argv[]) {

    string filename_in, filename_out = "";
    string input_text = ""; 
    int a, b;
    string version;

    parse_parameters(argc, argv, &a, &b, &version, &filename_in, &filename_out, &input_text);

    if(filename_in != "") {
        input_text = get_input_from_file(filename_in);
    }

    if (version == "e") {
        string encrypted_text;

        encrypted_text = encrypt(a, b, input_text);

        if(filename_out == "") {
            printf("%s", encrypted_text.c_str());
        } else {
            write_output_to_file(filename_out, encrypted_text);
        }
    } else if (version == "d") {
        string decrypted_text;

        decrypted_text = decrypt(a, b, input_text);

        if(filename_out == "") {
            printf("%s", decrypted_text.c_str());
        } else {
            write_output_to_file(filename_out, decrypted_text);
        }
    } else if (version == "c") {
        string decrypted_text;

        decrypted_text = hack(input_text);

        if(filename_out == "") {
            printf("%s", decrypted_text.c_str());
        } else {
            write_output_to_file(filename_out, decrypted_text);
        }
    }

    return 0;

}