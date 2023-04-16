#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct ResposeData {
    bool Success;
    std::string Response;

    ResposeData() : Success(true) {}

    ResposeData(const std::string message) : Success(false), Response(std::move(message)) {}
};


enum Status {
    USER, MODER, ADMIN, BANNED, NOT_FOUND
};


namespace chat {
    struct User {
        int id;
        Status status;

        User() : status(NOT_FOUND) {}

        explicit User(int _id) : status(USER), id(_id) {}
    };

    std::vector<User> list;

    User *unregistred(new User[20]);
    size_t count = 0;


    User& findUser(int user_id) {
        for (auto& x: list) {
            if (x.id == user_id) return x;
        }
        return unregistred[count++];
    }

    class AccessControl {
    public:
        static void setAdmin(int _admin) {
            User admin = User(_admin);
            admin.status = ADMIN;
            list.push_back(admin);
        }

        ResposeData giveUserStatus(int user_id, int sender_id, Status newStatus) {
            User sender = findUser(sender_id);
            if (sender.status == NOT_FOUND) return {"sender not found"};
            if (!(sender.status == ADMIN || sender.status == MODER)) return {"sender has not access"};
            if (newStatus == ADMIN && sender.status == MODER) return {"sender has not access"};

            User& user = findUser(user_id);
            if (user.status == newStatus) return {"user is already has this status"};
            if (newStatus == BANNED && (user.status == ADMIN || user.status == MODER)) {
                return {"user cannot be banned"};
            }
            if (user.status == NOT_FOUND) return {"user not found"};
            if (newStatus == NOT_FOUND) return {"))))))))))))))))))))))))))))"};


            user.status = newStatus;
            return {};
        }

        ResposeData registerNewUser(int user_id, int sender_id) {
            User sender = findUser(sender_id);
            if (sender.status != ADMIN) return {"sender has not access"};

            User& user = findUser(user_id);
            if (user.status != NOT_FOUND) return {"user is already registered"};

            user.status = USER;
            user.id = user_id;
            list.push_back(user);
            return {};
        }
    };

    class Pausable {
        bool isPaused = false;
    public:
        std::vector <User> getList() const {
            return list;
        }

        bool getState() const { return isPaused; }

        ResposeData Pause(int sender_id) {
            User sender = findUser(sender_id);
            if (sender.status != ADMIN) return {"sender has not access"};

            if (isPaused) return {"chat is already paused"};

            isPaused = true;
            return {};
        }

        ResposeData Resume(int sender_id) {
            User sender = findUser(sender_id);
            if (sender.status != ADMIN) return {"sender has not access"};

            if (!isPaused) return {"chat is not paused"};

            isPaused = false;
            return {};
        }
    };
};


class SystemBase : public chat::AccessControl, public chat::Pausable {
public:
    SystemBase(int admin) {
        setAdmin(admin);
    }

    ResposeData BanUser(int sender, int user) {
        return giveUserStatus(user, sender,BANNED);
    }
    ResposeData UnBanUser(int sender, int user) {
        return giveUserStatus(user, sender, USER);
    }
    ResposeData RegisterNewUser(int sender, int newUser) {
        return registerNewUser(newUser, sender);
    }
    ResposeData GrantModeratorRole(int sender, int user) {
        return giveUserStatus(user, sender, MODER);
    }
    ResposeData GrantAdminRole(int sender, int user) {
        return giveUserStatus(user, sender, ADMIN);
    }
    ResposeData StopChat(int sender) {
        return Pause(sender);
    }
    ResposeData ResumeChat(int sender) {
        return Resume(sender);
    }
};


void PrintResponse(const ResposeData& response) {
    std::cout << "result: " << response.Success << "\nresponse: " << response.Response << "\n";
}

void printList(std::vector <chat::User>& lst) {
    for (auto& x: lst) {
        std::cout << x.id << ' ' << x.status << std::endl;
    }
}


int main() {
    const int USER1 = 1;
    const int USER2 = 2;
    const int USER3 = 3;
    SystemBase chat(1);
    auto responseRegisterFromAdmin1 = chat.RegisterNewUser(USER1, USER2);
    PrintResponse(responseRegisterFromAdmin1);
    auto responseRegisterFromAdmin2 = chat.RegisterNewUser(USER1, USER2);
    PrintResponse(responseRegisterFromAdmin2);
    auto responseGrantFromAdmin = chat.GrantModeratorRole(USER1, USER2);
    PrintResponse(responseGrantFromAdmin);
    auto responseRegisterFromModerator = chat.RegisterNewUser(USER2, USER3);
    PrintResponse(responseRegisterFromModerator);
    auto responseRegisterFromAdmin3 = chat.RegisterNewUser(USER1, USER3);
    PrintResponse(responseRegisterFromAdmin3);
    auto responseBanFromModer = chat.BanUser(USER2, USER3);
    PrintResponse(responseBanFromModer);
    auto responseStopFromModer = chat.StopChat(USER2);
    PrintResponse(responseStopFromModer);
    auto responseStopFromAdmin = chat.StopChat(USER1);
    PrintResponse(responseStopFromAdmin);
}