#pragma execution_character_set("utf-8")
#include "XmlHignlighter.h"



XmlHignlighter::XmlHignlighter(QTextDocument* parent /*= nullptr*/)
	:QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	//标签
	elementNameFormat.setForeground(Qt::blue);
	elementNameFormat.setFontWeight(QFont::Bold);
	//rule.pattern = QRegExp("<[\\/]*[A-Za-z0-9_]+\\b|>");
	rule.pattern = QRegExp("(<[a-zA-Z:]+\\b|<\\?[a-zA-Z:]+\\b|\\?>|>|/>|</[a-zA-Z:]+>)");
	rule.pattern.setMinimal(true);
	rule.format = elementNameFormat;
	highlightingRules.append(rule);


	// 属性名
	propertyFormat.setForeground(Qt::red);
	//rule.pattern = QRegExp("\\b[A-Za-z0-9_]+[\\s]*(?=\\=)");
	rule.pattern = QRegExp("[a-zA-Z:]+=");
	rule.pattern.setMinimal(true);
	rule.format = propertyFormat;
	highlightingRules.append(rule);

	// 属性值 
	quotationFormat.setForeground(Qt::gray);
	rule.pattern = QRegExp("\".*\"");
	//rule.pattern = QRegExp("(\"[^\"]*\"|'[^']*')");
	rule.pattern.setMinimal(true); // 优先匹配最短的
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	

	// 多行注释
	multiLineCommentFormat.setForeground(Qt::lightGray);
	commentStartExpression = QRegExp("<!--");
	commentEndExpression = QRegExp("-->");
}

XmlHignlighter::~XmlHignlighter()
{}

void XmlHignlighter::highlightBlock(const QString& text)
{
	foreach(const HighlightingRule & rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	// 匹配多行注释
	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);

	while (startIndex >= 0) {
		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else {
			commentLength = endIndex - startIndex
				+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}
