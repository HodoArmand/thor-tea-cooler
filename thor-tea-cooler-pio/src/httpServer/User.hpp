#include <Arduino.h>

class User
{
private:
    int id;
    String name;
    String password;

public:
    User(/* args */);
    User(String name_, String password_);
    User(int id_, String name_, String password_);
    User(String templateType);
    ~User();

    String toString();

    String getPassword() const { return password; }
    void setPassword(const String &password_) { password = password_; }

    String getName() const { return name; }
    void setName(const String &name_) { name = name_; }

    int getId() const { return id; }
    void setId(int id_) { id = id_; }
};

User::User()
{
}

User::User(String name_, String password_)
{
    id = -2;
    name = name_;
    password = password_;
}

User::User(int id_, String name_, String password_)
{
    id = id_;
    name = name_;
    password = password_;
}

User::User(String templateType)
{
    if (templateType == "userNotFound")
    {
        id = -1;
        name = "Error: user not found";
        password = "Error: user not found";
    }
}

inline String User::toString()
{
    return ("Id: " + String(id) + " Name: " + name + " Password:" + password);
}

User::~User()
{
}
