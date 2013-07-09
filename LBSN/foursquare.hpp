#ifndef FOURSQUARE_H
#define FOURSQUARE_H
#include "lbsn_global.h"
#include <QString>
#include <vector>
#include <utility>
#include <unordered_map>

class LBSNSHARED_EXPORT Foursquare
{
public:
    Foursquare();

    struct UserPath
    {
        std::vector<std::pair<quint32, quint64>> path;//list of pair<place ID, created time>
    };

public:

    void SetPathOfDatabase(QString path);
protected:

    void ReadUsersPath();

protected:
    QString path_database;
    std::unordered_map<size_t, UserPath> list_user_path;

 //map raw placeID in Foursquare into a compact 32bit ID
    std::unordered_map<quint64, quint32> table_placeID; 

};

#endif // FOURSQUARE_H
