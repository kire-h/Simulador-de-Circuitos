#ifndef _CIRCUITO_H_
#define _CIRCUITO_H_

#include "bool3S.h"
#include "porta.h"

/// ###########################################################################
/// ATENCAO PARA A CONVENCAO DOS NOMES PARA OS PARAMETROS DAS FUNCOES:
/// int I: indice (de entrada de porta): de 0 a NInputs-1
/// int Id___: identificador (de entrada, saida ou porta): nao 0
/// - int IdInput: de -1 a -NumEntradasCircuito
/// - int IdOutput: de 1 a NumSaidasCircuito
/// - int IdPort: de 1 a NumPortasCircuito
/// - int IdOrig: de 1 a NumPortasCircuito ou de -1 a -NumEntradasCircuito
///       (id da origem de uma entrada de porta ou de uma saida do circuito)
/// ###########################################################################

///
/// CLASSE CIRCUITO
///

class Circuito
{
private:
  /// ***********************
  /// Dados
  /// ***********************

  // NUMERO DE ENTRADAS DO CIRCUITO
  int Nin_circ;

  // PORTAS DO CIRCUITO
  std::vector<ptr_Porta> ports;

  // VALORES DAS SAIDAS LOGICAS DO CIRCUITO
  std::vector<bool3S> out_circ;

  // CONECTIVIDADE DO CIRCUITO

  // As ids das origens das entradas das portas
  // Eh um vetor de vetores: o vetor principal deve ter dimensao "Nports"
  // Cada vetor id_in.at(i) deve ter dimensao igual ao numero de entradas da porta cuja id=i+1
  // se id_in.at(i).at(j)>0: a j-esima entrada da i-esima porta (id=i+1) vem da saida da porta
  // cuja id eh o valor desse elemento do array
  // se id_in.at(i).at(j)<0: a j-esima entrada da i-esima porta (id=i+1) vem da entrada do circuito
  // cuja id eh o valor desse elemento do array
  // se id_in.at(i).at(j)==0: a j-esima entrada da i-esima porta (id=i+1) estah indefinida
  std::vector< std::vector<int> > id_in;

  // As ids das origens dos sinais de saida do circuito
  // Deve ser um vetor com dimensao "Nout"
  // se id_out.at(i)>0: a i-esima saida do circuito (id=i+1) vem da saida da porta
  // cuja id eh o valor desse elemento do array
  // se id_out.at(i)<0: a i-esima saida do circuito (id=i+1) vem da entrada do circuito
  // cuja id eh o valor desse elemento do array
  // se id_out.at(i)==0: a i-esima saida do circuito (id=i+1) estah indefinida
  std::vector<int> id_out;

public:

  /// ***********************
  /// Inicializacao e finalizacao
  /// ***********************

  // Construtor default = circuito vazio
  Circuito():
    Nin_circ(0),
    ports(),
    out_circ(),
    id_in(),
    id_out()
  {}

  // Cria o circuito com NI entradas, NO saidas e NP portas,
  Circuito(int NI, int NO, int NP): Circuito()
  {
    resize(NI,NO,NP);
  }

  // Construtor por copia
  Circuito(const Circuito& C);
  // Construtor por movimento
  Circuito(Circuito&& C) noexcept;

  // Destrutor
  ~Circuito()
  {
    clear();
  }
  // Limpa todo o conteudo do circuito.
  void clear() noexcept;

  // Operador de atribuicao por copia
  Circuito& operator=(const Circuito& C);
  // Operador de atribuicao por movimento
  Circuito& operator=(Circuito&& C) noexcept;

  // Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP portas,
  // com todos os vetores com valores iniciais nulos.
  // Se algum parametro for invalido, gera excecao.
  void resize(int NI, int NO, int NP);

  /// ***********************
  /// Funcoes de testagem
  /// ***********************

  // Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
  // para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
  bool validIdOrig(int IdOrig) const
  {
    return ( (IdOrig>=1 && IdOrig<=getNumPorts()) ||
             (IdOrig<=-1 && IdOrig>=-getNumInputs()) );
  }

  // Retorna true se o circuito eh valido (estah com todos os dados corretos):
  // - numero de entradas, saidas e portas valido
  // - todas as portas validas
  // - todas as saidas com Id de origem validas
  bool valid() const;

  /// ***********************
  /// Funcoes de consulta
  /// ***********************

  // Caracteristicas gerais do circuito
  int getNumInputs() const
  {
    return Nin_circ;
  }
  int getNumOutputs() const
  {
    return int(out_circ.size());
  }
  int getNumPorts() const
  {
    return int(ports.size());
  }

  // Retorna o nome da porta cuja id eh IdPort: AN, NX, etc.
  // Gera excecao se o parametro for invalido.
  std::string getNamePort(int IdPort) const;

  // Retorna o numero de entradas da porta cuja id eh IdPort.
  // Gera excecao se o parametro for invalido.
  int getNumInputsPort(int IdPort) const;

  // Retorna o valor logico atual da saida da porta cuja id eh IdPort.
  // Gera excecao se o parametro for invalido.
  bool3S getOutputPort(int IdPort) const;

  // Retorna o valor logico atual da saida do circuito cuja id eh IdOutput.
  // Gera excecao se o parametro for invalido.
  bool3S getOutputCirc(int IdOutput) const;

  // Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort.
  // Gera excecao se o parametro for invalido.
  int getIdInPort(int IdPort, int I) const;

  // Retorna a origem (a id) da saida do circuito cuja id eh IdOutput.
  // Gera excecao se o parametro for invalido.
  int getIdOutputCirc(int IdOutput) const;

  /// ***********************
  /// Funcoes de modificacao
  /// ***********************

  // A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com Nin entradas.
  // Tambem altera o numero de conexoes dessa porta no vetor id_in, para ser igual ao novo
  // numero de entradas da porta.
  // Se algum parametro for invalido, gera excecao.
  void setPort(int IdPort, const std::string& Tipo, int Nin);

  // Altera a origem da I-esima entrada da porta cuja id eh IdPort, que passa a ser "IdOrig"
  // Se algum parametro for invalido, gera excecao.
  void setIdInPort(int IdPort, int I, int IdOrig);

  // Altera a origem da saida cuja id eh "IdOut", que passa a ser "IdOrig".
  // Se algum parametro for invalido, gera excecao.
  void setIdOutputCirc(int IdOutput, int IdOrig);

  /// ***********************
  /// E/S de dados
  /// ***********************

  // Entrada dos dados de um circuito via arquivo.
  // Se o arquivo for invalido, nao altera o circuito e gera excecao.
  void ler(const std::string& arq);

  // Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois).
  // Soh deve escrever se o circuito for valido.
  // Retorna uma referencia aa mesma ostream que recebeu como parametro.
  std::ostream& escrever(std::ostream& O=std::cout) const;

  // Salvar circuito em arquivo, caso o circuito seja valido.
  // Abre a stream, chama o metodo escrever e depois fecha a stream.
  // Se deu erro, gera excecao.
  void salvar(const std::string& arq) const;

  /// ***********************
  /// SIMULACAO (funcao principal do circuito)
  /// ***********************

  // Calcula as saidas do circuito para os valores de entrada passados como parametro,
  // caso o circuito e o parametro de entrada sejam validos.
  // Se o circuito ou o parametro forem invalidos, gera excecao.
  void simular(const std::vector<bool3S>& in_circ);
};

// Operador de impressao da classe Circuit
inline std::ostream& operator<<(std::ostream& O, const Circuito& C)
{
  return C.escrever(O);
}

#endif // _CIRCUITO_H_
