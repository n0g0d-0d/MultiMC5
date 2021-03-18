#pragma once

#include <QFile>
#include <QtNetwork/QtNetwork>
#include <memory>
#include <minecraft/auth/AuthSession.h>
#include "tasks/Task.h"
#include "multimc_logic_export.h"

typedef std::shared_ptr<class SkinDelete> SkinDeletePtr;

class MULTIMC_LOGIC_EXPORT SkinDelete : public Task
{
    Q_OBJECT
public:
    SkinDelete(QObject *parent, const QString & m_access_token);
    virtual ~SkinDelete() = default;

private:
    QString m_access_token;
    std::shared_ptr<QNetworkReply> m_reply;

protected:
    virtual void executeTask();

public slots:
    void downloadError(QNetworkReply::NetworkError);
    void downloadFinished();
};

