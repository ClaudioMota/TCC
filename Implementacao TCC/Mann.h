//Método de geração de imagens HDR baseado no artigo de Mann & Picard
//Dúvidas:
//O único problema é achar a melhor função resposta

class MetodoMann : public MetodoHDR
{    
     private:
     struct RelacaoImagens //Utilizado para localizar as imagens com melhor diferenca de tempo
     {
          int indiceMenor, indiceMaior;
          double k;
          
          void operator = (RelacaoImagens b)
          {
               k = b.k;
               indiceMenor = b.indiceMenor;
               indiceMaior = b.indiceMaior;    
          }  
     };
     
     struct PontoImagem
     {
          int x,y;
          PontoImagem()
          {x = y = -1;}
          
          void operator = (PontoImagem b)
          {
               x = b.x;
               y = b.y;         
          }     
     };
     
     static RelacaoImagens encontrarMenorIntervaloTempo(int numeroImagens,          //N
                                                       float* temposExposicao     //ti
                                                       )
     {
          RelacaoImagens menorIntervalo;
          menorIntervalo.k = 10000;
          int i,j;
          
          for(i = 0; i < numeroImagens-1; i++)
          {
               j = i+1;     
               if(temposExposicao[j]/temposExposicao[i] < menorIntervalo.k)
               {
                    menorIntervalo.indiceMenor = i;
                    menorIntervalo.indiceMaior = j;
                    menorIntervalo.k = temposExposicao[j]/temposExposicao[i];                                         
               }     
          }
          
          return menorIntervalo;
     }
     
     static void sort(float* vetor, int tamanho)
     {
          float aux;
          int i,j,min;
          for(i = 0; i < tamanho; i++)
          {
               min = i;
               for(j = i; j < tamanho; j++)
               {
                    if(vetor[min] < vetor[j])
                    {
                         min = j;       
                    }
               }
               aux = vetor[min];
               vetor[min] = vetor[i];
               vetor[i] = aux;      
          }
     }
     
     static PontoImagem encontrarPontoRelativamenteEscuro(IplImage* imagem, int valor)
     {
          int valorAtual = 10000;
          int imgX, imgY;
          PontoImagem posicao;
          
          //varredura partido do centro
          int iteracao = 0;
          while(iteracao <= imagem->width/2+1 || iteracao <= imagem->height/2+1)
          {
               int caso = 0;
               imgY = imagem->height/2-iteracao;
               imgX = imagem->width/2-iteracao;
               while(imgY < imagem->height/2+iteracao)
               {
                    if(imgX >= 0 && imgX < imagem->width &&
                         imgY >= 0 && imgY < imagem->height    //verifica se o pixel em questão está dentro da imagem
                    )
                    {
                         if(imgY == imagem->height/2-iteracao || imgY == imagem->height/2+iteracao-1 || 
                            imgX == imagem->width/2-iteracao || imgX == imagem->width/2+iteracao-1
                            )
                         {
                              int pixel = (unsigned char)imagem->imageData[imgX + imgY*imagem->widthStep];
                              if(pixel < valorAtual && pixel >= valor)
                              {
                                   valorAtual = pixel;
                                   posicao.x = imgX;
                                   posicao.y = imgY;           
                              }   
                         } 
                    }
                    
                    if(imgX == imagem->width/2+iteracao-1)
                    {imgY++;}
                    
                    if((imgY == imagem->height/2-iteracao || imgY == imagem->height/2+iteracao-1) && imgX != imagem->width/2+iteracao-1)
                    {imgX++;}
                    else if(imgX == imagem->width/2+iteracao-1)
                    {imgX = imagem->width/2-iteracao;}
                    else
                    {imgX = imagem->width/2+iteracao-1;}
               }    
               iteracao++;               
          }
          
          //varredura padrão
          /*
          for(imgX = 0; imgX < imagem->width; imgX++)
          {
               for(imgY = 0; imgY < imagem->height; imgY++)
               {
                    int pixel = (unsigned char)imagem->imageData[imgX + imgY*imagem->widthStep];
                    if(pixel < valorAtual && pixel >= valor)
                    {
                         valorAtual = pixel;
                         posicao.x = imgX;
                         posicao.y = imgY;           
                    }     
               }         
          }*/
          return posicao;       
     }
     
     static PontoImagem encontrarPonto(IplImage* imagem, int valor)
     {
          PontoImagem posicao;
          int imgX, imgY;
          
          //varredura partindo do centro
          int iteracao = 0;
          while(iteracao <= imagem->width/2+1 || iteracao <= imagem->height/2+1)
          {
               int caso = 0;
               imgY = imagem->height/2-iteracao;
               imgX = imagem->width/2-iteracao;
               while(imgY < imagem->height/2+iteracao)
               {
                    if(imgX >= 0 && imgX < imagem->width &&
                         imgY >= 0 && imgY < imagem->height    //verifica se o pixel em questão está dentro da imagem
                    )
                    {
                         if(imgY == imagem->height/2-iteracao || imgY == imagem->height/2+iteracao-1 || 
                            imgX == imagem->width/2-iteracao || imgX == imagem->width/2+iteracao-1
                            )
                         {
                              int pixel = (unsigned char)imagem->imageData[imgX + imgY*imagem->widthStep];
                              if(pixel == valor)
                              {
                                   posicao.x = imgX;
                                   posicao.y = imgY;
                                   return posicao;           
                              }  
                         } 
                    }
                    
                    if(imgX == imagem->width/2+iteracao-1)
                    {imgY++;}
                    
                    if((imgY == imagem->height/2-iteracao || imgY == imagem->height/2+iteracao-1) && imgX != imagem->width/2+iteracao-1)
                    {imgX++;}
                    else if(imgX == imagem->width/2+iteracao-1)
                    {imgX = imagem->width/2-iteracao;}
                    else
                    {imgX = imagem->width/2+iteracao-1;}
               }    
               iteracao++;               
          }
          
          //varredura padrão
          /*
          for(imgX = 0; imgX < imagem->width; imgX++)
          {
               for(imgY = 0; imgY < imagem->height; imgY++)
               {
                    int pixel = (unsigned char)imagem->imageData[imgX + imgY*imagem->widthStep];
                    if(pixel == valor)
                    {
                         posicao.x = imgX;
                         posicao.y = imgY;
                         return posicao;           
                    }     
               }         
          }*/
          return posicao;       
     }
     
     static int valorMaisClaro(IplImage *imagem)
     {
          int max = 0;
          int imgX, imgY;
          for(imgX = 0; imgX < imagem->width; imgX++)
          {
               for(imgY = 0; imgY < imagem->height; imgY++)
               {
                    int pixel = (unsigned char)imagem->imageData[imgX + imgY*imagem->widthStep];
                    if(max < pixel)
                    {
                         max = pixel;           
                    }     
               }         
          }
          return max;     
     }
     
     public:
     
     char* getNomeArquivo(){return "Mann.pfm";}
     
     void executarDebug(IplImage * imagem,
                        PontoImagem* pontos,
                        int numeroPontos,
                        char* nomeArquivo         
                                 )
     {
          int brushWidth = 3;
          int p = 0;
          for(p = 0; p < numeroPontos; p++)
          {
               int y,x;
               int px,py;
               for(x = -brushWidth; x <= brushWidth; x++)
               {
                 px = x + pontos[p].x;
                 if(px >= 0 && px < imagem->width)
                 for(y = -brushWidth; y <= brushWidth; y++)
                 {
                   py = y + pontos[p].y;
                   if(py >= 0 && py < imagem->height)
                   {
                      imagem->imageData[(imagem->widthStep*py+3*px)] = 230+p*(10-230)/numeroPontos;
                      imagem->imageData[(imagem->widthStep*py+3*px)+1] = 230+p*(10-230)/numeroPontos;
                      imagem->imageData[(imagem->widthStep*py+3*px)+2] = 255;
                   }
                 }
               }
          }
          cvSaveImage(nomeArquivo,imagem);
          system("pause");                         
     }
     
     FuncaoResposta obterFuncaoResposta(IplImage ** coloridas,int numeroImagens,          //N
                                                  IplImage ** imagensEntrada, //{Yij}
                                                  float* temposExposicao,     //ti
                                                  IplImage *escura
                                                  )
     {
          FuncaoResposta retorno;
          PontoImagem* q = new PontoImagem[255];
          RelacaoImagens imagensCalibrar = encontrarMenorIntervaloTempo(numeroImagens, temposExposicao);
          int i, numeroPontos = 0;
          
          //Subtrair o valor Dmin (somente quando possui uma foto com a proteção da lente colocada)
          
          q[0] = encontrarPontoRelativamenteEscuro(imagensEntrada[imagensCalibrar.indiceMenor], 10);
          
          for(i = 1; (unsigned char)imagensEntrada[imagensCalibrar.indiceMaior]->imageData[q[i-1].x + q[i-1].y*imagensEntrada[imagensCalibrar.indiceMaior]->widthStep] < 255; i++)
          {
               q[i] = encontrarPonto(imagensEntrada[imagensCalibrar.indiceMenor],
                                    (unsigned char)imagensEntrada[imagensCalibrar.indiceMaior]->imageData[q[i-1].x + q[i-1].y*imagensEntrada[imagensCalibrar.indiceMaior]->widthStep]);
               numeroPontos++;
               //printf("%d ",(int)(unsigned char)imagensEntrada[imagensCalibrar.indiceMenor]->imageData[q[i-1].x + q[i-1].y*imagensEntrada[imagensCalibrar.indiceMenor]->widthStep]);
               if(q[i].x == -1 || i >= 40)
               {
                    printf("Falhou em convergir");
                    delete [] q;
                    return retorno;     
               }      
          }
          
          executarDebug(coloridas[imagensCalibrar.indiceMaior],q,numeroPontos,"pintandoPontos.bmp");
          
          float* gamas = new float[numeroPontos];
          float* betas = new float[numeroPontos];
          
          //Metodo Proposto no artigo
          for(i = 0; i < numeroPontos-1; i++)
          {
               float a = (unsigned char)imagensEntrada[imagensCalibrar.indiceMenor]->imageData[q[i].x + q[i].y*imagensEntrada[imagensCalibrar.indiceMenor]->widthStep];
               float b = (unsigned char)imagensEntrada[imagensCalibrar.indiceMenor]->imageData[q[i+1].x + q[i+1].y*imagensEntrada[imagensCalibrar.indiceMenor]->widthStep];
               
               gamas[i] = log(b/a)/log(imagensCalibrar.k);
               
               retorno.gama += log(b/a)/log(imagensCalibrar.k);
          }
          //media
          retorno.gama /= numeroPontos;
          
          retorno.beta = 128;
          printf("beta:%f, gama: %f\n", retorno.beta, retorno.gama);
          
          delete [] q;
          return retorno;       
     } 
     
     float* obterImagemHDR(int numeroImagens,          //N
                           IplImage ** imagensEntrada, //{Yij}
                           float* temposExposicao,     //ti
                           FuncaoResposta funcaoResposta,
                           IplImage *escura
                           )
     {
          int i,k,imgX,imgY;
          float* imagem = new float[imagensEntrada[0]->width*imagensEntrada[0]->height];
          
          float* inversas = new float[256];
          float* pesos = new float[256];
          
          for(i = 0; i < 256; i++)
          {
               inversas[i] = funcaoResposta.aplicarInversa(i);
               pesos[i] = funcaoResposta.aplicarDerivadaLogaritmica(inversas[i]); 
          }
          
          for(imgX = 0; imgX < imagensEntrada[0]->width; imgX++)
          {
               for(imgY = 0; imgY < imagensEntrada[0]->height; imgY++)
               {
                    i = imgY*imagensEntrada[0]->width + imgX;
                    float soma = 0;
                    float luz = 0;
                    imagem[i] = 0;
                    for(k = 0; k < numeroImagens; k++)
                    {
                         unsigned char pixel = (unsigned char)imagensEntrada[k]->imageData[imgX + imgY*imagensEntrada[k]->widthStep];
                         float peso = pesos[pixel];
                         luz = inversas[pixel]/temposExposicao[k];
                         if(luz < 0) luz = 0;
                         soma += peso;
                         imagem[i] += peso*luz;
                    }
                    if(soma > 0.0f)
                    {
                         imagem[i] /= soma;        
                    }
               } 
          }
          
          delete [] pesos;
          delete [] inversas;
          
          return imagem;                        
     } 
};
