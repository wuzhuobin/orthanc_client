// me
#include "QOrthancToITKImage.h"
// qt
#include <QDebug>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QDir>
#include <QHash>
#include <QURL>
class QOrthancToITKImagePrivate final: public QObject
{
    Q_OBJECT;
private:
    explicit QOrthancToITKImagePrivate(QOrthancToITKImage *q) :
        QObject(q),
        q_ptr(q)
    {
        // connect(this, &QOrthancToITKImagePrivate::httpFinished, 
        //     q_ptr, &QOrthancToITKImage::responded);
    }

    void startRequest(const QUrl &url)
    {
        QNetworkRequest req(url);
        QNetworkReply *rep = this->networkAccessManager.get(req);
        connect(rep, &QNetworkReply::finished, this,
            &QOrthancToITKImagePrivate::httpFinished);
        connect(rep, &QIODevice::readyRead, this, 
            &QOrthancToITKImagePrivate::httpReadyRead);
        connect(this, &QOrthancToITKImagePrivate::internalAbort, 
            rep, &QNetworkReply::abort);
    }

    void openFileForWrite(const QUrl &url)
    {
        QRegularExpression regex("instances/(?<id>[a-z0-9]{8}-[a-z0-9]{8}-[a-z0-9]{8}-[a-z0-9]{8}-[a-z0-9]{8})/file");
        QRegularExpressionMatch match = regex.match(url.toString());
        if (!match.hasMatch())
        {
            qCritical() << "The url is not correct";
            return;
        }
        QString fileName = match.captured("id") + ".dcm";
        QFile *file = new QFile(this->dir.path() + "/" + fileName);
        if (!file->open(QIODevice::WriteOnly))
        {
            qCritical() << "File: " << fileName << "open failed. ";
            return;
        }
        files.insert(url, file);
    }
Q_SIGNALS: 
    void internalAbort();
private:
    Q_DECLARE_PUBLIC(QOrthancToITKImage);
    QOrthancToITKImage *q_ptr;
    QStringList urls;
    QStringList fileNames;
    QDir dir;
    QHash<QUrl, QFile*> files; 
    QNetworkAccessManager networkAccessManager; 

private Q_SLOTS: 
    void httpFinished()
    {
        Q_Q(QOrthancToITKImage);
        QNetworkReply *rep = reinterpret_cast<QNetworkReply *>(this->sender());
        if (rep->error())
        {
            qCritical() << rep->error();
        }
        QUrl url = rep->url();
        QFile *file = this->files.take(url);
        this->fileNames.append(file->fileName());
        if (file != nullptr)
        {
            file->close();
            delete file;
        }
        rep->deleteLater();
        if(this->files.isEmpty()){
            emit q->responded(this->fileNames);
        }
    }

    void httpReadyRead()
    {
        // this slot gets called every time the QNetworkReply has new data.
        // We read all of its new data and write it into the file.
        // That way we use less RAM than when reading it at the finished()
        // signal of the QNetworkReply
        QNetworkReply *rep = reinterpret_cast<QNetworkReply *>(this->sender());
        QUrl url = rep->url();
        QFile *file = this->files.value(url);
        if (file == nullptr || !file->isOpen())
        {
            qCritical() << "The url " << url << "cannot write to file " << file->fileName();
            return;
        }
        file->write(rep->readAll());
    }
};
#include "QOrthancToITKImage.moc"

QOrthancToITKImage::QOrthancToITKImage(QObject *parent)
    :QObject(parent), 
    d_ptr(new QOrthancToITKImagePrivate(this))
{
}

void QOrthancToITKImage::setUrls(QStringList urls)
{
    Q_D(QOrthancToITKImage);
    d->urls = urls;
}

void QOrthancToITKImage::setUrls(QString url)
{
    Q_D(QOrthancToITKImage);
    this->setUrls(QStringList() << url);
}

void QOrthancToITKImage::request()
{
    Q_D(QOrthancToITKImage);
    d->fileNames.clear();
    d->files.clear();
    d->dir = QDir::current();
    QString folderName = QDateTime::currentDateTime().toString("hhmmssdddMMMyyyy");
    if(!d->dir.mkpath("temp/" + folderName)){
        qCritical() << "Create path failed. ";
        return;
    }
    d->dir.cd("temp/" + folderName);
    for(QStringList::const_iterator it = d->urls.cbegin(); it != d->urls.cend(); ++it){
        QUrl url = QUrl::fromUserInput(it->trimmed());
        if(!url.isValid()){
            continue;
        }
        d->openFileForWrite(url);
        d->startRequest(url);
    }
}

void QOrthancToITKImage::abort()
{
    Q_D(QOrthancToITKImage);
    emit d->internalAbort();
}

void QOrthancToITKImage::waitForResponded()
{
    bool finished = false;
    while(finished){
        this->responded(finished);
    }
}

void QOrthancToITKImage::responded(bool &finished)
{
    Q_D(QOrthancToITKImage);
    finished = d->files.isEmpty();
}