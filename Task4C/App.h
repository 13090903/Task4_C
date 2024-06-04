#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QPlainTextEdit>
#include "Logger.h"

class App : public QWidget {
    Q_OBJECT

    public:
    App(QWidget *parent = nullptr);

    private slots:

        void processFiles();

private:
    QPlainTextEdit *plainTextEdit;
    Logger logger;
};

#endif // APP_H