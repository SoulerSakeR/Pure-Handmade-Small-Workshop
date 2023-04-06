#include "IO.h"

bool IO::createPathIfNotExists(const std::string& directory)
{
    return IO::createPathIfNotExists(QString::fromStdString(directory));
}

bool IO::createPathIfNotExists(const QString& directory) {
    QDir dir(directory);
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}


bool IO::copy(QString srcPath,QString desPath)
{
    desPath.replace("\\","/");
    int loc=srcPath.lastIndexOf("/");
    int cnt=srcPath.length();
    QString fileName=srcPath.right(cnt-loc-1);
    desPath+=fileName;
    if (srcPath == desPath){
        return true;
    }
    if (!QFile::exists(srcPath)){
        return false;
    }
    QDir *createfile  = new QDir;
    bool exist = createfile->exists(desPath);
    if (exist){
            createfile->remove(desPath);
    }//end if

    if(!QFile::copy(srcPath, desPath))
    {
        return false;
    }
    return true;
}

std::string IO::readText(const std::string& path)
{
    return readText(QString::fromStdString(path)).toStdString();
}

QString IO::readText(const QString& path)
{
    TextAsset tempTextAsset;
    QString displayString;
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull())
    {
        displayString.append(line).append('\n');
        line = in.readLine();
    }
    tempTextAsset.text = displayString;
    return displayString;
}

bool IO::write(QString content, QString path, int mode) {
    if (path.isEmpty() == false)
    {
        QFileInfo fileInfo(path);
        QString dirPath = fileInfo.absolutePath();

        // Create the directory if it does not exist
        if (!createPathIfNotExists(dirPath))
        {
            return false;
        }

        QFile file;
        // Associate the file name
        file.setFileName(path);
        if (mode == 0 && file.exists())
        {
            int loc = path.lastIndexOf(".");
            QString path1;
            path1 = path.left(loc) + "1" + path.mid(loc);
            file.setFileName(path1);
        }
        bool b_open;
        switch (mode)
        {
        case 0:
            b_open = file.open(QIODevice::ReadWrite);
            break;
        case 1:
            b_open = file.open(QIODevice::ReadWrite | QIODevice::Truncate);
            break;
        case 2:
            b_open = file.open(QIODevice::ReadWrite | QIODevice::Append);
            break;
        default:
            b_open = file.open(QIODevice::ReadWrite);
        }
        if (b_open == true)
        {
            // QFile only supports UTF-8 format
            QString str = content;
            file.write(str.toUtf8());
        }
        else
        {
            return false;
        }
        file.close();
    }
    else
    {
        return false;
    }
    return true;
}

bool IO::write(const std::string& content,const std::string& path, int mode)
{
    return write(QString::fromStdString(content), QString::fromStdString(path), mode);
}

Texture2D IO::loadTexture2D(QString path)
{
    QImage image=QImage(path);
    Texture2D temp2D;
    temp2D.widthT=image.width();
    temp2D.heightT=image.height();
    /*
    temp2D.data.resize(image.width());
    for(int j=0;j<temp2D.data.size();j++)
    {
        temp2D.data[j].resize(image.height());
    }
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QColor Colos= image.pixelColor(i,j);
            temp2D.data[i][j].r=Colos.red();
            temp2D.data[i][j].g=Colos.green();
            temp2D.data[i][j].b=Colos.blue();
        }
    }
    */
    return temp2D;
}

Texture2D IO::loadTexture2D(const std::string& path)
{
    return loadTexture2D(QString::fromStdString(path));
}

QString IO::loadTextAsset(QString path)
{
    TextAsset tempTextAsset;
    QString displayString;
    QFile file(path);
     file.open(QIODevice::ReadOnly | QIODevice::Text);
     QTextStream in(&file);
     QString line = in.readLine();

     while (!line.isNull())
     {
         displayString.append(line);
         line = in.readLine();
     }
     tempTextAsset.text=displayString;
    return displayString;
}
