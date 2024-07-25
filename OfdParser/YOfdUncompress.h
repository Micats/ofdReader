#pragma once

#include <QObject>
#include <QMap>

//将ofd压缩文件解压到内存中
class YOfdUncompress : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(YOfdUncompress)

public:
	static YOfdUncompress& instance();

	YOfdUncompress(QObject* parent = nullptr);
	bool loadFile(QString fileName);
	QByteArray getFile(QString absolutePath) ;
	QStringList getFilePaths() { return m_files.keys(); }
	void clear();

private:
	

	QString m_rootPath; //压缩包路径
	QMap<QString, QByteArray> m_files; //文件路径 文件字节
};

