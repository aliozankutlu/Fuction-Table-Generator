#ifndef FILECONTROL_H
#define FILECONTROL_H

#include <QWidget>
#include <QFile>

enum fileMode
{
    initialize   = 0,
    opened       = 1,
    notOpened    = 2,
    closed       = 3,
    notWritable  = 4,
    notReadable  = 5
};

class fileControl : public QWidget
{
    Q_OBJECT
public:
    explicit fileControl(QWidget *parent = 0);
    ~fileControl();
    quint8 close();
    quint8 init(QString logFileName, bool reInit=false);
signals:

private:
      QFile *file;
      quint8 fileState;
public slots:
      quint8 writeFileLine(QString data);
      quint8 readFileLine(QString *data, bool lineFeed=true);
      bool atEnd();
      void reset();
};

#endif // FILECONTROL_H
