#ifndef CODEGENERATE_H
#define CODEGENERATE_H

#include <QWidget>
#include <QDir>
#include <QDate>
#include "filecontrol.h"
#include "functionpointerdialog.h"
#include "settingdialog.h"
#include "structuredialog.h"
#include "elementdialog.h"

class codeGenerate : public QWidget
{
    Q_OBJECT
public:
    explicit codeGenerate(QWidget *parent = 0);
    ~codeGenerate();
    QString getCodeGeneratedDirectory(void);
signals:


public slots:
    void filesGenerate(fileParams *params);
    void generateFunctionPointerPrototype(fucPointerArr *data);
    void generateStructurePrototype(structureArr *data);
    void generateElementPrototype(elementArray *data);

private:
     fileControl *sourceFile;
     fileControl *headerFile;
     QString path;
     QString headerFileName;
     fucPointerArr *fucPointer;
     structureArr *structure;
     void generateSourceFileTemplate(QString fileName, QString author, QString version);
     void generateHeaderFileTemplate(QString fileName, QString author, QString version);
     void generateHeaderDefineStart(QString fileName);
     void generateHeaderDefineStop(QString fileName);
     void generateIncludeHeaderFile(QString includeHeaderFile);
     void addDefinition(fileControl *file, QString data);
     void addSetItemDefinition(fileControl *file, QString data);
     void addGetItemDefinition(fileControl *file, QString data, QString retType);
     void addFunctionDefinition(fileControl *file, QString funcName, fucPointerArr *data, bool funcIndexAdd=false);
     void generateEnumPrototype(elementArray *data);
     void generateStateControlVariablePrototype(elementArray *data);
     void generateFunctionPrototype(elementArray *data);
     void generateCallFuctionPrototype(elementArray *data);
     void generateControlFuctionPrototype(elementArray *data);
     void generateStructureArrayPrototype(elementArray *data);
     QString readSourceFile();
     QString readHeaderFile();
     void generateSetFunctionIndexPrototype(elementArray *data);
     void addSetFunctionIndexDefinition(fileControl *file, QString data);
     void generateSetStructureArgumentFunctionPrototype(elementArray *data);
     void generateGetStructureArgumentFunctionPrototype(elementArray *data);
signals:
    void codeGenerated(QString sourceData, QString headerData);
};

#endif // CODEGENERATE_H
