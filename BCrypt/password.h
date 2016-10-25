#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include "botan/bcrypt.h"
#include "botan/auto_rng.h"

class Password
{
public:
    static Password* getInstance();
    bool validate(const std::string &input);
private:
    Password();
    static Password *m_Instance;

    std::string m_HashPW;
};

#endif // PASSWORD_H
