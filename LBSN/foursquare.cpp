#include "foursquare.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QDateTime>

Foursquare::Foursquare()
{
}

void Foursquare::SetPathOfDatabase(QString path)
{
    path_database = path;
    ReadUsersPath();
}

void Foursquare::ReadUsersPath()
{
    list_user_path.clear();
    table_placeID.clear();
    //read file
    QFile input_file(path_database);
    input_file.open(QFile::ReadOnly);
    QTextStream input(&input_file);
    QString line = input.readLine();
    quint32 count_placeID = 0;
    quint32 count = 0;
    quint32 count_invalid_placeID = 0;
    while(!line.isNull())
    {
        QStringList list = line.split("\t");
        size_t user_id = list[0].toULong();
        //get created time
        quint64 created_time = QDateTime::fromString(list[4], "yyyy-MM-dd hh:mm:ss")
                .toMSecsSinceEpoch();
        //get place id
        bool ok;
        quint32 place_id;
        quint64 raw_placeID = list[6].toULongLong(&ok, 16);
        if(!ok)
        {
            count_invalid_placeID++;
            line = input.readLine();
            continue;
        }
        auto iter_id = table_placeID.find(raw_placeID);
        if(iter_id == table_placeID.end())
        {
            place_id = count_placeID++;
            table_placeID[raw_placeID] = place_id;
        }
        else
        {
            place_id = iter_id->second;
        }
        //save
        list_user_path[user_id].path.emplace_back(place_id, created_time);
        //next line
        line = input.readLine();
        if(count % 10000 == 0)
        {
            qDebug()<<"Read Line "<<count<<"...";
        }
        count++;
    }
    input_file.close();
    qDebug()<<"Number of valid check-in is "<<count;
    qDebug()<<"Number of invalid check-in is "<<count_invalid_placeID;
}
