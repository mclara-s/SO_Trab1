
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include <time.h>
#include <QString>
#include <cstdlib>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // if(!(ui->dimensoesV->text.isEmpty()) && !(ui->n_iteracoes->text.isEmpty()))
     //   ui->Executar->setEnabled(true);
// && (ui->checkBubble->isChecked() || ui->checkQuick->isChecked())

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Executar_clicked()
{
    if (!(ui->checkBubble->isChecked()) && !(ui->checkQuick->isChecked())){
         QMessageBox::information(this, tr("Erro"), tr("Voce deve escolher algum tipo de busca!"));
    }
    else{
        QStringList dim = (ui->dimensoesV->text()).split(":");

        tam_min = dim[0].toInt();
        passo = dim[1].toInt();
        tam_max = dim[2].toInt();

        iter = (ui->n_iteracoes->text()).toInt();

        int n_dados = ((tam_max-tam_min)/passo)+1;
        QVector<double> xBubble(n_dados), yBubble(n_dados), xQuick(n_dados), yQuick(n_dados);


        clock_t clock1, clock2;
        for (int i = tam_min, j = 0; i<= tam_max; i+=passo, j++)
        {
            //xBubble = 0; yBubble = 0; xQuick = 0; yQuick = 0;
            for(int it = 0; it < iter; it++)
            {
                vetor = new int[i];
                criarVetor(i,vetor);
                if(ui->checkBubble->isChecked()){
                    xBubble[j] += i;
                    clock1 = clock();
                    bubbleSort(vetor, i);
                    clock2 = clock();
                    yBubble[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                }
                if(ui->checkQuick->isChecked()){
                    xQuick[j] += i;
                    clock1 = clock();
                    qsort (vetor, i, sizeof(int), compare_ints);
                    clock2 = clock();
                    yQuick[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                }
            }
            //Calculo desvio padrão

            //Calculo da média
            xBubble[j] /= iter; yBubble[j] /= iter; xQuick[j] /= iter; yQuick[j] /= iter;
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(xBubble, yBubble);
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setData(xQuick, yQuick);
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->xAxis->setLabel("Tamanho do Vetor");
        ui->customPlot->yAxis->setLabel("Tempo (segundos)");
        ui->customPlot->graph(0)->rescaleAxes();
        ui->customPlot->graph(1)->rescaleAxes(true);
        ui->customPlot->xAxis->setRangeLower(0);
        ui->customPlot->yAxis->setRangeLower(0);
        ui->customPlot->replot();
}


}
