#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include <time.h>
#include <QString>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Executar_clicked()
{
    QStringList dim = (ui->dimensoesV->text()).split(":");

    tam_min = dim[0].toInt();
    passo = dim[1].toInt();
    tam_max = dim[2].toInt();

    iter = (ui->n_iteracoes->text()).toInt();

    int n_dados = ((tam_max-tam_min)/passo)+1;
    QVector<double> x(n_dados), y(n_dados);


    clock_t clock1, clock2;
    for (int i = tam_min, j = 0; i<= tam_max; i+=passo, j++)
    {
        vetor = new int[i];
        criarVetor(i,vetor);
        x[j] = i;
        clock1 = clock();
        bubbleSort(vetor, i);
        clock2 = clock();
        y[j] = (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->xAxis->setLabel("Tamanho do Vetor");
    ui->customPlot->yAxis->setLabel("Tempo (segundos)");
    ui->customPlot->xAxis->setRange(tam_min, tam_max);
    ui->customPlot->yAxis->setRange(0, 1);
    ui->customPlot->replot();



}
