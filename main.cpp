#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QtWidgets>
#include "save_and_build.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "C_2plus_syntax_highlighting_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    std::cout << " ________      _________     _________     __________" << std::endl;
    std::cout << "|   _____|    |    _____|   |  _____  |   |   ____   |" << std::endl;
    std::cout << "\\   \\         |   |_____    |      ___|   |  |    |  |" << std::endl;
    std::cout << " \\   \\        |    _____|   |  | \\        |  |    |  |" << std::endl;
    std::cout << " _\\   \\       |   |_____    |  |  \\__     |  |____|  |" << std::endl;
    std::cout << "|________|    |_________|   |__|______|   |__________|" << std::endl;
    Save_and_build save_and_build;

    // QTextEdit txt;
    // QFont fnt("Lucida Console", 9, QFont::Normal);
    // txt.document()->setDefaultFont(fnt);Ggdrop29!;
    // new SyntaxHighlighter(txt.document());

    // QPalette pal = txt.palette();
    // pal.setColor(QPalette::Base, Qt::darkBlue);
    // pal.setColor(QPalette::Text, Qt::yellow);
    // txt.setPalette(pal);

    // //txt.show();
    // txt.resize(640,480);

    save_and_build.show();

    return a.exec();
}
