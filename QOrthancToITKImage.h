#ifndef __Q_ORTHANC_TO_ITK_IMAGE_H__
#define __Q_ORTHANC_TO_ITK_IMAGE_H__
// me 
class QOrthancToITKImagePrivate;
// Qt
#include <QObject>
#include <QDir>
class QFile;
#include <QHash>
#include <QURL>
class QOrthancToITKImage final: public QObject
{
    Q_OBJECT;
public:
    explicit QOrthancToITKImage(QObject *parent = nullptr);
    void setUrls(QStringList urls);
    void setUrls(QString url);
    void request();
    void waitForResponded();
    void abort();
Q_SIGNALS:
    void responded();
private:
    QOrthancToITKImagePrivate *d_ptr;
    Q_DECLARE_PRIVATE(QOrthancToITKImage);
    Q_DISABLE_COPY(QOrthancToITKImage);
}; 
#endif // !__Q_ORTHANC_TO_ITK_IMAGE_H__