#include <QtGui>
#include <QtXmlPatterns>

#include "mainwindow.h"
#include "xmlsyntaxhighlighter.h"

class MessageHandler : public QAbstractMessageHandler {
public:
    MessageHandler()
            : QAbstractMessageHandler(0) {
    }

    QString statusMessage() const {
        return m_description;
    }

    int line() const {
        return m_sourceLocation.line();
    }

    int column() const {
        return m_sourceLocation.column();
    }

protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation) {
        Q_UNUSED(type);
        Q_UNUSED(identifier);

        m_description = description;
        m_sourceLocation = sourceLocation;
    }

private:
    QString m_description;
    QSourceLocation m_sourceLocation;
};

MainWindow::MainWindow() {
    setupUi(this);

    new XmlSyntaxHighlighter(schemaView->document());
    new XmlSyntaxHighlighter(instanceEdit->document());

    schemaSelection->addItem(tr("Contact Schema"));
    schemaSelection->addItem(tr("Recipe Schema"));
    schemaSelection->addItem(tr("Order Schema"));

    schemaSelection->addItem(tr("GDML"));
    schemaSelection->addItem(tr("GDML Core"));
    schemaSelection->addItem(tr("GDML Define"));
    schemaSelection->addItem(tr("GDML Extensions"));
    schemaSelection->addItem(tr("GDML Materials"));
    schemaSelection->addItem(tr("GDML Parameterised"));
    schemaSelection->addItem(tr("GDML Solids"));

    schemaSelection->addItem(tr("Test"));

    instanceSelection->addItem(tr("Valid Contact Instance"));
    instanceSelection->addItem(tr("Invalid Contact Instance"));

    connect(schemaSelection, SIGNAL(currentIndexChanged(int)), SLOT(schemaSelected(int)));
    connect(instanceSelection, SIGNAL(currentIndexChanged(int)), SLOT(instanceSelected(int)));
    connect(validateButton, SIGNAL(clicked()), SLOT(validate()));
    connect(instanceEdit, SIGNAL(textChanged()), SLOT(textChanged()));
    connect(openFile, SIGNAL(clicked(bool)), SLOT(openFileDialog()));

    validationStatus->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    schemaSelected(0);
    instanceSelected(0);
}

void MainWindow::schemaSelected(int index) {
    instanceSelection->clear();

    if (schemaSelection->currentText() == "Contact Schema") {
        instanceSelection->addItem(tr("Valid Contact Instance"));
        instanceSelection->addItem(tr("Invalid Contact Instance"));
    } else if (schemaSelection->currentText() == "Recipe Schema") {
        instanceSelection->addItem(tr("Valid Recipe Instance"));
        instanceSelection->addItem(tr("Invalid Recipe Instance"));
    } else if (schemaSelection->currentText() == "Order Schema") {
        instanceSelection->addItem(tr("Valid Order Instance"));
        instanceSelection->addItem(tr("Invalid Order Instance"));
    } else if (schemaSelection->currentText() == "GDML") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Core") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Define") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Extensions") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Materials") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Parameterised") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if (schemaSelection->currentText() == "GDML Solids") {
        instanceSelection->addItem(tr("Simple GDML"));
        instanceSelection->addItem(tr("LHCBHCA1 GDML"));
        instanceSelection->addItem(tr("LHCBVELO GDML"));
        instanceSelection->addItem(tr("PARAMETERIZED GDML"));
        instanceSelection->addItem(tr("Solids GDML"));
    } else if(schemaSelection->currentText() == "Test"){
        instanceSelection->addItem(tr("Test XML"));
    }

    textChanged();

    const QString fileName = QStringLiteral(":/schema_") + QString::number(index +1) + QStringLiteral(".xsd");
    QFile schemaFile(fileName);

    if (!schemaFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open" << QDir::toNativeSeparators(fileName)
                   << ':' << schemaFile.errorString();
        return;
    }

    const QString schemaText(QString::fromUtf8(schemaFile.readAll()));
    schemaView->setPlainText(schemaText);

    validate();
}

void MainWindow::instanceSelected(int index) {
    if (index < 0) {
        instanceEdit->setPlainText(QString());
        return;
    }

    QString fileName;

    if (instanceSelection->count() < 3) {
        fileName =
                QStringLiteral(":/instance_") + QString::number(instanceSelection->currentIndex()) + ".xml";


    } else {
        fileName =
                QStringLiteral(":/instance_") + QString::number(instanceSelection->currentIndex() + 6) + ".xml";
    }

    if(instanceSelection->currentText() == "Test XML"){
        fileName = QStringLiteral(":/instance_11.xml");
    }

    QFile instanceFile(fileName);
    if (!instanceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open" << QDir::toNativeSeparators(fileName)
                   << ':' << instanceFile.errorString();
        return;
    }
    const QString instanceText(QString::fromUtf8(instanceFile.readAll()));
    instanceEdit->setPlainText(instanceText);

    validate();
}

void MainWindow::validate() {
    const QByteArray schemaData = schemaView->toPlainText().toUtf8();
    const QByteArray instanceData = instanceEdit->toPlainText().toUtf8();

    MessageHandler messageHandler;

    QXmlSchema schema;
    schema.setMessageHandler(&messageHandler);

    schema.load(schemaData);

    bool errorOccurred = false;
    if (!schema.isValid()) {
        errorOccurred = true;
    } else {
        QXmlSchemaValidator validator(schema);
        if (!validator.validate(instanceData))
            errorOccurred = true;
    }

    if (errorOccurred) {
        validationStatus->setText(messageHandler.statusMessage());
        moveCursor(messageHandler.line(), messageHandler.column());
    } else {
        validationStatus->setText(tr("validation successful"));
    }

    const QString styleSheet = QString("QLabel {background: %1; padding: 3px}")
            .arg(errorOccurred ? QColor(Qt::red).lighter(160).name() :
                 QColor(Qt::green).lighter(160).name());
    validationStatus->setStyleSheet(styleSheet);
}

void MainWindow::textChanged() {
    instanceEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void MainWindow::moveCursor(int line, int column) {
    instanceEdit->moveCursor(QTextCursor::Start);
    for (int i = 1; i < line; ++i)
        instanceEdit->moveCursor(QTextCursor::Down);

    for (int i = 1; i < column; ++i)
        instanceEdit->moveCursor(QTextCursor::Right);

    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    const QColor lineColor = QColor(Qt::red).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = instanceEdit->textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    instanceEdit->setExtraSelections(extraSelections);

    instanceEdit->setFocus();
}


void MainWindow::openFileDialog() {

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open GDML"),
                                                    QStandardPaths::displayName(QStandardPaths::DownloadLocation),
                                                    tr("GDML Files (*.gdml *.GDML)"));

    if (fileName.isEmpty()) {
        qDebug() << "GDML seçilmedi";
    } else {
        qDebug() << "GDML: " << fileName;


        QFile instanceFile(fileName);
        if (!instanceFile.open(QIODevice::ReadOnly | QFile::Text)) {
            qWarning() << "Cannot open" << QDir::toNativeSeparators(fileName)
                       << ':' << instanceFile.errorString();
            return;
        }
        const QString instanceText(QString::fromUtf8(instanceFile.readAll()));
        instanceEdit->setPlainText(instanceText);
        instanceFile.close();

        textChanged();
    }
}