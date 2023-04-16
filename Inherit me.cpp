#include <string>
#include <iostream>

class Zxc : protected SecretClass {
public:
    bool IsMessageRight(const std::string& message) const {
        return CheckMessage(message);
    }
};

int main() {
    Zxc a;
    std::string str;
    std::cin >> str;
    std::cout << a.IsMessageRight(str) << std::endl;
}