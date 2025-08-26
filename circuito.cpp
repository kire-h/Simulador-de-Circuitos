#include <fstream>
#include "circuito.h"

///
/// CLASSE CIRCUITO
///

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

/// Construtor por copia
Circuito::Circuito(const Circuito& C)
    : Nin_circ(C.Nin_circ),
      ports(C.ports.size(),nullptr),
      out_circ(C.out_circ),
      id_in(C.id_in),
      id_out(C.id_out)
{
    for (size_t i=0; i<C.ports.size(); ++i){
        if(C.ports.at(i)!=nullptr) ports.at(i) = C.ports.at(i)->clone();
    }
}

/// Construtor por movimento
Circuito::Circuito(Circuito&& C) noexcept
    : Nin_circ(C.Nin_circ),
      ports(),
      out_circ(),
      id_in(),
      id_out()
{
    ports.swap(C.ports);
    out_circ.swap(C.out_circ);
    id_in.swap(C.id_in);
    id_out.swap(C.id_out);
    C.Nin_circ = 0;
}

/// Limpa todo o conteudo do circuito.
void Circuito::clear() noexcept
{
    Nin_circ = 0;
    out_circ.clear();
    id_in.clear();
    id_out.clear();
    for (auto p : ports) delete p;
    ports.clear();
}

/// Operador de atribuicao por copia
Circuito& Circuito::operator=(const Circuito& C)
{
    if (this == &C) return *this;
    clear();
    Nin_circ = C.Nin_circ;
    ports.resize(C.ports.size(),nullptr);
    out_circ = C.out_circ;
    id_in = C.id_in;
    id_out = C.id_out;
    for (size_t i=0; i<C.ports.size(); ++i){
        if(C.ports.at(i)!=nullptr) ports.at(i) = C.ports.at(i)->clone();
    }
    return *this;
}

/// Operador de atribuicao por movimento
Circuito& Circuito::operator=(Circuito&& C) noexcept
{
    clear();
    Nin_circ = C.Nin_circ;
    out_circ.swap(C.out_circ);
    id_in.swap(C.id_in);
    id_out.swap(C.id_out);
    ports.swap(C.ports);
    C.Nin_circ = 0;
    return *this;
}

/// Redimensiona o circuito
void Circuito::resize(int NI, int NO, int NP)
{
  if (NI<=0 || NO<=0 || NP<=0)
    throw std::invalid_argument("resize: invalid parameter(s)");
  clear();
  Nin_circ = NI;
  out_circ.resize(NO, bool3S::UNDEF);
  id_out.resize(NO,0);
  ports.resize(NP,nullptr);
  id_in.resize(NP, std::vector<int>(0));
}

/// ***********************
/// Funcoes de testagem
/// ***********************

/// Testa circuito valido
bool Circuito::valid() const
{
  int id;
  // Testa o numero de entradas, saidas e portas
  if (getNumInputs()<=0 || getNumOutputs()<=0 || getNumPorts()<=0) return false;
  // Testa cada porta
  for (id=1; id<=getNumPorts(); ++id)
  {
    if (ports.at(id-1)==nullptr) return false;
    for (int j=0; j<getNumInputsPort(id); ++j)
    {
      if (!validIdOrig(getIdInPort(id,j))) return false;
    }
  }
  // Testa cada saida
  for (id=1; id<=getNumOutputs(); ++id)
  {
    if (!validIdOrig(getIdOutputCirc(id))) return false;
  }
  // Tudo valido!
  return true;
}

/// ***********************
/// Funcoes de consulta
/// ***********************

/// Retorna o nome da porta cuja id eh IdPort: AN, NX, etc.
/// Gera excecao se o parametro for invalido.
std::string Circuito::getNamePort(int IdPort) const
{
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("getNamePort: invalid ID");
  if (ports.at(IdPort-1)==nullptr) return "??";
  return ports.at(IdPort-1)->getName();
}

/// Retorna o numero de entradas da porta cuja id eh IdPort.
/// Gera excecao se o parametro for invalido.
int Circuito::getNumInputsPort(int IdPort) const
{
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("getNumInputsPort: invalid ID");
  if (ports.at(IdPort-1)==nullptr) return 0;
  return ports.at(IdPort-1)->getNumInputs();
}

/// Retorna o valor logico atual da saida da porta cuja id eh IdPort.
/// Gera excecao se o parametro for invalido.
bool3S Circuito::getOutputPort(int IdPort) const
{
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("getOutputPort: invalid ID");
  if (ports.at(IdPort-1)==nullptr) return bool3S::UNDEF;
  return ports.at(IdPort-1)->getOutput();
}

/// Retorna o valor logico atual da saida do circuito cuja id eh IdOutput.
/// Gera excecao se o parametro for invalido.
bool3S Circuito::getOutputCirc(int IdOutput) const
{
  if (IdOutput<1 || IdOutput>getNumOutputs()) throw std::out_of_range("getOutputCirc: invalid ID");
  return out_circ.at(IdOutput-1);
}

/// Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort.
/// Gera excecao se algum parametro for invalido.
int Circuito::getIdInPort(int IdPort, int I) const
{
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("getIdInPort: invalid ID");
  if (ports.at(IdPort-1)==nullptr) throw std::invalid_argument("getIdInPort: port not allocated");
  if (I<0 || I>=ports.at(IdPort-1)->getNumInputs()) throw std::out_of_range("getIdInPort: invalid index");
  return id_in.at(IdPort-1).at(I);
}

/// Retorna a origem (a id) da saida do circuito cuja id eh IdOutput.
/// Gera excecao se o parametro for invalido.
int Circuito::getIdOutputCirc(int IdOutput) const
{
  if (IdOutput<1 || IdOutput>getNumOutputs()) throw std::out_of_range("getOutputCirc: invalid ID");
  return id_out.at(IdOutput-1);
}

/// ***********************
/// Funcoes de modificacao
/// ***********************

/// A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com Nin entradas.
/// Tambem altera o numero de conexoes dessa porta no vetor id_in, para ser igual ao novo
/// numero de entradas da porta.
/// Se algum parametro for invalido, gera excecao.
void Circuito::setPort(int IdPort, const std::string& Tipo, int Nin)
{
  // Chegagem dos parametros
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("setPort: invalid ID");
  if (Tipo!="NT" &&
      Tipo!="AN" && Tipo!="NA" &&
      Tipo!="OR" && Tipo!="NO" &&
      Tipo!="XO" && Tipo!="NX") throw std::invalid_argument("setPort: invalid port type");
  if ( (Tipo=="NT" && Nin!=1) ||
       (Tipo!="NT" && Nin<2) ) throw std::range_error("setPort: invalid number of inputs");

  // Altera a porta:
  // - cria a nova porta
  // - redimensiona o vetor de conexoes da porta
  Porta* novaPorta = nullptr;

  if (Tipo == "NT") novaPorta = new PortaNOT();
  else if (Tipo == "AN") novaPorta = new PortaAND(Nin);
  else if (Tipo == "NA") novaPorta = new PortaNAND(Nin);
  else if (Tipo == "OR") novaPorta = new PortaOR(Nin);
  else if (Tipo == "NO") novaPorta = new PortaNOR(Nin);
  else if (Tipo == "XO") novaPorta = new PortaXOR(Nin);
  else if (Tipo == "NX") novaPorta = new PortaNXOR(Nin);

  ports.at(IdPort - 1) = novaPorta;

  id_in.at(IdPort - 1).resize(Nin, 0);
  // perguntar pra ele
}

/// Altera a origem de uma entrada de uma porta
void Circuito::setIdInPort(int IdPort, int I, int IdOrig)
{
  // Chegagem dos parametros
  if (IdPort<1 || IdPort>getNumPorts()) throw std::out_of_range("setIdInPort: invalid IdPort");
  if (ports.at(IdPort-1)==nullptr) throw std::invalid_argument("setIdInPort: port not allocated");
  if (I<0 || I>=ports.at(IdPort-1)->getNumInputs()) throw std::out_of_range("setIdInPort: invalid index");
  if (!validIdOrig(IdOrig)) throw std::out_of_range("setIdInPort: invalid IdOrig");
  // Fixa a origem da entrada
  id_in.at(IdPort-1).at(I) = IdOrig;
}

/// Altera a origem de uma saida
void Circuito::setIdOutputCirc(int IdOutput, int IdOrig)
{
  if (IdOutput<1 || IdOutput>getNumOutputs()) throw std::out_of_range("setIdOutputCirc: invalid IdOutput");
  if (!validIdOrig(IdOrig)) throw std::out_of_range("setIdOutputCirc: invalid IdOrig");
  id_out.at(IdOutput-1) = IdOrig;
}

/// ***********************
/// E/S de dados
/// ***********************

/// Entrada dos dados de um circuito via arquivo.
/// Se o arquivo for invalido, nao altera o circuito e gera excecao.
void Circuito::ler(const std::string& arq)
{
  // Novo circuito provisorio a ser lido do arquivo
  Circuito prov;
  // A stream do arquivo a ser lido
  std::ifstream myfile(arq);

  if (!myfile.is_open()) throw std::ios_base::failure(std::string("ler: could not open file ")+arq);

  // Variaveis temporarias para leitura
  std::string pS;
  int NI,NO,NP;
  char c;
  std::string Tipo;
  int Nin_port;
  int id_orig;
  int i,id,I;

  // Lendo as dimensoes do circuito
  myfile >> pS >> NI >> NO >> NP;
  if (!myfile.good() || pS!="CIRCUITO") throw std::ios_base::failure("ler: invalid file (CIRCUITO)");
  // Redimensionando o novo circuito
  // Gera excecao se parametros invalidos
  prov.resize(NI, NO, NP);

  // Lendo as portas do circuito
  myfile >> pS;
  if (!myfile.good() || pS!="PORTAS") throw std::ios_base::failure("ler: invalid file (PORTAS)");
  for (i=0; i<prov.getNumPorts(); ++i)
  {
    // Lendo o tipo e o numero de entradas de uma porta
    myfile >> id >> c >> Tipo >> Nin_port;
    if (!myfile.good() || id != i+1 || c!=')') throw std::ios_base::failure("ler: invalid port "+std::to_string(i));
    // Fixa a porta
    // Gera excecao se parametros invalidos
    prov.setPort(id,Tipo,Nin_port);
  }

  // Lendo a conectividade das portas
  myfile >> pS;
  if (!myfile.good() || pS!="CONEXOES") throw std::ios_base::failure("ler: invalid file (CONEXOES)");
  for (i=0; i<prov.getNumPorts(); ++i)
  {
    // Lendo a id da porta
    myfile >> id >> c;
    if (!myfile.good() || id != i+1 || c!=')') std::ios_base::failure("ler: invalid connexion "+std::to_string(i));
    // Lendo as ids das entradas da porta
    for (I=0; I<prov.getNumInputsPort(id); ++I)
    {
      // Lendo a id de uma entrada da porta
      myfile >> id_orig;
      if (!myfile.good()) std::ios_base::failure("ler: invalid input "+std::to_string(I));
      // Fixa a entrada da porta
      // Gera excecao se parametros invalidos
      prov.setIdInPort(id, I, id_orig);
    }
  }

  // Lendo as saidas do circuito
  myfile >> pS;
  if (!myfile.good() || pS!="SAIDAS") throw std::ios_base::failure("ler: invalid file (SAIDAS)");
  for (i=0; i<prov.getNumOutputs(); ++i)
  {
    // Lendo a id de uma saida do circuito
    myfile >> id >> c >> id_orig;
    if (!myfile.good() || id != i+1 || c!=')') std::ios_base::failure("ler: invalid output "+std::to_string(i));
    // Fixa a saida do circuito
    // Gera excecao se parametros invalidos
    prov.setIdOutputCirc(id, id_orig);
  }

  // Leitura OK
  // Faz o circuito assumir as caracteristicas lidas do arquivo
  *this = std::move(prov);
}

/// Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois).
/// Soh deve escrever se o circuito for valido.
/// Retorna uma referencia aa mesma ostream que recebeu como parametro.
std::ostream& Circuito::escrever(std::ostream& O) const
{
  // Soh imprime se o circuito for valido
  if (!valid()) return O;

  int id,I;

  O << "CIRCUITO "
    << getNumInputs() << ' '
    << getNumOutputs() << ' '
    << getNumPorts() << std::endl;
  O << "PORTAS\n";
  for (id=1; id<=getNumPorts(); ++id)
  {
    O << id << ") " << getNamePort(id) << ' '
      << getNumInputsPort(id) << std::endl;
  }
  O << "CONEXOES\n";
  for (id=1; id<=getNumPorts(); ++id)
  {
    O << id << ')';
    for (I=0; I<getNumInputsPort(id); ++I)
    {
      O << ' ' << getIdInPort(id,I);
    }
    O << std::endl;
  }
  O << "SAIDAS\n";
  for (id=1; id<=getNumOutputs(); ++id)
  {
    O << id << ") " << getIdOutputCirc(id) << std::endl;
  }
  return O;
}

/// Salvar circuito em arquivo, caso o circuito seja valido.
/// Abre a stream, chama o metodo escrever e depois fecha a stream.
/// Se deu erro, gera excecao.
void Circuito::salvar(const std::string& arq) const
{
  if (!valid()) throw std::logic_error("salvar: invalid circuit");

  std::ofstream myfile(arq);
  if (!myfile.is_open()) throw std::ios_base::failure(std::string("salvar: could not open file ")+arq);
  escrever(myfile);
}

/// ***********************
/// SIMULACAO (funcao principal do circuito)
/// ***********************

/// Calcula as saidas do circuito para os valores de entrada passados como parametro,
/// caso o circuito e o parametro de entrada sejam validos.
/// Se o circuito ou o parametro forem invalidos, gera excecao.
void Circuito::simular(const std::vector<bool3S>& in_circ)
{
  // Soh simula se o cicuito e o parametro forem validos
  if (!valid()) throw std::logic_error("simular: invalid circuit");
  if (static_cast<int>(in_circ.size()) != getNumInputs())
    throw std::range_error("simular: incompatible parameter size");

  for (int i = 0; i < getNumPorts(); ++i) {
      ports.at(i)->setOutput(bool3S::UNDEF);
  }

  bool tudo_def, alguma_def;

  do {
      tudo_def = true;
      alguma_def = false;

      for (int id = 1; id <= getNumPorts(); ++id) {
          if (getOutputPort(id) == bool3S::UNDEF) {

              std::vector<bool3S> in_port(getNumInputsPort(id));

              for (int j = 0; j < getNumInputsPort(id); ++j) {
                  int id_orig = getIdInPort(id,j);
                  if (id_orig > 0)
                      in_port.at(j) = ports.at(id_orig - 1)->getOutput();
                  else if (id_orig < 0)
                      in_port.at(j) = in_circ.at(-id_orig - 1);
              }
              ports.at(id-1)->simular(in_port);
              if (getOutputPort(id) == bool3S::UNDEF)
                  tudo_def = false;
              else
                  alguma_def = true;
          }
      }
  } while (!tudo_def && alguma_def);

  for (int id = 1; id <= getNumOutputs(); ++id) {
      int id_orig = getIdOutputCirc(id);
      if (id_orig > 0)
          out_circ.at(id-1) = ports.at(id_orig - 1)->getOutput();
      else if (id_orig < 0)
          out_circ.at(id-1) = in_circ.at(-id_orig - 1);
  }
}
