#include "codegenerate.h"

codeGenerate::codeGenerate(QWidget *parent) : QWidget(parent)
{
    sourceFile = new fileControl();
    headerFile = new fileControl();
}

codeGenerate::~codeGenerate()
{
    delete sourceFile;
    delete headerFile;
}

void codeGenerate::filesGenerate(fileParams *params)
{

    headerFileName = params->HeaderFileName;
    QDir dir;
    path = params->filePath+"/"+params->fileName+"/";
    dir.mkpath(path);
    sourceFile->init(path+params->sourceFileName,true);
    headerFile->init(path+params->HeaderFileName,true);

    generateSourceFileTemplate(params->sourceFileName,params->author,params->version);
    generateHeaderFileTemplate(params->HeaderFileName,params->author,params->version);
    generateIncludeHeaderFile(params->includeHeaderFile);

}

QString codeGenerate::getCodeGeneratedDirectory(void)
{
    return path;
}

void codeGenerate::generateSourceFileTemplate(QString fileName, QString author, QString version)
{
    sourceFile->writeFileLine("/**");
    sourceFile->writeFileLine(" *@file " + fileName);
    sourceFile->writeFileLine(" *@date " + QDate::currentDate().toString("dd.MM.yyyy"));
    sourceFile->writeFileLine(" *@author " + author);
    if(version != "")
    {
        sourceFile->writeFileLine(" *@version " + version);
    }
    sourceFile->writeFileLine(" *@brief ");
    sourceFile->writeFileLine(" */");
}

void codeGenerate::generateHeaderFileTemplate(QString fileName, QString author, QString version)
{
    headerFile->writeFileLine("/**");
    headerFile->writeFileLine(" *@file " + fileName);
    headerFile->writeFileLine(" *@date " + QDate::currentDate().toString("dd.MM.yyyy"));
    headerFile->writeFileLine(" *@author " + author);
    if(version != "")
    {
        headerFile->writeFileLine(" *@version " + version);
    }
    headerFile->writeFileLine(" *@brief ");
    headerFile->writeFileLine(" */");
    generateHeaderDefineStart(fileName);
}

void codeGenerate::generateHeaderDefineStart(QString fileName)
{
    QString name=fileName.replace(".h","");
    name = name.toUpper();
    headerFile->writeFileLine("\n#ifndef " + name +"_H_");
    headerFile->writeFileLine("#define " + name +"_H_");
}

void codeGenerate::generateHeaderDefineStop(QString fileName)
{
    QString name=fileName.replace(".h","");
    name = name.toUpper();
    headerFile->writeFileLine("#endif /* " + name +" */");
}

void codeGenerate::generateIncludeHeaderFile(QString includeHeaderFile)
{
    if(includeHeaderFile != "")
    {
        sourceFile->writeFileLine("\n#include \"" + includeHeaderFile + ".h\"");
    }
}

void codeGenerate::addDefinition(fileControl *file, QString data)
{
    file->writeFileLine("/**");
    file->writeFileLine(" * " + data);
    file->writeFileLine(" *@brief ");
    file->writeFileLine(" */");
}

void codeGenerate::addSetFunctionIndexDefinition(fileControl *file, QString data)
{
    file->writeFileLine("/**");
    file->writeFileLine(" * " + data);
    file->writeFileLine(" *@param funcIndex");
    file->writeFileLine(" *@return void");
    file->writeFileLine(" *@brief ");
    file->writeFileLine(" */");
}

void codeGenerate::addSetItemDefinition(fileControl *file, QString data)
{
    file->writeFileLine("/**");
    file->writeFileLine(" * " + data);
    file->writeFileLine(" *@param argIndex");
    file->writeFileLine(" *@param argData");
    file->writeFileLine(" *@return void");
    file->writeFileLine(" *@brief ");
    file->writeFileLine(" */");
}

void codeGenerate::addGetItemDefinition(fileControl *file, QString data, QString retType)
{
    file->writeFileLine("/**");
    file->writeFileLine(" * " + data);
    file->writeFileLine(" *@param argIndex");
    file->writeFileLine(" *@return " + retType);
    file->writeFileLine(" *@brief ");
    file->writeFileLine(" */");
}

void codeGenerate::addFunctionDefinition(fileControl *file, QString funcName, fucPointerArr *data, bool funcIndexAdd)
{
    file->writeFileLine("/**");
    file->writeFileLine(" * " + funcName);
    if(funcIndexAdd)
    {
        file->writeFileLine(" *@param funcIndex");
    }
    for(int i=0; i<data->argSize;i++)
    {
        file->writeFileLine(" *@param " + data->argNameArr[i]);
    }
    file->writeFileLine(" *@return " + data->returnType);
    file->writeFileLine(" *@brief ");
    file->writeFileLine(" */");
}

void codeGenerate::generateFunctionPointerPrototype(fucPointerArr *data)
{
    fucPointer = data;
    addDefinition(headerFile,data->name);
    if(data->argSize == 0)
    {
        headerFile->writeFileLine("typedef " + data->returnType + " (*" + data->name + ")(void);");
    }
    else
    {
        QString headerData="";
        headerData+="typedef " + data->returnType + " (*" + data->name + ")(";
        for(int i=0;i<data->argSize-1;i++)
        {
            headerData+=data->argTypeArr[i] + " " + data->argNameArr[i]+ ",";
        }
        headerData+=data->argTypeArr[data->argSize-1] + " " + data->argNameArr[data->argSize-1] + ");\n";
        headerFile->writeFileLine(headerData);
    }
}

void codeGenerate::generateStructurePrototype(structureArr *data)
{
    structure = data;
    if(data->argSize != 0)
    {
        addDefinition(headerFile,data->name);
        headerFile->writeFileLine("typedef struct");
        headerFile->writeFileLine("{");
        for(int i=0; i<data->argSize;i++)
        {
            headerFile->writeFileLine("	" + data->argTypeArr[i]+ " " +  data->argNameArr[i]+";");
        }
        headerFile->writeFileLine("} " +data->name + ";");
    }
}

void codeGenerate::generateEnumPrototype(elementArray *data)
{
    if(data->argSize != 0)
    {
        addDefinition(headerFile,data->name+"State");
        headerFile->writeFileLine("typedef enum");
        headerFile->writeFileLine("{");
        for(int i=0; i<data->argSize;i++)
        {
            headerFile->writeFileLine("	" +  data->argNameArr[i]+"State = "+ QString::number(i) + ",");
        }
        headerFile->writeFileLine("} " +data->name+"State" + ";");
    }
}

void codeGenerate::generateStateControlVariablePrototype(elementArray *data)
{
    addDefinition(sourceFile,data->name +"Control");
    sourceFile->writeFileLine("static " + fucPointer->indexType + " " + data->name +"Control = " + data->argNameArr[0]+"State;");
}

void codeGenerate::generateFunctionPrototype(elementArray *data)
{
    for(int i=0;i<data->argSize;i++)
    {
        addFunctionDefinition(sourceFile,data->argNameArr[i],fucPointer);
        addFunctionDefinition(headerFile,data->argNameArr[i],fucPointer);
        if(fucPointer->argSize == 0)
        {
            sourceFile->writeFileLine(fucPointer->returnType + " " + data->argNameArr[i] + "(void)");
            headerFile->writeFileLine(fucPointer->returnType + " " + data->argNameArr[i] + "(void);");
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("");
            sourceFile->writeFileLine("}\n");
        }
        else
        {
            QString sourceData="";
            QString headerData="";
            sourceData+=fucPointer->returnType + " " + data->argNameArr[i] + "(";
            headerData+=fucPointer->returnType + " " + data->argNameArr[i] + "(";
            for(int i=0;i<fucPointer->argSize-1;i++)
            {
                sourceData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
                headerData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
            }
            sourceData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ")";
            headerData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ");";
            sourceFile->writeFileLine(sourceData);
            headerFile->writeFileLine(headerData);
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("");
            sourceFile->writeFileLine("}\n");
        }
    }
}


void codeGenerate::generateCallFuctionPrototype(elementArray *data)
{
    addFunctionDefinition(sourceFile,"call"+data->name+"Function",fucPointer,true);
    addFunctionDefinition(headerFile,"call"+data->name+"Function",fucPointer,true);
    if(fucPointer->argSize == 0)
    {

        sourceFile->writeFileLine("void call" + fucPointer->name + data->name +  "Function(" + fucPointer->indexType + " funcIndex)");
        if(data->typeSelection == 0)
        {
            headerFile->writeFileLine("void call" + data->name + "Function(" + fucPointer->indexType + " funcIndex);");
        }
        else
        {
            headerFile->writeFileLine("extern void call" + data->name + "Function(" + fucPointer->indexType + " funcIndex);");
        }
        sourceFile->writeFileLine("{");
        sourceFile->writeFileLine("    " + data->name + "[funcIndex]." + structure->argNameArr[0] + "();");
        sourceFile->writeFileLine("}\n");
    }
    else
    {
        QString sourceData="";
        QString headerData="";
        sourceData+="void call" + data->name + "Function(" + fucPointer->indexType + " funcIndex,";
        if(data->typeSelection == 0)
        {
            headerData+="void call" + data->name +  "Function(" + fucPointer->indexType + " funcIndex,";
        }
        else
        {
            headerData+="extern void call" + data->name +  "Function(" + fucPointer->indexType + " funcIndex,";
        }
        for(int i=0;i<fucPointer->argSize-1;i++)
        {
            sourceData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
            headerData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
        }
        sourceData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ")";
        headerData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ");";
        sourceFile->writeFileLine(sourceData);
        headerFile->writeFileLine(headerData);
        sourceFile->writeFileLine("{");
        sourceData="";
        sourceData+="    " + data->name + "[funcIndex]." + structure->argNameArr[0] + "(";
        for(int i=0;i<fucPointer->argSize-1;i++)
        {
            sourceData+=" " + fucPointer->argNameArr[i] + ",";
        }
        sourceData+=" " + fucPointer->argNameArr[fucPointer->argSize-1] + ");";
        sourceFile->writeFileLine(sourceData);
        sourceFile->writeFileLine("}\n");
    }
}

void codeGenerate::generateControlFuctionPrototype(elementArray *data)
{
    addFunctionDefinition(sourceFile,"control"+data->name+"Function",fucPointer);
    addFunctionDefinition(headerFile,"control"+data->name+"Function",fucPointer);
    if(fucPointer->argSize == 0)
    {
        sourceFile->writeFileLine("void control" + fucPointer->name + data->name +  "Function(void)");
        headerFile->writeFileLine("extern void control" + data->name + "Function(void)");
        sourceFile->writeFileLine("{");
        sourceFile->writeFileLine("    " + data->name + "["+ data->name +"Control]." + structure->argNameArr[0] + "();");
        sourceFile->writeFileLine("}\n");
    }
    else
    {
        QString sourceData="";
        QString headerData="";
        sourceData+="void control" + data->name + + "Function(";
        headerData+="extern void control" + data->name + "Function(";
        for(int i=0;i<fucPointer->argSize-1;i++)
        {
            sourceData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
            headerData+=fucPointer->argTypeArr[i] + " " + fucPointer->argNameArr[i] + ",";
        }
        sourceData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ")";
        headerData+=fucPointer->argTypeArr[fucPointer->argSize-1] + " " + fucPointer->argNameArr[fucPointer->argSize-1] + ");";
        sourceFile->writeFileLine(sourceData);
        headerFile->writeFileLine(headerData);
        sourceFile->writeFileLine("{");
        sourceData="";
        sourceData+="    " + data->name + "["+ data->name +"Control]." + structure->argNameArr[0] + "(";
        for(int i=0;i<fucPointer->argSize-1;i++)
        {
            sourceData+=" " + fucPointer->argNameArr[i] + ",";
        }
        sourceData+=" " + fucPointer->argNameArr[fucPointer->argSize-1] + ");";
        sourceFile->writeFileLine(sourceData);
        sourceFile->writeFileLine("}\n");
    }
}

void codeGenerate::generateStructureArrayPrototype(elementArray *data)
{
    addDefinition(headerFile,"nof" + data->name);

    if(data->fncPtrName == "")
    {
        headerFile->writeFileLine("#define nof" + data->name +" " + QString::number(data->argSize));
        addDefinition(sourceFile,data->name);
        sourceFile->writeFileLine("static " + structure->name + " " + data->name + "[nof" + data->name +"] =");

        sourceFile->writeFileLine("{");
        for(int i=0;i<data->argSize;i++)
        {
            sourceFile->writeFileLine("     {");
            for(int j=0;j<structure->argSize;j++)
            {
                sourceFile->writeFileLine("       ." + structure->argNameArr[j]+" = 0,");
            }
            sourceFile->writeFileLine("     },");
        }
        sourceFile->writeFileLine("};");
    }
    else
    {
        headerFile->writeFileLine("#define nof" + data->name +" " + QString::number(data->argSize));
        addDefinition(sourceFile,data->name);
        sourceFile->writeFileLine("static " + structure->name + " " + data->name + "[nof" + data->name +"] =");
        sourceFile->writeFileLine("{");
        for(int i=0;i<data->argSize;i++)
        {
            sourceFile->writeFileLine("     {");
            sourceFile->writeFileLine("       ." + structure->argNameArr[0]+" = " + data->argNameArr[i] + ",");
            for(int j=1;j<structure->argSize;j++)
            {
                sourceFile->writeFileLine("       ." + structure->argNameArr[j]+" = 0,");
            }
            sourceFile->writeFileLine("     },");
        }
        sourceFile->writeFileLine("};\n");
    }
}

void codeGenerate::generateSetFunctionIndexPrototype(elementArray *data)
{
    addSetFunctionIndexDefinition(sourceFile,"set" + data->name + "FunctionIndex");
    addSetFunctionIndexDefinition(headerFile,"set" + data->name + "FunctionIndex");

    sourceFile->writeFileLine("void set" +  data->name + "FunctionIndex(" +fucPointer->indexType + " funcIndex)");
    headerFile->writeFileLine("extern void set" +  data->name + "FunctionIndex(" + fucPointer->indexType + " funcIndex);");
    sourceFile->writeFileLine("{");
    sourceFile->writeFileLine("    " + data->name +"Control = funcIndex;");
    sourceFile->writeFileLine("}\n");
}

void codeGenerate::generateSetStructureArgumentFunctionPrototype(elementArray *data)
{

    if(data->fncPtrName == "")
    {
        for(int i=0;i<structure->argSize;i++)
        {
            addSetItemDefinition(sourceFile,"set" + structure->argNameArr[i] + "ArgumentValue");
            sourceFile->writeFileLine("void set" +  structure->argNameArr[i] + "ArgumentValue(int argIndex," + structure->argTypeArr[i] + " argData)");
            addSetItemDefinition(headerFile,"set" + structure->argNameArr[i] + "ArgumentValue");
            if(data->typeSelection == 0)
            {
                headerFile->writeFileLine("void set" +  structure->argNameArr[i] + "ArgumentValue(int argIndex," + structure->argTypeArr[i] + " argData);");
            }
            else
            {
                headerFile->writeFileLine("extern void set" +  structure->argNameArr[i] + "ArgumentValue(int argIndex," + structure->argTypeArr[i] + " argData);");
            }
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("    " + data->name + "[argIndex]." + structure->argNameArr[i] + " = argData");
            sourceFile->writeFileLine("}\n");
        }
    }
    else
    {
        for(int i=1;i<structure->argSize;i++)
        {
            addSetItemDefinition(sourceFile,"set" + structure->argNameArr[i] + "ArgumentValue");
            sourceFile->writeFileLine("void set" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex," + structure->argTypeArr[i] + " argData)");
            addSetItemDefinition(headerFile,"set" + structure->argNameArr[i] + "ArgumentValue");
            if(data->typeSelection == 0)
            {
                headerFile->writeFileLine("void set" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex," + structure->argTypeArr[i] + " argData);");
            }
            else
            {
                headerFile->writeFileLine("extern void set" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex," + structure->argTypeArr[i] + " argData);");
            }
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("    " + data->name + "[argIndex]." + structure->argNameArr[i] + " = argData;");
            sourceFile->writeFileLine("}\n");
        }
    }
}

void codeGenerate::generateGetStructureArgumentFunctionPrototype(elementArray *data)
{
    if(data->fncPtrName == "")
    {
        for(int i=0;i<structure->argSize;i++)
        {
            addGetItemDefinition(sourceFile,"get" + structure->argNameArr[i] + "ArgumentValue",structure->argTypeArr[i]);
            addGetItemDefinition(headerFile,"get" + structure->argNameArr[i] + "ArgumentValue",structure->argTypeArr[i]);
            sourceFile->writeFileLine(structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(int argIndex);");
            if(data->typeSelection == 0)
            {

                headerFile->writeFileLine(structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(int argIndex);");
            }
            else
            {
                headerFile->writeFileLine("extern " + structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(int argIndex);");
            }
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("    return " + data->name + "[argIndex]." + structure->argNameArr[i] + ";");
            sourceFile->writeFileLine("}\n");
        }
    }
    else
    {
        for(int i=1;i<structure->argSize;i++)
        {
            addGetItemDefinition(sourceFile,"get" + structure->argNameArr[i] + "ArgumentValue",structure->argTypeArr[i]);
            sourceFile->writeFileLine(structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex)");
            addGetItemDefinition(headerFile,"get" + structure->argNameArr[i] + "ArgumentValue",structure->argTypeArr[i]);
            if(data->typeSelection == 0)
            {

                headerFile->writeFileLine(structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex);");
            }
            else
            {
                headerFile->writeFileLine("extern " + structure->argTypeArr[i]+ " get" +  structure->argNameArr[i] + "ArgumentValue(" +fucPointer->indexType + " argIndex);");
            }
            sourceFile->writeFileLine("{");
            sourceFile->writeFileLine("    return " + data->name + "[argIndex]." + structure->argNameArr[i] + ";");
            sourceFile->writeFileLine("}\n");
        }
    }
}

QString codeGenerate::readSourceFile()
{
    QString data,dataContainer;
    sourceFile->reset();
    while(!sourceFile->atEnd())
    {
        sourceFile->readFileLine(&data,false);
        dataContainer+=data;
    }
    return dataContainer;
}

QString codeGenerate::readHeaderFile()
{
    QString data,dataContainer;
    headerFile->reset();
    while(!headerFile->atEnd())
    {
        headerFile->readFileLine(&data,false);
        dataContainer+=data;
    }
    return dataContainer;
}

void  codeGenerate::generateElementPrototype(elementArray *data)
{
    if(data->fncPtrName == "")
    {
        if(structure->name != "")
        {
            generateStructureArrayPrototype(data);
            generateSetStructureArgumentFunctionPrototype(data);
            generateGetStructureArgumentFunctionPrototype(data);
            generateHeaderDefineStop(headerFileName);
            emit codeGenerated(readSourceFile(),readHeaderFile());
        }
    }
    else
    {
        if(structure->name != "")
        {
            generateStateControlVariablePrototype(data);
            generateEnumPrototype(data);
            generateStructureArrayPrototype(data);
            generateFunctionPrototype(data);
            generateCallFuctionPrototype(data);
            generateControlFuctionPrototype(data);
            generateSetFunctionIndexPrototype(data);
            generateSetStructureArgumentFunctionPrototype(data);
            generateGetStructureArgumentFunctionPrototype(data);
            generateHeaderDefineStop(headerFileName);
            emit codeGenerated(readSourceFile(),readHeaderFile());
        }
    }
}
