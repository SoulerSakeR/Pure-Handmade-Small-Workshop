#include <Global.h>

Global::Global()
{

}

bool Global::copy(QString srcPath,QString desPath)
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

bool Global::write(QString content, QString path,int mode)
{
    if (path.isEmpty() == false)
    {
        QFile file;
        //关联文件名字
        file.setFileName(path);
        if(mode==0&&file.exists())
        {
          int loc=path.lastIndexOf(".");
          QString path1;
          path1=path.left(loc)+"1"+path.mid(loc);
          file.setFileName(path1);
        }
        bool b_open;
        switch (mode) {
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
            //QFile只支持UTF-8格式
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

Texture2D Global::loadTexture2D(QString path)
{
    QImage image=QImage(path);
    Texture2D temp2D;
    temp2D.widthT=image.width();
    temp2D.heightT=image.height();
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
    return temp2D;
}

QString Global::loadTextAsset(QString path)
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
