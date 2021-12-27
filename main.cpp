#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <math.h>

using namespace std;


map<string, double> log_tetragrammes {};
int total_tetragrammes = 0;

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void set_total(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()){   //checking whether the file is open
      string line;
      while(getline(file, line)){ //read data from file object and put it into string.
        vector<string> v = split (line, '\t');
        total_tetragrammes += atoi(v[1].c_str());
      }
      file.close(); //close the file object.
   }
}

void log_init(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()){   //checking whether the file is open
      string line;
      while(getline(file, line)){ //read data from file object and put it into string.
        vector<string> v = split (line, '\t');
        string key = v[0];
        double value = log(atof(v[1].c_str()) / total_tetragrammes);
        log_tetragrammes[key] = value;
      }
      file.close(); //close the file object.
   }
}

double prob(string str) {
    return log_tetragrammes[str];
}

double score(string text) {
    double score = 0.0;

    for (int i = 0; i < text.length()-3; i++) {
        string txt = text.substr(i, 4);
        double p = prob(txt);
        cout << txt << " " << p << endl;
        score += p;
    }

    return score;
}

int main(int argc, char const *argv[])
{
    cout << 95675200 << endl;
    set_total(argv[1]);
    cout << total_tetragrammes << endl;
    log_init(argv[1]);
    cout << score(argv[2]) << endl;
    return 0;
}
