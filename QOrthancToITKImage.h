#ifndef __Q_ORTHANC_TO_ITK_IMAGE_H__
#define __Q_ORTHANC_TO_ITK_IMAGE_H__
// me 
class QOrthancToITKImagePrivate;
// Qt
#include <QObject>
class QOrthancToITKImage final: public QObject
{
    Q_OBJECT;
public:
    explicit QOrthancToITKImage(QObject *parent = nullptr);
    void setUrls(QStringList urls);
    void setUrls(QString url);
    void request();
    void abort();
    void waitForResponded();
    void responded(bool &finished);
Q_SIGNALS:
    void responded(QStringList fileNames);
private:
    QOrthancToITKImagePrivate *d_ptr;
    Q_DECLARE_PRIVATE(QOrthancToITKImage);
    Q_DISABLE_COPY(QOrthancToITKImage);
}; 
#endif // !__Q_ORTHANC_TO_ITK_IMAGE_H__