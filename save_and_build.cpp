#include "save_and_build.h"
#include "syntaxHighlighter.h"
#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

Save_and_build::Save_and_build(QWidget* pwgt) : QWidget(pwgt) {
    fileName_line = new QLineEdit(this);
    fileSave_btn = new QPushButton("save_and_build", this);

    QFont fnt("Lucida Console", 9, QFont::Normal);
    code_edit.document()->setDefaultFont(fnt);
    new SyntaxHighlighter(code_edit.document());

    QPalette pal = code_edit.palette();
    pal.setColor(QPalette::Base, Qt::darkBlue);
    pal.setColor(QPalette::Text, Qt::yellow);
    code_edit.setPalette(pal);

    output_text.setReadOnly(true);
    code_edit.resize(640, 480);

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    QHBoxLayout* lay_save_and_build = new QHBoxLayout;
    lay_save_and_build->addWidget(fileName_line);
    lay_save_and_build->addWidget(fileSave_btn);

    mainLay->addLayout(lay_save_and_build);
    mainLay->addWidget(&code_edit);
    mainLay->addWidget(&output_text);

    connect(fileSave_btn, &QPushButton::clicked, this, &Save_and_build::slotSaveButtonClicked);
}

void Save_and_build::buildFile(const QString& filePath, const QString& fileName) {
    // Генерируем уникальное имя для выходного файла
    QFileInfo fi(filePath);
    QString outputName = fi.completeBaseName() + "_bin";
    QString outputPath = fi.path() + "/" + outputName;

    // Экранируем пути с пробелами
    QString command = QString("g++ \"%1\" -o \"%2\" --std=c++14 -I/usr/local/include && \"%2\"")
                      .arg(filePath, outputPath);

    qDebug() << "Executing:" << command;

    FILE* pipe = popen(command.toLocal8Bit().constData(), "r");
    if (!pipe) {
        output_text.append("Failed to start compiler!\n");
        return;
    }

    // Чтение вывода
    char buffer[128];
    QString outputStr;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        outputStr += QString::fromLocal8Bit(buffer);
    }

    // Проверка статуса компиляции
    int status = pclose(pipe);
    output_text.setText(outputStr);
    if (status != 0) {
        output_text.append("\nCompilation failed (code: " + QString::number(status) + ")");
    }
}

void Save_and_build::slotSaveButtonClicked() {
    QString fileName = fileName_line->text().trimmed();
    
    // Проверка пустого имени
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "File name cannot be empty!");
        return;
    }
    
    // Добавляем расширение .cpp если нужно
    if (!fileName.endsWith(".cpp", Qt::CaseInsensitive)) {
        fileName += ".cpp";
    }

    // Создаем директорию если не существует
    QDir saveDir("/home/zero/Documents/cppfiles");
    if (!saveDir.exists()) {
        saveDir.mkpath(".");
    }

    QString filePath = saveDir.filePath(fileName);
    
    // Удаляем старый файл если существует
    if (QFile::exists(filePath)) {
        if (!QFile::remove(filePath)) {
            QMessageBox::critical(this, "Error", "Can't remove old file!");
            return;
        }
    }

    // Сохраняем код
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << code_edit.toPlainText();
        file.close();
        
        // Запускаем сборку
        buildFile(filePath, fileName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to save file: " + file.errorString());
    }
}
