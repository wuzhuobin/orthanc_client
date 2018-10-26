// me
#include "QOrthancToITKImage.h"
// qt
#include <QDebug>
#include <QDateTime>
class QOrthancToITKImagePrivate final: public QObject
{
    Q_OBJECT;
public:
    explicit QOrthancToITKImagePrivate(QOrthancToITKImage *q) :
        QObject(q),
        q_ptr(q)
    {

    }
    Q_DECLARE_PUBLIC(QOrthancToITKImage);
    QOrthancToITKImage *q_ptr;
    QStringList urls;
    QDir dir;
    QHash<QUrl, QFile*> files; 
    void startRequest(QUrl url);
    void openFileForWrite(QUrl url);
private Q_SLOTS: 
    void httpFinished();
    void httpReadyRead();
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

void QOrthancToITKImagePrivate::startRequest(QUrl url)
{

}

void QOrthancToITKImagePrivate::openFileForWrite(QUrl url)
{

}

void QOrthancToITKImagePrivate::httpReadyRead()
{

}

void QOrthancToITKImagePrivate::httpFinished()
{

}
