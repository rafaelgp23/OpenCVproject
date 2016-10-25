#include "password.h"

Password* Password::m_Instance = new Password();

Password::Password()
{
    m_HashPW = "$2a$06$ldcpJqrDMqJWrKZ5Adb/KeEL1BiYYdX44RRtnQ1AMC6dYNAYSKuSe";
}

Password* Password::getInstance()
{
    return m_Instance;
}

bool Password::validate(const std::string &password)
{
    if (m_HashPW.empty())
        return true;
    return Botan::check_bcrypt(password,m_HashPW);
}
