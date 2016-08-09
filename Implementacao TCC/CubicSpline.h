//http://www.physics.utah.edu/~detar/phys6720/handouts/cubic_spline/cubic_spline/node1.html

bool zero(double x)
{
     return (x > -1e-30 && x < 1e-30);        
}

struct Ponto
{
     double x,y;
     Ponto()
     {x = y = 0.0f;}
     
     void set(double x,double y)
     {
          this->x = x;
          this->y = y;     
     }
          
     void operator = (Ponto b)
     {
          x = b.x;
          y = b.y;         
     }     
};

struct CubicSpline
{
     struct Trecho
     {
          Ponto P1,P2;
          double a, b , c, d;
          
          double aplicar(double x)
          {
               //Linear Spline
               //x = x-P1.x;
               //return (P2.y-P1.y)/(P2.x-P1.x)*x + P1.y;
               
               x = x-P1.x;
               return a*pow(x,3) + b*pow(x,2) + c*x + d;
          }
          
          double aplicarDerivada(double x)
          {    
               //Linear Spline
               //x = x-P1.x;
               //return (P2.y-P1.y)/(P2.x-P1.x);
               
               x = x-P1.x;
               return 3*a*pow(x,2) + 2*b*x + c;
          }
          
          double aplicarDerivadaSegunda(double x)
          {
               x = x-P1.x;
               return 6*a*x + 2*b;
          }
     };
     
     int numeroTrechos;
     Trecho* trecho;
     
     CubicSpline(Ponto* pontos, int numeroPontos, double derivadaInicio = 1e-10, double derivadaFim = 1e-10)
     {
          numeroTrechos = numeroPontos-1;
          trecho = new Trecho[numeroTrechos];
          int i;
          for(i = 0; i < numeroTrechos; i++)
          {
               trecho[i].P1 = pontos[i];
               trecho[i].P2 = pontos[i+1];     
          }
          gerarFuncao(derivadaInicio,derivadaFim);
     }
     
     static double* escalonar(int numeroLinhas, int numeroColunas, double** matriz)
     {
          if(numeroLinhas >= numeroColunas) return 0;
  
          int casa = 0;
          int n, m;
  
          for(casa = 0; casa < numeroLinhas; casa++)
          {
               n = 0;
               if(!zero(matriz[casa][casa]))
               {
                    double valorAnterior = matriz[casa][casa];
                    matriz[casa][casa] = 1;
                    for(m = casa+1; m < numeroColunas; m++)
                    {
                         matriz[casa][m] = matriz[casa][m]/valorAnterior;  
                    }                            
               }else
               {
                    for(n = 0; n < numeroLinhas; n++)
                    {
                         if(!zero(matriz[n][casa]))
                         {
                              double valorAnterior = matriz[n][casa];
                              matriz[casa][casa] = 1;
                              for(m = casa+1; m < numeroColunas; m++)
                              {
                                   matriz[casa][m] +=  matriz[n][m]/valorAnterior; 
                              }
                              break;
                         }      
                    }
               }
               if(n < numeroLinhas)
               {
                    for(n = 0; n < numeroLinhas; n++)
                    {
                         if(!zero(matriz[n][casa]) && n != casa)
                         {
                              double valorAnterior = matriz[n][casa];
                              for(m = casa; m < numeroColunas; m++)
                              {
                                   matriz[n][m] -=  matriz[casa][m]*valorAnterior; 
                              }
                         }      
                    }   
               }
          }
          double* retorno = new double [numeroColunas-1];
          
          for(m = 0; m < numeroColunas-1; m++)
          {
               retorno[m] = matriz[m][numeroColunas-1];      
          }
          return retorno;
     }
     
     void gerarFuncao(double derivadaInicio, double derivadaFim)
     {
          int i,j;
          for(i = 0; i < numeroTrechos; i++)
          {
               trecho[i].d = trecho[i].P1.y;
          }
          int numeroLinhas = 3*(numeroTrechos+1)-1;
          int numeroColunas = numeroLinhas+1;
          double** matriz = new double*[numeroLinhas];
          for(i = 0; i < numeroLinhas; i++)
          {
               matriz[i] = new double[numeroColunas];
               for(j = 0; j < numeroColunas; j++)
               {
                    Trecho* atual = &(trecho[i/3]);
                    if(i >= numeroLinhas-2)
                    {
                         matriz[i][j] = 0.0f;  
                    }else if(i%3 == 0)
                    {
                         if(j == numeroColunas-1)
                         {
                              //parte numerica
                              matriz[i][j] = atual->P2.y - atual->P1.y;   
                         }else if(j >= (i/3)*3 && j < (i/3)*3 + 3)
                         {
                              double x = atual->P2.x - atual->P1.x;
                              matriz[i][(i/3)*3] = pow(x,3);
                              matriz[i][(i/3)*3+1] = pow(x,2);
                              matriz[i][(i/3)*3+2] = x;
                              
                              j += 2;
                         }
                         else
                         {
                              matriz[i][j] = 0.0f;
                         }            
                    }else if(i%3 == 1)
                    {
                         if(j == numeroColunas-1)
                         {
                              //parte numerica
                              matriz[i][j] = 0.0f;   
                         }else if(j >= (i/3)*3 && j < (i/3)*3 + 6)
                         {
                              double x = atual->P2.x - atual->P1.x;
                              matriz[i][(i/3)*3] = 3.0f*pow(x,2);
                              matriz[i][(i/3)*3+1] = 2.0f*x;
                              matriz[i][(i/3)*3+2] = 1;
                              matriz[i][(i/3)*3+3] = 0;
                              matriz[i][(i/3)*3+4] = 0;
                              matriz[i][(i/3)*3+5] = -1;
                              if(i == numeroLinhas-4)
                              {
                                   matriz[i][(i/3)*3+4] = -1;
                                   matriz[i][(i/3)*3+5] = 0.0f;     
                              }
                              j += 5;
                         }
                         else
                         {
                              matriz[i][j] = 0.0f;
                         } 
                    }else if(i%3 == 2)
                    {
                         if(j == numeroColunas-1)
                         {
                              //parte numerica
                              matriz[i][j] = 0.0f;   
                         }else if(j >= (i/3)*3 && j < (i/3)*3 + 5)
                         {
                              double x = atual->P2.x - atual->P1.x;
                              matriz[i][(i/3)*3] = 6.0f*x;
                              matriz[i][(i/3)*3+1] = 2.0f;
                              matriz[i][(i/3)*3+2] = 0.0f;
                              matriz[i][(i/3)*3+3] = 0.0f;
                              matriz[i][(i/3)*3+4] = -2.0f;
                              if(i == numeroLinhas-3)
                              {
                                matriz[i][(i/3)*3+3] = -2.0f;        
                                matriz[i][(i/3)*3+4] = 0.0f;
                              }
                              j += 4;
                         }
                         else
                         {
                              matriz[i][j] = 0.0f;
                         }
                    } 
               }      
          }
          
          //Definicao das derivadas dos extremos
          matriz[numeroLinhas -2][2] = 1.0f;
          matriz[numeroLinhas -2][numeroColunas-1] = derivadaInicio;
          matriz[numeroLinhas -1][numeroColunas-2] = 1.0f;
          matriz[numeroLinhas -1][numeroColunas-1] = derivadaFim;
          
          double* valores = escalonar(numeroLinhas, numeroColunas, matriz);
          
          for(i = 0; i < numeroTrechos; i++)
          {
               trecho[i].a = valores[i*3];
               trecho[i].b = valores[i*3+1]; 
               trecho[i].c = valores[i*3+2];     
          }
          
          for(j = 0; j < numeroLinhas; j++)
          {
               delete [] matriz[j];      
          }
          delete [] valores;
          delete [] matriz;
     }
     
     double aplicar(double x)
     {
          int i;
          for(i = 0; i < numeroTrechos; i++)
          {
               if(trecho[i].P2.x >= x)
               {
                    return trecho[i].aplicar(x);                 
               }      
          }
          return trecho[numeroTrechos-1].aplicar(x);     
     }
     
     double aplicarInversa(double y)
     {
          float x;
          float xProx = 0.0f,diff = 1000000;
          for(x = trecho[0].P1.x; x <= trecho[numeroTrechos-1].P1.x; x += 0.01)
          {
               if(abs((double)(aplicar(x)-y)) < diff)
               {
                    xProx = x;
                    diff = abs(aplicar(x)-y);
               }
          }
          return xProx;      
     }
     
     double aplicarDerivada(double x)
     {
          int i;
          for(i = 0; i < numeroTrechos; i++)
          {
               if(trecho[i].P2.x >= x)
               {
                    return trecho[i].aplicarDerivada(x);                 
               }      
          } 
          return trecho[numeroTrechos-1].aplicarDerivada(x);     
     }
     
     ~CubicSpline()
     {
          delete [] trecho;              
     }
};
