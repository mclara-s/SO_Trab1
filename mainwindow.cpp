
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

        int n_dados = ((tam_max-tam_min)/passo)+1;
        QVector<double> x(n_dados), yBubble(n_dados), yQuick(n_dados);
        QVector<double>  yDpBubble(iter), yDpQuick(iter), DpBubble(n_dados), DpQuick(n_dados);


        clock_t clock1, clock2;
        for (int i = tam_min, j = 0; i<= tam_max; i+=passo, j++)
        {
            DpBubble[j] = 0;
            DpQuick[j] = 0;
            x[j] = i;

            //xBubble = 0; yBubble = 0; xQuick = 0; yQuick = 0;
            for(int it = 0; it < iter; it++)
            {
                vetor = new int[i];
                criarVetor(i,vetor);
                if(ui->checkBubble->isChecked()){
                    clock1 = clock();
                    bubbleSort(vetor, i);
                    clock2 = clock();
                    yBubble[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;       //Somatório para calcular a média
                    yDpBubble[it] = (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;     //Tempo em cada iteração para calcular D.p.
                }
                if(ui->checkQuick->isChecked()){
                    clock1 = clock();
                    qsort (vetor, i, sizeof(int), compare_ints);
                    clock2 = clock();
                    yQuick[j] += (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;        //Somatório para calcular a média
                    yDpQuick[it] = (double)(clock2-clock1)/(double)CLOCKS_PER_SEC;      //Tempo em cada iteração para calcular D.p.
                }
            }

            //Calculo da média
            yBubble[j] /= iter;
            yQuick[j] /= iter;

            //Calculo desvio padrão = raizq(soma de todos os quadrados dos desvios dividida pelo numero de ocorrencias)
            //desvio = valor - media;
            for (int it = 0; it< iter; it++)
            {
                DpBubble[j] += (yDpBubble[it] - yBubble[j])*(yDpBubble[it] - yBubble[j]);
                DpQuick[j] += (yDpQuick[it] - yQuick[j])*(yDpQuick[it] - yQuick[j]);
            }
            DpBubble[j] = sqrt(DpBubble[j]/iter);
            DpQuick[j] = sqrt(DpQuick[j]/iter);

        }

        ui->customPlot->clearGraphs();
        ui->customPlot->legend->clearItems();

        //Plotando o Gráfico do Bubble Sort
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setErrorType(QCPGraph::etValue);
        ui->customPlot->graph(0)->setErrorPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setDataValueError(x, yBubble, DpBubble);
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        ui->customPlot->graph(0)->setName("Bubble Sort");

        //Plotando o Gráfico do Quick Sort
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setErrorType(QCPGraph::etValue);
        ui->customPlot->graph(1)->setErrorPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setDataValueError(x, yQuick, DpQuick);
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        ui->customPlot->graph(1)->setName("Quick Sort");

        //Reescalar os eixos para os maiores valores
        if (ui->checkQuick->isChecked() && !(ui->checkBubble->isChecked()))
        {
            ui->customPlot->graph(1)->rescaleAxes();
        }
        else{
            ui->customPlot->graph(0)->rescaleAxes();
        }

        //Ajustar Legenda
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignTop);
        ui->customPlot->legend->setVisible(true);

        //Ajustar Eixos
        ui->customPlot->xAxis->setLabel("Tamanho do Vetor");
        ui->customPlot->yAxis->setLabel("Tempo (segundos)");
        ui->customPlot->xAxis->setRange(0,tam_max+passo);
        ui->customPlot->yAxis->setRangeLower(0);

        ui->customPlot->replot();
}


}
