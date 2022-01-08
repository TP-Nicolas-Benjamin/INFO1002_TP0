#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include "CLI11.hpp"

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

        // cout << "last y : " << last_y << endl;
        // cout << "last x : " << last_x << endl;

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

    // cout << "Current key is : " << endl;
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

    // cout << xa << "\t" << xb << endl;
    // cout << ya << "\t" << yb << endl;
    if (xa != -1 && xb != -1 && ya != -1 && yb != -1) {
        pair<char, char> res;

        if (ya == yb) {
            // cout << "on same line" << endl;
            xa = (xa+1) % k[ya].size();
            xb = (xb+1) % k[yb].size();
            res.first = k[ya][xa];
            res.second = k[yb][xb];
        } else if (xa == xb) {
            // cout << "on same column" << endl;
            ya = (ya+1) % k.size();
            yb = (yb+1) % k.size();
            res.first = k[ya][xa];
            res.second = k[yb][xb];
        } else {
            // cout << "different column and line" << endl;
            res.first = k[ya][xb];
            res.second = k[yb][xa];
        }
        return res;
    } else {
        // cout << "Une des lettres n'est pas disponible dans la clé !\n";
        return make_pair(a,b);
    }
}

string chiffre_texte(string text, vector<vector<char>> k) {

    string res = "";
    char charToCipher = '\0';

    for (char& c : text) {
        // if (key_contains_char(k, c)) {
            if (charToCipher == '\0') charToCipher = c;
            else {
                if (c != charToCipher) {
                    // cout << endl << "----------\n" << charToCipher << "\t" << c << endl;
                    pair<char, char> p = chiffre_pair(k, charToCipher, c);
                    // cout << charToCipher << " -> " << p.first << "\t" << c << " -> " << p.second << endl;
                    res.push_back(p.first);
                    res.push_back(p.second);
                    charToCipher = '\0';
                } else {
                    pair<char, char> p = chiffre_pair(k, charToCipher, 'X');
                    res.push_back(p.first);
                    res.push_back(p.second);
                    charToCipher = c;
                }
            }
        // }
    }
    if (charToCipher != '\0') {
        pair<char, char> p = chiffre_pair(k, charToCipher, 'X');
        res.push_back(p.first);
        res.push_back(p.second);
    }

    return res;
}

vector<vector<char>> inverse_cle(vector<vector<char>> k) {
    vector<vector<char>> inverse_k = k;
    for (int y = 0; y < k.size(); y++) {
        for (int x = 0; x < k[y].size(); x++) {
            inverse_k[4-y][4-x] = k[y][x];
        }
    }
    return inverse_k;
}

string dechiffre_texte(string cipher, vector<vector<char>> k) {
    vector<vector<char>> inverse_k = inverse_cle(k);
    string res = "";
    char cipheredChar = '\0';

    for (char& c : cipher) {
        // if (key_contains_char(k, c)) {
            if (cipheredChar == '\0') cipheredChar = c;
            else {
                if (c != cipheredChar) {
                    // cout << endl << "----------\n" << cipheredChar << "\t" << c << endl;
                    pair<char, char> p = chiffre_pair(inverse_k, cipheredChar, c);
                    // cout << cipheredChar << " -> " << p.first << "\t" << c << " -> " << p.second << endl;
                    res.push_back(p.first);
                    res.push_back(p.second);
                    cipheredChar = '\0';
                } else {
                    res.push_back(cipheredChar);
                    res.push_back('X');
                    res.push_back(c);
                    cipheredChar = '\0';
                }
            }
        // }
    }
    if (cipheredChar != '\0') {
        pair<char, char> p = chiffre_pair(inverse_k, cipheredChar, 'X');
        res.push_back(p.first);
        res.push_back(p.second);
    }

    if (res.back() == 'X') res.pop_back();

    char lastChar = '\0';
    for (int i = 0; i < res.size(); i++) {
        if (res[i] == 'X' && lastChar == res[i+1]) res.erase(res.begin() + i);
        lastChar = res[i];
    }

    return res;
}

vector<vector<char>> perturbe_cle(vector<vector<char>> k) {
    int maxRand = 0;
    for (int i = 0; i < k.size(); i++) {
        maxRand += k[i].size();
    }
    int rand1 = rand() % maxRand;
    int rand2 = rand1;
    while (rand2 == rand1) {
        rand2 = rand() % maxRand;
    }

    int y1 = (int) rand1 / k.size();
    int x1 = rand1 % k[y1].size();
    int y2 = (int) rand2 / k.size();
    int x2 = rand2 % k[y2].size();

    //cout << k[y1][x1] << " -> " << k[y2][x2] << "\t" << k[y2][x2] << " -> " << k[y1][x1] << endl;
    char tmp = k[y1][x1];
    k[y1][x1] = k[y2][x2];
    k[y2][x2] = tmp;
    return k;
}

vector<vector<char>> make_random_key() {
    vector<vector<char>> kcpy = alphabet;
    vector<vector<char>> k = {
        {'\0','\0','\0','\0','\0'}, 
        {'\0','\0','\0','\0','\0'}, 
        {'\0','\0','\0','\0','\0'}, 
        {'\0','\0','\0','\0','\0'}, 
        {'\0','\0','\0','\0','\0'}
    };

    int maxRand = k.size();

    for (vector<char> v : kcpy) {
        for (char& c : v) {
            int x = rand() % maxRand;
            int y = rand() % maxRand;
            while (k[y][x] != '\0') {
                x = rand() % maxRand;
                y = rand() % maxRand;
            }
            k[y][x] = c;
        }
    }

    return k;
}

vector<vector<char>> craque(string cipher, int n) {
    /*
    --- pseudo code ---
    clé = clé aléatoire
    --- on teste N permutations de la clé ---
    pour k de 1 à N:
        clé_tmp = echange 2 caractères dans clé
        si clé_tmp meilleur que clé:
            clé = clé_tmp
    */

    double best_score = -100000000000000;
    vector<vector<char>> k = make_random_key();
    // affiche_cle(k);
    vector<vector<char>> tmp_key;

    for (int i = 0; i < n-1; i++) {
        tmp_key = perturbe_cle(k);
        // string k_decipher = dechiffre_texte(cipher, k);
        string tmp_decipher = dechiffre_texte(cipher, tmp_key);
        double score_tmp = score(tmp_decipher);

        if (score_tmp > best_score) {
            // cout << "BETTER SCORE ---" << endl;
            best_score = score_tmp;
            k = tmp_key;
        }
    }

    return inverse_cle(k);
}

void text_to_valid(string& text) {
    // string tmp = text;
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    replace(text.begin(), text.end(), 'J', 'I');
    text.erase(std::remove_if(text.begin(), text.end(), [](char c) { return !std::isalpha(c); }), text.end());
    // return tmp;
}

void test(vector<vector<char>> k, string text) {
    // Perturbating key
    k = perturbe_cle(k);
    cout << endl << endl << "----- Perturbated key :" << endl;
    affiche_cle(k);

    // Avec 2 lettres trouvées
    cout << endl << "----- Changement de lettres : " << endl;
    cout << "----------\nI\tK" << endl;
    auto p = chiffre_pair(k, 'I', 'K');
    cout << "I -> " << p.first << "\t" << "K -> " << p.second << endl;

    // Avec 2 lettres trouvées sur la même ligne
    cout << endl << "----------\nN\tA" << endl;
    p = chiffre_pair(k, 'N', 'A');
    cout << "N -> " << p.first << "\t" << "A -> " << p.second << endl;

    // Avec 2 lettres trouvées
    cout << endl << "----------\nB\tV" << endl;
    p = chiffre_pair(k, 'B', 'V');
    cout << "B -> " << p.first << "\t" << "V -> " << p.second << endl;

    // Avec 1 lettre qui n'existe pas (J)
    cout << endl << "----------\nM\tJ" << endl;
    p = chiffre_pair(k, 'M', 'J');
    cout << "M -> " << p.first << "\t" << "J -> " << p.second << endl;

    string cipheredText = chiffre_texte(text, key);
    // Dechiffre le texte avec la fonction naïve
    cout << endl << "----- Chiffre le texte" << endl;
    cout << text << endl << cipheredText << endl;
    cout << endl << "----- Dechiffre le texte avec la fonction naïve" << endl;
    cout << cipheredText << endl << dechiffre_texte(cipheredText, key) << endl;

    cout << endl << endl << "----- CRAQUE (don't think it's working) : " << endl;
    auto cle = craque(cipheredText, 1000);
    cout << "Clé avec la score le plus élevé : " << endl;
    affiche_cle(cle);
    cout << endl << "Texte décrypté avec la clé ci-dessus : "<< endl << dechiffre_texte(cipheredText, inverse_cle(cle)) << endl;
}

int main(int argc, char const *argv[])
{
    string filename = argv[1];
    string keyword_gen = argv[2];
    // string text = argv[3];
    string textToCipher = argv[3];
    text_to_valid(keyword_gen);
    // text_to_valid(text);
    text_to_valid(textToCipher);
    
    // Initialisation
    set_total(filename);
    cout << "----- Total tetragrammes : " << total_tetragrammes << endl;
    log_init(filename);
    cout << "----- Initialisation done -----\n\n" << endl;

    // Calculate score of text
    cout << "----- Score of " << textToCipher << " : " << score(textToCipher) << endl << endl;

    key = genere_cle(keyword_gen);
    cout << "----- Clé générée à partir du mot \"" << keyword_gen << "\"" << endl;
    affiche_cle(key);

    test(key, textToCipher);

    return 0;
}
