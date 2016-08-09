// Programa principal para geração de Imagens HDR
// autor: Claudio Mota Oliveira
// Ultima modificação: 18/10/2015
// 

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <fstream>

using namespace std;

#include <CubicSpline.h>
#include <FuncaoResposta.h>
#include <MetodoHDR.h>
#include <ImagemHDR.h>
#include <Robertson.h>
#include <Mann.h>

char* gerarNomePadrao()
{
     char* retorno = new char [6];
     retorno[1] = '.'; retorno[5] = 0;
     retorno[2] = 'j'; retorno[3] = 'p'; retorno[4] = 'g';
     //retorno[2] = 'p'; retorno[3] = 'n'; retorno[4] = 'g';
     return retorno;
}

FuncaoResposta* carregarFuncoesResposta()
{
     FuncaoResposta* retorno = new FuncaoResposta[3];
     if(retorno[0].carregar("Vermelho.fr") &&
        retorno[1].carregar("Verde.fr") &&
        retorno[2].carregar("Azul.fr")) 
     { 
       return retorno;
     }
     else
     {
          delete [] retorno;
          return NULL;   
     }
              
}

int main()
{    
     int i;
     int numeroImagens;
     float* temposExposicao; //Assume que vão estar em ordem crescente
     
     ifstream arquivoConfiguracao;
     arquivoConfiguracao.open("Configuracao.txt");
     if(!arquivoConfiguracao.is_open()) return 1;
     
     arquivoConfiguracao >> numeroImagens;
     
     temposExposicao = new float [numeroImagens];
     IplImage * imagemEscura = cvLoadImage("Escura.jpg", 0);
     IplImage ** imagensEntrada = new IplImage* [numeroImagens];
     
     char* nomeArquivo = gerarNomePadrao();
     
     for(i = 0; i < numeroImagens; i++)
     {
          arquivoConfiguracao >> temposExposicao[i];
          nomeArquivo[0] = 48+i+1;
          imagensEntrada[i] = cvLoadImage(nomeArquivo, 1);
     }
     
     arquivoConfiguracao.close();
     
     //
     FuncaoResposta* frs = carregarFuncoesResposta();
     //
     
     printf("Gerando Imagem\n");
     MetodoHDR* metodo = new MetodoMann;//new MetodoRobertson;
     ImagemHDR* resultado = NULL;
     if(frs)
     {
          frs[0].print("frVermelho.txt");
          frs[1].print("frVerde.txt");
          frs[2].print("frAzul.txt");
          resultado = ImagemHDR::gerarUsando(metodo, numeroImagens, imagensEntrada, imagemEscura, frs, temposExposicao);
     }
     else
     {resultado = ImagemHDR::gerarUsando(metodo, numeroImagens, imagensEntrada, imagemEscura, temposExposicao);}
     
     
     printf("Geração terminada. Salvando Imagem...\n");
     resultado->salvarPFM(metodo->getNomeArquivo());
     
     system("pause");
     
     delete resultado;
     
     cvReleaseImage(&(imagemEscura));
     for(i = 0; i < numeroImagens; i++)
     {
          cvReleaseImage(&(imagensEntrada[i]));
     }
     
     delete [] nomeArquivo;
     delete [] imagensEntrada;
     delete [] temposExposicao;
     
     return 0;
}
