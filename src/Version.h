#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

#include <iostream>

struct Version
{
    unsigned major;
    unsigned minor;

    inline friend std::istream& operator>> (std::istream& is, Version& ver)
    {
        is.ignore(); // skipping 'v'
        is >> ver.major;
        is.ignore(); // skipping '.'
        is >> ver.minor;
        return is;
    }

    inline friend std::ostream& operator<< (std::ostream& os, const Version& ver)
    {
        os << "v" << ver.major << "." << ver.minor;
        return os;
    }

    bool operator== (const Version& ver)
    {
        return (major == ver.major && minor == ver.minor);
    }

    bool operator!= (const Version& ver)
    {
        return !(*this == ver);
    }
};

#endif // VERSION_H_INCLUDED
