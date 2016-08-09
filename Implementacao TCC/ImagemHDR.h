class ImagemHDR
{
     protected:
     int width, height;
     float* r;
     float* g;
     float* b;
     
     public:
     
     static ImagemHDR* gerarUsando(MetodoHDR * metodoHDR, 
                                   int numeroImagens,
                                   IplImage ** imagens,
                                   IplImage * escura,
                                   float * temposExposicao)
     {
          if(!metodoHDR || !numeroImagens) return NULL;
          ImagemHDR* retorno = new ImagemHDR;
          retorno->width = imagens[0]->width;
          retorno->height = imagens[0]->height;
          
          IplImage** imagensVermelho = new IplImage*[numeroImagens];
          IplImage** imagensVerde = new IplImage*[numeroImagens];
          IplImage** imagensAzul = new IplImage*[numeroImagens];
          
          int i;
          for(i = 0; i < numeroImagens; i++)
          {
               imagensAzul[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               imagensVerde[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               imagensVermelho[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               cvSplit(imagens[i],imagensAzul[i],imagensVerde[i],imagensVermelho[i], NULL);        
          }
          
          FuncaoResposta funcaoResposta[3];
          printf("Obtendo funcoes resposta:\n");
          printf("Vermelho...\n");
          funcaoResposta[0] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensVermelho,temposExposicao,escura);
          printf("Verde...\n");
          funcaoResposta[1] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensVerde,temposExposicao,escura);
          printf("Azul...\n");
          funcaoResposta[2] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensAzul,temposExposicao,escura);
          
          funcaoResposta[0].salvar("Vermelho.fr");
          funcaoResposta[1].salvar("Verde.fr");
          funcaoResposta[2].salvar("Azul.fr");
          
          printf("Obtendo imagemHDR:\n");
          printf("Vermelho\n");
          retorno->r = metodoHDR->obterImagemHDR(numeroImagens, imagensVermelho,temposExposicao,funcaoResposta[0],escura);
          printf("Verde\n");
          retorno->g = metodoHDR->obterImagemHDR(numeroImagens, imagensVerde,temposExposicao,funcaoResposta[1],escura);
          printf("Azul\n");
          retorno->b = metodoHDR->obterImagemHDR(numeroImagens, imagensAzul,temposExposicao, funcaoResposta[2],escura);
          
          for(i = 0; i < numeroImagens; i++)
          {
               cvReleaseImage(&(imagensAzul[i]));
               cvReleaseImage(&(imagensVerde[i]));
               cvReleaseImage(&(imagensVermelho[i]));
          }
          
          for(i = 0; i < 3; i++)
          {
              funcaoResposta[i].limpar();      
          }
          
          delete [] imagensVermelho;
          delete [] imagensVerde;
          delete [] imagensAzul;
          return retorno;       
     }
     
     static ImagemHDR* gerarUsando(MetodoHDR * metodoHDR, 
                                   int numeroImagens,
                                   IplImage ** imagens,
                                   IplImage * escura,
                                   FuncaoResposta* funcaoResposta,
                                   float * temposExposicao)
     {
          if(!metodoHDR || !numeroImagens) return NULL;
          ImagemHDR* retorno = new ImagemHDR;
          retorno->width = imagens[0]->width;
          retorno->height = imagens[0]->height;
          
          IplImage** imagensVermelho = new IplImage*[numeroImagens];
          IplImage** imagensVerde = new IplImage*[numeroImagens];
          IplImage** imagensAzul = new IplImage*[numeroImagens];
          
          int i;
          for(i = 0; i < numeroImagens; i++)
          {
               imagensAzul[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               imagensVerde[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               imagensVermelho[i] = cvCreateImage(cvGetSize(imagens[i]),IPL_DEPTH_8U,1);
               cvSplit(imagens[i],imagensAzul[i],imagensVerde[i],imagensVermelho[i], NULL);        
          }
          
          printf("Obtendo imagemHDR:\n");
          printf("Vermelho\n");
          retorno->r = metodoHDR->obterImagemHDR(numeroImagens, imagensVermelho,temposExposicao,funcaoResposta[0],escura);
          printf("Verde\n");
          retorno->g = metodoHDR->obterImagemHDR(numeroImagens, imagensVerde,temposExposicao,funcaoResposta[1],escura);
          printf("Azul\n");
          retorno->b = metodoHDR->obterImagemHDR(numeroImagens, imagensAzul,temposExposicao, funcaoResposta[2],escura);
          
          for(i = 0; i < numeroImagens; i++)
          {
               cvReleaseImage(&(imagensAzul[i]));
               cvReleaseImage(&(imagensVerde[i]));
               cvReleaseImage(&(imagensVermelho[i]));
          }
          
          delete [] imagensVermelho;
          delete [] imagensVerde;
          delete [] imagensAzul;
          return retorno;       
     }
     
     bool salvarPFM(char* nomeArquivo)
     {
          float a = 0;
          ofstream arquivo;
          arquivo.open(nomeArquivo, ios::binary);
          if(arquivo.is_open())
          {
               arquivo << "PF" << (char)10;
               arquivo << width << " " << height << (char)10;
               arquivo << "-1.000000\n";
          
               int i,j;
               unsigned int p;
               for(j = height-1; j >= 0; j--)
               {
                    for(i = 0; i < width; i++)
                    {
                         p = j*width + i;
                         arquivo.write((char*)&(r[p]),4);
                         arquivo.write((char*)&(g[p]),4);
                         arquivo.write((char*)&(b[p]),4);
                    }     
               }
          }else
          {
               return false;
          }
          arquivo.close();
          return true;
     }
     
     ~ImagemHDR()
     {
          delete [] r;
          delete [] g;
          delete [] b;            
     }     
};
