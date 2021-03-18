#pragma once

#include <QString>
#include <vector>

#include <QtNetwork/QtNetwork>
#include <minecraft/auth/AuthSession.h>
#include "tasks/Task.h"
#include "multimc_logic_export.h"
#include <QObjectPtr.h>

typedef unique_qobject_ptr<class GetProfile> GetProfilePtr;

class MULTIMC_LOGIC_EXPORT GetProfile : public Task
{
    Q_OBJECT
public:
    GetProfile(QObject *parent, const QString & m_auth_token);
    virtual ~GetProfile();

private:
    QString m_auth_token;
    unique_qobject_ptr<QNetworkReply> m_reply;
protected:
    virtual void executeTask();

public slots:
    void downloadFinished(QNetworkReply * reply);
    void updateDownloadProgress(qint64 now,qint64 total);
};
