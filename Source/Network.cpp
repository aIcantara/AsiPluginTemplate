#include "Network.h"

using namespace std::placeholders;

CNetwork::~CNetwork()
{
    rakhook::destroy();

    StringCompressor::RemoveReference();
}

bool CNetwork::Initialize()
{
    static bool initialized = false;

    if (!initialized && rakhook::initialize())
    {
        StringCompressor::AddReference();

        rakhook::on_receive_rpc += std::bind(&CNetwork::OnReceiveRPC, this, _1, _2);
        rakhook::on_send_rpc += std::bind(&CNetwork::OnSendRPC, this, _1, _2, _3, _4, _5, _6);
        rakhook::on_receive_packet += std::bind(&CNetwork::OnReceivePacket, this, _1);
        rakhook::on_send_packet += std::bind(&CNetwork::OnSendPacket, this, _1, _2, _3, _4);

        initialized = true;
    }

    return initialized;
}

bool CNetwork::OnReceiveRPC(unsigned char& id, RakNet::BitStream* pBitStream)
{
    if (id == 93)
    {
        unsigned long color;
        std::string message;

        pBitStream->Read(color);
        ReadWithSize<unsigned int>(*pBitStream, message);

        message = "[HOOK] " + message;

        pBitStream->Reset();

        pBitStream->Write(color);
        WriteWithSize<unsigned int>(*pBitStream, message);
    }

    return true;
}

bool CNetwork::OnSendRPC(int& id, RakNet::BitStream* pBitStream, PacketPriority& priority, PacketReliability& reliability, char& ordChannel, bool& shTimestamp)
{
    return true;
}

bool CNetwork::OnReceivePacket(Packet* pPacket)
{
    return true;
}

bool CNetwork::OnSendPacket(RakNet::BitStream* pBitStream, PacketPriority& priority, PacketReliability& reliability, char& ordChannel)
{
    return true;
}

template<typename T>
bool CNetwork::ReadWithSize(RakNet::BitStream& bitStream, std::string& string)
{
    T size;

    if (!bitStream.Read(size) || size <= 0)
    {
        return false;
    }

    std::string text(size, '\0');

    bitStream.Read(text.data(), size);

    string = text;

    return true;
}

template<typename T>
void CNetwork::WriteWithSize(RakNet::BitStream& bitStream, std::string_view string)
{
    T size = static_cast<T>(string.size());

    bitStream.Write(size);
    bitStream.Write(string.data(), size);
}