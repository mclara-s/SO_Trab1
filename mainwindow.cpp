
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

   // if(!() && !())
     //   ui->Executar->setEnabled(true);
// && (ui->checkBubble->isChecked() || ui->checkQuick->isChecked())

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

    if (!(ui->checkBubble->isChecked()) && !(ui->checkQuick->isChecked())){
         QMessageBox::information(this, tr("Erro"), tr("Voce deve escolher algum tipo de busca!"));
    }
    if (ui->dimensoesV->text().isEmpty() || iter == 0){
         QMessageBox::information(this, tr("Erro"), tr("Você deve fazer pelo menos uma iteração!"));
    }

    if (dim[0].isEmpty() || dim[1].isEmpty() || dim[2].isEmpty() || tam_max < tam_min){
       QMessageBox::information(this, tr("Erro"), tr("As dimensões não estão corretas!"));
    }

    else{

        int n_dados = ((tam_max-tam_min)/passo)+1, Dp = 0;
        QVector<double> xBubble(n_dados), yBubble(n_dados), xQuick(n_dados), yQuick(n_dados), yDpBubble(iter), yDpQuick(iter);


        clock_t clock1, clock2;
        for (int i = tam_min, j = 0; i<= tam_max; i+=passo, j++)
        {
            xBubble[j] = i;
            xQuick[j] = i;

            //xBubble = 0; yBubble = 0; xQuick = 0; yQuick = 0;
            for(int it = 0; it < iter; it++)
            {
                vetor = new int[i];
                criarVetor(i,vetor);
                if(ui->checkBubble->isChecked()){
                    clock1 = clock();
                    bubbleSort(vetor, i);
                    clock2 = clock();
                    yBubble[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                    yDpBubble[it] = (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                }
                if(ui->checkQuick->isChecked()){
                    clock1 = clock();
                    qsort (vetor, i, sizeof(int), compare_ints);
                    clock2 = clock();
                    yQuick[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                    yDpQuick[it] = (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;
                }
            }

            //Calculo da média
            yBubble[j] /= iter;
            yQuick[j] /= iter;

            //Calculo desvio padrão = raizq(soma de todos os quadrados dos desvios dividida pelo numero de ocorrencias)
            //desvio = valor - media;
            for (int it = 0; it< iter; it++)
            {
                Dp += (yDpBubble[it] - yBubble[j])*(yDpBubble[it] - yBubble[j]);
            }
            Dp = sqrt(Dp/iter);

        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(xBubble, yBubble);
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setData(xQuick, yQuick);
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
        ui->customPlot->xAxis->setLabel("Tamanho do Vetor");
        ui->customPlot->yAxis->setLabel("Tempo (segundos)");
        ui->customPlot->graph(0)->rescaleAxes();
        ui->customPlot->graph(1)->rescaleAxes(true);
        ui->customPlot->xAxis->setRangeLower(0);
        ui->customPlot->yAxis->setRangeLower(0);
        ui->customPlot->replot();
}


}
