#pragma once

#include <QSyntaxHighlighter>
#include <QObject>

class XmlHignlighter  : public QSyntaxHighlighter
{
	Q_OBJECT
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

public:
	explicit XmlHignlighter(QTextDocument *parent = nullptr);
	~XmlHignlighter();

protected:
	void highlightBlock(const QString& text);

private:
	QVector<HighlightingRule> highlightingRules;
	QRegExp commentStartExpression;
	QRegExp commentEndExpression;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat elementNameFormat;
	QTextCharFormat propertyFormat;
};
