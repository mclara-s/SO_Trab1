#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int tam_min, tam_max, passo, iter;
    int *vetor;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Executar_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
