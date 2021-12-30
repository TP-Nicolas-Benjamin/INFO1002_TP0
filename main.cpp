#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;


map<string, double> log_tetragrammes {};
int total_tetragrammes = 0;

vector<vector<char>> alphabet = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'K'},
    {'L', 'M', 'N', 'O', 'P'},
    {'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z'}
};
vector<vector<char>> key = {{}, {}, {}, {}, {}};

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
    return log_tetragrammes.count(str) ? log_tetragrammes[str] : log(0.01/total_tetragrammes);
}

double score(string text) {
    double score = 0.0;

    for (int i = 0; i < text.length()-3; i++) {
        string txt = text.substr(i, 4);
        double p = prob(txt);
        // cout << txt << " " << p << endl;
        score += p;
    }

    return score;
}

bool key_contains_char(vector<vector<char>> k, char c) {
    for (vector<char> v : k ) {
        if (find(v.begin(), v.end(), c) != v.end()) {
            return true;
        }
    }
    return false;
}

vector<vector<char>> genere_cle(string texte) {
    vector<vector<char>> k = {{}, {}, {}, {}, {}};
    if (texte == "") {
        k = alphabet;
    } else {
        int last_y = 0;
        int last_x = 0;

        int txtLenght = texte.length();
        int kSize = k.size();

        for (
            int y = 0;
            y < (
                txtLenght < kSize * kSize 
                ? ((int) txtLenght / kSize) + 1
                : kSize
            );
            y++)
        {
            for (char& c : texte.substr(y*5, txtLenght)) {
                if (!(key_contains_char(k, c))) {
                    if (c == 'J') k[y].push_back('I');
                    if (c == 'W') k[y].push_back('V');
                    else k[y].push_back(c);
                    last_x = (last_x + 1) % 5;
                    if (last_x == 0) {
                        last_y = (last_y + 1) % 5;
                    }
                }
                if (k[y].size() == 5) break;
            } 
        }

        cout << "last y : " << last_y << endl;
        cout << "last x : " << last_x << endl;

        for (vector<char> v : alphabet) {
            if (last_x == 5) {
                last_y++;
            }
            for (char& c : v) {
                if (!key_contains_char(k, c)) {
                    k[last_y].push_back(c);
                    last_x = (last_x + 1) % 5;
                    if (last_x == 0) {
                        last_y = (last_y + 1) % 5;
                    }
                }
            }
        }
    }

    return k;
}

void affiche_cle(vector<vector<char>> k) {
    int counter = 0;

    cout << "Current key is : " << endl;
    for (vector<char> v : k) {
        for (char& c : v) {
            cout << c << " ";
        }
        cout << endl;
    }
}

pair<int, int> coords_of_char(vector<vector<char>> k, char c) {
    for (int y = 0; y < k.size(); y++) {
        for (int x = 0; x < k[y].size(); x++) {
            if (k[y][x] == c) return make_pair(y,x);
        }
    }
    cout << c << " not found !" << endl;
    return make_pair(-1,-1);
}

pair<char, char> chiffre_pair(vector<vector<char>> k, char a, char b) {
    int xa = -1;
    int xb = -1;
    int ya = -1;
    int yb = -1;

    pair<int, int> coords;
    for (vector<char> v : k)  {
        for (char& c : v) {
            if (c == a) {
                coords = coords_of_char(k, c);
                // cout << (coords.first != -1 && coords.second != -1) << endl;
                if (coords.first != -1 && coords.second != -1) {
                    ya = coords.first;
                    xa = coords.second;
                }
            }
            if (c == b) {
                coords = coords_of_char(k, c);
                // cout << (coords.first != -1 && coords.second != -1) << endl;
                if (coords.first != -1 && coords.second != -1) {
                    // cout << "not -1" << endl;
                    yb = coords.first;
                    xb = coords.second;
                }
            }
            if (xa != -1 && xb != -1 && ya != -1 && yb != -1) break;
        }
    }

    cout << xa << "\t" << xb << endl;
    cout << ya << "\t" << yb << endl;
    if (xa != -1 && xb != -1 && ya != -1 && yb != -1) {
        pair<char, char> res;

        if (ya == yb) {
            cout << "on same line" << endl;
            res.first = k[ya][(xa+1) % k[ya].size()];
            res.second = k[yb][(xb+1) % k[yb].size()];
        } else if (xa == xb) {
            cout << "on same column" << endl;
            res.first = k[(ya+1) % k.size()][xa];
            res.second = k[(yb+1) % k.size()][xb];
        } else {
            cout << "different column and line" << endl;
            res.first = k[ya][xb];
            res.second = k[yb][xa];
        }
        return res;
    } else {
        cout << "Une des lettres n'est pas disponible dans la clé !\n";
        return make_pair(a,b);
    }
}

int main(int argc, char const *argv[])
{
    string filename = argv[1];
    string keyword_gen = argv[2];
    string text = argv[3];
    
    // Initialisation
    set_total(filename);
    cout << "\n\nTotal tetragrammes : " << total_tetragrammes << endl;
    log_init(filename);
    cout << "Initialisation done -----\n\n" << endl;

    // Calculate score of text
    cout << "Score of " << text << " : " << score(text) << endl << endl;

    key = genere_cle(keyword_gen);
    affiche_cle(key);

    // Avec 2 lettres trouvées
    cout << endl << "----------\nI\tK" << endl;
    auto p = chiffre_pair(key, 'I', 'K');
    cout << "I -> " << p.first << "\t" << "K -> " << p.second << endl;

    // Avec 2 lettres trouvées sur la même ligne
    cout << endl << "----------\nN\tA" << endl;
    p = chiffre_pair(key, 'N', 'A');
    cout << "N -> " << p.first << "\t" << "A -> " << p.second << endl;

    // Avec 2 lettres trouvées
    cout << endl << "----------\nB\tV" << endl;
    p = chiffre_pair(key, 'B', 'V');
    cout << "B -> " << p.first << "\t" << "V -> " << p.second << endl;

    // Avec 1 lettre qui n'existe pas (J)
    cout << endl << "----------\nM\tJ" << endl;
    p = chiffre_pair(key, 'M', 'J');
    cout << "M -> " << p.first << "\t" << "J -> " << p.second << endl;

    return 0;
}
