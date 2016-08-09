//Método de geração de imagens HDR baseado no artigo de Robertson et.al
//Baseado também na implementação do pfstools por Grzegorz Krawczyk 2004
//Duvidas:

class MetodoRobertson : public MetodoHDR
{
     protected:
     static float ERRO;
     
     static float* gerarPesos()
     {
          float* pesos = new float[256];
          int i;
          for(i = 0; i < 256; i++)
          {
               pesos[i] = exp(-4.0f*pow(-127.5+i,2)/pow(127.5,2));
          }
          
          return pesos;
     }
     
     static void normalizar128(float* funcao)
     {
          float fatorEscala = 1.0f/funcao[128];
          int i;
          for(i = 0; i < 255; i++)
          {     funcao[i] *= fatorEscala;}
     }
     
     static void normalizarInversa(float* funcao)
     {
          float max = 0.0f;
          int i;
          for(i = 0; i < 255; i++)
          {if(max < funcao[i]) max = funcao[i];}
          for(i = 0; i < 255; i++)
          {     funcao[i] /= max;}
     }
     
     static bool verificarParada(float* a, float* b, float threshold)
     {
          float diferenca = 0.0f;
          int i;
          for(i = 0; i < 255; i++)
          {
            float dif = a[i] - b[i];
            if(dif >= 0)
              diferenca += dif;
            else
              diferenca -= dif;
            if(diferenca >= threshold) return false;   
          }
          return true;     
     }
     
     static void gerarImagemHDR(   float max,
                                   float* x,
                                   int numeroImagens,          //N
                                   IplImage ** imagensEntrada, 
                                   float* temposExposicao,     //ti
                                   float* funcaoResposta,      //Iyij
                                   float* pesos                //Wij
                                   )
     {
          int imgX,imgY;
          for(imgX = 0; imgX < imagensEntrada[0]->width; imgX++)
          {
               for(imgY = 0; imgY < imagensEntrada[0]->height; imgY++)
               {
                    int i,j;
                    float div = 0.0f;
                    j = imgX + imgY*imagensEntrada[0]->width;
                    x[j] = 0.0f;
                    
                    for(i = 0; i < numeroImagens; i++)
                    {
                         unsigned char pixel = imagensEntrada[i]->imageData[imgX + imgY*imagensEntrada[i]->widthStep];
                         float w = pesos[pixel];
                         if(pixel < 255)
                         {
                           //forma do pfstools
                           x[j] += w/temposExposicao[i]*funcaoResposta[pixel];
                           div += w;
                           
                           //forma direta do artigo
                           //x[j] += w*temposExposicao[i]*funcaoResposta[pixel];
                           //div += w*pow(temposExposicao[i],2);
                         }
                    }
                    if(!zero(div))
                    { x[j] = x[j]/div;}
                    else if((unsigned char)(imagensEntrada[0]->imageData[imgX + imgY*imagensEntrada[0]->widthStep]) > 100)
                    { 
                      x[j] = max;
                      //printf("(%i,%i) saturado!!\n",imgX,imgY);
                    }else
                    {
                      x[j] = 0.0f;
                      //printf("(%i,%i) zerado!!\n",imgX,imgY);     
                    }
               }         
          }  
     }
     
     public:
     
     char* getNomeArquivo(){return "Robertson.pfm";}
     
     FuncaoResposta obterFuncaoResposta(int numeroImagens,
                                        IplImage ** imagensEntrada,
                                        float* temposExposicao,
                                        IplImage* imagemEscura //Não usado
                                        )
     {
          int i;
          float* pesos = gerarPesos();
          
          float* Inversa = new float[255];
          
          int v;
          for(v = 0; v < 255; v++)
          {
               Inversa[v] = v*1.0f/128;      
          }
          
          float* x = new float[imagensEntrada[0]->width*imagensEntrada[0]->height];
          gerarImagemHDR(0.0f, x, numeroImagens, imagensEntrada, temposExposicao, Inversa, pesos);
          
          int* cardinalidadeE = new int[255];
          
          int m;
          for(m = 0; m < 255; m++)
          {
               cardinalidadeE[m] = 0;
          }
          
          int imgX,imgY;
          unsigned char pixel = 0;
          for(imgX = 0; imgX < imagensEntrada[0]->width; imgX++)
          {
               for(imgY = 0; imgY < imagensEntrada[0]->height; imgY++)
               {
                    for(i = 0; i < numeroImagens; i++)
                    {
                         pixel = imagensEntrada[i]->imageData[imgX + imgY*imagensEntrada[i]->widthStep];
                         if(pixel < 255) cardinalidadeE[pixel]++;
                    }
               }         
          }
          
          bool parar = false;
          int vezes = 0;
          while(!parar)
          {    
               float* InversaAnterior = Inversa;
               Inversa = new float [255];
               for(m = 0; m < 255; m++)
               {
                    Inversa[m] = 0;
               }
               
               for(imgX = 0; imgX < imagensEntrada[0]->width; imgX++)
               {
                    for(imgY = 0; imgY < imagensEntrada[0]->height; imgY++)
                    {
                         for(i = 0; i < numeroImagens; i++)
                         {
                              int j = imgX + imgY*imagensEntrada[0]->width;
                              pixel = imagensEntrada[i]->imageData[imgX + imgY*imagensEntrada[i]->widthStep];
                              if(pixel < 255) Inversa[pixel] += temposExposicao[i] * x[j];
                         }
                    }         
               }
               for(m = 0; m < 255; m++)
               {
                    if(cardinalidadeE[m] != 0)
                    { Inversa[m] /= cardinalidadeE[m];}
                    else
                    { Inversa[m] = 0;
                      printf("Erro: Divisao por zero evitada!\n Possivelmente as imagens nao possibilitam uma boa inferencia da funcao resposta(2)\n");
                    }
               }
               
               normalizar128(Inversa);
               
               printf("gerando parcial\n");
               
               gerarImagemHDR(0.0f, x, numeroImagens, imagensEntrada, temposExposicao, Inversa, pesos);
               
               parar = verificarParada(InversaAnterior,Inversa, ERRO);
               delete [] InversaAnterior;
          }
          
          //normalizarInversa(Inversa);
          
          FuncaoResposta retorno;
          
          for(i = 0; i < imagensEntrada[0]->width*imagensEntrada[0]->height; i++)
          {
               if(retorno.max < x[i]) retorno.max = x[i];   
          }
          
          delete [] pesos;
          delete [] x;
          delete [] cardinalidadeE;
          
          for(i = 0; i < 255; i++)
          if(!isfinite(Inversa[i]))
          {
               printf("Erro: Nao foi possivel convergir a funcao resposta!\n");
               delete [] Inversa;
               Inversa = NULL;                             
          }
          
          retorno.vetor = Inversa;
          retorno.gerarSpline();
          return retorno;
     }
     
     float* obterImagemHDR(int numeroImagens,          //N
                           IplImage ** imagensEntrada, 
                           float* temposExposicao,     //ti
                           FuncaoResposta funcaoResposta,      //Iyij
                           IplImage * imagemEscura  //Não usa
                           )
     {    
          int i, numeroPontos = 0;
          
          
          float* pesos = new float[256];
          float* vetor = new float[255];
          //Tentando fazer a unificação das funções resposta
          
          float max = 0;
          for(i = 0; i < 255; i++)
          {
               vetor[i] = funcaoResposta.aplicarInversa(i);
               pesos[i] = funcaoResposta.aplicarDerivadaLogaritmica(vetor[i]);
               if(pesos[i] < 1e-30 || i == 255) {pesos[i] = 1e-30;}
               if(pesos[i] > max) max = pesos[i];
          }
          
          for(i = 0; i < 255; i++)
          {
               pesos[i] = pesos[i]/max;
          }
          float * imagemHDR = new float[imagensEntrada[0]->width*imagensEntrada[0]->height];
          gerarImagemHDR(funcaoResposta.max, imagemHDR , numeroImagens, imagensEntrada, temposExposicao, vetor, pesos);
          
          delete [] pesos;
          delete [] vetor;
          
          return imagemHDR;                         
     }      
};

float MetodoRobertson::ERRO = 0.002f;
