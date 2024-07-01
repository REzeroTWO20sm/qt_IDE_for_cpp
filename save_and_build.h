#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QTextEdit>

class Save_and_build : public QWidget{
    Q_OBJECT
private:
    QLineEdit* fileName_line;
    QPushButton* fileSave_btn;
    QTextEdit code_edit;
    QString filePath;
    QString fileName;
    QTextEdit output_text;
public:
    Save_and_build(QWidget* pwgt = 0);
    void buildFile(const QString& filePath, const QString& fileName);
public slots:
    void slotSaveButtonClicked();
};
