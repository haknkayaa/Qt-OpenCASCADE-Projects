#include <iostream>
#include <vector>

int main() {
    short variable_number;

    std::cout << "> ";
    std::cin >> variable_number;

    std::vector<int> my_var(variable_number);
    for (int i = 0; i < variable_number; i++) {
        my_var[i] = i;
    }


    for(auto temp : my_var){
        std::cout << "Değişken " << temp << std::endl;
    }

    return 0;
}