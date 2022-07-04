//
// Created by hkn on 30.06.2022.
//

#ifndef OPENCASCADEPROJECT_FILEEXPLORER_H
#define OPENCASCADEPROJECT_FILEEXPLORER_H

#include <QtWidgets>

class MyFileItem : public QLabel {
Q_OBJECT
public:
    MyFileItem(QWidget *parent = nullptr) : QLabel(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAlignment(Qt::AlignCenter);
        setStyleSheet("QLabel{background-color: #ffffff; border: 1px solid #e0e0e0; border-radius: 4px;}");
    }

    void setFilePath(const QString &filePath) {
        m_filePath = filePath;
    }

    void setFileName(const QString &fileName) {
        m_fileName = fileName;
    }

    void setFileSize(qint64 size) {
        m_size = size;
    }

    void setCreatedDate(const QDateTime &created) {
        m_createdDate = created;
    }

    void setModifiedDate(const QDateTime &modified) {
        m_modifiedDate = modified;
    }

    void setLastReadDate(const QDateTime &read) {
        m_lastReadDate = read;
    }

    QWidget *getLabel() {
        QWidget *container = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        container->setLayout(layout);
        container->setStyleSheet("QWidget{background-color: #ffffff; border: 1px solid #e0e0e0; border-radius: 4px;}");

//        this->setPixmap(QPixmap("folder_icon.png"));
//        layout->addWidget(this);
        layout->addWidget(new QLabel("<b>" + m_fileName + "</b>"));

        QLabel *descriptionLabel = new QLabel();
        QString description;
        description.append("<b>Path:</b> " + m_filePath + "<br>");
        description.append("<b>Size:</b> " + QString::number(m_size) + " bytes<br>");
        description.append("<b>Created:</b> " + m_createdDate.toString("dd.MM.yyyy hh:mm:ss") + "<br>");
        description.append("<b>Modified:</b> " + m_modifiedDate.toString("dd.MM.yyyy hh:mm:ss") + "<br>");
        description.append("<b>Last Read:</b> " + m_lastReadDate.toString("dd.MM.yyyy hh:mm:ss") + "<br>");
        descriptionLabel->setText(description);
        descriptionLabel->setWordWrap(true);
        layout->addWidget(descriptionLabel);

        return container;
    }

private:
    QString m_filePath;
    QString m_fileName;
    qint64 m_size;
    QDateTime m_createdDate;
    QDateTime m_modifiedDate;
    QDateTime m_lastReadDate;
};


class MyFileModel : public QWidget {

Q_OBJECT
public:
    MyFileModel(QWidget *parent = nullptr) : QWidget(parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumSize(50, 50);
        setMaximumSize(300,200);

        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(0, 0, 0, 0);
        m_layout->setSpacing(0);
        this->setLayout(m_layout);
    }

    void setAfterWidget(QWidget *widget) {
        m_afterWidget = widget;
    }

    void setBeforeWidget(QString fileName) {

        QWidget *container = new QWidget();
        container->setStyleSheet("QWidget{background-color: #ffffff; border: 1px solid #e0e0e0; border-radius: 4px;}");

        QVBoxLayout *layout = new QVBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignCenter);

        auto pixmap = new QPixmap("folder_icon.png");
        auto label = new QLabel();
        label->setPixmap(*pixmap);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        auto label2 = new QLabel();
        label2->setText("<b>" + fileName + "</b>");
        layout->addWidget(label2);
        container->setLayout(layout);

        m_beforeWidget = container;
        m_layout->addWidget(m_beforeWidget);
    }

    QWidget *getWidget() {
        return this;
    }

private:
    QWidget *m_afterWidget;
    QWidget *m_beforeWidget;
    QVBoxLayout *m_layout;

protected:
    void enterEvent(QEvent *ev) override {
        qDebug() << "enterEvent";
        m_layout->removeWidget(m_beforeWidget);
        m_beforeWidget->hide();
        m_layout->addWidget(m_afterWidget);
        m_afterWidget->show();
    }

    void leaveEvent(QEvent *ev) override {
        qDebug() << "leaveEvent";
        m_layout->removeWidget(m_afterWidget);
        m_afterWidget->hide();
        m_layout->addWidget(m_beforeWidget);
        m_beforeWidget->show();
    }

    void mousePressEvent(QMouseEvent *ev) override {
        qDebug() << "mousePressEvent";
    }
};


class FileExplorer : public QWidget {
Q_OBJECT
public:
    explicit FileExplorer(QString searchPath, QWidget *parent = nullptr);
    ~FileExplorer();

private:
    QWidget *centralWidget;
    QGridLayout *layout;
    QList<QWidget *> fileWidgets;

protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif //OPENCASCADEPROJECT_FILEEXPLORER_H
