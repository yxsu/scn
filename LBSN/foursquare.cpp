#include "foursquare.hpp"
#include <QFile>
#include <QTextStream>

Foursquare::Foursquare()
{
}

void Foursquare::SetPathOfDatabase(QString path)
{
    path_database = path;
}

void Foursquare::ReadUsersPath()
{
   list_user_path.clear();
   table_placeID.clear();
   //read file
   QTextStream input(path_database, QIODevice::ReadOnly);
   
}
