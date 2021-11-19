#include "filecontrol.h"

fileControl::fileControl(QWidget *parent) : QWidget(parent)
{
    file = new QFile();
    fileState=initialize;
}

fileControl::~fileControl()
{
    close();
}

quint8 fileControl::init(QString logFileName, bool reInit)
{
    if(fileState==initialize)
    {
        file->setFileName(logFileName);
        if(!file->open(QFile::ReadWrite|QFile::Text |QFile::Append))
        {
            fileState=notOpened;
        }
        else
        {
            fileState=opened;
            if(reInit)
            {
                if(file->exists())
                {
                    file->resize(0);
                }
            }
        }
    }
    return fileState;
}

quint8 fileControl::close()
{
    file->flush();
    file->close();
    fileState = closed;
    return fileState;
}

quint8 fileControl::writeFileLine(QString data)
{
    if(fileState==opened)
    {
        if(file->isWritable())
        {
            file->write(QString(data + "\n").toUtf8());
            file->flush();
        }
        else
        {
              fileState=notWritable;
        }
    }
    return fileState;
}

quint8 fileControl::readFileLine(QString *data, bool lineFeed)
{
    if(fileState==opened)
    {
        if(file->isReadable())
        {
            if(lineFeed)
            {
                *data= QString(file->readLine()).replace("\n","");
            }
            else
            {
                *data= QString(file->readLine());
            }
            file->flush();
        }
        else
        {
             fileState=notReadable;
        }
    }
    return fileState;
}

bool fileControl::atEnd()
{
    return file->atEnd();
}

void fileControl::reset()
{
    file->reset();
}
