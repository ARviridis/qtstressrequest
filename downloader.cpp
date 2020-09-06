#include "Downloader.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

Downloader::Downloader(QObject* parent) :
    BaseClass(parent)
{
    connect(&a_manager, &QNetworkAccessManager::finished, this, &Downloader::onReply);
    //"тянем" сигнал
}
bool Downloader::get(const QString& targetFolder, const QUrl& url)
{
    if (targetFolder.isEmpty() || url.isEmpty())
    {
        return false;
    }
    m_file = new QFile(targetFolder + QDir::separator() + url.fileName());
    if (!m_file->open(QIODevice::WriteOnly)) // проверка
    {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    // Создаём запрос
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);//для пересылки
    m_currentReply = a_manager.get(request);//скачивание через гет запрос

    connect(m_currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &Downloader::updateDownloadProgress);
    connect(m_currentReply, &QNetworkReply::finished,this, &Downloader::sonDownloadButtonClicked);//
    return true;
}
void Downloader::onReadyRead()
{
    if (m_file)
    {
        m_file->write(m_currentReply->readAll());
    }
}
void Downloader::cancelDownload()
{
    if (m_currentReply)
    {
        m_currentReply->abort();
    }
}
void Downloader::onReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        m_file->flush();
        m_file->close();
        m_file->remove();
    }
    else
    {
        m_file->remove();
    }
    delete m_file;
    m_file = nullptr;
    reply->deleteLater();
}
