#include "save_and_build.h"
#include "syntaxHighlighter.h"
#include <QDebug>
#include <QProcess>

Save_and_build::Save_and_build(QWidget* pwgt/*=0*/):QWidget(pwgt){
    fileName_line = new QLineEdit;
    fileSave_btn = new QPushButton("save_and_build");

    QFont fnt("Lucida Console", 9, QFont::Normal);
    code_edit.document()->setDefaultFont(fnt);
    new SyntaxHighlighter(code_edit.document());

    QPalette pal = code_edit.palette();
    pal.setColor(QPalette::Base, Qt::darkBlue);
    pal.setColor(QPalette::Text, Qt::yellow);
    code_edit.setPalette(pal);

    //code_edit.show();
    output_text.setReadOnly(true);
    code_edit.resize(640,480);

    // Layour step
    QVBoxLayout* mainLay = new QVBoxLayout;

    QHBoxLayout* lay_save_and_build = new QHBoxLayout;
    lay_save_and_build->addWidget(fileName_line);
    lay_save_and_build->addWidget(fileSave_btn);

    mainLay->addLayout(lay_save_and_build);
    mainLay->addWidget(&code_edit);
    mainLay->addWidget(&output_text);

    connect(fileSave_btn,SIGNAL(clicked()), SLOT(slotSaveButtonClicked()));

    setLayout(mainLay);
}

void Save_and_build::buildFile(const QString& filePath, const QString& fileName){
    QString relise_file_name = fileName;
    int dotIndex = fileName.lastIndexOf('.');
    if (dotIndex != -1) {
        relise_file_name = fileName.left(dotIndex);
    }
    qDebug() << relise_file_name;
    QString command = QString("g++ %1 -o /home/rezero20sm/CppFiles/%2 --std=c++14 -I/usr/local/include && /home/rezero20sm/CppFiles/%3").arg(filePath, relise_file_name,relise_file_name);
    qDebug() << command;

    // Используем popen для запуска команды и получения вывода
    FILE* pipe = popen(command.toLocal8Bit().constData(), "r");
    if (!pipe) {
        qDebug() << "Ошибка запуска команды";
    }

    // Читаем вывод команды
    char buffer[128];
    QString outputStr;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        outputStr += QString::fromLocal8Bit(buffer);
    }

    // Закрываем пайп
    pclose(pipe);

    output_text.clear();
    output_text.setText(outputStr);

    qDebug() << "Output:" << outputStr;

}

void Save_and_build::slotSaveButtonClicked(){
    fileName = fileName_line->text();
    filePath = "/home/rezero20sm/CppFiles/"+fileName_line->text();

    QFile remove_old_file(filePath);
    remove_old_file.remove();
    remove_old_file.close();

    QFile file(filePath);
    file.open(QIODevice::ReadWrite);


    QTextStream out(&file);

    out << code_edit.toPlainText();

    qDebug() << fileName << filePath;

    file.close();

    buildFile(filePath, fileName);
}
