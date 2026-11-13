#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;

int main(){
    const int n = 10;
    int arr[10] = {1, 2, 3, 5, 3, 3, 3, 3, 1, 10};
    unordered_map<int, int> numOcc = {};
    for(int next: arr){
        auto key = numOcc.find(next);
        if(key == numOcc.end()){
            numOcc.insert({next, 1});
        } else {
            key->second += 1;
        }
    }
    for(const auto& [key, value] : numOcc){
        if(value > n / 2){
            cout << key << " is majority" << endl;
        }
    }
    return 0;
}