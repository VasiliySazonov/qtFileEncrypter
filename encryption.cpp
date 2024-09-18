#include "encryption.h"

QByteArray Encryption::encrypt(const QByteArray &rawData, const QString &password)
{
    QByteArray keyDataByteArray = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    uint64_t keyData[] = {0x0L, 0x0L, 0x0L, 0x0L};
    uint64_t keyTweak[] = {0x0L, 0x0L};

    memcpy(keyData, keyDataByteArray.data(), TREEFISH_BLOCK_SIZE);

    threefish_key_t key;
    threefish_set_key(&key, keyData, keyTweak);

    uint64_t originalFileSize = rawData.size();
    uint64_t originalFileSizeBigEndian = qToBigEndian<quint64>(originalFileSize);

    QByteArray result;
    result.push_back(QByteArray((char*)&originalFileSizeBigEndian, sizeof(uint64_t)));

    for (int i = 0; i < (int)(originalFileSize / TREEFISH_BLOCK_SIZE) + 1; i++)
    {
        QByteArray rawBlock(TREEFISH_BLOCK_SIZE, 0x0);
        QByteArray encryptedBlock(TREEFISH_BLOCK_SIZE, 0x0);

        for (int j = 0; j < (int)TREEFISH_BLOCK_SIZE; j++)
        {
            if (TREEFISH_BLOCK_SIZE * i + j >= originalFileSize)
                break;
            rawBlock[j] = rawData[(int)(TREEFISH_BLOCK_SIZE * i + j)];
        }

        threefish_encrypt(&key, (uint64_t*)rawBlock.data(), (uint64_t*)encryptedBlock.data());

        uint64_t *encryptedU64 = (uint64_t*)encryptedBlock.data();
        for (int j = 0; j < KEY_SIZE_WORDS; j++)
            encryptedU64[j] = qToBigEndian<quint64>(encryptedU64[j]);

        result.push_back(encryptedBlock);
    }

    return result;
}

QByteArray Encryption::decrypt(const QByteArray &encryptedData, const QString &password)
{
    /*
     * TODO: Check if the file is valid
     */

    QByteArray keyDataByteArray = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    uint64_t keyData[] = {0x0L, 0x0L, 0x0L, 0x0L};
    uint64_t keyTweak[] = {0x0L, 0x0L};

    memcpy(keyData, keyDataByteArray.data(), TREEFISH_BLOCK_SIZE);

    threefish_key_t key;
    threefish_set_key(&key, keyData, keyTweak);

    uint64_t originalFileSize = qFromBigEndian<quint64>(((uint64_t*)encryptedData.data())[0]);

    QByteArray result;

    for (int i = 0; i < (int)(originalFileSize / TREEFISH_BLOCK_SIZE) + 1; i++)
    {
        QByteArray encryptedBlock(TREEFISH_BLOCK_SIZE, 0x0);
        QByteArray decryptedBlock(TREEFISH_BLOCK_SIZE, 0x0);

        for (int j = 0; j < (int)TREEFISH_BLOCK_SIZE; j++)
            encryptedBlock[j] = encryptedData[(int)(i * TREEFISH_BLOCK_SIZE + j + sizeof(uint64_t))];

        uint64_t *encryptedU64 = (uint64_t*)encryptedBlock.data();
        for (int j = 0; j < KEY_SIZE_WORDS; j++)
            encryptedU64[j] = qFromBigEndian<quint64>(encryptedU64[j]);

        threefish_decrypt(&key, (uint64_t*)encryptedBlock.data(), (uint64_t*)decryptedBlock.data());

        result.push_back(decryptedBlock);
    }

    result.remove(originalFileSize, result.size() - originalFileSize);

    return result;
}
