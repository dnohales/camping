/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*#include "richtexteditor_p.h"
#include "htmlhighlighter_p.h"
#include "iconselector_p.h"
#include "ui_addlinkdialog.h"
#include "abstractsettings_p.h"

#include "iconloader_p.h"*/

#include "RichTextEditorWidget.h"

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QPointer>

#include <QtGui/QAction>
#include <QtGui/QColorDialog>
#include <QtGui/QComboBox>
#include <QtGui/QFontDatabase>
#include <QtGui/QTextCursor>
#include <QtGui/QPainter>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMoveEvent>
#include <QtGui/QTabWidget>
#include <QtGui/QTextDocument>
#include <QtGui/QTextBlock>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QSyntaxHighlighter>
#include <QtCore/QFile>
#include <QWebFrame>

void HtmlTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *menu = createStandardContextMenu();
	QMenu *htmlMenu = new QMenu(tr("Insertar entidad HTML"), menu);

	typedef struct {
		const char *text;
		const char *entity;
	} Entry;

	const Entry entries[] = {
		{ "&&amp; (&&)", "&amp;" },
		{ "&&nbsp;", "&nbsp;" },
		{ "&&lt; (<)", "&lt;" },
		{ "&&gt; (>)", "&gt;" },
		{ "&&copy; (Copyright)", "&copy;" },
		{ "&&reg; (Trade Mark)", "&reg;" },
	};

	for (int i = 0; i < 6; ++i) {
		QAction *entityAction = new QAction(QLatin1String(entries[i].text),
											htmlMenu);
		entityAction->setData(QLatin1String(entries[i].entity));
		htmlMenu->addAction(entityAction);
	}

	menu->addMenu(htmlMenu);
	connect(htmlMenu, SIGNAL(triggered(QAction*)),
					  SLOT(actionTriggered(QAction*)));
	menu->exec(event->globalPos());
	delete menu;
}

void HtmlTextEdit::actionTriggered(QAction *action)
{
	insertPlainText(action->data().toString());
}

RichTextEditor::RichTextEditor(QWidget *parent)
	: QWebEngineView(parent)
{
	connect(this->page(), SIGNAL(contentsChanged()), this, SIGNAL(textChanged()));
	this->page()->setContentEditable(true);
}

QString RichTextEditor::html() const
{
	return this->page()->currentFrame()->toHtml();
}

HtmlHighlighter::HtmlHighlighter(QTextEdit *textEdit)
	: QSyntaxHighlighter(textEdit)
{
	QTextCharFormat entityFormat;
	entityFormat.setForeground(Qt::red);
	setFormatFor(Entity, entityFormat);

	QTextCharFormat tagFormat;
	tagFormat.setForeground(Qt::darkMagenta);
	tagFormat.setFontWeight(QFont::Bold);
	setFormatFor(Tag, tagFormat);

	QTextCharFormat commentFormat;
	commentFormat.setForeground(Qt::gray);
	commentFormat.setFontItalic(true);
	setFormatFor(Comment, commentFormat);

	QTextCharFormat attributeFormat;
	attributeFormat.setForeground(Qt::black);
	attributeFormat.setFontWeight(QFont::Bold);
	setFormatFor(Attribute, attributeFormat);

	QTextCharFormat valueFormat;
	valueFormat.setForeground(Qt::blue);
	setFormatFor(Value, valueFormat);
}

void HtmlHighlighter::setFormatFor(Construct construct,
								   const QTextCharFormat &format)
{
	m_formats[construct] = format;
	rehighlight();
}

void HtmlHighlighter::highlightBlock(const QString &text)
{
	static const QLatin1Char tab = QLatin1Char('\t');
	static const QLatin1Char space = QLatin1Char(' ');
	static const QLatin1Char amp = QLatin1Char('&');
	static const QLatin1Char startTag = QLatin1Char('<');
	static const QLatin1Char endTag = QLatin1Char('>');
	static const QLatin1Char quot = QLatin1Char('"');
	static const QLatin1Char apos = QLatin1Char('\'');
	static const QLatin1Char semicolon = QLatin1Char(';');
	static const QLatin1Char equals = QLatin1Char('=');
	static const QLatin1String startComment = QLatin1String("<!--");
	static const QLatin1String endComment = QLatin1String("-->");
	static const QLatin1String endElement = QLatin1String("/>");

	int state = previousBlockState();
	int len = text.length();
	int start = 0;
	int pos = 0;

	while (pos < len) {
		switch (state) {
		case NormalState:
		default:
			while (pos < len) {
				QChar ch = text.at(pos);
				if (ch == startTag) {
					if (text.mid(pos, 4) == startComment) {
						state = InComment;
					} else {
						state = InTag;
						start = pos;
						while (pos < len && text.at(pos) != space
							   && text.at(pos) != endTag
							   && text.at(pos) != tab
							   && text.mid(pos, 2) != endElement)
							++pos;
						if (text.mid(pos, 2) == endElement)
							++pos;
						setFormat(start, pos - start,
								  m_formats[Tag]);
						break;
					}
					break;
				} else if (ch == amp) {
					start = pos;
					while (pos < len && text.at(pos++) != semicolon)
						;
					setFormat(start, pos - start,
							  m_formats[Entity]);
				} else {
					// No tag, comment or entity started, continue...
					++pos;
				}
			}
			break;
		case InComment:
			start = pos;
			while (pos < len) {
				if (text.mid(pos, 3) == endComment) {
					pos += 3;
					state = NormalState;
					break;
				} else {
					++pos;
				}
			}
			setFormat(start, pos - start, m_formats[Comment]);
			break;
		case InTag:
			QChar quote = QChar::Null;
			while (pos < len) {
				QChar ch = text.at(pos);
				if (quote.isNull()) {
					start = pos;
					if (ch == apos || ch == quot) {
						quote = ch;
					} else if (ch == endTag) {
						++pos;
						setFormat(start, pos - start, m_formats[Tag]);
						state = NormalState;
						break;
					} else if (text.mid(pos, 2) == endElement) {
						pos += 2;
						setFormat(start, pos - start, m_formats[Tag]);
						state = NormalState;
						break;
					} else if (ch != space && text.at(pos) != tab) {
						// Tag not ending, not a quote and no whitespace, so
						// we must be dealing with an attribute.
						++pos;
						while (pos < len && text.at(pos) != space
							   && text.at(pos) != tab
							   && text.at(pos) != equals)
							++pos;
						setFormat(start, pos - start, m_formats[Attribute]);
						start = pos;
					}
				} else if (ch == quote) {
					quote = QChar::Null;

					// Anything quoted is a value
					setFormat(start, pos - start, m_formats[Value]);
				}
				++pos;
			}
			break;
		}
	}
	setCurrentBlockState(state);
};

RichTextEditorWidget::RichTextEditorWidget(QWidget *parent)  :
	QWidget(parent),
	m_editor(new RichTextEditor()),
	m_text_edit(new HtmlTextEdit),
	m_tab_widget(new QTabWidget),
	m_state(Clean)
{
	m_text_edit->setAcceptRichText(false);
	new HtmlHighlighter(m_text_edit);

	connect(m_editor, SIGNAL(textChanged()), this, SLOT(richTextChanged()));
	connect(m_text_edit, SIGNAL(textChanged()), this, SLOT(sourceChanged()));

	QWidget *rich_edit = new QWidget;
	QVBoxLayout *rich_edit_layout = new QVBoxLayout(rich_edit);
	rich_edit_layout->addWidget(m_editor);

	QWidget *plain_edit = new QWidget;
	QVBoxLayout *plain_edit_layout = new QVBoxLayout(plain_edit);
	plain_edit_layout->addWidget(m_text_edit);

	m_tab_widget->setTabPosition(QTabWidget::South);
	m_tab_widget->addTab(rich_edit, tr("Texto con formato"));
	m_tab_widget->addTab(plain_edit, tr("Código HTML"));
	connect(m_tab_widget, SIGNAL(currentChanged(int)),
						  SLOT(tabIndexChanged(int)));

	m_editor->setFocus();

	this->setLayout(new QVBoxLayout(this));
	this->layout()->addWidget(m_tab_widget);
}

RichTextEditorWidget::~RichTextEditorWidget()
{
}

void RichTextEditorWidget::setText(const QString &text)
{
	m_editor->setHtml(text);
	m_text_edit->setPlainText(text);
	m_state = Clean;
}

QString RichTextEditorWidget::text() const
{
	if(m_tab_widget->currentIndex() == RichTextIndex){
		return m_editor->html();
	} else{
		return m_text_edit->toPlainText();
	}
}

void RichTextEditorWidget::tabIndexChanged(int newIndex)
{
	// Anything changed, is there a need for a conversion?
	if (newIndex == SourceIndex && m_state != RichTextChanged)
		return;
	if (newIndex == RichTextIndex && m_state != SourceChanged)
		return;
	const State oldState = m_state;

	if (newIndex == SourceIndex)
		m_text_edit->setPlainText(m_editor->html());
	else
		m_editor->setHtml(m_text_edit->toPlainText());

	m_state = oldState; // Changed is triggered by setting the text
}

void RichTextEditorWidget::richTextChanged()
{
	m_state = RichTextChanged;
}

void RichTextEditorWidget::sourceChanged()
{
	m_state = SourceChanged;
}

