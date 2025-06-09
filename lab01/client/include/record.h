#ifndef RECORD_H
#define RECORD_H

#include <QString>

struct Record {
    QString id;
    QString badge;
    QString lastName;
    QString firstName;
    QString experience;
    QString carrierId;

    bool operator!=(const Record &other) const {
        return  id         != other.id ||
                badge      != other.badge ||
                lastName   != other.lastName ||
                firstName  != other.firstName ||
                experience != other.experience ||
                carrierId  != other.carrierId;
    }
};

#endif // RECORD_H
