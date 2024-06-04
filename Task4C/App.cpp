#include "App.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

#define MAXN 100000

App::App(QWidget *parent) : QWidget(parent), logger("../logs/app_log.txt") {
    setWindowTitle("Сonnected сomponents in graph");
    resize(800, 600);

    QGridLayout *gridLayout = new QGridLayout(this);

    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setReadOnly(true);
    QPushButton *button = new QPushButton("Select Files", this);

    gridLayout->addWidget(plainTextEdit, 0, 0, 1, 2);
    gridLayout->addWidget(button, 1, 0, 1, 2);

    connect(button, &QPushButton::clicked, this, &App::processFiles);
}

int n;
vector<int> g[MAXN];
bool used[MAXN];
vector<int> comp;

void dfs(int v) {
    used[v] = true;
    comp.push_back(v);
    for (int to: g[v]) {
        if (!used[to]) {
            dfs(to);
        }
    }
}

void App::processFiles() {
    try {
        QString graphFileName = QFileDialog::getOpenFileName(this, "Select graph file", "../files/",
                                                             "Text Files (*.txt);");
        if (graphFileName.isEmpty()) return;


        ifstream graphFile(graphFileName.toStdString());

        if (!graphFile.is_open()) {
            throw runtime_error("Unable to open file");
        }

        string line;
        string vertexes;
        getline(graphFile, vertexes);
        int n = atoi(vertexes.c_str());

        while (getline(graphFile, line)) {
            istringstream lineStream(line);
            string v;
            string vertex;

            lineStream >> v;
            int vert = atoi(v.c_str()) - 1;

            while (lineStream >> vertex) {
                g[vert].push_back(atoi(vertex.c_str()) - 1);
            }
        }

        QString outputFileName = QFileDialog::getSaveFileName(this, "Select Output File", "../files/",
                                                              "Text Files (*.txt);");
        if (outputFileName.isEmpty()) return;

        ofstream outputFile(outputFileName.toStdString());
        if (!outputFile.is_open()) {
            throw runtime_error("Unable to open output file");
        }

        ostringstream outputContent;
        for (int i = 0; i < n; ++i)
            used[i] = false;
        for (int i = 0; i < n; ++i) {
            if (!used[i]) {
                comp.clear();
                dfs(i);

                outputFile << "Component:";
                outputContent << "Component:";
                for (int j: comp) {
                    outputFile << ' ' << (j + 1);
                    outputContent << ' ' << (j + 1);
                }
                outputFile << endl;
                outputContent << endl;
            }
        }

        outputFile.close();
        plainTextEdit->setPlainText(QString::fromStdString(outputContent.str()));
        logger.logMessage("Components found", "INFO");
        QMessageBox::information(this, "Success", "Components found");
    } catch (const exception &e) {
        logger.logMessage(e.what(), "ERROR");
        QMessageBox::critical(this, "Error", e.what());
    }
}
