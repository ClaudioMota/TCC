struct FuncaoResposta
{
     double alpha, beta, gama;
     float* vetor;
     CubicSpline* spline;
     float max;
          
     FuncaoResposta()
     {
          max = 0.0f;
          alpha = 0.0;
          beta = 0.0;
          gama = 0.0;
          vetor = NULL;
          spline = NULL;          
     }
     
     void salvar(char* nome)
     {
          int i;
          FILE* arquivo;
          arquivo = fopen(nome,"w");
          if(arquivo)
          {
               fprintf(arquivo,"%f\n%lf\n%lf\n%lf\n",max,alpha,beta,gama);
  
               if(vetor)
               {
                    fprintf(arquivo,"255\n");
                    for(i = 0; i < 255; i++)
                    {
                      fprintf(arquivo, "%f\n", vetor[i]);      
                    }
               }else
               {
                    fprintf(arquivo,"0\n");    
               }          
               fclose(arquivo);
          }  
     }
     
     bool carregar(char* nome)
     {
          int i;
          int tamanhoVetor;
          FILE* arquivo;
          arquivo = fopen(nome,"r");
          if(arquivo)
          {
               fscanf(arquivo,"%f%lf%lf%lf%i",&max,&alpha,&beta,&gama,&tamanhoVetor);
               if(tamanhoVetor)
               {
                    vetor = new float[tamanhoVetor];
                    for(i = 0; i < tamanhoVetor; i++)
                    {
                      fscanf(arquivo, "%f", &(vetor[i]));      
                    }
               }          
               fclose(arquivo);
          }else
          {
               return false;     
          }
          gerarSpline();
          return true;    
     }
     
     void gerarSpline()
     {
          if(vetor)
          {
               int i;
               Ponto* pontos = new Ponto[256];
               int numeroPontos = 0;
               for(i = 0; i < 255; i++)
               {
                    if(i == 0 || vetor[i-1] < vetor[i])
                    {
                    
                         pontos[numeroPontos].x = vetor[i];
                         pontos[numeroPontos].y = i;
                    
                         numeroPontos++;
                         if(i < 233) i += 20;
                         else if(i < 253) i = 253;
                    }
               }
               pontos[255].x = pontos[254].x + 10*(pontos[254].x-pontos[253].x);
               numeroPontos++;
               spline = new CubicSpline(pontos, numeroPontos);
               delete [] pontos;
          } 
     }
     
     void limpar()
     {
          if(vetor) delete [] vetor;
          if(spline) delete spline;
     }
     
     void operator = (FuncaoResposta b)
     {
          max = b.max;
          alpha = b.alpha;
          beta = b.beta;
          gama = b.gama;
          vetor = b.vetor;
          spline = b.spline;
     }
          
     double aplicar(double q)
     {
          if(q < 1e-10) q = 1e-10;
          if(spline) return spline->aplicar(q);
          return alpha + beta*pow(q,gama); 
     }
     
     double aplicarDerivada(double q)
     {
          if(q < 1e-10) q = 1e-10;
          if(spline) return spline->aplicarDerivada(q);
          return beta*gama*pow(q,gama-1.0f);
     }
     
     double aplicarInversa(double p)
     {
          if(p < 1e-10) p = 1e-10;
          if(spline)
          {    if(p >= 254) p = 254;
               return spline->aplicarInversa(p);
          }
          return pow((p-alpha)/beta,1.0f/gama);
     }
     
     double aplicarDerivadaLogaritmica(double q)
     {
          if(q < 1e-10) q = 1e-10;
          return aplicarDerivada(q)/(1.0/q*log10(exp(1)));
     }
     
     void print(char* nome)
     {
       int x;
       FILE* arquivo;
       printf("Imprimindo pontos das funcao resposta %s\n", nome);
       arquivo = fopen(nome, "w");
       fprintf(arquivo, "x = [");
       fprintf(arquivo, "%lf", aplicarInversa(0));
       for(x = 1; x < 256; x++)
       {
            double irradiacao = aplicarInversa(x);
            fprintf(arquivo,",%lf", irradiacao);      
       }
       fprintf(arquivo, "]\n");
       
       fprintf(arquivo, "y = [");
       fprintf(arquivo,"%i", 0);
       for(x = 1; x < 256; x++)
       {
            fprintf(arquivo,",%i", x);      
       }
       fprintf(arquivo, "]\n");
       fclose(arquivo);
     }
};
