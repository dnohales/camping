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

#ifndef RITCHTEXTEDITORWIDGET_H
#define RITCHTEXTEDITORWIDGET_H

#include <QAction>
#include <QComboBox>
#include <QDialog>
#include <QPointer>
#include <QSyntaxHighlighter>
#include <QToolBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class QTabWidget;
class QToolBar;

class RichTextEditor;

class RichTextEditorWidget : public QWidget
{
	Q_OBJECT
public:
	explicit RichTextEditorWidget(QWidget *parent = 0);
	~RichTextEditorWidget();

	void setText(const QString &text);
	QString text() const;

private slots:
	void tabIndexChanged(int newIndex);
	void richTextChanged();
	void sourceChanged();

private:
	enum TabIndex { RichTextIndex,
					SourceIndex };
	enum State { Clean,
				 RichTextChanged,
				 SourceChanged };
	RichTextEditor *m_editor;
	QTextEdit *m_text_edit;
	QTabWidget *m_tab_widget;
	State m_state;
};

class RichTextEditor : public QWebView
{
	Q_OBJECT
public:
	RichTextEditor(QWidget *parent = 0);
	QString html() const;

signals:
	void textChanged();
};

class HtmlTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	HtmlTextEdit(QWidget *parent = 0)
		: QTextEdit(parent)
	{
	}

	void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void actionTriggered(QAction *action);
};

/* HTML syntax highlighter based on Qt Quarterly example */
class HtmlHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	enum Construct {
		Entity,
		Tag,
		Comment,
		Attribute,
		Value,
		LastConstruct = Value
	};

	HtmlHighlighter(QTextEdit *textEdit);

	void setFormatFor(Construct construct, const QTextCharFormat &format);

	QTextCharFormat formatFor(Construct construct) const
	{
		return m_formats[construct];
	}

protected:
	enum State {
		NormalState = -1,
		InComment,
		InTag
	};

	void highlightBlock(const QString &text);

private:
	QTextCharFormat m_formats[LastConstruct + 1];
};

#endif // RITCHTEXTEDITORWIDGET_H
