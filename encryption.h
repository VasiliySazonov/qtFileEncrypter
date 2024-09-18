#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <treefish.h>

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QtEndian>
#include <QCryptographicHash>

#define TREEFISH_BLOCK_SIZE (sizeof(uint64_t) * 4)

class Encryption
{
public:
    static QByteArray encrypt(const QByteArray &rawData, const QString &password);
    static QByteArray decrypt(const QByteArray &encryptedData, const QString &password);
};

#endif // ENCRYPTION_H
