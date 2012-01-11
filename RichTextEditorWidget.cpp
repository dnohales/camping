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

QIcon createIconSet(const QString &name)
{
    return QIcon(QString::fromUtf8(":/imgs/richtexteditor/") + name);
}

QIcon emptyIcon()
{
    return QIcon(QLatin1String(":/trolltech/formeditor/images/emptyicon.png"));
}

AddLinkDialog::AddLinkDialog(RichTextEditor *editor, QWidget *parent) :
    QDialog(parent)
//    m_ui(new Ui::AddLinkDialog)
{
//    m_ui->setupUi(this);

//    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

//    m_editor = editor;
}

AddLinkDialog::~AddLinkDialog()
{
//    delete m_ui;
}

int AddLinkDialog::showDialog()
{
//    // Set initial focus
//    const QTextCursor cursor = m_editor->textCursor();
//    if (cursor.hasSelection()) {
//        m_ui->titleInput->setText(cursor.selectedText());
//        m_ui->urlInput->setFocus();
//    } else {
//        m_ui->titleInput->setFocus();
//    }

//    return exec();
}

void AddLinkDialog::accept()
{
//    const QString title = m_ui->titleInput->text();
//    const QString url = m_ui->urlInput->text();

//    if (!title.isEmpty()) {
//        QString html = QLatin1String("<a href=\"");
//        html += url;
//        html += QLatin1String("\">");
//        html += title;
//        html += QLatin1String("</a>");

//        m_editor->insertHtml(html);
//    }

//    m_ui->titleInput->clear();
//    m_ui->urlInput->clear();

//    QDialog::accept();
}

void HtmlTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    QMenu *htmlMenu = new QMenu(tr("Insert HTML entity"), menu);

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


ColorAction::ColorAction(QObject *parent):
    QAction(parent)
{
    setText(tr("Color del texto"));
    setColor(Qt::black);
    connect(this, SIGNAL(triggered()), this, SLOT(chooseColor()));
}

void ColorAction::setColor(const QColor &color)
{
    if (color == m_color)
        return;
    m_color = color;
    QPixmap pix(24, 24);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.fillRect(pix.rect(), m_color);
    painter.setPen(m_color.darker());
    painter.drawRect(pix.rect().adjusted(0, 0, -1, -1));
    setIcon(pix);
}

void ColorAction::chooseColor()
{
    const QColor col = QColorDialog::getColor(m_color, 0);
    if (col.isValid() && col != m_color) {
        setColor(col);
        emit colorChanged(m_color);
    }
}



static QAction *createCheckableAction(const QIcon &icon, const QString &text,
                                      QObject *receiver, const char *slot,
                                      QObject *parent = 0)
{
    QAction *result = new QAction(parent);
    result->setIcon(icon);
    result->setText(text);
    result->setCheckable(true);
    result->setChecked(false);
    if (slot)
        QObject::connect(result, SIGNAL(triggered(bool)), receiver, slot);
    return result;
}

RichTextEditorToolBar::RichTextEditorToolBar(RichTextEditor *editor,
                                             QWidget *parent) :
    QToolBar(parent),
    m_link_action(new QAction(this)),
    m_image_action(new QAction(this)),
    m_color_action(new ColorAction(this)),
    m_font_size_input(new QComboBox),
    m_editor(editor)
{
    // Font size combo box
    m_font_size_input->setEditable(false);
    const QList<int> font_sizes = QFontDatabase::standardSizes();
    foreach (int font_size, font_sizes)
        m_font_size_input->addItem(QString::number(font_size));

    connect(m_font_size_input, SIGNAL(activated(QString)),
            this, SLOT(sizeInputActivated(QString)));
    addWidget(m_font_size_input);

    addSeparator();

    // Bold, italic and underline buttons

    m_bold_action = createCheckableAction(
            createIconSet(QLatin1String("textbold.png")),
            tr("Negrita"), editor, SLOT(setFontBold(bool)), this);
    m_bold_action->setShortcut(tr("CTRL+B"));
    addAction(m_bold_action);

    m_italic_action = createCheckableAction(
            createIconSet(QLatin1String("textitalic.png")),
            tr("Cursiva"), editor, SLOT(setFontItalic(bool)), this);
    m_italic_action->setShortcut(tr("CTRL+I"));
    addAction(m_italic_action);

    m_underline_action = createCheckableAction(
            createIconSet(QLatin1String("textunder.png")),
            tr("Subrayado"), editor, SLOT(setFontUnderline(bool)), this);
    m_underline_action->setShortcut(tr("CTRL+U"));
    addAction(m_underline_action);

    addSeparator();

    // Left, center, right and justified alignment buttons

    QActionGroup *alignment_group = new QActionGroup(this);
    connect(alignment_group, SIGNAL(triggered(QAction*)),
                             SLOT(alignmentActionTriggered(QAction*)));

    m_align_left_action = createCheckableAction(
            createIconSet(QLatin1String("textleft.png")),
            tr("Alinear a la izquierda"), editor, 0, alignment_group);
    addAction(m_align_left_action);

    m_align_center_action = createCheckableAction(
            createIconSet(QLatin1String("textcenter.png")),
            tr("Centrado"), editor, 0, alignment_group);
    addAction(m_align_center_action);

    m_align_right_action = createCheckableAction(
            createIconSet(QLatin1String("textright.png")),
            tr("Alinear a la derecha"), editor, 0, alignment_group);
    addAction(m_align_right_action);

    m_align_justify_action = createCheckableAction(
            createIconSet(QLatin1String("textjustify.png")),
            tr("Justificado"), editor, 0, alignment_group);
    addAction(m_align_justify_action);

    addSeparator();

    // Superscript and subscript buttons

    m_valign_sup_action = createCheckableAction(
            createIconSet(QLatin1String("textsuperscript.png")),
            tr("Superíndice"),
            this, SLOT(setVAlignSuper(bool)), this);
    addAction(m_valign_sup_action);

    m_valign_sub_action = createCheckableAction(
            createIconSet(QLatin1String("textsubscript.png")),
            tr("Subíndice"),
            this, SLOT(setVAlignSub(bool)), this);
    addAction(m_valign_sub_action);

    addSeparator();

    // Insert hyperlink and image buttons

    /*m_link_action->setIcon(createIconSet(QLatin1String("textanchor.png")));
    m_link_action->setText(tr("Insert &Link"));
    connect(m_link_action, SIGNAL(triggered()), SLOT(insertLink()));
    addAction(m_link_action);

    m_image_action->setIcon(createIconSet(QLatin1String("insertimage.png")));
    m_image_action->setText(tr("Insert &Image"));
    connect(m_image_action, SIGNAL(triggered()), SLOT(insertImage()));
    addAction(m_image_action);

    addSeparator();*/

    // Text color button
    connect(m_color_action, SIGNAL(colorChanged(QColor)),
            this, SLOT(colorChanged(QColor)));
    addAction(m_color_action);

    connect(editor, SIGNAL(textChanged()), this, SLOT(updateActions()));
    connect(editor, SIGNAL(stateChanged()), this, SLOT(updateActions()));

    updateActions();
	
	this->setIconSize(QSize(16, 16));
}

void RichTextEditorToolBar::alignmentActionTriggered(QAction *action)
{
    Qt::Alignment new_alignment;

    if (action == m_align_left_action) {
        new_alignment = Qt::AlignLeft;
    } else if (action == m_align_center_action) {
        new_alignment = Qt::AlignCenter;
    } else if (action == m_align_right_action) {
        new_alignment = Qt::AlignRight;
    } else {
        new_alignment = Qt::AlignJustify;
    }

    m_editor->setAlignment(new_alignment);
}

void RichTextEditorToolBar::colorChanged(const QColor &color)
{
    m_editor->setTextColor(color);
    m_editor->setFocus();
}

void RichTextEditorToolBar::sizeInputActivated(const QString &size)
{
    bool ok;
    int i = size.toInt(&ok);
    if (!ok)
        return;

    m_editor->setFontPointSize(i);
    m_editor->setFocus();
}

void RichTextEditorToolBar::setVAlignSuper(bool super)
{
    const QTextCharFormat::VerticalAlignment align = super ?
        QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal;

    QTextCharFormat charFormat = m_editor->currentCharFormat();
    charFormat.setVerticalAlignment(align);
    m_editor->setCurrentCharFormat(charFormat);

    m_valign_sub_action->setChecked(false);
}

void RichTextEditorToolBar::setVAlignSub(bool sub)
{
    const QTextCharFormat::VerticalAlignment align = sub ?
        QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal;

    QTextCharFormat charFormat = m_editor->currentCharFormat();
    charFormat.setVerticalAlignment(align);
    m_editor->setCurrentCharFormat(charFormat);

    m_valign_sup_action->setChecked(false);
}

void RichTextEditorToolBar::insertLink()
{
    /*AddLinkDialog linkDialog(m_editor, this);
    linkDialog.showDialog();
    m_editor->setFocus();*/
}

void RichTextEditorToolBar::insertImage()
{
    /*const QString path = IconSelector::choosePixmapResource(m_core, m_core->resourceModel(), QString(), this);
    if (!path.isEmpty())
        m_editor->insertHtml(QLatin1String("<img src=\"") + path + QLatin1String("\"/>"));*/
}

void RichTextEditorToolBar::updateActions()
{
    if (m_editor == 0) {
        setEnabled(false);
        return;
    }

    const Qt::Alignment alignment = m_editor->alignment();
    const QTextCursor cursor = m_editor->textCursor();
    const QTextCharFormat charFormat = cursor.charFormat();
    const QFont font = charFormat.font();
    const QTextCharFormat::VerticalAlignment valign =
        charFormat.verticalAlignment();
    const bool superScript = valign == QTextCharFormat::AlignSuperScript;
    const bool subScript = valign == QTextCharFormat::AlignSubScript;

    if (alignment & Qt::AlignLeft) {
        m_align_left_action->setChecked(true);
    } else if (alignment & Qt::AlignRight) {
        m_align_right_action->setChecked(true);
    } else if (alignment & Qt::AlignHCenter) {
        m_align_center_action->setChecked(true);
    } else {
        m_align_justify_action->setChecked(true);
    }

    m_bold_action->setChecked(font.bold());
    m_italic_action->setChecked(font.italic());
    m_underline_action->setChecked(font.underline());
    m_valign_sup_action->setChecked(superScript);
    m_valign_sub_action->setChecked(subScript);

    const int size = font.pointSize();
    const int idx = m_font_size_input->findText(QString::number(size));
    if (idx != -1)
        m_font_size_input->setCurrentIndex(idx);

    m_color_action->setColor(m_editor->textColor());
}

RichTextEditor::RichTextEditor(QWidget *parent)
    : QTextEdit(parent)
{
    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SIGNAL(stateChanged()));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SIGNAL(stateChanged()));
}

QToolBar *RichTextEditor::createToolBar(QWidget *parent)
{
    return new RichTextEditorToolBar(this, parent);
}

void RichTextEditor::setFontBold(bool b)
{
    if (b)
        setFontWeight(QFont::Bold);
    else
        setFontWeight(QFont::Normal);
}

void RichTextEditor::setFontPointSize(double d)
{
    QTextEdit::setFontPointSize(qreal(d));
}

void RichTextEditor::setText(const QString &text)
{
    if (Qt::mightBeRichText(text))
        setHtml(text);
    else
        setPlainText(text);
}

void RichTextEditor::setDefaultFont(const QFont &font)
{
    document()->setDefaultFont(font);
    if (font.pointSize() > 0)
        setFontPointSize(font.pointSize());
    else
        setFontPointSize(QFontInfo(font).pointSize());
    emit textChanged();
}

QString RichTextEditor::text(Qt::TextFormat format) const
{
    switch (format) {
    case Qt::LogText:
    case Qt::PlainText:
        return toPlainText();
    case Qt::RichText:
        return toHtml();
    case Qt::AutoText:
        break;
    }
    const QString html = toHtml();
    const QString plain = toPlainText();
    QTextEdit tester;
    tester.setPlainText(plain);
    return tester.toHtml() == html ? plain : html;
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

    // The toolbar needs to be created after the RichTextEditor
    QToolBar *tool_bar = m_editor->createToolBar();
    tool_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QWidget *rich_edit = new QWidget;
    QVBoxLayout *rich_edit_layout = new QVBoxLayout(rich_edit);
    rich_edit_layout->addWidget(tool_bar);
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

void RichTextEditorWidget::setDefaultFont(const QFont &font)
{
    m_editor->setDefaultFont(font);
}

void RichTextEditorWidget::setText(const QString &text)
{
    m_editor->setText(text);
    m_text_edit->setPlainText(text);
    m_state = Clean;
}

QString RichTextEditorWidget::text(Qt::TextFormat format) const
{
    // In autotext mode, if the user has changed the source, use that
    if (format == Qt::AutoText && (m_state == Clean || m_state == SourceChanged))
        return m_text_edit->toPlainText();
    // If the plain text HTML editor is selected, first copy its contents over
    // to the rich text editor so that it is converted to Qt-HTML or actual
    // plain text.
    if (m_tab_widget->currentIndex() == SourceIndex && m_state == SourceChanged)
        m_editor->setHtml(m_text_edit->toPlainText());
    return m_editor->text(format);
}

void RichTextEditorWidget::tabIndexChanged(int newIndex)
{
    // Anything changed, is there a need for a conversion?
    if (newIndex == SourceIndex && m_state != RichTextChanged)
        return;
    if (newIndex == RichTextIndex && m_state != SourceChanged)
        return;
    const State oldState = m_state;
    // Remember the cursor position, since it is invalidated by setPlainText
    QTextEdit *new_edit = (newIndex == SourceIndex) ? m_text_edit : m_editor;
    const int position = new_edit->textCursor().position();

    if (newIndex == SourceIndex)
        m_text_edit->setPlainText(m_editor->text(Qt::RichText));
    else
        m_editor->setHtml(m_text_edit->toPlainText());

    QTextCursor cursor = new_edit->textCursor();
    cursor.movePosition(QTextCursor::End);
    if (cursor.position() > position) {
        cursor.setPosition(position);
    }
    new_edit->setTextCursor(cursor);
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
