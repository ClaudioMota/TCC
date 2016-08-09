class MetodoHDR
{
    public:
	 virtual char* getNomeArquivo(){return "MetodoHDR.pfm";}
     
     virtual FuncaoResposta obterFuncaoResposta(int numeroImagens,
                                                IplImage ** imagens,
                                                float* temposExposicao,
                                                IplImage* imagemEscura){}
     virtual float* obterImagemHDR(int numeroImagens,          //N
                                   IplImage ** imagensEntrada, 
                                   float* temposExposicao,     //ti
                                   FuncaoResposta funcaoResposta,      //Iyij
                                   IplImage * imagemEscura
                                  ){}
};
