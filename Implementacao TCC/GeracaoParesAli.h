class MetodoParesAli_Mann : public ImagemHDR
{
     //Esse método não está fiel ao artigo... Revisar
     /*static float* passoRecursivo(MetodoHDR * metodoHDR, 
                                  int numeroImagens,
                                  IplImage ** imagens,
                                  float * temposExposicao,
                                  FuncaoResposta funcaoResposta,
                                  IplImage * escura
                                  )
     {
       //retorno = metodoHDR->obterImagemHDR(numeroImagens, imagensVermelho,temposExposicao,funcaoResposta[0],escura);
       int i,j;
       float* retorno = 0;
       
       if(numeroImagens > 2)
       {
         IplImage** vetor1 = new IplImage* [numeroImagens-1];
         IplImage** vetor2 = new IplImage* [numeroImagens-1];
         float* tempos1 = new float[numeroImagens-1];
         float* tempos2 = new float[numeroImagens-1];
         
         for(i = 0; i < numeroImagens -1; i++)
         {
           vetor1[i] = imagens[i];
           vetor2[i] = imagens[i+1];
           tempos1[i] = temposExposicao[i];
           tempos2[i] = temposExposicao[i+1];
         }
         
         float* retorno1 = passoRecursivo(metodoHDR, numeroImagens-1, vetor1,tempos1,funcaoResposta,escura);
         float* retorno2 = passoRecursivo(metodoHDR, numeroImagens-1, vetor2,tempos2,funcaoResposta,escura); 
         
         IplImage* img1 = cvCloneImage(imagens[0]);
         IplImage* img2 = cvCloneImage(imagens[0]);
         
         //Preencher imagem
         for(i = 0; i < img1->width;i++)
         {
              for(j = 0; j < img1->height; j++)
              {
                   int a = (int)funcaoResposta.aplicar(retorno1[img1->width*j + i]);
                   int b = (int)funcaoResposta.aplicar(retorno2[img2->width*j + i]);
                   if(a > 255) a = 255;
                   else if(a < 0) a = 0;
                   img1->imageData[i+j*img1->widthStep] = a;
                   
                   if(b > 255) b = 255;
                   else if(b < 0) b = 0;
                   img2->imageData[i+j*img2->widthStep] = b;
                   
              }      
         }
         
         vetor1[0] = img1;
         vetor1[1] = img2;
         tempos1[0] = tempos1[0];
         tempos1[1] = tempos2[0];
         
         retorno = metodoHDR->obterImagemHDR(2, vetor1,tempos1,funcaoResposta,escura);
         
         cvReleaseImage(&img1);
         cvReleaseImage(&img2);
         
         delete [] retorno1;
         delete [] retorno2;
         delete [] vetor1;
         delete [] vetor2;
         delete [] tempos1;
         delete [] tempos2;
       }else
       {
         retorno = metodoHDR->obterImagemHDR(numeroImagens, imagens,temposExposicao,funcaoResposta,escura);
       }
       
       return retorno;     
     }*/
     
     static double estimarQ(FuncaoResposta funcao, double f1, double f2 , double desvio, double k)
     {
        return ;       
     }
     
     static float* passoRecursivo(MetodoHDR * metodoHDR, 
                                  int numeroImagens,
                                  IplImage ** imagens,
                                  float * temposExposicao,
                                  FuncaoResposta funcaoResposta,
                                  IplImage * escura
                                  )
     {
       int i,j;
       float* retorno = 0;
       
       if(numeroImagens > 2)
       {
         IplImage** vetor1 = new IplImage* [numeroImagens-1];
         IplImage** vetor2 = new IplImage* [numeroImagens-1];
         float* tempos1 = new float[numeroImagens-1];
         float* tempos2 = new float[numeroImagens-1];
         
         for(i = 0; i < numeroImagens -1; i++)
         {
           vetor1[i] = imagens[i];
           vetor2[i] = imagens[i+1];
           tempos1[i] = temposExposicao[i];
           tempos2[i] = temposExposicao[i+1];
         }
         
         float* retorno1 = passoRecursivo(metodoHDR, numeroImagens-1, vetor1,tempos1,funcaoResposta,escura);
         float* retorno2 = passoRecursivo(metodoHDR, numeroImagens-1, vetor2,tempos2,funcaoResposta,escura); 
         
         IplImage* img1 = cvCloneImage(imagens[0]);
         IplImage* img2 = cvCloneImage(imagens[0]);
         
         //Preencher imagem
         for(i = 0; i < img1->width;i++)
         {
              for(j = 0; j < img1->height; j++)
              {
                   int a = (int)funcaoResposta.aplicar(retorno1[img1->width*j + i]);
                   int b = (int)funcaoResposta.aplicar(retorno2[img2->width*j + i]);
                   if(a > 255) a = 255;
                   else if(a < 0) a = 0;
                   img1->imageData[i+j*img1->widthStep] = a;
                   
                   if(b > 255) b = 255;
                   else if(b < 0) b = 0;
                   img2->imageData[i+j*img2->widthStep] = b;
                   
              }      
         }
         
         vetor1[0] = img1;
         vetor1[1] = img2;
         tempos1[0] = tempos1[0];
         tempos1[1] = tempos2[0];
         
         retorno = metodoHDR->obterImagemHDR(2, vetor1,tempos1,funcaoResposta,escura);
         
         cvReleaseImage(&img1);
         cvReleaseImage(&img2);
         
         delete [] retorno1;
         delete [] retorno2;
         delete [] vetor1;
         delete [] vetor2;
         delete [] tempos1;
         delete [] tempos2;
       }else
       {
         retorno = metodoHDR->obterImagemHDR(numeroImagens, imagens,temposExposicao,funcaoResposta,escura);
       }
       
       return retorno;     
     }
     
     public:
     static ImagemHDR* gerarUsando(MetodoHDR * metodoHDR, 
                                   int numeroImagens,
                                   IplImage ** imagens,
                                   IplImage * escura,
                                   float * temposExposicao)
     {
          if(!metodoHDR || !numeroImagens) return NULL;
          MetodoParesAli_Mann* retorno = new MetodoParesAli_Mann;
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
          
          printf("gerando vermelho\n");
          FuncaoResposta funcaoResposta[3];
          funcaoResposta[0] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensVermelho,temposExposicao,escura);
          retorno->r = passoRecursivo(metodoHDR, numeroImagens, imagensVermelho,temposExposicao,funcaoResposta[0],escura);
          printf("gerando verde\n");
          funcaoResposta[1] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensVerde,temposExposicao,escura);
          retorno->g = passoRecursivo(metodoHDR, numeroImagens, imagensVerde,temposExposicao,funcaoResposta[1],escura);
          printf("gerando azul\n");
          funcaoResposta[2] = metodoHDR->obterFuncaoResposta(numeroImagens,imagensAzul,temposExposicao,escura);
          retorno->b = passoRecursivo(metodoHDR, numeroImagens, imagensAzul,temposExposicao,funcaoResposta[2],escura);
          printf("salvando...\n");
          
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
};
