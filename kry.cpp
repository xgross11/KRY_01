#include <iostream>
#include <fstream>
#include<sstream>
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
        order_number = order_number-32; // ASCII a is 97
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

int main (int argc, char* argv[]) {

    string filename_in, filename_out = "";
    string input_text = ""; 
    int a, b;
    string version;

    parse_parameters(argc, argv, &a, &b, &version, &filename_in, &filename_out, &input_text);

    if (version == "e") {
        string encrypted_text;

        if(filename_in != "") {
            input_text = get_input_from_file(filename_in);
        } 

        encrypted_text = encrypt(a, b, input_text);

        if(filename_out == "") {
            printf("%s", encrypted_text.c_str());
        } else {
            write_output_to_file(filename_out, encrypted_text);
        }

    } else if (version == "d") {
        string decrypted_text;

        if(filename_in != "") {
            input_text = get_input_from_file(filename_in);
        } 

        decrypted_text = decrypt(a, b, input_text);

        if(filename_out == "") {
            printf("%s", decrypted_text.c_str());
        } else {
            write_output_to_file(filename_out, decrypted_text);
        }
    } else if (version == "c") {
        //TODO
    }

    return 0;

}