#include "GetProfile.h"
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <Env.h>

#include "Json.h"

namespace {
struct Skin {
    QString id;
    QString state;
    QString url;
    QString variant;
};

struct Cape {
    QString id;
    QString state;
    QString url;
    QString alias;
};

struct MinecraftProfile {
    MinecraftProfile fromJson(const QByteArray & jsonData) {
        MinecraftProfile result;
        QJsonParseError jsonError;
        QByteArray replyData = jsonData;
        QJsonDocument doc = QJsonDocument::fromJson(replyData, &jsonError);
        if(jsonError.error != QJsonParseError::NoError) {
            return result;
        }
        result.valid = true;
        return result;
    }
    QString id;
    QString name;
    std::vector<Skin> skins;
    std::vector<Cape> capes;
    bool valid = false;
};
}

GetProfile::GetProfile(QObject *parent, const QString &auth_token)
    : Task(parent), m_auth_token(auth_token)
{
}

void GetProfile::executeTask()
{
    auto &qnam = ENV.qnam();
    QNetworkRequest request(QUrl("https://api.minecraftservices.com/minecraft/profile"));
    request.setRawHeader("Authorization", QString("Bearer %1").arg(m_auth_token).toLocal8Bit());

    connect(&qnam, &QNetworkAccessManager::finished, this, &GetProfile::downloadFinished);

    auto * reply = qnam.get(request);
    m_reply.reset(reply);

    setStatus(tr("Getting Minecraft Profile"));
    qDebug() << "GetProfile: " << request.url();

    connect(m_reply.get(), SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgress(qint64,qint64)));
}

void GetProfile::downloadFinished(QNetworkReply * reply)
{
    if(reply != m_reply.get()) {
        return;
    }
    // if the download failed
    if (reply->error() != QNetworkReply::NetworkError::NoError)
    {
        auto headerPairs = reply->rawHeaderPairs();
        if(headerPairs.size()) {
            qDebug() << "Response headers:";
            for (auto & pair: headerPairs) {
                qDebug() << pair.first << " : " << pair.second;
            }
        }

        auto response = QString::fromUtf8(reply->readAll());
        qDebug() << "Response: " << response;
        emitFailed(QString("Network error: %1").arg(reply->errorString()));
        m_reply.reset();
        return;
    }
    emitSucceeded();
}


void GetProfile::updateDownloadProgress(qint64 now,qint64 total) {
    qDebug() << "GetProfile: " << now << " / " << total;
}

GetProfile::~GetProfile(){
    qDebug() << "Destroying GetProfile";
}
