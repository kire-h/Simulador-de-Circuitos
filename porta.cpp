#include "porta.h"

///
/// AS PORTAS
///

/// Porta NOT

void PortaNOT::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() != 1) throw std::invalid_argument("PortaNOT: precisa de exatamente uma entrada.");

    out_port = ~in_port[0];

}

/// Porta AND


PortaAND::PortaAND(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaAND deve ter pelo menos 2 entradas.");
}

void PortaAND::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()== static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res & in_port[i];

        }
        out_port = res;
    }
    else
    {
        throw std::invalid_argument("PortaAND: número de entradas inválido.");
    }
}


/// Porta NAND

PortaNAND::PortaNAND(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaNAND deve ter pelo menos 2 entradas.");
}

void PortaNAND::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()==static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res & in_port[i];

        }
        out_port = ~res;
    }
    else
    {
        throw std::invalid_argument("PortaNAND: número de entradas inválido.");
    }
}

/// Porta OR

PortaOR::PortaOR(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaOR deve ter pelo menos 2 entradas.");
}

void PortaOR::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()==static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res | in_port[i];

        }
        out_port = res;
    }
    else
    {
        throw std::invalid_argument("PortOR: número de entradas inválido.");
    }
}

/// Porta NOR

PortaNOR::PortaNOR(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaNOR deve ter pelo menos 2 entradas.");
}

void PortaNOR::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()==static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res | in_port[i];

        }
        out_port = ~res;
    }
    else
    {
        throw std::invalid_argument("PortNOR: número de entradas inválido.");
    }
}

/// Porta XOR

PortaXOR::PortaXOR(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaXOR deve ter pelo menos 2 entradas.");
}

void PortaXOR::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()==static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res ^ in_port[i];

        }
        out_port = res;
    }
    else
    {
        throw std::invalid_argument("PortaXOR: número de entradas inválido.");
    }
}

/// Porta NXOR

PortaNXOR::PortaNXOR(int NI) : Porta(NI)
{
    if (NI < 2) throw std::invalid_argument("PortaNXOR deve ter pelo menos 2 entradas.");
}

void PortaNXOR::simular(const std::vector<bool3S>& in_port)
{
    if (in_port.size() >0 && in_port.size()==static_cast<size_t>(Nin_port))
    {
        bool3S res = in_port[0];

        for (size_t i = 1; i < in_port.size(); ++i)
        {

            res = res ^ in_port[i];

        }
        out_port = ~res;
    }
    else
    {
        throw std::invalid_argument("PortaNXOR: número de entradas inválido.");
    }
}


